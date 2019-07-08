cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

project(benni-lib-z C)

if (TARGET z)
	message("${BoldYellow}[benni] HEADER_GUARD: ${PROJECT_NAME}${ColourReset}")
else()
	set(ZLIB_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
	set(ZLIB_INCLUDE_DIR ${BENNI_SDK_INCLUDE_DIR}/zlib)

	set(ZLIB_HEADERS 
		${ZLIB_SOURCE_DIR}/crc32.h
		${ZLIB_SOURCE_DIR}/deflate.h
		${ZLIB_SOURCE_DIR}/inflate.h
		${ZLIB_SOURCE_DIR}/inffast.h
		${ZLIB_SOURCE_DIR}/inffixed.h
		${ZLIB_SOURCE_DIR}/inftrees.h
		${ZLIB_SOURCE_DIR}/trees.h
		${ZLIB_SOURCE_DIR}/zconf.h
		${ZLIB_SOURCE_DIR}/zlib.h
		${ZLIB_SOURCE_DIR}/zutil.h)
		
	set(ZLIB_SOURCES
		${ZLIB_SOURCE_DIR}/adler32.c
		${ZLIB_SOURCE_DIR}/compress.c
		${ZLIB_SOURCE_DIR}/crc32.c
		${ZLIB_SOURCE_DIR}/deflate.c
		${ZLIB_SOURCE_DIR}/inflate.c
		${ZLIB_SOURCE_DIR}/inffast.c
		${ZLIB_SOURCE_DIR}/inftrees.c
		${ZLIB_SOURCE_DIR}/trees.c
		${ZLIB_SOURCE_DIR}/uncompr.c
		${ZLIB_SOURCE_DIR}/zutil.c)

	message("${BoldRed}[benni] BUILD_TARGET: ${PROJECT_NAME}${ColourReset}")
	message("CMAKE_CURRENT_LIST_DIR = ${CMAKE_CURRENT_LIST_DIR}")
	message("ZLIB_INCLUDE_DIR = ${ZLIB_INCLUDE_DIR}")
	message("ZLIB_SOURCE_DIR = ${ZLIB_SOURCE_DIR}")	
	message("ZLIB_HEADERS = ${ZLIB_HEADERS}")
	message("ZLIB_SOURCES = ${ZLIB_SOURCES}")
	
	add_library(z STATIC "")
	### <Add option="-Os -O3 -O2 -Wall -W -Wno-unused-parameter -U__STRICT_ANSI__ -DWIN32 -DNDEBUG -D_WINDOWS -D_IRR_STATIC_LIB_ -D_CRT_SECURE_NO_DEPRECATE -D__GNUWIN32__
	
	### target_compile_definitions(z PUBLIC -DZLIB_DLL=1) # 
	
	target_include_directories(z PRIVATE ${ZLIB_SOURCE_DIR})
								
	target_sources(z PRIVATE ${ZLIB_SOURCES} ${ZLIB_HEADERS})

	target_include_directories(z PUBLIC ${ZLIB_INCLUDE_DIR}) # For the SDK
	
	file(COPY ${ZLIB_HEADERS} DESTINATION ${ZLIB_INCLUDE_DIR}) # For the SDK

endif()
