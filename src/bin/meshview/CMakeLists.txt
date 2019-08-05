cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

project(meshview)

set(MESHVIEW_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/src/meshview")
set(MESHVIEW_INCLUDE_DIR "${BENNI_SDK_INCLUDE_DIR}/meshview")

set(MESHVIEW_SOURCES
	${MESHVIEW_SOURCE_DIR}/meshview.cpp
	${MESHVIEW_SOURCE_DIR}/main_meshview.cpp)

set(MESHVIEW_HEADERS
	${MESHVIEW_SOURCE_DIR}/meshview.hpp)

# message("===============================================")
message("${BoldBlue}PROGRAM_NAME: ${PROJECT_NAME}${ColourReset}")
# message("===============================================")
# message("CMAKE_CURRENT_LIST_DIR = ${CMAKE_CURRENT_LIST_DIR}")
# message("PROJ_SOURCE_DIR = ${PROJ_SOURCE_DIR}")
# message("meshview_SOURCE_DIR = ${MESHVIEW_SOURCE_DIR}")
# message("meshview_INCLUDE_DIR = ${MESHVIEW_INCLUDE_DIR}")
add_executable(meshview "")
target_include_directories(meshview PRIVATE ${MESHVIEW_SOURCE_DIR}/..)
target_sources(meshview PRIVATE ${MESHVIEW_SOURCES} ${MESHVIEW_HEADERS})
target_link_libraries(meshview PUBLIC de.hampe.common)
