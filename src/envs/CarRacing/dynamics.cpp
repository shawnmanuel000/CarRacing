#include "dynamics.h"

CarState::CarState(float X, float Y, float yaw, float Vx, float Vy, float S, float yaw_dot, float steer, float pedals, std::map<std::string,float> info)
{
	this->X = X;
	this->Y = Y;
	this->yaw = yaw;
	this->Vx = Vx;
	this->Vy = Vy;
	this->S = S;
	this->yaw_dot = yaw_dot;
	this->steer = steer;
	this->pedals = pedals;
	this->info = info;
}

xt::xarray<float> CarState::observation()
{
	return xt::xarray<float>({this->X, this->Y, this->yaw, this->Vx, this->Vy, this->S, this->yaw_dot, this->steer, this->pedals});
}

void CarState::print()
{
	std::cout << "X: " << X << ", Y: " << Y << ", yaw: " << yaw << ", Vx: " << Vx << ", Vy: " << Vy << ", S: " << S << ", steer: " << steer << ", pedals: " << pedals << std::endl;
}

CarDynamics::CarDynamics(const std::string& tire_name)
{
	this->tire_name = tire_name;
}

void CarDynamics::reset(CarState init_state)
{
	this->state = init_state;
	this->turn_scale = TURN_SCALE;
	this->pedal_scale = PEDAL_SCALE;
	this->turn_limit = TURN_LIMIT;
	this->pedal_limit = PEDAL_LIMIT;
}

void CarDynamics::step(const xt::xarray<float>& action, float dt, bool use_delta, int integration_steps)
{
	float turn_rate = action[0];
	float pedal_rate = action[1];
	dt = dt/integration_steps;
	CarState state = this->state;
	float turn_limit = this->turn_limit*std::pow(50.0/state.Vx,2);

	for (size_t i = 0; i < integration_steps; i++)
	{
		float F_ZF_Aero = this->tire_model.calc_F_Aero(state.Vx, state.Vy, constants::C_LF, constants::rho, constants::SA);
		float F_ZR_Aero = this->tire_model.calc_F_Aero(state.Vx, state.Vy, constants::C_LR, constants::rho, constants::SA);
		float F_X_Aero = this->tire_model.calc_F_Aero(state.Vx, state.Vy, constants::C_D, constants::rho, constants::SA);
		float Fy_scale = std::min(std::abs(state.Vx), 1.0f);

		float steer = use_delta ? clamp(state.steer + clamp(turn_rate*this->turn_scale-state.steer, turn_limit)*dt, turn_limit) : turn_rate*turn_limit;
		float alpha_f = -std::atan2((state.Vy + constants::l_f * state.yaw_dot),state.Vx) + steer;
		float alpha_r = -std::atan2((state.Vy - constants::l_r * state.yaw_dot),state.Vx) + 0.0;
		
		float pedals = use_delta ? clamp(state.pedals + clamp(pedal_rate*this->pedal_scale-state.pedals, this->pedal_limit)*dt, PEDAL_LIMIT) : pedal_rate*this->pedal_limit;
		float acc = std::max(pedals, 0.0f);
		float accel = std::max(-std::pow(acc,3)*10523.0f + std::pow(acc,2)*12394.0f + (acc)*1920.0f, 0.0);
		float brake = std::min(pedals, 0.0f) * 22500.0f * (state.Vx > 0.0f);

		float FzF = this->tire_model.calc_Fz(constants::F_ZFStatic, F_ZF_Aero);
		float FzR = this->tire_model.calc_Fz(constants::F_ZRStatic, F_ZR_Aero);
		float FyF = this->tire_model.calc_Fy(alpha_f, constants::mu_f, constants::C_alpha_f, FzF) * Fy_scale;
		float FyR = this->tire_model.calc_Fy(alpha_r, constants::mu_r, constants::C_alpha_r, FzR) * Fy_scale;
		float FxF = clamp(brake*0.6f, this->tire_model.calc_Fx(constants::mu_f, FyF, FzF));
		float FxR = clamp(accel+brake*0.4f, this->tire_model.calc_Fx(constants::mu_r, FyR, FzR));
		
		float yaw_ddot = (1.0/constants::I_zz) * ((2*FxF * std::sin(steer) + 2*FyF * std::cos(steer)) * constants::l_f - 2*FyR * constants::l_r);
		float Vx_dot = (1.0/constants::m) * (2*FxF * std::cos(steer) - 2*FyF * std::sin(steer) + 2*FxR - F_X_Aero) + state.yaw_dot * state.Vy;
		float Vy_dot = (1.0/constants::m) * (2*FyF * std::cos(steer) + 2*FxF * std::sin(steer) + 2*FyR) - state.yaw_dot * state.Vx;
		
		float yaw_dot = state.yaw_dot + yaw_ddot * dt;
		float Vx = state.Vx + Vx_dot * dt;
		float Vy = state.Vy + Vy_dot * dt;
		float V = std::sqrt(Vx*Vx + Vy*Vy);
		
		float beta = std::atan2(Vy,Vx);
		float yaw = (state.yaw + yaw_dot * dt);
		float X = state.X + (Vx * std::cos(yaw) - Vy * std::sin(yaw)) * dt;
		float Y = state.Y + (Vy * std::cos(yaw) + Vx * std::sin(yaw)) * dt;
		float S = (state.S + V * dt);

		std::map<std::string,float> info = {{"FxF",FxF},{"FyF",FyF},{"FzF",FzF},{"yaw_ddot",yaw_ddot},{"Vx_dot",Vx_dot},{"Vy_dot",Vy_dot}};
		state = CarState(X, Y, yaw, Vx, Vy, S, yaw_dot, steer, pedals, info);
	}
	this->state = state;
}

xt::xarray<float> CarDynamics::observation()
{
	return this->state.observation();
}