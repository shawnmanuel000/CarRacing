#ifndef GAME
#define GAME

#include "../../main.h"
#include "track.h"
#include "dynamics.h"
#include "viewer/vertex.h"
#include "viewer/primitives.h"
#include "viewer/shader.h"
#include "viewer/texture.h"
#include "viewer/material.h"
#include "viewer/mesh.h"
#include "viewer/model.h"
#include "viewer/light.h"
#include "viewer/camera.h"
#include "viewer/objLoader.h"

enum shader_enum { SHADER_CORE_PROGRAM=0 };
enum texture_enum { TEX_SKY=0, TEX_GRASS, TEX_ROAD };
class Track;

class Viewer
{
	public:
		int fbWidth;
		int fbHeight;
		GLFWwindow* window = NULL;
		Track* track = NULL;
		Camera camera;
		
		float dt;
		float curTime;
		float simTime;
		float startTime;

		double lastMouseX;
		double lastMouseY;
		double mouseX;
		double mouseY;
		double mouseOffsetX;
		double mouseOffsetY;
		bool firstMouse;
		bool follow = true;		
		glm::vec3 leftAction = glm::vec3(0.0f);
		glm::vec3 rightAction = glm::vec3(0.0f);

		size_t shader_id = 0;
		Model* car = NULL;
		Model* ref = NULL;
		std::vector<Shader*> shaders;
		std::vector<Texture*> textures;
		std::vector<Model*> models;
		std::vector<PointLight*> pointLights;

		void initWindow(const char* title, float width, float height, bool resizable);
		void initShaders();
		void initTextures();
		void initModels(Track* track);
		void initLights();

	public:
		Viewer(const char* title, int width, int height, bool resizable, Track* track=NULL, glm::vec3 pos=glm::vec3(), float heading=M_PI/2);
		virtual ~Viewer();
		int getWindowShouldClose();
		void setWindowShouldClose();
		void updateDt();
		void updateMouseInput();
		void updateKeyboardInput();
		void updateInput();
		void update();
		void update(CarState state, float pitch=0.0);
		void render();
		void set_ref(glm::vec3 pos, float heading);
		void set_car(glm::vec3 pos, float heading);
		
	static void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH);
};

#endif