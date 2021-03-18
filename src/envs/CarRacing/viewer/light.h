#ifndef LIGHT
#define LIGHT

#include "../../../main.h"
#include "shader.h"

class Light
{
	protected:
		float intensity;

	public:
		Light(float intensity) : intensity(intensity) { }
		~Light() { }
		virtual void sendToShader(Shader* shader) = 0;
};

class PointLight : public Light
{
	protected:
		glm::vec3 position;
		float constant;
		float linear;
		float quadratic;

	public:
		PointLight(glm::vec3 position, float intensity=0.0f, float constant=1.0f, float linear=0.045f, float quadratic=0.00001f) : Light(intensity)
		{
			this->position = position;
			this->constant = constant;
			this->linear = linear;
			this->quadratic = quadratic;
		}

		~PointLight() {	}

		void setPosition(const glm::vec3 position)
		{
			this->position = position;
		}

		void sendToShader(Shader* shader)
		{
			shader->setVec3f(this->position, "pointLight.position");
			shader->set1f(this->intensity, "pointLight.intensity");
			shader->set1f(this->constant, "pointLight.constant");
			shader->set1f(this->linear, "pointLight.linear");
			shader->set1f(this->quadratic, "pointLight.quadratic");
		}
};

#endif