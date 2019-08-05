cmake_minimum_required(VERSION 3.1)

project(benni-lib-tinyxml2)

if (TARGET tinyxml2)
	message("${BoldYellow}[benni] HEADER_GUARD: ${PROJECT_NAME}${ColourReset}")
else()
	set(TINYXML2_HEADER_DIR ${CMAKE_CURRENT_LIST_DIR}/include/tinyxml2)
	set(TINYXML2_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
	set(TINYXML2_INCLUDE_DIR ${BENNI_SDK_INCLUDE_DIR}/tinyxml2)
	set(TINYXML2_HEADERS ${TINYXML2_HEADER_DIR}/tinyxml2.h)
	set(TINYXML2_SOURCES ${TINYXML2_SOURCE_DIR}/tinyxml2.cpp)

	if (BENNI_SDK_SHARED)
		message("${Blue}SHARED_LIBRARY: ${PROJECT_NAME}${ColourReset}")
		add_library(tinyxml2 SHARED "")	
		# target_compile_definitions(z PUBLIC -DZLIB_DLL=1)
	else()
		message("${Blue}STATIC_LIBRARY: ${PROJECT_NAME}${ColourReset}")
		add_library(tinyxml2 STATIC "")
	endif()
	
	target_include_directories(tinyxml2 PRIVATE ${TINYXML2_HEADER_DIR}/..)		
	target_sources(tinyxml2 PRIVATE ${TINYXML2_SOURCES} ${TINYXML2_HEADERS})
	file(COPY ${TINYXML2_HEADERS} DESTINATION ${TINYXML2_INCLUDE_DIR}) # For the SDK

endif()