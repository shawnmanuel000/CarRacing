#ifndef ENVS_CARRACING_DYNAMICS
#define ENVS_CARRACING_DYNAMICS

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#include "../../main.h"
#include "tires.h"

static const bool USE_DELTA = false;
static const float DELTA_T = 0.02f;
static const float TURN_SCALE = 0.05f;
static const float PEDAL_SCALE = 1.0f;
static const float TURN_LIMIT = 0.025f;
static const float PEDAL_LIMIT = 1.0f;

struct constants 
{
	static constexpr float m = 1370.0f; 			// Mass (kg)
	static constexpr float I_zz = 4453.0f; 			// Inertia (kg m^2)
	static constexpr float l_f = 1.293f; 			// Distance from CG to front axle (m)
	static constexpr float l_r = 1.475f; 			// Distance from CG to rear axle (m)
	static constexpr float steer_ratio = 17.85f; 	// Steer ratio
	static constexpr float F_ZFStatic = 3577.0f; 	// F Static tire normal force (N)
	static constexpr float F_ZRStatic = 3136.0f; 	// R Static tire normal force (N)
	static constexpr float rho = 1.205f; 			// Air density (kg/m^3)
	static constexpr float SA = 2.229f; 			// Surface area (m^2)
	static constexpr float C_LF = 0.392f; 			// Coefficient of front down force
	static constexpr float C_LR = 0.918f; 			// Coefficient of rear down force
	static constexpr float C_D = 0.6f; 				// Coefficient of drag
	static constexpr float C_alpha_f = 312631.0f; 	// Front tire cornering stiffness (N/rad)
	static constexpr float C_alpha_r = 219079.0f; 	// Rear tire cornering stiffness (N/rad)
	static constexpr float mu_f = 1.612f; 			// Front tire friction
	static constexpr float mu_r = 1.587f; 			// Rear tire friction
	static constexpr float Mz = 0.0f;				// Tire aligning torque (Nm)
	static constexpr float F_RR = 0.0f;				// Tire rolling resistance force (N)
	static constexpr float F_YAero = 0.0f;			// Aero side force (N)
	static constexpr float M_ZAero = 0.0f;			// Aero yaw moment (Nm)
};

struct CarState
{
	float X = 0.0f;									// Car X position (m)
	float Y = 0.0f;									// Car Y position (m)
	float yaw = 0.0f;								// Car heading from +ve x (rad)
	float Vx = 0.0f;								// Longitudinal car velocity (m/s)
	float Vy = 0.0f;								// Lateral car velocity (m/s)
	float S = 0.0f;									// Distance travelled (km)
	float yaw_dot = 0.0f;			 				// Yaw velocity (rad/s) 
	float steer = 0.0f;								// Steer angle (rad)
	float pedals = 0.0f;							// Throttle/brake (ratio, 0->1 is throttle & -1-> 0 is brake)			
	std::map<std::string,float> info;				// Non essential debugging info

	CarState(float X=0.0f, float Y=0.0f, float yaw=0.0f, float Vx=0.0f, float Vy=0.0f, float S=0.0f, float yaw_dot=0.0f, float steer=0.0f, float pedals=0.0f, std::map<std::string,float> info=std::map<std::string,float>());
	xt::xarray<float> observation();
	void print();
};

class CarDynamics
{
	public:
		CarDynamics(const std::string& tire_name);
		void reset(CarState init_state);
		void step(const xt::xarray<float>& action, float dt=DELTA_T, bool use_delta=USE_DELTA, int integration_steps=1);
		xt::xarray<float> observation();

	public:
		std::string tire_name;
		BicycleTireModel tire_model;
		CarState state;
		float turn_scale;
		float pedal_scale;
		float turn_limit;
		float pedal_limit;
};

#endif // ENVS_CARRACING_DYNAMICS
