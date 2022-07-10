cmake_minimum_required(VERSION 3.22)
project(FishEye)

set(CMAKE_CXX_STANDARD 23)

add_executable(FishEye main.cpp Particle.cpp Particle.h)
target_link_libraries(FishEye SDL2 SDL2_image)