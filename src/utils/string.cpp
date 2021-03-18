#include "string.h"

std::vector<std::string> split(const std::string& s, char c) 
{
	std::vector<std::string> fields;
	std::string::size_type i = 0;
	std::string::size_type j = s.find(c);
	while (j != std::string::npos) 
	{
		fields.push_back(s.substr(i, j-i));
		i = ++j;
		j = s.find(c, j);

		if (j == std::string::npos)
		{
			fields.push_back(s.substr(i, s.length()));
		}
	}
	return fields;
}

std::string joinpath(const std::vector<std::string>& path)
{
	std::string filepath = "";
	for (size_t i = 0; i<path.size(); i++)
	{
		if (i>0 && path[i] == "") continue;
		filepath += path[i];
		if (i<path.size()-1) filepath += DELIM;
	}
	return filepath;
}

std::string join(const std::string& root, const std::vector<std::string>& path)
{
	std::string filepath = "";
	std::vector<std::string> fields = split(root, DELIM);
	for (std::string s : path)
	{
		if (s == "..")
		{
			fields.pop_back();
		}
		else if (s != ".")
		{
			fields.push_back(s);
		}
	}
	return joinpath(fields);
}

std::string parent(const std::string& path)
{
	std::vector<std::string> fields = split(path, DELIM);
	fields.pop_back();
	return joinpath(fields);
}