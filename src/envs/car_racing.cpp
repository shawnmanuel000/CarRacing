#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>

#include "car_racing.h"

std::string extract_track_name(const std::string& env_name)
{
	std::vector<std::string> fields = split(env_name, '-');
	std::string track_name = fields.size() > 2 ? fields[1] : "sebring";
	return track_name;
}

CarRacingV1::CarRacingV1(const std::string& env_name, float max_time, float delta_t)
{
	this->track_name = extract_track_name(env_name);
	this->dynamics = new CarDynamics("bicycle");
	this->init_track(this->track_name, max_time, delta_t > 0.0f ? delta_t : DELTA_T);
}

CarRacingV1::~CarRacingV1()
{
	delete this->track;
	delete this->dynamics;
	if (this->viewer != nullptr)
	{
		delete this->viewer;
		this->viewer = nullptr;
	}
}

void CarRacingV1::init_track(const std::string& track_name, float max_time, float delta_t)
{
	int stride = std::strcmp(track_name.c_str(), "sebring")==0 ? 19 : 1;
	this->ref = new RefDriver(track_name);
	this->track = new Track(track_name, stride);
	this->delta_t = delta_t;
	this->max_time = max_time;
}

xt::xarray<float> CarRacingV1::reset(bool train, bool sample_track)
{
	this->time = 0;
	this->realtime = 0.0f;
	this->dynamics->reset(this->ref->state(0.0f));
	xt::xarray<float> state = this->dynamics->observation();
	return state;
}

void CarRacingV1::step(const xt::xarray<float>& action)
{
	this->time += 1;
	this->realtime = this->time * this->delta_t;
	this->dynamics->step(action, this->delta_t);
}

bool CarRacingV1::render()
{
	glm::vec3 car_pos = glm::vec3(this->dynamics->state.X, 0.0, -this->dynamics->state.Y);
	float yaw = this->dynamics->state.yaw;
	if (this->viewer == nullptr)
	{
		this->viewer = new Viewer("CarRacing", 640, 480, false, this->track, car_pos, yaw);
	}
	CarState state = this->ref->state(this->realtime);
	glm::vec3 ref_pos = glm::vec3(state.X, 0, -state.Y);
	this->viewer->set_ref(ref_pos, state.yaw);
	this->viewer->set_car(car_pos, yaw);
	if (this->viewer->follow)
	{
		this->viewer->update(this->dynamics->state, -M_PI/6);
	}
	else
	{
		this->viewer->update();
	}
	this->viewer->render();
	return this->viewer->getWindowShouldClose();
}

xt::xarray<float> CarRacingV1::get_viewer_action()
{
	xt::xarray<float> action = {0.0f, 0.0f};
	if (this->viewer != nullptr)
	{
		action[0] = -this->viewer->rightAction.x;
		action[1] = this->viewer->rightAction.z;
	}
	return action;
}
