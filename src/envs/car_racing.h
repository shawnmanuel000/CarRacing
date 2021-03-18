#ifndef ENVS_CARRACING
#define ENVS_CARRACING

#include <stdlib.h>
#include <iostream>
#include <string>

#include "../main.h"
#include "CarRacing/track.h"
#include "CarRacing/viewer.h"
#include "CarRacing/ref.h"
#include "CarRacing/dynamics.h"

class Track;
class Viewer;
class RefDriver;
class CarDynamics;
struct CarState;

class CarRacingV1
{
	public:
		CarRacingV1(const std::string& env_name, float max_time=-1.0f, float delta_t=-1.0f);
		~CarRacingV1();
		void init_track(const std::string& track_name, float max_time, float delta_t);
		xt::xarray<float> reset(bool train=true, bool sample_track=false);
		xt::xarray<float> get_viewer_action();
		void step(const xt::xarray<float>& action);
		bool render();

	public:
		std::string track_name;
		Track* track = nullptr;
		Viewer* viewer = nullptr;
		RefDriver* ref = nullptr;
		CarDynamics* dynamics = nullptr;
		float delta_t;
		float max_time;
		int time;
		float realtime;
		bool done;
		xt::xarray<float> action;
		CarState state_spec;
};

#endif // ENVS_CARRACING