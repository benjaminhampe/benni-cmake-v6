cmake_minimum_required(VERSION 3.1)

project(test.assimp)

set(TEST_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})
set(TEST_SOURCES ${TEST_SOURCE_DIR}/main.cpp)

message("===============================================")
message("${BoldBlue}PROJECT_NAME: ${PROJECT_NAME}${ColourReset}")
message("===============================================")
message("CMAKE_CURRENT_LIST_DIR = ${CMAKE_CURRENT_LIST_DIR}")
message("TEST_SOURCE_DIR = ${TEST_SOURCE_DIR}")
message("TEST_INCLUDE_DIR = ${TEST_INCLUDE_DIR}")

add_executable(${PROJECT_NAME} "")

target_include_directories(${PROJECT_NAME} PRIVATE ${TEST_SOURCE_DIR})

target_sources(${PROJECT_NAME} PRIVATE ${TEST_SOURCES})

target_link_libraries(${PROJECT_NAME} PUBLIC assimp irrExt)

# target_include_directories(${PROJECT_NAME} PUBLIC ${BENNI_SDK_INCLUDE_DIR})