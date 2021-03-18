#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <utility>
#include <regex>
#include <map>

#include "xarray.h"
#include "string.h"

float clamp(float val, float limit)
{
	return std::clamp(val, -limit, limit);
}

std::pair<std::map<std::string,size_t>,xt::xarray<float>> read_csv(const std::string& filename, int headers)
{
	std::ifstream stream = std::ifstream(filename);
	std::vector<std::vector<std::string>> headerList;
	std::vector<std::vector<std::string>> dataList;
	std::string line;
	while (getline(stream, line)) 
	{
		std::vector<std::string> fields = split(line, ',');
		if (headerList.size() < headers)
		{
			headerList.push_back(fields);
		}
		else
		{
			dataList.push_back(fields);
		}
    }
	long ncols = headerList[0].size();
	xt::xarray<float> data = xt::zeros<float>({dataList.size()*ncols});
	std::vector<std::string> columns = headerList[0];
	std::map<std::string,size_t> header_map;
	for (size_t j = 0; j < ncols; j++)
	{
		header_map[columns[j]] = j;
	}
	for (size_t i = 0; i < dataList.size(); i++)
	{
		for (size_t j = 0; j < ncols; j++)
		{
			data[i*ncols + j] = atof(dataList[i][j].c_str());
		}
	}
	data.reshape({dataList.size(), (unsigned long)ncols});
	std::pair<std::map<std::string,size_t>,xt::xarray<float>> pair = std::make_pair(header_map, data);
	return pair;
}

xt::xarray<float> xarray_from_csv(const std::string& filename)
{
	std::ifstream stream = std::ifstream(filename);
	std::vector<std::vector<std::string>> dataList;
	std::string line;
	long ncols = -1;
	while (getline(stream, line)) 
	{
		std::vector<std::string> fields = split(line, ',');
		int new_ncols = fields.size();
		if (ncols != -1 && new_ncols != ncols)
		{
			throw "Unequal number of columns";
		}
		ncols = new_ncols;
		dataList.push_back(fields);
    }
	xt::xarray<float> data = xt::zeros<float>({dataList.size()*ncols});
	for (size_t i = 0; i < dataList.size(); i++)
	{
		for (size_t j = 0; j < ncols; j++)
		{
			data[i*ncols + j] = atof(dataList[i][j].c_str());
		}
	}
	data.reshape({dataList.size(), (unsigned long)ncols});
	return data;
}

xt::xarray<float> xarray_from_nparray_float(const cnpy::NpyArray& arr)
{
	xt::xarray<float> xarray = xt::zeros<float>({arr.num_vals});
	xt::xarray<int> shape = xt::zeros<int>({arr.shape.size()});
	if (arr.word_size == 4)
	{
		const float* data = arr.data<const float>();
		for (size_t i = 0; i < arr.num_vals; i++)
		{
			xarray[i] = data[i];
		}
	}
	else if (arr.word_size == 8)
	{
		const double* data = arr.data<const double>();
		for (size_t i = 0; i < arr.num_vals; i++)
		{
			double entry = data[i];
			xarray[i] = (float)entry;
		}
	}
	else 
	{
		throw "Invalid value size";
	}
	for (size_t i = 0; i < arr.shape.size(); i++)
	{
		shape[i] = arr.shape[i];
	}	
	xarray.reshape(shape);
	return xarray;
}

xt::xarray<int> xarray_from_nparray_int(const cnpy::NpyArray& arr)
{
	xt::xarray<int> xarray = xt::zeros<int>({arr.num_vals});
	xt::xarray<int> shape = xt::zeros<int>({arr.shape.size()});
	if (arr.word_size == 4)
	{
		const int* data = arr.data<const int>();
		for (size_t i = 0; i < arr.num_vals; i++)
		{
			xarray[i] = data[i];
		}
	}
	else if (arr.word_size == 8)
	{
		const long* data = arr.data<const long>();
		for (size_t i = 0; i < arr.num_vals; i++)
		{
			xarray[i] = (int)data[i];
		}
	}
	else 
	{
		throw "Invalid value size";
	}
	for (size_t i = 0; i < arr.shape.size(); i++)
	{
		shape[i] = arr.shape[i];
	}	
	xarray.reshape(shape);
	return xarray;
}