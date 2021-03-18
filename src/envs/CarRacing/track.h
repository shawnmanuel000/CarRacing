#ifndef ENVS_CARRACING_TRACK
#define ENVS_CARRACING_TRACK

#include <stdlib.h>
#include <iostream>
#include <string>
#include <regex>
#include <stdlib.h>
#include <vector>

#include "../../main.h"

xt::xarray<int> xarray_from_nparray_int(const cnpy::NpyArray& arr);
xt::xarray<float> xarray_from_nparray_float(const cnpy::NpyArray& arr);
xt::xarray<float> xarray_from_csv(const std::string& filename, int headers);

class Track
{
	public:
		Track(const std::string& track_name, int stride=1);
		xt::xarray<size_t> get_map_index(const xt::xarray<float>& point) const;
		xt::xarray<float> get_nearest_index(const xt::xarray<float>& point) const;
		xt::xarray<float> get_nearest_distance(const xt::xarray<float>& point) const;
		void load_point_map(const std::string& track_name);

	public:
		std::string track_name;
		xt::xarray<float> track;
		xt::xarray<float> boundaries;
		xt::xarray<float> X;
		xt::xarray<float> Y;
		xt::xarray<float> min_point;
		xt::xarray<float> max_point;
		xt::xarray<float> Xmap;
		xt::xarray<float> Ymap;
		xt::xarray<int> idx_map;
		xt::xarray<float> dist_map;
		float res;
		int stride;
};

#endif // ENVS_CARRACING_TRACK
