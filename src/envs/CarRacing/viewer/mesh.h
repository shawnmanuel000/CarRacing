#ifndef MESH
#define MESH

#include <iostream>
#include <vector>

#include "../../../main.h"
#include "vertex.h"
#include "primitives.h"
#include "shader.h"
#include "material.h"

class Mesh
{
	private:
		std::string name = "";
		Vertex* vertexArray;
		GLuint* indexArray;
		unsigned nV;
		unsigned nI;
		GLuint VAO;
		GLuint VBO;
		GLuint EBO;
		Material material;

		void initVAO()
		{
			glGenVertexArrays(1, &this->VAO);
			glBindVertexArray(this->VAO);

			glGenBuffers(1, &this->VBO);
			glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
			glBufferData(GL_ARRAY_BUFFER, this->nV * sizeof(Vertex), this->vertexArray, GL_STATIC_DRAW);

			if (this->nI > 0)
			{
				glGenBuffers(1, &this->EBO);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->nI * sizeof(GLuint), this->indexArray, GL_STATIC_DRAW);
			}

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
			glEnableVertexAttribArray(3);

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		void initMesh(Vertex* vertexArray, const unsigned& nV, GLuint* indexArray, const unsigned& nI, Material material, std::string name, bool invNorm=false)
		{
			this->name = name;
			this->nV = nV;
			this->nI = nI;
			this->vertexArray = new Vertex[this->nV];
			float wN = invNorm ? -1.0 : 1.0;
			for (size_t i = 0; i < nV; i++)
			{
				Vertex v = vertexArray[i];
				glm::vec3 newNorm = glm::vec3(wN*v.normal.x, wN*v.normal.y, wN*v.normal.z);
				this->vertexArray[i] = {v.position, v.texcoord, newNorm, v.color};
			}
			this->indexArray = new GLuint[this->nI];
			for (size_t i = 0; i < nI; i++)
			{
				this->indexArray[i] = indexArray[i];
			}
			this->material = material;
			this->initVAO();
		}

	public:
		Mesh() {}

		Mesh(Vertex* vertexArray, const unsigned& nV, GLuint* indexArray=NULL, const unsigned& nI=0, Material material=Material(), std::string name="", bool invNorm=false)
		{
			this->initMesh(vertexArray, nV, indexArray, nI, material, name, invNorm);
		}

		Mesh(Primitive* primitive, Material material=Material(), std::string name="")
		{
			this->initMesh(primitive->getVertices(), primitive->getNrOfVertices(), primitive->getIndices(), primitive->getNrOfIndices(), material, name);
		}

		Mesh(const Mesh& obj, glm::vec3 color_map=glm::vec3(0,1,2))
		{
			Material mapped = Material(obj.material, color_map);
			this->initMesh(obj.vertexArray, obj.nV, obj.indexArray, obj.nI, mapped, obj.name);
		}

		~Mesh()
		{
			glDeleteVertexArrays(1, &this->VAO);
			glDeleteBuffers(1, &this->VBO);
			if (this->nI > 0)
			{
				glDeleteBuffers(1, &this->EBO);
			}
			delete[] this->vertexArray;
			delete[] this->indexArray;
		}

		inline std::string getName() { return this->name; }

		void render(Shader* shader)
		{
			this->material.use(shader);
			shader->use();
			glBindVertexArray(this->VAO);
			if (this->nI == 0)
			{
				glDrawArrays(GL_TRIANGLES, 0, this->nV);
			}
			else
			{
				glDrawElements(GL_TRIANGLES, this->nI, GL_UNSIGNED_INT, 0);
			}
			glBindVertexArray(0);
			shader->unuse();
			this->material.unuse();
		}
};





#endif