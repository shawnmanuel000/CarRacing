#ifndef UTILS_XARRAY
#define UTILS_XARRAY

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include "../../deps/cnpy/cnpy.h"
#include "../../deps/xtensor/xtensor.hpp"
#include "../../deps/xtensor/include/xtensor/xnpy.hpp"

float clamp(float val, float limit);
xt::xarray<float> xarray_from_csv(const std::string& filename);
xt::xarray<float> xarray_from_nparray_float(const cnpy::NpyArray& arr);
xt::xarray<int> xarray_from_nparray_int(const cnpy::NpyArray& arr);
std::pair<std::map<std::string,size_t>,xt::xarray<float>> read_csv(const std::string& filename, int headers);

#endif // UTILS_XARRAY

