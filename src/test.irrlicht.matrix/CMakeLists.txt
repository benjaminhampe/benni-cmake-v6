cmake_minimum_required(VERSION 3.0)

project(test.irrlicht.matrix CXX)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC Irrlicht)

set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

file(COPY ${CMAKE_CURRENT_LIST_DIR}/matrix.jpg DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}) ### copy resource files