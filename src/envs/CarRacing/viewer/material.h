#ifndef MATERIAL
#define MATERIAL

#include "../../../main.h"
#include "shader.h"
#include "texture.h"

const glm::vec3 ROAD_COLOR = glm::vec3(100.0f/255, 100.0f/255, 100.0f/255);
const glm::vec3 GRASS_COLOR = glm::vec3(90.0f/255, 160.0f/255, 90.0f/255);

struct Material
{
	std::string name = "unnamed";		// Material Name
	glm::vec3 Ka = glm::vec3(0.0);		// Ambient Color
	glm::vec3 Kd = glm::vec3(0.0);		// Diffuse Color
	glm::vec3 Ks = glm::vec3(0.0);		// Specular Color
	int Ns = 35;						// Specular Exponent
	float d = 1;						// Dissolve
	int illum = 1;						// Illumination
	GLint diffuseTexId = 0;				// Diffuse Texture Map
	GLint specularTexId = 1;			// Specular Texture Map
	Texture* diffuseTex = nullptr;
	Texture* specularTex = nullptr;

	Material() {}

	Material(std::string name, glm::vec3 Kd, float Ns=35)
	{
		this->name = name;
		this->Kd = Kd;
		this->Ns = Ns;
	}

	Material(std::string name, Texture* diffuseTex=nullptr, Texture* specularTex=nullptr)
	{
		this->name = name;
		this->diffuseTex = diffuseTex;
		this->specularTex = specularTex;
	}

	Material(const Material& obj, glm::vec3 color_map=glm::vec3(0,1,2))
	{
		this->name = obj.name;
		this->Ka = obj.Ka;
		this->Kd = obj.Kd;
		this->Ks = obj.Ks;
		this->Ns = obj.Ns;
		this->d = obj.d;
		this->illum = obj.illum;
		this->diffuseTexId = obj.diffuseTexId;
		this->specularTexId = obj.specularTexId;
		this->diffuseTex = obj.diffuseTex;
		this->specularTex = obj.specularTex;
		this->map_color(color_map);
	}

	~Material(){}

	inline float aspect() { return this->diffuseTex ? this->diffuseTex->aspect() : 1; }

	void map_color(glm::vec3 color_map)
	{
		float x = color_map.x==0 ? this->Kd.x : color_map.x==1 ? this->Kd.y : this->Kd.z;
		float y = color_map.y==0 ? this->Kd.x : color_map.y==1 ? this->Kd.y : this->Kd.z;
		float z = color_map.z==0 ? this->Kd.x : color_map.z==1 ? this->Kd.y : this->Kd.z;
		this->Kd = glm::vec3(x,y,z);
	}

	void use(Shader* program)
	{
		program->setVec3f(this->Ka, "material.Ka");
		program->setVec3f(this->Kd, "material.Kd");
		program->setVec3f(this->Ks, "material.Ks");
		program->set1i(this->Ns, "material.Ns");
		program->set1f(this->d, "material.d");
		program->set1i(this->illum, "material.illum");
		program->set1i(this->diffuseTexId, "material.diffuseTex");
		program->set1i(this->specularTexId, "material.specularTex");
		if (this->diffuseTex) { this->diffuseTex->bind(this->diffuseTexId); }
		if (this->specularTex) { this->specularTex->bind(this->specularTexId); }
	}

	void unuse()
	{
		if (this->diffuseTex) { this->diffuseTex->unbind(); }
		if (this->specularTex) { this->specularTex->unbind(); }
	}
};

#endif