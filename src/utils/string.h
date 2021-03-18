#ifndef UTILS_STRING
#define UTILS_STRING

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#ifdef _WIN32
static const char DELIM = '\\';
#else
static const char DELIM = '/';
#endif

std::vector<std::string> split(const std::string& s, char c);
std::string joinpath(const std::vector<std::string>& path);
std::string join(const std::string& root, const std::vector<std::string>& path);
std::string parent(const std::string& path);

#endif // UTILS_STRING
