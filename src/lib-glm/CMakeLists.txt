cmake_minimum_required(VERSION 3.7)

project(benni-lib-glm)

set(GLM_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/include")

file(GLOB_RECURSE GLM_HEADERS "${GLM_INCLUDE_DIR}/glm/*.hpp")
file(GLOB_RECURSE GLM_SOURCES "${GLM_INCLUDE_DIR}/glm/*.cpp")
set(GLM_HEADERS "${GLM_HEADERS} ${GLM_INCLUDE_DIR}/ostreamGLM.hpp")

message("===============================================")
message("${BoldMagenta}PROJECT_NAME: ${PROJECT_NAME}${ColourReset}")
message("===============================================")
message("CMAKE_CURRENT_LIST_DIR = ${CMAKE_CURRENT_LIST_DIR}")
message("GLM_INCLUDE_DIR = ${GLM_INCLUDE_DIR}")

# add_library(glm STATIC "")
# target_compile_definitions(glm PUBLIC -DUSE_LIB_GLM=1) # Now we can check in C++ code if library is available
# target_sources(glm PRIVATE ${GLM_SOURCES} PUBLIC ${GLM_HEADERS})
# target_include_directories(glm PUBLIC ${GLM_INCLUDE_DIR})

add_library(glm INTERFACE)
target_compile_definitions(glm INTERFACE -DUSE_LIB_GLM=1) # Now we can check in C++ code if library is available
# target_sources(glm PRIVATE ${GLM_SOURCES} PUBLIC ${GLM_HEADERS})
target_include_directories(glm INTERFACE ${GLM_INCLUDE_DIR})
