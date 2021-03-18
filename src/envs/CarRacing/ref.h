#ifndef ENVS_CARRACING_REF
#define ENVS_CARRACING_REF

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#include "../../main.h"
#include "dynamics.h"

xt::xarray<int> xarray_from_nparray_int(const cnpy::NpyArray& arr);
xt::xarray<float> xarray_from_nparray_float(const cnpy::NpyArray& arr);
std::pair<std::map<std::string,size_t>,xt::xarray<float>> read_csv(const std::string& filename, int headers);

class RefDriver
{
	public:
		RefDriver(const std::string& track_name, float dt=0.01f);
		CarState state(float time);
		xt::xarray<float> state(const xt::xarray<float>& time) const;
		xt::xarray<float> get_time(const xt::xarray<float>& point) const;
		void load_point_map(const std::string& ref_name);

	public:
		std::string ref_name;
		std::map<std::string,size_t> ref_fields;
		xt::xarray<float> ref;
		xt::xarray<float> min_point;
		xt::xarray<float> max_point;
		xt::xarray<float> Xmap;
		xt::xarray<float> Ymap;
		xt::xarray<float> Smap;
		xt::xarray<int> time_map;
		float start_posx;
		float start_posy;
		float start_yaw;
		float start_vel;
		float max_time;
		float sres;
		float res;
		float dt;
		float ref_dt;
};

#endif // ENVS_CARRACING_REF
