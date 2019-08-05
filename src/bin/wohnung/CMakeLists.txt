cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

project(wohnung)

set(WOHNUNG_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/src/wohnung")
set(WOHNUNG_INCLUDE_DIR "${BENNI_SDK_INCLUDE_DIR}/wohnung")

set(WOHNUNG_SOURCES
	${WOHNUNG_SOURCE_DIR}/PlanCAL.cpp
	${WOHNUNG_SOURCE_DIR}/Wohnung.cpp
	${WOHNUNG_SOURCE_DIR}/main_Wohnung.cpp)

set(WOHNUNG_HEADERS
	${WOHNUNG_SOURCE_DIR}/PlanCAL.hpp
	${WOHNUNG_SOURCE_DIR}/Wohnung.hpp)

message("===============================================")
message("${BoldBlue}PROJECT_NAME: ${PROJECT_NAME}${ColourReset}")
message("===============================================")
message("CMAKE_CURRENT_LIST_DIR = ${CMAKE_CURRENT_LIST_DIR}")
message("PROJ_SOURCE_DIR = ${PROJ_SOURCE_DIR}")
message("WOHNUNG_SOURCE_DIR = ${WOHNUNG_SOURCE_DIR}")
message("WOHNUNG_INCLUDE_DIR = ${WOHNUNG_INCLUDE_DIR}")

add_executable(${PROJECT_NAME} "")

target_include_directories(${PROJECT_NAME} PRIVATE ${WOHNUNG_SOURCE_DIR}/..)

target_sources(${PROJECT_NAME} PRIVATE ${WOHNUNG_SOURCES} ${WOHNUNG_HEADERS})

target_link_libraries(${PROJECT_NAME} PUBLIC de.hampe.common)