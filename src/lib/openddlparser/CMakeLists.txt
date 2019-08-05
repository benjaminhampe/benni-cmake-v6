cmake_minimum_required(VERSION 3.1)

project(benni-lib-openddlparser CXX)

if (TARGET openddlparser)
	message("${BoldYellow}[benni] HEADER_GUARD: ${PROJECT_NAME}${ColourReset}")
else()

	add_definitions( -DOPENDDL_STATIC_LIBARY )
	add_definitions( -DOPENDDLPARSER_BUILD )
	add_definitions( -D_VARIADIC_MAX=10 )
	add_definitions( -DGTEST_HAS_PTHREAD=0 )
	add_definitions( -DDDL_DEBUG_HEADER_NAME)
	
	set(OPENDDL_HEADER_DIR ${CMAKE_CURRENT_LIST_DIR}/include/openddlparser)
	set(OPENDDL_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
	set(OPENDDL_INCLUDE_DIR ${BENNI_SDK_INCLUDE_DIR}/openddlparser)

	set(OPENDDL_HEADERS 
		${OPENDDL_HEADER_DIR}/OpenDDLParser.h
		${OPENDDL_HEADER_DIR}/OpenDDLParserUtils.h
		${OPENDDL_HEADER_DIR}/OpenDDLCommon.h
		${OPENDDL_HEADER_DIR}/OpenDDLExport.h
		${OPENDDL_HEADER_DIR}/OpenDDLStream.h
		${OPENDDL_HEADER_DIR}/DDLNode.h
		${OPENDDL_HEADER_DIR}/Value.h)
		
	set(OPENDDL_SOURCES
		${OPENDDL_SOURCE_DIR}/OpenDDLParser.cpp
		${OPENDDL_SOURCE_DIR}/DDLNode.cpp
		${OPENDDL_SOURCE_DIR}/OpenDDLCommon.cpp
		${OPENDDL_SOURCE_DIR}/OpenDDLExport.cpp
		${OPENDDL_SOURCE_DIR}/Value.cpp
		${OPENDDL_SOURCE_DIR}/OpenDDLStream.cpp)
		
	# message("===============================================")
	message("${BoldMagenta}LIBRARY_NAME: ${PROJECT_NAME}${ColourReset}")
	# message("===============================================")
	# message("CMAKE_CURRENT_LIST_DIR = ${CMAKE_CURRENT_LIST_DIR}")
	# message("OPENDDL_HEADER_DIR = ${OPENDDL_HEADER_DIR}")
	# message("OPENDDL_INCLUDE_DIR = ${OPENDDL_INCLUDE_DIR}")
	# message("OPENDDL_SOURCE_DIR = ${OPENDDL_SOURCE_DIR}")	
	# message("OPENDDL_HEADERS = ${OPENDDL_HEADERS}")
	# message("OPENDDL_SOURCES = ${OPENDDL_SOURCES}")
	
	add_library(openddlparser STATIC "")
	
	target_include_directories(openddlparser PRIVATE ${OPENDDL_HEADER_DIR}/..)
								
	target_sources(openddlparser PRIVATE ${OPENDDL_SOURCES} ${OPENDDL_HEADERS})

	target_include_directories(openddlparser PUBLIC ${BENNI_SDK_INCLUDE_DIR}) # For the SDK
	
	file(COPY ${OPENDDL_HEADERS} DESTINATION ${OPENDDL_INCLUDE_DIR}) # For the SDK

endif()
