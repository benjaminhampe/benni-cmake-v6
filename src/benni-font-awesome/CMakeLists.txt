cmake_minimum_required(VERSION 3.7)

project(ext-lib-fontawesome)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE "${CMAKE_CURRENT_LIST_DIR}")
# target_sources(${PROJECT_NAME} INTERFACE "${CMAKE_CURRENT_LIST_DIR}/FontAwesome.hpp")

message("PROJECT_NAME = ${PROJECT_NAME}")
message("CMAKE_CURRENT_LIST_DIR = ${CMAKE_CURRENT_LIST_DIR}")
message("HOST_ADDRESS_SIZE = ${HOST_ADDRESS_SIZE}")
