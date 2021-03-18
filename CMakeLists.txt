cmake_minimum_required(VERSION 3.18.4)

project(CarRacing)

# set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(CARRACING_SOURCE
	src/envs/CarRacing/viewer.cpp
	src/envs/CarRacing/track.cpp
	src/envs/CarRacing/ref.cpp
	src/envs/CarRacing/tires.cpp
	src/envs/CarRacing/dynamics.cpp
    src/envs/car_racing.cpp
	src/utils/lodepng.cpp
	src/utils/xarray.cpp
	src/utils/string.cpp
	src/main.cpp
	src/test.cpp
)

set(CARRACING_HEADER
	src/envs/CarRacing/viewer/camera.h
	src/envs/CarRacing/viewer/material.h
	src/envs/CarRacing/viewer/mesh.h
	src/envs/CarRacing/viewer/model.h
	src/envs/CarRacing/viewer/objLoader.h
	src/envs/CarRacing/viewer/primitives.h
	src/envs/CarRacing/viewer/shader.h
	src/envs/CarRacing/viewer/texture.h
	src/envs/CarRacing/viewer/vertex.h
	src/envs/CarRacing/viewer.h
    src/envs/CarRacing/track.h
    src/envs/CarRacing/ref.h
    src/envs/CarRacing/tires.h
    src/envs/CarRacing/dynamics.h
    src/envs/car_racing.h
	src/utils/lodepng.h
	src/utils/xarray.h
	src/utils/string.h
)

add_executable(CarRacing ${CARRACING_SOURCE} ${CARRACING_HEADER})

find_package(OpenGL REQUIRED)

add_subdirectory("./deps/glew")
include_directories("./deps/glew/include")
target_link_libraries(CarRacing glew ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})

add_subdirectory("${CarRacing_SOURCE_DIR}/deps/glfw")
include_directories("${CarRacing_SOURCE_DIR}/deps/glfw/include")
target_link_libraries(CarRacing glfw ${GLFW_LIBRARIES})

add_subdirectory("${CarRacing_SOURCE_DIR}/deps/glm")
include_directories("${CarRacing_SOURCE_DIR}/deps/glm/glm")
target_link_libraries(CarRacing glm ${GLM_LIBRARIES})

add_subdirectory("${CarRacing_SOURCE_DIR}/deps/cnpy")
target_link_libraries(CarRacing cnpy ${CNPY_LIBRARIES})

add_subdirectory("${CarRacing_SOURCE_DIR}/deps/xtl")
include_directories("${CarRacing_SOURCE_DIR}/deps/xtl/include")
target_link_libraries(CarRacing xtl ${XTL_LIBRARIES})

add_subdirectory("${CarRacing_SOURCE_DIR}/deps/xtensor")
include_directories("${CarRacing_SOURCE_DIR}/deps/xtensor/include")
target_link_libraries(CarRacing xtensor ${XTENSOR_LIBRARIES})

