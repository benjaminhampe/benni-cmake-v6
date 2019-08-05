cmake_minimum_required(VERSION 3.1)

project(benni-lib-clipper CXX)

if (TARGET clipper)
	message("${BoldYellow}[benni] HEADER_GUARD: ${PROJECT_NAME}${ColourReset}")
else()
	set(CLIPPER_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
	set(CLIPPER_INCLUDE_DIR ${BENNI_SDK_INCLUDE_DIR}/clipper)
	set(CLIPPER_HEADERS ${CLIPPER_SOURCE_DIR}/clipper.hpp)
	set(CLIPPER_SOURCES ${CLIPPER_SOURCE_DIR}/clipper.cpp)

	# message("===============================================")
	message("${BoldMagenta}LIBRARY_NAME: ${PROJECT_NAME}${ColourReset}")
	# message("===============================================")
	# message("CMAKE_CURRENT_LIST_DIR = ${CMAKE_CURRENT_LIST_DIR}")
	# message("CLIPPER_INCLUDE_DIR = ${CLIPPER_INCLUDE_DIR}")
	# message("CLIPPER_SOURCE_DIR = ${CLIPPER_SOURCE_DIR}")	
	# message("CLIPPER_HEADERS = ${CLIPPER_HEADERS}")
	# message("CLIPPER_SOURCES = ${CLIPPER_SOURCES}")
	
	add_library(clipper STATIC "")
	target_include_directories(clipper PRIVATE ${CLIPPER_SOURCE_DIR})		
	target_sources(clipper PRIVATE ${CLIPPER_SOURCES} ${CLIPPER_HEADERS})
	target_include_directories(clipper PUBLIC ${CLIPPER_INCLUDE_DIR}) # For the SDK
	file(COPY ${CLIPPER_HEADERS} DESTINATION ${CLIPPER_INCLUDE_DIR}) # For the SDK

endif()
