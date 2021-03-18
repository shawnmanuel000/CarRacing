#include "ref.h"
#include "dynamics.h"

static std::string get_ref_file(const std::string& ref_name)
{
	std::string filepath = join(parent(__FILE__), {"spec","refs",ref_name+".csv"});
	return filepath;
}

static std::string get_map_file(const std::string& ref_name)
{
	std::string filepath = join(parent(__FILE__), {"spec","point_maps",ref_name+"_ref.npz"});
	return filepath;
}

static std::pair<std::map<std::string,size_t>,xt::xarray<float>> load_ref(const std::string& ref_name)
{
	std::string ref_file = get_ref_file(ref_name);
	std::pair<std::map<std::string,size_t>,xt::xarray<float>> df = read_csv(ref_file, 2);
	return df;
}

RefDriver::RefDriver(const std::string& track_name, float dt)
{
	this->ref_name = track_name;
	std::pair<std::map<std::string,size_t>,xt::xarray<float>> df = load_ref(this->ref_name);
	this->ref_fields = df.first;
	this->ref = df.second;
	xt::xarray<float> times = xt::col(this->ref, this->ref_fields["Time"]);
	this->ref_dt = times[1] - times[0];
	this->load_point_map(this->ref_name);
	this->start_posx = xt::col(this->ref, this->ref_fields["PathX"])[0];
	this->start_posy = xt::col(this->ref, this->ref_fields["PathY"])[0];
	this->start_yaw = xt::col(this->ref, this->ref_fields["PathHeading"])[0];
	this->start_vel = xt::col(this->ref, this->ref_fields["Speed"])[0];
	this->max_time = std::min(max_time, times[times.shape()[0]-1]);
}

CarState RefDriver::state(float time)
{
	size_t index = std::fmod(time, this->max_time) / this->ref_dt;
	xt::xarray<float> row = xt::row(this->ref, index);
	float X = row[this->ref_fields["PathX"]];
	float Y = row[this->ref_fields["PathY"]];
	float yaw = row[this->ref_fields["PathHeading"]];
	float Vx = row[this->ref_fields["Vx"]];
	float Vy = row[this->ref_fields["Vy"]];
	float S = row[this->ref_fields["Dist"]];
	float yaw_dot = row[this->ref_fields["YawVel"]];
	float steer = row[this->ref_fields["Steer"]];
	float pedals = 0.0;
	return CarState(X, Y, yaw, Vx, Vy, S, yaw_dot, steer, pedals);
}


void RefDriver::load_point_map(const std::string& ref_name)
{
	std::string map_file = get_map_file(ref_name);
    cnpy::npz_t data = cnpy::npz_load(map_file);
	this->Xmap = xarray_from_nparray_float(data["X"]);
	this->Ymap = xarray_from_nparray_float(data["Y"]);
	this->Smap = xarray_from_nparray_float(data["S"]);
	this->time_map = xarray_from_nparray_float(data["time"]);
	this->sres = xarray_from_nparray_float(data["sres"])[0];
	this->res = xarray_from_nparray_int(data["res"])[0];
	this->dt = xarray_from_nparray_float(data["dt"])[0];
	this->min_point = xt::xarray<float>({this->Xmap(0), this->Ymap(0)});
	this->max_point = xt::xarray<float>({this->Xmap(this->Xmap.shape(0)-1), this->Ymap(this->Ymap.shape(0)-1)});
	this->max_time = this->time_map[xt::argmax(this->time_map)[0]];
}