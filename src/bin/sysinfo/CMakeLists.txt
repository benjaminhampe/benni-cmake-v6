cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

project(sysinfo)

# message("===============================================")
message("${BoldBlue}PROGRAM_NAME: ${PROJECT_NAME}${ColourReset}")
# message("===============================================")
# message("CMAKE_CURRENT_LIST_DIR = ${CMAKE_CURRENT_LIST_DIR}")

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC de.hampe.common)