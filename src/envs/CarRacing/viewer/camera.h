#ifndef CAMERA
#define CAMERA

#include <iostream>
#include "../../../main.h"
#include "shader.h"

enum direction {FORWARD=0, BACKWARD, LEFT, RIGHT, UP, DOWN};

class Camera
{
	private:
		glm::mat4 ProjectionMatrix;
		glm::mat4 ViewMatrix;
		glm::mat3 world2cam;
		glm::vec3 worldUp;
		glm::vec3 position;
		glm::vec3 front;
		glm::vec3 right;
		glm::vec3 up;

		GLfloat movementSpeed;
		GLfloat sensitivity;
		GLfloat pitch = 0.0;
		GLfloat yaw = 0.0;
		GLfloat roll = 0.0;

		float fov;
		float nearPlane;
		float farPlane;

		void updateCameraVectors()
		{
			this->front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
			this->front.y = sin(glm::radians(this->pitch));
			this->front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
			this->front = glm::normalize(this->front);
			this->right = glm::normalize(glm::cross(this->front, this->worldUp));
			this->up = glm::normalize(glm::cross(this->right, this->front));
			this->world2cam = glm::mat3(this->right, this->up, this->front);
		}

	public:
		Camera(glm::vec3 position=glm::vec3(0.0f), float yaw=0.0f)
		{
			this->movementSpeed = 100.f;
			this->sensitivity = 5.f;
			this->worldUp = glm::vec3(0.f, 1.f, 0.f);
			this->position = position;
			this->yaw = -glm::degrees(yaw);
			this->fov = 90.f;
			this->nearPlane = 0.1f;
			this->farPlane = 2000.f;
			this->updateCameraVectors();
		}

		const glm::mat4 getProjectionMatrix(float aspect) { return glm::perspective(glm::radians(this->fov), aspect, this->nearPlane, this->farPlane); }
		const glm::mat4 getViewMatrix() { return glm::lookAt(this->position, this->position + this->front, this->up); }
		const glm::vec3 getPosition() const { return this->position; }
		const float getHeading() const { return -this->yaw; }
		inline void addMovementSpeed(float amount) { this->movementSpeed = std::max(this->movementSpeed+amount, 0.0f); }

		void move(const float& dt, const glm::vec3 directions)
		{
			glm::vec3 cam_directions = this->world2cam * directions;
			this->position += cam_directions * this->movementSpeed * dt;
		}

		void updateInput(const float& dt, const int direction, const double& offsetX, const double& offsetY)
		{
			this->pitch = clamp(this->pitch + static_cast<GLfloat>(offsetY) * this->sensitivity * dt, 80.0f);
			this->yaw = std::fmod(this->yaw + static_cast<GLfloat>(offsetX) * this->sensitivity * dt, 360.0);
			this->updateCameraVectors();
		}

		void setPosition(glm::vec3 pos, float yaw, float pitch)
		{
			this->position = pos;
			this->yaw = -glm::degrees(yaw);
			this->pitch = glm::degrees(pitch);
			this->updateCameraVectors();
		}

		void setLookat(glm::vec3 pos, glm::vec3 at)
		{
			this->position = pos;
			this->front = at-pos;
		}

		void sendToShader(Shader *shader, float aspect)
		{
			this->ViewMatrix = this->getViewMatrix();
			this->ProjectionMatrix = this->getProjectionMatrix(aspect);

			shader->setMat4fv(this->ViewMatrix, "ViewMatrix");
			shader->setMat4fv(this->ProjectionMatrix, "ProjectionMatrix");
			shader->setVec3f(this->getPosition(), "cameraPos");
		}

};

#endif