#include "main.h"
#include "envs/CarRacing/track.h"
#include "envs/CarRacing/viewer.h"
#include "envs/car_racing.h"

int WIDTH = 160;
int HEIGHT = 120;
float scale = 0.02f;
float track_width = 15.0f;
int subsample_rate = 4.0f;
float f = 1.0f;
float theta_x = M_PI/4.0f;
float cam_width = 4.0f;
float cam_height = 3.0f;

xt::xarray<uint8_t> sky_color = xt::xarray<uint8_t>({135,206,235});
xt::xarray<uint8_t> road_color = xt::xarray<uint8_t>({100,100,100});
xt::xarray<uint8_t> grass_color = xt::xarray<uint8_t>({90,160,90});

int run()
{
	GLFWwindow* window;
	if (!glfwInit())
	{
		return EXIT_FAILURE;
	}

	window = glfwCreateWindow(HEIGHT, WIDTH, "Viewer", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	float frame = 0;
	float scale = 0.001;
	std::chrono::time_point<std::chrono::system_clock> sys_last = std::chrono::system_clock::now();
	std::chrono::time_point<std::chrono::system_clock> sys_curr = sys_last;
	for (size_t t = 0; t < 500; t++)
	{
		sys_curr = std::chrono::system_clock::now();
		double elapsed = ((std::chrono::duration<double>) (sys_curr - sys_last)).count();
		int width;
		int height;
		glfwGetFramebufferSize(window, &width, &height);
		uint8_t framebuffer[height*width*3];
		for (size_t i = 0; i < height; i++)
		{
			for (size_t j = 0; j < width; j++)
			{
				framebuffer[3*(j+i*width)+0] = (i+j) % 255;
				framebuffer[3*(j+i*width)+1] = i % 255;
				framebuffer[3*(j+i*width)+2] = j % 255;
			}
		}
		
		glClear(GL_COLOR_BUFFER_BIT);
		glPushAttrib(GL_VIEWPORT_BIT);
		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0, width, 0, height, 0, 0);
		glMatrixMode( GL_MODELVIEW );
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(-1, 1, 0);
		glRasterPos2f(0, 0);
		glPixelZoom(1.0, -1.0);
		glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, &framebuffer[0]);
		glPixelZoom(1.0, 1.0);
		glPopAttrib();
		glMatrixMode( GL_PROJECTION ); glPopMatrix();
		glMatrixMode( GL_MODELVIEW  ); glPopMatrix();

		glfwSwapBuffers(window);
		glfwPollEvents();
		frame++;
		if (elapsed >= 1.0f)
		{
			printf("%f\n", frame);
			frame = 0;
			sys_last = sys_curr; 
		}
		if (glfwWindowShouldClose(window))
		{
			break;
		}
		
	}

	glfwDestroyWindow(window);

	try
	{
		glfwTerminate();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
	return EXIT_SUCCESS;
}

void test_npz()
{
	std::string root = parent(__FILE__);
	std::string load = join(root, {"test.npz"});
	std::string trackpath = join(root, {"envs/CarRacing/spec/point_maps/curve.npz"});
	std::string point_map = std::regex_replace(trackpath, std::regex("c\\+\\+"), "python");
	std::cout << point_map << std::endl;
    cnpy::npz_t my_npz = cnpy::npz_load(load);
	std::cout << xarray_from_nparray_float(my_npz["xf64"]) << std::endl;
	std::cout << xarray_from_nparray_float(my_npz["xf32"]) << std::endl;
	std::cout << xarray_from_nparray_int(my_npz["xi64"]) << std::endl;
	std::cout << xarray_from_nparray_int(my_npz["xi32"]) << std::endl;
}

void test_track()
{
	Track track = Track("curve");
	std::cout << xt::view(track.idx_map, xt::range(0,5), xt::range(0,5)) << std::endl;
	std::cout << track.min_point << std::endl;
	xt::xarray<float> point = xt::xarray<float>({0.0,0.0});
	std::cout << track.get_nearest_index(point) << std::endl;
	return;
}

xt::xarray<float> cross(const xt::xarray<float>& a, xt::xarray<float> b)
{
	float x = a[1]*b[2] - a[2]*b[1];
	float y = a[0]*b[2] - a[2]*b[0];
	float z = a[0]*b[1] - a[1]*b[0];
	return xt::xarray<float>({x,-y,z}) / std::pow(x*x+y*y+z*z, 0.5);
}

xt::xarray<float> supersample(const xt::xarray<float>& arr, size_t height, size_t width)
{
	size_t old_height = arr.shape()[0];
	size_t old_width = arr.shape()[1];
	xt::xarray<float> new_arr = xt::zeros<float>({height, width});
	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			float u = (float)i / (height-1);
			float v = (float)j / (width-1);
			float x = std::min(std::max((float)0.5, v*old_width), (float)(old_width-0.5))-0.5;
  			float y = std::min(std::max((float)0.5, u*old_height), (float)(old_height-0.5))-0.5;
			int collow = std::floor(x);
			int colhigh = std::ceil(x);
			int rowlow = std::floor(y);
			int rowhigh = std::ceil(y);
			float lr = (x-collow);
  			float tb = (y-rowlow);
			float tl = arr(rowlow, collow);
			float tr = arr(rowlow, colhigh);
			float bl = arr(rowhigh, collow);
			float br = arr(rowhigh, colhigh);
			new_arr(i,j) = (tl*(1-lr) + tr*lr)*(1-tb) + (bl*(1-lr) + br*lr)*tb;
		}
	}
	return new_arr;
}

xt::xarray<float> map2d_to_3d(const xt::xarray<float>& arr, size_t depth=3)
{
	size_t height = arr.shape()[0];
	size_t width = arr.shape()[1];
	xt::xarray<float> new_arr = xt::broadcast(xt::expand_dims(arr, arr.dimension()), {height, width, depth});
	return new_arr;
}

void draw_broadcast(float x, float y, float z, float theta_z, float dx, float dy, float dz, uint8_t *framebuffer, int width, int height, const Track& track)
{
	int subwidth = width / subsample_rate;
	int subheight = height / subsample_rate;
	xt::xarray<float> is = xt::arange(0, subheight);
	xt::xarray<float> js = xt::arange(0, subwidth);
	auto [ii, jj, kk] = xt::meshgrid(is, js, xt::xarray<float>{1});
	xt::xarray<float> u = (jj + 0.5)/(float)subwidth;
	xt::xarray<float> v = (ii + 0.5)/(float)subheight;

	xt::xarray<float> up = xt::xarray<float>({0.0,0.0,1.0});
	xt::xarray<float> fpoint = xt::xarray<float>({x,y,z});
	xt::xarray<float> look_dir = xt::xarray<float>({dx,dy,dz});
	xt::xarray<float> lat = cross(look_dir, up);
	xt::xarray<float> lon = cross(look_dir, lat);
	xt::xarray<float> cam_mid = fpoint - f*look_dir;
	xt::xarray<float> cam_bl = cam_mid - cam_width/2.0*lat - cam_height/2.0*lon;

	xt::xarray<float> broadcast_lat = xt::view(lat, xt::newaxis(), xt::newaxis(), xt::all())*cam_width;
	xt::xarray<float> broadcast_lon = xt::view(lon, xt::newaxis(), xt::newaxis(), xt::all())*cam_height;
	xt::xarray<float> broadcast_cam_bl = xt::view(cam_bl, xt::newaxis(), xt::newaxis(), xt::all());
	xt::xarray<float> broadcast_fpoint = xt::view(fpoint, xt::newaxis(), xt::newaxis(), xt::all());
	xt::xarray<float> broadcast_image_bl = broadcast_fpoint - broadcast_cam_bl;
	xt::xarray<float> ray_dirn = broadcast_image_bl - u*broadcast_lat - v*broadcast_lon;
	xt::xarray<float> t_int = fpoint[2]/xt::view(ray_dirn, xt::all(), xt::all(), 2);
	xt::xarray<float> z_int = broadcast_fpoint - xt::expand_dims(t_int,t_int.dimension()) * ray_dirn;
	xt::xarray<float> points = xt::view(z_int, xt::all(), xt::all(), xt::range(0,2));

	xt::xarray<float> min_dist = track.get_nearest_distance(points);
	xt::xarray<float> min_dist_3d = map2d_to_3d(supersample(min_dist, height, width));
	xt::xarray<float> t_int_3d = map2d_to_3d(supersample(t_int, height, width));
	xt::xarray<uint8_t> image = xt::zeros<uint8_t>({height, width, 3});
	xt::masked_view(image, t_int_3d>0) = sky_color;
	xt::masked_view(image, t_int_3d<0 && min_dist_3d<=track_width) = road_color;
	xt::masked_view(image, t_int_3d<0 && min_dist_3d>track_width) = grass_color;

	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			framebuffer[3*(j+i*width)+0] = image(height-1-i,width-1-j,0);
			framebuffer[3*(j+i*width)+1] = image(height-1-i,width-1-j,1);
			framebuffer[3*(j+i*width)+2] = image(height-1-i,width-1-j,2);
		}
	}
}

void draw_glm(float x, float y, float z, float theta_z, float dx, float dy, float dz, uint8_t *framebuffer, int width, int height, const Track& track)
{
	float k = (width-1.0f) / cam_width;
	float l = (height-1.0f) / cam_height;
	float cx = (width-1.0f) / 2.0f;
	float cy = (height-1.0f) / 2.0f;
	float alpha = f * k;
	float beta = f * l;
	glm::mat3 K = glm::mat3(alpha, 0.0f, 0.0f, 0.0f, beta, 0.0f, cx, cy, 1.0f);
	glm::mat3 Kinv = glm::inverse(K);

	glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 fpoint = glm::vec3(x, y, z);
	glm::vec3 look_dir = glm::normalize(glm::vec3(dx, dy, dz));
	glm::vec3 lat = glm::normalize(glm::cross(look_dir, up));
	glm::vec3 lon = -glm::normalize(glm::cross(look_dir, lat));
	glm::vec3 t = -fpoint;
	glm::mat3 r = glm::transpose(glm::mat3(lat, lon, look_dir));
	glm::mat4 T = glm::translate(glm::mat4(1.0f), t);
	glm::mat4 R = glm::mat4(r);
	glm::mat4x3 M = K * glm::mat4x3(R * T);

	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			glm::vec3 pixel = glm::vec3(j, height-1-i, 1);
			glm::vec3 ray = glm::transpose(r) * Kinv * pixel;
			float t_int = t.z / ray.z;
			glm::vec3 z_int = fpoint + t_int * ray;
			xt::xarray<float> point = xt::xarray<float>({z_int.x, z_int.y});
			xt::xarray<float> min_dist = track.get_nearest_distance(point);
			xt::xarray<float> color = t_int<=0 ? sky_color : min_dist[0]<15 ? road_color : grass_color;
			framebuffer[3*(j+i*width)+0] = color[0];
			framebuffer[3*(j+i*width)+1] = color[1];
			framebuffer[3*(j+i*width)+2] = color[2];
		}
	}
}

void viz3d()
{
    if (!glfwInit())
	{
        return;
	}
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Viewer", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return;
    }

	int width;
	int height;
    glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwGetFramebufferSize(window, &width, &height);
	uint8_t framebuffer[height*width*3];
	
	Track track = Track("curve");

	float frame = 0;
	std::chrono::time_point<std::chrono::system_clock> sys_last = std::chrono::system_clock::now();
	std::chrono::time_point<std::chrono::system_clock> sys_curr = sys_last;
	for (size_t time = 0; time < 500; time++)
    {
		sys_curr = std::chrono::system_clock::now();
  		double elapsed = ((std::chrono::duration<double>) (sys_curr - sys_last)).count();

		float x = 1.5422f+125*(std::sin(time*scale)+1);
		float y = 2.2428f+125*std::sin(time*scale);
		float z = 15.0f;
		float theta_z = M_PI/2.0 - time*scale;
		float dx = std::cos(theta_z)*std::cos(theta_x);
		float dy = std::sin(theta_z)*std::cos(theta_x);
		float dz = -std::sin(theta_x);

		draw_broadcast(x, y, z, theta_z, dx, dy, dz, framebuffer, width, height, track);
		// draw_glm(x, y, z, theta_z, dx, dy, dz, framebuffer, width, height, track);
		
        glClear(GL_COLOR_BUFFER_BIT);
  		glPushAttrib(GL_VIEWPORT_BIT);
		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0, width, 0, height, 0, 0);
		glMatrixMode( GL_MODELVIEW );
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(-1, 1, 0);
		glRasterPos2f(0, 0);
		glPixelZoom(1.0, -1.0);
		glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, &framebuffer[0]);
		glPixelZoom(1.0, 1.0);
		glPopAttrib();
		glMatrixMode( GL_PROJECTION ); glPopMatrix();
		glMatrixMode( GL_MODELVIEW  ); glPopMatrix();

        glfwSwapBuffers(window);
        glfwPollEvents();
		frame++;
		if (elapsed >= 1.0f)
		{
			printf("%f\n", frame);
			sys_last = sys_curr; 
			frame = 0;
		}
    	if (glfwWindowShouldClose(window))
		{
			break;
		}
		
    }

	glfwDestroyWindow(window);

	try
	{
	    glfwTerminate();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
	return;
}