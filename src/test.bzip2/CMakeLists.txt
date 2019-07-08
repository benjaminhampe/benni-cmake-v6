cmake_minimum_required(VERSION 3.0)

project(test.bzip2 C)

message("${BoldRed}[benni] PROJECT_NAME = ${PROJECT_NAME}${ColourReset}")

add_executable(${PROJECT_NAME} test.c)
set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
target_link_libraries(${PROJECT_NAME} bzip2)