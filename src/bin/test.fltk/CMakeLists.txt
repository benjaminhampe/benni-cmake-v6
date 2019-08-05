cmake_minimum_required(VERSION 3.1)

project(test.fltk)

set(TEST_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})
set(TEST_SOURCES ${TEST_SOURCE_DIR}/main.cpp)

message("${BoldRed}[benni] PROJECT_NAME: ${PROJECT_NAME}${ColourReset}")

add_executable(${PROJECT_NAME} "")
target_include_directories(${PROJECT_NAME} PRIVATE ${TEST_SOURCE_DIR})
target_sources(${PROJECT_NAME} PRIVATE ${TEST_SOURCES})
target_link_libraries(${PROJECT_NAME} PUBLIC fltk)