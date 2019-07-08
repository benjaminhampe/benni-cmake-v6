cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

project(benni-lib-bzip2 C)

if (TARGET bzip2)
	message("${BoldYellow}[benni] HEADER_GUARD: ${PROJECT_NAME}${ColourReset}")
else()
	set(BZIP2_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
	set(BZIP2_INCLUDE_DIR ${BENNI_SDK_INCLUDE_DIR}/bzip2)

	set(BZIP2_HEADERS 
		${BZIP2_SOURCE_DIR}/bzlib.h)
		
	set(BZIP2_SOURCES
		${BZIP2_SOURCE_DIR}/blocksort.c
		${BZIP2_SOURCE_DIR}/bzcompress.c
		${BZIP2_SOURCE_DIR}/bzlib.c
		${BZIP2_SOURCE_DIR}/bzlib_private.h
		${BZIP2_SOURCE_DIR}/crctable.c
		${BZIP2_SOURCE_DIR}/decompress.c
		${BZIP2_SOURCE_DIR}/huffman.c
		${BZIP2_SOURCE_DIR}/randtable.c)
		
	message("${BoldRed}[benni] BUILD_TARGET: ${PROJECT_NAME}${ColourReset}")
	message("CMAKE_CURRENT_LIST_DIR = ${CMAKE_CURRENT_LIST_DIR}")
	message("BZIP2_INCLUDE_DIR = ${BZIP2_INCLUDE_DIR}")
	message("BZIP2_SOURCE_DIR = ${BZIP2_SOURCE_DIR}")	
	message("BZIP2_HEADERS = ${BZIP2_HEADERS}")
	message("BZIP2_SOURCES = ${BZIP2_SOURCES}")
	
	add_library(bzip2 STATIC "")
	### <Add option="-Os -O3 -O2 -Wall -W -Wno-unused-parameter -U__STRICT_ANSI__ -DWIN32 -DNDEBUG -D_WINDOWS -D_IRR_STATIC_LIB_ -D_CRT_SECURE_NO_DEPRECATE -D__GNUWIN32__
	
	target_include_directories(bzip2 PRIVATE ${BZIP2_SOURCE_DIR})
								
	target_sources(bzip2 PRIVATE ${BZIP2_SOURCES} ${BZIP2_HEADERS})

	target_include_directories(bzip2 PUBLIC ${BZIP2_INCLUDE_DIR}) # For the SDK
	
	file(COPY ${BZIP2_HEADERS} DESTINATION ${BZIP2_INCLUDE_DIR}) # For the SDK

endif()
