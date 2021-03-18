#ifndef PRIMITIVES
#define PRIMITIVES

#include <vector>

#include "../../../main.h"
#include "vertex.h"

class Primitive
{
protected:
	std::vector<Vertex> vertices = std::vector<Vertex>();
	std::vector<GLuint> indices = std::vector<GLuint>();

public:
	Primitive() {}
	virtual ~Primitive() {}

	std::vector<Vertex> getVerticesVector() { return this->vertices; }
	std::vector<GLuint> getIndicesVector() { return this->indices; }
	inline Vertex* getVertices() { return this->vertices.data(); }
	inline GLuint* getIndices() { return this->indices.data(); }
	inline const unsigned getNrOfVertices() { return this->vertices.size(); }
	inline const unsigned getNrOfIndices() { return this->indices.size(); }
};

class Triangle : public Primitive
{
public:
	Triangle() : Primitive()
	{
		this->vertices =
		{
			//Position						//Texcoords				//Normals						//Color						
			{glm::vec3( 0.0f,  0.5f, 0.0f),	glm::vec2(0.5f, 1.0f),	glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec3(1.0f, 0.0f, 0.0f)},	
			{glm::vec3(-0.5f, -0.5f, 0.0f),	glm::vec2(0.0f, 0.0f),	glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec3(0.0f, 1.0f, 0.0f)},	
			{glm::vec3( 0.5f, -0.5f, 0.0f),	glm::vec2(1.0f, 0.0f),	glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec3(0.0f, 0.0f, 1.0f)},	
		};
		this->indices =
		{
			0, 1, 2	//Triangle 1
		};
	}
};

class Quad : public Primitive
{
public:
	Quad(glm::vec3 color=glm::vec3(0.0f, 0.0f, 0.0f)) : Primitive()
	{
		this->vertices =
		{
			//Position						//Texcoords				//Normals						//Color			
			{glm::vec3(-0.5f,  0.5f, 0.0f),	glm::vec2(0.0f, 1.0f),	glm::vec3(0.0f, 0.0f, 1.0f),	color},			
			{glm::vec3(-0.5f, -0.5f, 0.0f),	glm::vec2(0.0f, 0.0f),	glm::vec3(0.0f, 0.0f, 1.0f),	color},			
			{glm::vec3( 0.5f, -0.5f, 0.0f),	glm::vec2(1.0f, 0.0f),	glm::vec3(0.0f, 0.0f, 1.0f),	color},			
			{glm::vec3( 0.5f,  0.5f, 0.0f),	glm::vec2(1.0f, 1.0f),	glm::vec3(0.0f, 0.0f, 1.0f),	color},			
		};
		this->indices =
		{
			0, 1, 2,	//Triangle 1
			0, 2, 3		//Triangle 2
		};
	}
};

class Tile : public Primitive
{
public:
	Tile(float xl1=-0.5, float yl1=-0.5, float xr1=0.5, float yr1=-0.5, float xl2=-0.5, float yl2=0.5, float xr2=0.5, float yr2=0.5, glm::vec4 tex=glm::vec4(0,1,0,1), glm::vec3 color=glm::vec3(1.0)) : Primitive()
	{
		this->vertices =
		{
			//Position						//Texcoords					//Normals						//Color	
			{glm::vec3(xl1, 0.0f, -yl1),	glm::vec2(tex.x, tex.z),	glm::vec3(0.0f, 1.0f, 0.0f),	color},	
			{glm::vec3(xl2, 0.0f, -yl2),	glm::vec2(tex.x, tex.w),	glm::vec3(0.0f, 1.0f, 0.0f),	color},	
			{glm::vec3(xr1, 0.0f, -yr1),	glm::vec2(tex.y, tex.z),	glm::vec3(0.0f, 1.0f, 0.0f),	color},	
			{glm::vec3(xr2, 0.0f, -yr2),	glm::vec2(tex.y, tex.w),	glm::vec3(0.0f, 1.0f, 0.0f),	color},	
		};
		this->indices =
		{
			1, 0, 2,	//Triangle 1
			1, 2, 3		//Triangle 2
		};
	}
};

class Pyramid : public Primitive
{
public:
	Pyramid() : Primitive()
	{
		this->vertices =
		{
			//Position						//Texcoords				//Normals					//Color					
			//Triangle front
			{glm::vec3( 0.0f, 0.5f, 0.0f),	glm::vec2(0.5f, 1.0f),	glm::vec3(0.0f, 0.0f, 1.0f), 	glm::vec3(0.0f, 0.0f, 0.0f)},
			{glm::vec3(-0.5f,-0.5f, 0.5f),	glm::vec2(0.0f, 0.0f),	glm::vec3(0.0f, 0.0f, 1.0f), 	glm::vec3(0.0f, 0.0f, 0.0f)},
			{glm::vec3( 0.5f,-0.5f, 0.5f),	glm::vec2(1.0f, 0.0f),	glm::vec3(0.0f, 0.0f, 1.0f), 	glm::vec3(0.0f, 0.0f, 0.0f)},

			//Triangle left
			{glm::vec3( 0.0f, 0.5f, 0.0f),	glm::vec2(0.5f, 1.0f),	glm::vec3(-1.0f, 0.0f, 0.0f),	glm::vec3(0.0f, 0.0f, 0.0f)},
			{glm::vec3(-0.5f,-0.5f,-0.5f),	glm::vec2(0.0f, 0.0f),	glm::vec3(-1.0f, 0.0f, 0.0f),	glm::vec3(0.0f, 0.0f, 0.0f)},
			{glm::vec3(-0.5f,-0.5f, 0.5f),	glm::vec2(1.0f, 0.0f),	glm::vec3(-1.0f, 0.0f, 0.0f),	glm::vec3(0.0f, 0.0f, 0.0f)},

			//Triangle back
			{glm::vec3( 0.0f, 0.5f, 0.0f),	glm::vec2(0.5f, 1.0f),	glm::vec3(0.0f, 0.0f, -1.0f),	glm::vec3(0.0f, 0.0f, 0.0f)},
			{glm::vec3( 0.5f,-0.5f,-0.5f),	glm::vec2(0.0f, 0.0f),	glm::vec3(0.0f, 0.0f, -1.0f),	glm::vec3(0.0f, 0.0f, 0.0f)},
			{glm::vec3(-0.5f,-0.5f,-0.5f),	glm::vec2(1.0f, 0.0f),	glm::vec3(0.0f, 0.0f, -1.0f),	glm::vec3(0.0f, 0.0f, 0.0f)},

			//Triangles right
			{glm::vec3( 0.0f, 0.5f, 0.0f),	glm::vec2(0.5f, 1.0f),	glm::vec3(1.0f, 0.0f, 0.0f), 	glm::vec3(0.0f, 0.0f, 0.0f)},
			{glm::vec3( 0.5f,-0.5f, 0.5f),	glm::vec2(0.0f, 0.0f),	glm::vec3(1.0f, 0.0f, 0.0f), 	glm::vec3(0.0f, 0.0f, 0.0f)},
			{glm::vec3( 0.5f,-0.5f,-0.5f),	glm::vec2(1.0f, 0.0f),	glm::vec3(1.0f, 0.0f, 0.0f), 	glm::vec3(0.0f, 0.0f, 0.0f)},
		};
	}
};

class Cube : public Primitive
{
public:
	Cube() : Primitive()
	{
		this->vertices =
		{
			//Position						//Texcoords					//Normals						//Color					
			{glm::vec3(-0.5f, 0.5f, 0.5f),	glm::vec2(0.0f, 1.0f),		glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec3(1.0f, 0.0f, 0.0f)},
			{glm::vec3(-0.5f,-0.5f, 0.5f),	glm::vec2(0.0f, 0.0f),		glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec3(0.0f, 1.0f, 0.0f)},
			{glm::vec3( 0.5f,-0.5f, 0.5f),	glm::vec2(1.0f, 0.0f),		glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec3(0.0f, 0.0f, 1.0f)},
			{glm::vec3( 0.5f, 0.5f, 0.5f),	glm::vec2(1.0f, 1.0f),		glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec3(1.0f, 1.0f, 0.0f)},

			{glm::vec3( 0.5f, 0.5f,-0.5f),	glm::vec2(0.0f, 1.0f),		glm::vec3(0.0f, 0.0f,-1.0f),	glm::vec3(1.0f, 0.0f, 0.0f)},
			{glm::vec3( 0.5f,-0.5f,-0.5f),	glm::vec2(0.0f, 0.0f),		glm::vec3(0.0f, 0.0f,-1.0f),	glm::vec3(0.0f, 1.0f, 0.0f)},
			{glm::vec3(-0.5f,-0.5f,-0.5f),	glm::vec2(1.0f, 0.0f),		glm::vec3(0.0f, 0.0f,-1.0f),	glm::vec3(0.0f, 0.0f, 1.0f)},
			{glm::vec3(-0.5f, 0.5f,-0.5f),	glm::vec2(1.0f, 1.0f),		glm::vec3(0.0f, 0.0f,-1.0f),	glm::vec3(1.0f, 1.0f, 0.0f)},
		};
		this->indices =
		{
			0, 1, 2,
			0, 2, 3,

			7, 6, 1,
			7, 1, 0,

			4, 5, 6,
			4, 6, 7,

			3, 2, 5,
			3, 5, 4
		};
	}
};

#endif