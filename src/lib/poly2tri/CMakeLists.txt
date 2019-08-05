cmake_minimum_required(VERSION 3.0)

project(benni-lib-poly2tri CXX)

if (TARGET poly2tri)
	message("${BoldYellow}[benni] HEADER_GUARD: ${PROJECT_NAME}${ColourReset}")
else()
	set(P2TRI_HEADER_DIR ${CMAKE_CURRENT_LIST_DIR}/include/poly2tri)
	set(P2TRI_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
	set(P2TRI_INCLUDE_DIR ${BENNI_SDK_INCLUDE_DIR}/poly2tri)

	set(P2TRI_HEADERS 
		${P2TRI_HEADER_DIR}/poly2tri.h
		${P2TRI_HEADER_DIR}/utils.h
		${P2TRI_HEADER_DIR}/shapes.h
		${P2TRI_HEADER_DIR}/advancing_front.h
		${P2TRI_HEADER_DIR}/cdt.h
		${P2TRI_HEADER_DIR}/sweep.h
		${P2TRI_HEADER_DIR}/sweep_context.h)
		
	set(P2TRI_SOURCES
		${P2TRI_SOURCE_DIR}/shapes.cc
		${P2TRI_SOURCE_DIR}/advancing_front.cc
		${P2TRI_SOURCE_DIR}/cdt.cc
		${P2TRI_SOURCE_DIR}/sweep.cc
		${P2TRI_SOURCE_DIR}/sweep_context.cc)

	# message("===============================================")
	message("${BoldMagenta}LIBRARY_NAME: ${PROJECT_NAME}${ColourReset}")
	# message("===============================================")
	# message("CMAKE_CURRENT_LIST_DIR = ${CMAKE_CURRENT_LIST_DIR}")
	# message("P2TRI_INCLUDE_DIR = ${P2TRI_INCLUDE_DIR}")
	# message("P2TRI_SOURCE_DIR = ${P2TRI_SOURCE_DIR}")	
	# message("P2TRI_HEADERS = ${P2TRI_HEADERS}")
	# message("P2TRI_SOURCES = ${P2TRI_SOURCES}")
	add_library(poly2tri STATIC "")
	target_include_directories(poly2tri PRIVATE ${P2TRI_HEADER_DIR}/..)
	target_sources(poly2tri PRIVATE ${P2TRI_SOURCES} ${P2TRI_HEADERS})
	target_include_directories(poly2tri PUBLIC ${P2TRI_INCLUDE_DIR}) # For the SDK
	file(COPY ${P2TRI_HEADERS} DESTINATION ${P2TRI_INCLUDE_DIR}) # For the SDK

endif()
