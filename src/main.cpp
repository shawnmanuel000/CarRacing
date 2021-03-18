#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include "main.h"
#include "envs/CarRacing/track.h"
#include "envs/CarRacing/viewer.h"
#include "envs/car_racing.h"

int main(void)
{
	xt::print_options::set_line_width(2000);
	xt::print_options::set_precision(3);
	CarRacingV1 env = CarRacingV1("CarRacing-curve-v1", 100);
	xt::xarray<float> state = env.reset();
	bool done = false;
	while (!done)
	{
		xt::xarray<float> action = env.get_viewer_action();
		env.step(action);
		done = env.render();
	}
	return EXIT_SUCCESS;
}