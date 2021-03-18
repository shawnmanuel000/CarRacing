#include "tires.h"

float BicycleTireModel::calc_F_Aero(float Vx, float Vy, float C, float rho, float SA)
{
	return (0.5*rho) * (Vx*Vx + Vy*Vy) * SA * C;
}

float BicycleTireModel::calc_Fz(float F_ZStatic, float F_Z_Aero)
{
	return F_ZStatic + 0.5*F_Z_Aero;
}

float BicycleTireModel::calc_Fy(float alpha, float mu, float C_alpha, float F_Z)
{
	float Fy = std::abs(alpha) < (3*mu*F_Z/C_alpha) 
		? C_alpha*std::tan(alpha) - C_alpha*C_alpha/(3*mu*F_Z)*std::abs(std::tan(alpha))*std::tan(alpha) + std::pow(C_alpha,3)/(27*std::pow(mu*F_Z,2))*(std::pow(std::tan(alpha),3))
		: mu*F_Z*std::abs(alpha) / alpha;
	return Fy;
}

float BicycleTireModel::calc_Fx(float mu, float F_Y, float F_Z)
{
	return F_Z * std::abs(std::sqrt(std::max(std::pow(mu,2) - std::pow(F_Y/F_Z,2), 0.0)));
}