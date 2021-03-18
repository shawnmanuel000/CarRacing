#ifndef OBJLOADER
#define OBJLOADER

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <set>

#include "../../../main.h"
#include "vertex.h"
#include "material.h"
#include "mesh.h"

static bool parse_mtl(const std::string& filename, std::map<std::string, Material>& mtl_map)
{
	std::ifstream in_file = std::ifstream(filename);
	if (!in_file.is_open())
	{
		std::cout << "ERROR::OBJLOADER::Could not open file." << std::endl;
		return false;
	}
	std::stringstream ss;
	std::string line = "";
	std::string prefix = "";
	std::string curr_name = "";
	glm::vec3 temp_vec3;
	glm::vec2 temp_vec2;
	GLfloat temp_float;
	GLint temp_int;
	while (std::getline(in_file, line))
	{
		ss.clear();
		ss.str(line);
		ss>> prefix;
		if (!line.empty() && line[line.size()-1]=='\r') line.erase(line.size()-1);

		if (prefix == "newmtl")
		{
			curr_name = line.substr(7, line.size()-7);
			mtl_map[curr_name] = Material(curr_name);
		}
		if (prefix == "Ka" || prefix == "Kd" || prefix == "Ks")
		{
			ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
			if (prefix == "Ka") { mtl_map[curr_name].Ka = temp_vec3; }
			if (prefix == "Kd") { mtl_map[curr_name].Kd = temp_vec3; }
			if (prefix == "Ks") { mtl_map[curr_name].Ks = temp_vec3; }
		}
		if (prefix == "Ns" || prefix == "d")
		{
			ss >> temp_float;
			if (prefix == "Ns") { mtl_map[curr_name].Ns = temp_float; }
			if (prefix == "d") { mtl_map[curr_name].d = temp_float; }
		}
		if (prefix == "illum")
		{
			ss >> temp_int;
			mtl_map[curr_name].illum = temp_int;
		}
	}
	return true;
}

static std::vector<Mesh> loadOBJMesh(const std::string& filename, Material default_material=Material("default"), bool invNorm=false)
{
	std::vector<Vertex> vertices;
	std::ifstream in_file = std::ifstream(filename);
	if (!in_file.is_open())
	{
		std::cout << "ERROR::OBJLOADER::Could not open file." << std::endl;
		throw std::exception();
	}

	//Vertex portions
	std::vector<glm::fvec3> vertex_positions;
	std::vector<glm::fvec2> vertex_texcoords;
	std::vector<glm::fvec3> vertex_normals;

	//Face vectors
	std::vector<GLint> vertex_position_indicies;
	std::vector<GLint> vertex_texcoord_indicies;
	std::vector<GLint> vertex_normal_indicies;
	std::vector<std::string> vertex_color_mtls;

	//Vertex array
	std::string curr_obj = "";
	std::string curr_mtl = default_material.name;
	std::set<std::string> obj_names = {curr_obj};
	std::set<std::string> mtl_names = {curr_mtl};
	std::map<std::string, Material> mtl_map = {{curr_mtl, default_material}};
	std::map<std::string, std::map<std::string, Mesh>> obj_map;
	std::map<std::string, std::map<std::string, std::vector<GLint>>> obj_index_map;

	std::stringstream ss;
	std::string line = "";
	glm::vec3 temp_vec3;
	glm::vec2 temp_vec2;
	GLint temp_glint = 0;
	int counter = 0;
	while (std::getline(in_file, line))
	{
		std::string prefix = "";
		counter++;
		ss.clear();
		ss.str(line);
		ss >> prefix;
		if (!line.empty() && line[line.size()-1]=='\r') { line.erase(line.size()-1); }

		if (prefix == "mtllib")
		{
			std::string mtllib;
			ss >> mtllib;
			std::string mtl_file = join(parent(filename), {mtllib});
			parse_mtl(mtl_file, mtl_map);
		}
		if (prefix == "o")
		{
			curr_obj = line.substr(2, line.size()-2);
			obj_names.insert(curr_obj);
		}
		if (prefix == "usemtl")
		{
			curr_mtl = line.substr(7, line.size()-7);
			mtl_names.insert(curr_mtl);
		}
		if (prefix == "s")
		{
		}
		if (prefix == "v")
		{
			ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
			vertex_positions.push_back(temp_vec3);
		}
		else if (prefix == "vt")
		{
			ss >> temp_vec2.x >> temp_vec2.y;
			vertex_texcoords.push_back(temp_vec2);
		}
		else if (prefix == "vn")
		{
			ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
			vertex_normals.push_back(temp_vec3);
		}
		else if (prefix == "f")
		{			
			int itype = 0;
			int vcount = 0;
			size_t last = vertex_position_indicies.size();
			while (ss >> temp_glint)
			{
				if (itype == 0)
				{
					vertex_color_mtls.push_back(curr_mtl);
					vertex_position_indicies.push_back(temp_glint);
					if (obj_index_map.count(curr_obj) == 0) { obj_index_map[curr_obj] = std::map<std::string, std::vector<GLint>>(); }
					if (obj_index_map[curr_obj].count(curr_mtl) == 0) { obj_index_map[curr_obj][curr_mtl] = std::vector<GLint>(); }
					obj_index_map[curr_obj][curr_mtl].push_back(vertex_position_indicies.size()-1);
				}
				else if (itype == 1)
				{
					vertex_texcoord_indicies.push_back(temp_glint);
				}
				else if (itype == 2)
				{
					vertex_normal_indicies.push_back(temp_glint);
				}

				if (ss.peek() == '/')
				{
					++itype;
					ss.ignore(1, '/');
				}
				else if (ss.peek() == ' ')
				{
					itype = 0;
					vcount++;
					ss.ignore(1, ' ');
				}
			}	
			if (vertex_position_indicies.size()-last > 3)
			{
				obj_index_map[curr_obj][curr_mtl].push_back(last);
				obj_index_map[curr_obj][curr_mtl].push_back(last+2);
			}
		}
		else if (prefix == "#<<")
		{
			std::string next_file;
			ss >> next_file;
			line = "";
			prefix = "";
			in_file.close();
			in_file = std::ifstream(join(parent(filename), {next_file}));
			if (!in_file.is_open())
			{
				std::cout << "ERROR::OBJLOADER::Could not open file." << std::endl;
				throw std::exception();
			}
		}
	}

	// For each vertex of each triangle
    for (unsigned int i=0; i<vertex_position_indicies.size(); i++)
	{
		glm::vec3 position = vertex_positions[vertex_position_indicies[i]-1];
		glm::vec2 texcoord = vertex_texcoords[vertex_texcoord_indicies[i]-1];
		glm::vec3 normal = vertex_normals[vertex_normal_indicies[i]-1];
		glm::vec3 color = vertex_color_mtls[i]!="" ? mtl_map[vertex_color_mtls[i]].Kd : default_material.Kd;
		Vertex vertex = {position, texcoord, normal, color};
		vertices.push_back(vertex);
	}

	std::vector<Mesh> meshes;
	for (std::string obj_name : obj_names)
	{
		if (obj_index_map.count(obj_name) == 0) continue;
		// obj_map[obj_name] = std::map<std::string, Mesh>();
		for (std::string mtl_name : mtl_names)
		{
			if (obj_index_map[obj_name].count(mtl_name) == 0) continue;
			std::vector<Vertex> verts;
			for (GLint index : obj_index_map[obj_name][mtl_name])
			{
				int tri_ind = index % 3;
				int vert_ind = invNorm ? 2-tri_ind : tri_ind;
				verts.push_back(vertices[index-tri_ind+vert_ind]);
			}
			Mesh mesh = Mesh(verts.data(), verts.size(), NULL, 0, mtl_map[mtl_name], obj_name+"_"+mtl_name, invNorm);
			meshes.push_back(mesh);
			// obj_map[obj_name][mtl_name] = mesh;
		}
	}

	//DEBUG
	std::cout << "Nr of vertices: " << vertices.size() << "\n";

	//Loaded success
	std::cout << "OBJ file loaded!" << "\n";
	return meshes;
}


#endif