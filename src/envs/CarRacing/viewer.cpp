#include "viewer.h"
#ifdef __APPLE__
const int GL_VERSION_MAJOR = 3;
const int GL_VERSION_MINOR = 2;
const int GLSL_VERSION_MAJOR = 1;
const int GLSL_VERSION_MINOR = 5;
#else //linux
const int GL_VERSION_MAJOR = 4;
const int GL_VERSION_MINOR = 4;
const int GLSL_VERSION_MAJOR = 4;
const int GLSL_VERSION_MINOR = 4;
#endif

const std::string ROOT = parent(__FILE__);
const std::string MEDIA_DIR = join(ROOT, {"viewer","media"});

std::string get_abs_path(std::string filename)
{
	return join(parent(__FILE__), std::vector<std::string>{filename});
}

void Viewer::initWindow(const char* title, float width, float height, bool resizable)
{
	if (!glfwInit())
	{
		std::cout << "ERROR::GLFW_INIT_FAILED" << "\n";
		glfwTerminate();
	}

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VERSION_MINOR);
	glfwWindowHint(GLFW_RESIZABLE, resizable);

	this->window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (this->window == nullptr)
	{
		std::cout << "ERROR::GLFW_WINDOW_INIT_FAILED" << "\n";
		glfwTerminate();
	}

	glfwGetFramebufferSize(this->window, &this->fbWidth, &this->fbHeight);
	glfwSetFramebufferSizeCallback(this->window, Viewer::framebuffer_resize_callback);
	glViewport(0, 0, this->fbWidth, this->fbHeight);
	glfwMakeContextCurrent(this->window);
	glfwSwapInterval(1);
	
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "ERROR::MAIN.CPP::GLEW_INIT_FAILED" << "\n";
		glfwTerminate();
	}
	
	glFrontFace(GL_CCW);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPolygonMode(GL_FRONT, GL_FILL);
	glEnable(GL_POLYGON_STIPPLE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Viewer::initShaders()
{
	std::string vertex = join(MEDIA_DIR, {"shaders","vertex_core.glsl"});
	std::string fragment = join(MEDIA_DIR, {"shaders","fragment_core.glsl"});
	std::string fragment_spec = join(MEDIA_DIR, {"shaders","fragment_core_spec.glsl"});
	this->shaders.push_back(new Shader(GLSL_VERSION_MAJOR, GLSL_VERSION_MINOR, vertex, fragment_spec));
	this->shaders.push_back(new Shader(GLSL_VERSION_MAJOR, GLSL_VERSION_MINOR, vertex, fragment));
}

void Viewer::initTextures()
{
	this->textures.push_back(new Texture(join(MEDIA_DIR, {"textures","sky.png"}), GL_TEXTURE_2D));
	this->textures.push_back(new Texture(join(MEDIA_DIR, {"textures","grassy.png"}), GL_TEXTURE_2D));
	this->textures.push_back(new Texture(join(MEDIA_DIR, {"textures","lanegrey.png"}), GL_TEXTURE_2D, true));
}

void Viewer::initModels(Track* track)
{
	Material grass_material = Material("grass", this->textures[TEX_GRASS]);
	Material road_material = Material("road", this->textures[TEX_ROAD]);
	std::vector<Mesh> skies = loadOBJMesh(join(MEDIA_DIR, {"models","sphere.obj"}), Material("sky", this->textures[TEX_SKY]), true);
	std::vector<Mesh> cars = loadOBJMesh(join(MEDIA_DIR, {"models","supra1.obj"}));
	std::vector<Mesh> grounds;
	std::vector<Mesh> roads;
	if (track != nullptr)
	{
		float xll = this->track->min_point[0];
		float yll = this->track->min_point[1];
		float xtr = this->track->max_point[0];
		float ytr = this->track->max_point[1];
		float area = (xtr-xll)*(ytr-yll);
		float x0 = 0.5*(xll + xtr);
		float y0 = 0.5*(yll + ytr);
		float scale = 1.2f * std::sqrt(1.4e6 / area);
		xll = (xll - x0) * scale + x0;
		yll = (yll - y0) * scale + y0;
		xtr = (xtr - x0) * scale + x0;
		ytr = (ytr - y0) * scale + y0;
		Tile grass = Tile(xll, yll, xtr, yll, xll, ytr, xtr, ytr, glm::vec4(0,75,0,75));
		grounds.push_back(Mesh(&grass, grass_material));
		
		std::vector<Vertex> road_vertices;
		std::vector<GLuint> road_indices;
		float acc_len = 0.0;
		for (size_t j = 0; j < track->boundaries.shape(0)-1; j++)
		{
			size_t k = (j+1) % track->boundaries.shape(0);
			float xl1 = this->track->boundaries(j,0,0);
			float yl1 = this->track->boundaries(j,0,1);
			float xr1 = this->track->boundaries(j,1,0);
			float yr1 = this->track->boundaries(j,1,1);
			float xl2 = this->track->boundaries(k,0,0);
			float yl2 = this->track->boundaries(k,0,1);
			float xr2 = this->track->boundaries(k,1,0);
			float yr2 = this->track->boundaries(k,1,1);
			float blen = std::sqrt(std::pow(xr1-xl1,2) + std::pow(yr1-yl1,2));
			float tlen = std::sqrt(std::pow(xr2-xl2,2) + std::pow(yr2-yl2,2));
			float llen = std::sqrt(std::pow(xl2-xl1,2) + std::pow(yl2-yl1,2));
			float rlen = std::sqrt(std::pow(xr2-xr1,2) + std::pow(yr2-yr1,2));
			float aspect = road_material.aspect();
			float len = 0.5*aspect * (llen + rlen) / (blen + tlen);
			Tile segment = Tile(xl1, yl1, xr1, yr1, xl2, yl2, xr2, yr2, glm::vec4(0,1,acc_len,acc_len+len));
			for (GLuint i : segment.getIndicesVector()) { road_indices.push_back(i+road_vertices.size()); }
			for (Vertex v : segment.getVerticesVector()) { road_vertices.push_back(v); }
			acc_len += len;
		}
		roads.push_back(Mesh(road_vertices.data(), road_vertices.size(), road_indices.data(), road_indices.size(), road_material));
	}
	this->car = new Model(cars, glm::vec3(0), glm::vec3(0), glm::vec3(0.5), glm::vec3(0,1,2));
	this->ref = new Model(cars, glm::vec3(0), glm::vec3(0), glm::vec3(0.5), glm::vec3(2,1,0));
	this->models.push_back(new Model(skies, glm::vec3(0), glm::vec3(0), glm::vec3(60)));
	this->models.push_back(new Model(grounds, glm::vec3(0,-1,0), glm::vec3(0), glm::vec3(1)));
	this->models.push_back(new Model(roads, glm::vec3(0), glm::vec3(0), glm::vec3(1)));
}

void Viewer::initLights()
{
	this->pointLights.push_back(new PointLight(glm::vec3(0.f, 1000.0f, 0.f), 10000.0));
}

Viewer::Viewer(const char* title, int width, int height, bool resizable, Track* track, glm::vec3 pos, float heading)
{
	this->camera = Camera(pos + glm::vec3(0,15,0), heading);
	this->track = track;
	this->initWindow(title, width, height, resizable);
	this->initShaders();
	this->initTextures();
	this->initModels(track);
	this->initLights();

	this->dt = 0.f;
	this->curTime = 0.f;
	this->simTime = 0.f;
	this->startTime = static_cast<float>(glfwGetTime());

	this->lastMouseX = 0.0;
	this->lastMouseY = 0.0;
	this->mouseX = 0.0;
	this->mouseY = 0.0;
	this->mouseOffsetX = 0.0;
	this->mouseOffsetY = 0.0;
	this->firstMouse = true;
}

Viewer::~Viewer()
{
	glfwDestroyWindow(this->window);
	glfwTerminate();

	if (this->car) { delete this->car; }
	if (this->ref) { delete this->ref; }

	for (size_t i = 0; i < this->shaders.size(); i++)
		delete this->shaders[i];
	
	for (size_t i = 0; i < this->textures.size(); i++)
		delete this->textures[i];

	for (auto*& i : this->models)
		delete i;

	for (size_t i = 0; i < this->pointLights.size(); i++)
		delete this->pointLights[i];
}

int Viewer::getWindowShouldClose()
{
	return glfwWindowShouldClose(this->window);
}

void Viewer::setWindowShouldClose()
{
	glfwSetWindowShouldClose(this->window, GLFW_TRUE);
}

void Viewer::updateMouseInput()
{
	glfwGetCursorPos(this->window, &this->mouseX, &this->mouseY);

	if (this->firstMouse)
	{
		this->lastMouseX = this->mouseX;
		this->lastMouseY = this->mouseY;
		this->firstMouse = false;
	}

	this->mouseOffsetX = this->mouseX - this->lastMouseX;
	this->mouseOffsetY = this->lastMouseY - this->mouseY;

	this->lastMouseX = this->mouseX;
	this->lastMouseY = this->mouseY;

	if (glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
	{
	}
}

void Viewer::updateKeyboardInput()
{
	if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(this->window, GLFW_TRUE);
	}
	this->leftAction = glm::vec3(0.0f);
	this->rightAction = glm::vec3(0.0f);
	if (glfwGetKey(this->window, GLFW_KEY_1) == GLFW_PRESS)    	{ this->shader_id = 0; } 
	if (glfwGetKey(this->window, GLFW_KEY_2) == GLFW_PRESS)    	{ this->shader_id = 1; } 
	if (glfwGetKey(this->window, GLFW_KEY_EQUAL) == GLFW_PRESS) { this->camera.addMovementSpeed(1.0); } 
	if (glfwGetKey(this->window, GLFW_KEY_MINUS) == GLFW_PRESS) { this->camera.addMovementSpeed(-1.0); } 
	if (glfwGetKey(this->window, GLFW_KEY_UP) == GLFW_PRESS)    { this->rightAction.z += 1.0; } 
	if (glfwGetKey(this->window, GLFW_KEY_DOWN) == GLFW_PRESS)  { this->rightAction.z -= 1.0; } 
	if (glfwGetKey(this->window, GLFW_KEY_LEFT) == GLFW_PRESS)  { this->rightAction.x -= 1.0; } 
	if (glfwGetKey(this->window, GLFW_KEY_RIGHT) == GLFW_PRESS) { this->rightAction.x += 1.0; } 
	if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS)     { this->leftAction.z += 1.0; } 
	if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS)     { this->leftAction.z -= 1.0; } 
	if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS)     { this->leftAction.x -= 1.0; } 
	if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS)     { this->leftAction.x += 1.0; } 
	if (glfwGetKey(this->window, GLFW_KEY_C) == GLFW_PRESS)     { this->leftAction.y += 1.0; } 
	if (glfwGetKey(this->window, GLFW_KEY_SPACE) == GLFW_PRESS) { this->leftAction.y -= 1.0; } 
	if (glfwGetKey(this->window, GLFW_KEY_F) == GLFW_PRESS) 	{ this->follow = true; } 
	if (glfwGetKey(this->window, GLFW_KEY_G) == GLFW_PRESS) 	{ this->follow = false; } 
}

void Viewer::updateDt()
{
	this->curTime = static_cast<float>(glfwGetTime());
	this->dt = -this->simTime;
	this->simTime = this->curTime - this->startTime;
	this->dt += this->simTime;
}

void Viewer::updateInput()
{
	glfwPollEvents();

	this->updateKeyboardInput();
	this->updateMouseInput();
}

void Viewer::update()
{
	this->updateDt();
	this->updateInput();
	this->camera.move(this->dt, this->leftAction);
	this->camera.updateInput(this->dt, -1, this->mouseOffsetX, this->mouseOffsetY);
}

void Viewer::update(CarState state, float pitch)
{
	this->updateDt();
	this->updateInput();
	glm::vec3 pos = glm::vec3(state.X, 0.0, -state.Y);
	float lerp = 0.1;
	float lateral = clamp(state.Vy, 0.25);
	float yaw = (1.0-lerp)*glm::radians(this->camera.getHeading()) + lerp*state.yaw;
	glm::vec4 offset_dirn = glm::vec4(-5,5,0,1);
	glm::vec3 old_pos = this->camera.getPosition();
	glm::mat4 offset_rotation = glm::rotate(glm::mat4(1.0), state.yaw, glm::vec3(0,1,0));
	glm::vec3 cam_offset = offset_rotation * offset_dirn;
	glm::vec3 new_pos = glm::vec3(1.0-lerp)*old_pos + glm::vec3(lerp)*(pos+cam_offset);
	this->camera.setPosition(new_pos, yaw, pitch);
	// this->camera.setLookat(new_pos, pos);
}

void Viewer::set_ref(glm::vec3 pos, float heading)
{
	if (this->ref)
	{
		this->ref->setRotation(glm::vec3(0,glm::degrees(heading)+90,0));
		this->ref->setPosition(pos);
	}
}

void Viewer::set_car(glm::vec3 pos, float heading)
{
	if (this->car)
	{
		this->car->setRotation(glm::vec3(0,glm::degrees(heading)+90,0));
		this->car->setPosition(pos);
	}
}

xt::xarray<uint8_t> get_pixels(size_t fbHeight, size_t fbWidth)
{
	size_t depth = 3;
	GLubyte framebuffer[fbHeight*fbWidth*depth];
	GLubyte flippedbuffer[fbHeight*fbWidth*depth];
	std::vector<size_t> shape = {fbHeight, fbWidth, depth};
	xt::xarray<uint8_t> image = xt::zeros<uint8_t>(shape);
	glReadPixels(0, 0, fbWidth, fbHeight, GL_RGB, GL_UNSIGNED_BYTE, framebuffer);
	for (size_t i = 0; i < fbHeight; i++)
	{
		for (size_t j = 0; j < fbWidth; j++)
		{
			for (size_t k = 0; k < depth; k++)
			{
				image(i,j,k) = framebuffer[depth*((fbHeight-1-i)*fbWidth + j) + k];
				flippedbuffer[depth*(i*fbWidth + j) + k] = image(i,j,k);
			}
		}
	}
	cnpy::npy_save("image.npy", &flippedbuffer[0], shape);
	return image;
}

void Viewer::render()
{
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glfwGetFramebufferSize(this->window, &this->fbWidth, &this->fbHeight);

	size_t shader_idx = this->shader_id < this->shaders.size() ? this->shader_id : SHADER_CORE_PROGRAM;
	Shader* shader = this->shaders[shader_idx];
	float aspect = static_cast<float>(this->fbWidth) / this->fbHeight;
	this->camera.sendToShader(shader, aspect);
	if (this->car) { this->car->render(shader); }
	if (this->ref) { this->ref->render(shader); }
	for (auto&i : this->models)
	{
		i->render(shader);
	}

	for(PointLight* pl : this->pointLights)
	{
		pl->sendToShader(shader);
	}

	// xt::xarray<uint8_t> image = get_pixels(fbHeight, fbWidth);
	glfwSwapBuffers(window);
	glFlush();

	glBindVertexArray(0);
	glUseProgram(0);
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Viewer::framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH)
{
	glViewport(0, 0, fbW, fbH);
};


