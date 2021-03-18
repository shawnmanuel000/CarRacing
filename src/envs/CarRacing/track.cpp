#include "track.h"

static std::string get_track_file(const std::string& track_name)
{
	std::string filepath = join(parent(__FILE__), {"spec","tracks",track_name+".csv"});
}

static std::string get_map_file(const std::string& track_name)
{
	std::string filepath = join(parent(__FILE__), {"spec","point_maps",track_name+".npz"});
}

static xt::xarray<float> load_track(const std::string& track_name, int stride)
{
	std::string track_file = get_track_file(track_name);
	xt::xarray<float> track = xarray_from_csv(track_file);
	track = xt::view(track, xt::range(0, track.shape(0), stride), xt::all());
	return track;
}

static xt::xarray<float> compute_boundaries(const xt::xarray<float>& points, float lane_width)
{
	xt::svector<size_t> shape = points.shape();
	xt::xarray<float> left_boundary = xt::zeros<float>(shape);
	xt::xarray<float> right_boundary = xt::zeros<float>(shape);
	xt::xarray<float> grad = xt::row(points,1) - xt::row(points,0);
	xt::xarray<float> normal = xt::xarray<float>({-grad[1], grad[0]}) / xt::norm_l2(grad);
	xt::row(left_boundary,0) = xt::row(points,0) + lane_width*normal;
	xt::row(right_boundary,0) = xt::row(points,0) - lane_width*normal;
	xt::xarray<float> newgrad, avggrad;
	for (size_t i = 1; i < shape[0]-1; i++)
	{
		newgrad = xt::row(points,i+1) - xt::row(points,i);
		avggrad = (newgrad + grad) / 2;
		normal = xt::xarray<float>({-avggrad[1], avggrad[0]}) / xt::norm_l2(avggrad);
		xt::row(left_boundary,i) = xt::row(points,i) + lane_width*normal;
		xt::row(right_boundary,i) = xt::row(points,i) - lane_width*normal;
		grad = newgrad;
	}
	normal = xt::xarray<float>({-newgrad[1], newgrad[0]}) / xt::norm_l2(newgrad);
	xt::row(left_boundary,shape[0]-1) = xt::row(points,shape[0]-1) + lane_width*normal;
	xt::row(right_boundary,shape[0]-1) = xt::row(points,shape[0]-1) - lane_width*normal;
	xt::xarray<float> boundaries = xt::stack(xtuple(left_boundary, right_boundary), shape.size()-1);
	return boundaries;
}

Track::Track(const std::string& track_name, int stride)
{
	this->stride = stride;
	this->track_name = track_name;
	this->track = load_track(track_name, stride);
	this->boundaries = compute_boundaries(this->track, 15.0f);
	this->X = xt::col(this->track, 0);
	this->Y = xt::col(this->track, 1);
	this->load_point_map(track_name);
}

xt::xarray<size_t> Track::get_map_index(const xt::xarray<float>& point) const
{
	size_t ndims = point.dimension();
	size_t dimension = point.shape()[ndims-1];
	xt::xarray<size_t> broadcast_shape = xt::concatenate(xtuple(xt::ones<size_t>({ndims-1}), xt::xarray<size_t>({dimension})));
	xt::xarray<float> minref = xt::broadcast(xt::view(this->min_point, xt::range(0, dimension)), broadcast_shape);
	xt::xarray<float> maxref = xt::broadcast(xt::view(this->max_point, xt::range(0, dimension)), broadcast_shape);
	xt::xarray<float> clip_point = xt::clip(point, minref, maxref);
	xt::xarray<size_t> index = xt::round((clip_point-minref)/this->res);
	return index;
}


xt::xarray<float> Track::get_nearest_index(const xt::xarray<float>& point) const
{
	xt::xarray<size_t> index = this->get_map_index(point);
	xt::xarray<size_t> grid_shape = xt::view(xt::adapt(index.shape()), xt::range(0, index.dimension()-1));
	xt::xarray<size_t> indexing_shape = xt::xarray<size_t>({xt::prod(grid_shape)(), index.shape()[index.dimension()-1]});
	index.reshape(indexing_shape);
	std::vector<xt::xindex> indexers;
	for (size_t i = 0; i < indexing_shape[0]; i++)
	{
		indexers.push_back(xt::xindex({index(i,0),index(i,1)}));
	}
	xt::xarray<int> indices = xt::index_view(this->idx_map, indexers);
	indices.reshape(grid_shape);
	return indices;
}

xt::xarray<float> Track::get_nearest_distance(const xt::xarray<float>& point) const
{
	xt::xarray<size_t> index = this->get_map_index(point);
	xt::xarray<size_t> grid_shape = xt::view(xt::adapt(index.shape()), xt::range(0, index.dimension()-1));
	xt::xarray<size_t> indexing_shape = xt::xarray<size_t>({xt::prod(grid_shape)(), index.shape()[index.dimension()-1]});
	index.reshape(indexing_shape);
	std::vector<xt::xindex> indexers;
	for (size_t i = 0; i < indexing_shape[0]; i++)
	{
		indexers.push_back(xt::xindex({index(i,0),index(i,1)}));
	}
	xt::xarray<float> min_dists = xt::index_view(this->dist_map, indexers);
	min_dists.reshape(grid_shape);
	return min_dists;
}

void Track::load_point_map(const std::string& track_name)
{
	std::string map_file = get_map_file(track_name);
    cnpy::npz_t data = cnpy::npz_load(map_file);
	this->Xmap = xarray_from_nparray_float(data["X"]);
	this->Ymap = xarray_from_nparray_float(data["Y"]);
	this->idx_map = xarray_from_nparray_int(data["idx"]);
	this->dist_map = xarray_from_nparray_float(data["min_dist"]);
	this->res = xarray_from_nparray_int(data["res"])[0];
	this->stride = xarray_from_nparray_int(data["stride"])[0];
	this->min_point = xt::xarray<float>({this->Xmap(0), this->Ymap(0)});
	this->max_point = xt::xarray<float>({this->Xmap(this->Xmap.shape(0)-1), this->Ymap(this->Ymap.shape(0)-1)});
}