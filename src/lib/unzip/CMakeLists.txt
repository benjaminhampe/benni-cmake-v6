cmake_minimum_required(VERSION 3.0)

project(benni-lib-unzip C)

if (TARGET unzip)
	message("${BoldYellow}[benni] HEADER_GUARD: ${PROJECT_NAME}${ColourReset}")
else()
	set(UNZIP_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
	set(UNZIP_INCLUDE_DIR ${BENNI_SDK_INCLUDE_DIR}/unzip)

	set(UNZIP_HEADERS
		${UNZIP_SOURCE_DIR}/crypt.h
		${UNZIP_SOURCE_DIR}/ioapi.h
		${UNZIP_SOURCE_DIR}/unzip.h
	)

	set(UNZIP_SOURCES
		${UNZIP_SOURCE_DIR}/ioapi.c
		${UNZIP_SOURCE_DIR}/unzip.c
	)
	
	# message("===============================================")
	message("${BoldMagenta}LIBRARY_NAME: ${PROJECT_NAME}${ColourReset}")
	# message("===============================================")
	# message("CMAKE_CURRENT_LIST_DIR = ${CMAKE_CURRENT_LIST_DIR}")
	# message("UNZIP_INCLUDE_DIR = ${UNZIP_INCLUDE_DIR}")
	# message("UNZIP_SOURCE_DIR = ${UNZIP_SOURCE_DIR}")	
	# message("UNZIP_HEADERS = ${UNZIP_HEADERS}")
	# message("UNZIP_SOURCES = ${UNZIP_SOURCES}")
	
	add_library(unzip STATIC "")

	target_include_directories(unzip PRIVATE ${UNZIP_SOURCE_DIR})
								
	target_sources(unzip PRIVATE ${UNZIP_SOURCES} ${UNZIP_HEADERS})

	target_include_directories(unzip PUBLIC ${UNZIP_INCLUDE_DIR}) # For the SDK
	
	file(COPY ${UNZIP_HEADERS} DESTINATION ${UNZIP_INCLUDE_DIR}) # For the SDK

endif()
