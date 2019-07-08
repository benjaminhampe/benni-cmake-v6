cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

project(test.glm)

message("${BoldRed}[benni] PROJECT_NAME = ${PROJECT_NAME}${ColourReset}")

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} glm)