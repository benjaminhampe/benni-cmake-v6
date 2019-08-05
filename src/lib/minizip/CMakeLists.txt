cmake_minimum_required(VERSION 3.0)

project(benni-lib-minizip C)

if (TARGET minizip)
	message("${BoldYellow}[benni] HEADER_GUARD: ${PROJECT_NAME}${ColourReset}")
else()
	set(MINIZIP_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
	set(MINIZIP_INCLUDE_DIR ${BENNI_SDK_INCLUDE_DIR}/minizip)

	set(MINIZIP_HEADERS 
		${MINIZIP_SOURCE_DIR}/miniz.h
		${MINIZIP_SOURCE_DIR}/zip.h)
		
	set(MINIZIP_SOURCES
		${MINIZIP_SOURCE_DIR}/zip.c)

	# message("===============================================")
	message("${BoldMagenta}LIBRARY_NAME: ${PROJECT_NAME}${ColourReset}")
	# message("===============================================")
	# message("CMAKE_CURRENT_LIST_DIR = ${CMAKE_CURRENT_LIST_DIR}")
	# message("MINIZIP_INCLUDE_DIR = ${MINIZIP_INCLUDE_DIR}")
	# message("MINIZIP_SOURCE_DIR = ${MINIZIP_SOURCE_DIR}")	
	# message("MINIZIP_HEADERS = ${MINIZIP_HEADERS}")
	# message("MINIZIP_SOURCES = ${MINIZIP_SOURCES}")
	
	add_library(minizip STATIC "")
	target_include_directories(minizip PRIVATE ${MINIZIP_SOURCE_DIR})
	target_sources(minizip PRIVATE ${MINIZIP_SOURCES} ${MINIZIP_HEADERS})
	target_include_directories(minizip PUBLIC ${MINIZIP_INCLUDE_DIR}) # For the SDK	
	file(COPY ${MINIZIP_HEADERS} DESTINATION ${MINIZIP_INCLUDE_DIR}) # For the SDK

endif()
