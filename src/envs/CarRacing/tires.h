#ifndef ENVS_CARRACING_TIRES
#define ENVS_CARRACING_TIRES

#include <stdlib.h>
#include <cmath>

#include "../../main.h"

struct BicycleTireModel
{
	float calc_F_Aero(float Vx, float Vy, float C, float rho, float SA);
	float calc_Fz(float F_ZStatic, float F_Z_Aero);
	float calc_Fy(float alpha, float mu, float C_alpha, float F_Z);
	float calc_Fx(float mu, float F_Y, float F_Z);
};

struct TRDTireModel : BicycleTireModel
{
	float calc_Fy(float alpha, float mu, float C_alpha, float F_Z);
};

#endif // ENVS_CARRACING_TIRES
