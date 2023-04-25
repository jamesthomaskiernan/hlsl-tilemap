cmake_minimum_required(VERSION 3.5.1)
project (tilemap)
set (CMAKE_CXX_STANDARD 17)

add_executable (tilemap

# core
../src/main.cpp
../src/application.cpp
../src/window.cpp
../src/input.cpp
../src/graphics.cpp
../src/debug.cpp
../src/camera.cpp
../src/camera_controller.cpp
../src/texture.cpp
../src/timer.cpp
../src/tilemap.cpp

# shaders
../src/vertex_shader.cpp
../src/pixel_shader.cpp
../src/compute_shader.cpp

# buffers
../src/vertex_buffer.cpp
../src/const_buffer.cpp
../src/struct_buffer.cpp
)

# include project files
target_include_directories(${PROJECT_NAME} PUBLIC "../include/")

# include dependencies
target_include_directories(${PROJECT_NAME} PUBLIC "../dependencies/directx_math/include")
target_include_directories(${PROJECT_NAME} PUBLIC "../dependencies/simple_math/include")
target_include_directories(${PROJECT_NAME} PUBLIC "../dependencies/stb_image")

# link d3d11 libraries
target_link_libraries(${PROJECT_NAME}
    d3d11.lib d3dcompiler.lib libucrt.lib
)