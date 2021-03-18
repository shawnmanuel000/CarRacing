#ifndef MODEL
#define MODEL

#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "material.h"
#include "objLoader.h"

class Model
{
	private:
		std::map<std::string, Mesh*> meshes;
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 rotation = glm::vec3(0.0f);
		glm::vec3 scale = glm::vec3(1.0f);
		glm::vec3 origin = glm::vec3(0.0f);
		glm::mat4 ModelMatrix;

		void updateUniforms(Shader* shader)
		{
			shader->setMat4fv(this->ModelMatrix, "ModelMatrix");
		}

		void updateModelMatrix()
		{
			this->ModelMatrix = glm::mat4(1.f);
			this->ModelMatrix = glm::translate(this->ModelMatrix, this->position-this->origin);
			this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.x), glm::vec3(1.f, 0.f, 0.f));
			this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.y), glm::vec3(0.f, 1.f, 0.f));
			this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.z), glm::vec3(0.f, 0.f, 1.f));
			this->ModelMatrix = glm::translate(this->ModelMatrix, this->origin);
			this->ModelMatrix = glm::scale(this->ModelMatrix, this->scale);
		}

	public:
		Model(std::vector<Mesh>& meshes, glm::vec3 position=glm::vec3(0.0f), glm::vec3 rotation=glm::vec3(0.0f), glm::vec3 scale=glm::vec3(1.0f), glm::vec3 color_map=glm::vec3(0,1,2))
		{
			this->position = position;
			this->rotation = rotation;
			this->scale = scale;
			for (Mesh mesh : meshes)
			{
				Mesh* new_mesh = new Mesh(mesh, color_map);
				this->meshes[new_mesh->getName()] = new_mesh;
			}
		}

		~Model()
		{
			for (auto& i : this->meshes)
			{
				delete i.second;
			}
		}

		inline void setPosition(const glm::vec3 position) { this->position = position; }
		inline void setRotation(const glm::vec3 rotation) { this->rotation = rotation; }
		inline void setScale(const glm::vec3 scale) { this->scale = scale; }
		inline void setOrigin(const glm::vec3 origin) { this->origin = origin; }
		inline void move(const glm::vec3 position) { this->position += position; }
		inline void rotate(const glm::vec3 rotation) { this->rotation += rotation; }
		inline void scaleUp(const glm::vec3 scale) { this->scale += scale; }

		void render(Shader* shader)
		{
			this->updateModelMatrix();
			this->updateUniforms(shader);
			shader->use();
			for (auto& i : this->meshes)
			{   
				i.second->render(shader); //Activates shader also
			}
		}
};

#endif