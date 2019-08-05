cmake_minimum_required(VERSION 3.0)

project(benni-lib-utf8cpp CXX)

if (TARGET utf8cpp)
	message("${BoldYellow}[benni] HEADER_GUARD: ${PROJECT_NAME}${ColourReset}")
else()
	set(UTF8CPP_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
	set(UTF8CPP_INCLUDE_DIR ${BENNI_SDK_INCLUDE_DIR}/utf8cpp)

	set(UTF8CPP_HEADERS
		${UTF8CPP_SOURCE_DIR}/core.h
		${UTF8CPP_SOURCE_DIR}/checked.h
		${UTF8CPP_SOURCE_DIR}/unchecked.h
		${UTF8CPP_SOURCE_DIR}/utf8.h
	)
	
	# message("===============================================")
	message("${BoldMagenta}LIBRARY_NAME: ${PROJECT_NAME}${ColourReset}")
	# message("===============================================")
	# message("CMAKE_CURRENT_LIST_DIR = ${CMAKE_CURRENT_LIST_DIR}")
	# message("UTF8CPP_INCLUDE_DIR = ${UTF8CPP_INCLUDE_DIR}")
	# message("UTF8CPP_SOURCE_DIR = ${UTF8CPP_SOURCE_DIR}")	
	# message("UTF8CPP_HEADERS = ${UTF8CPP_HEADERS}")
	
	file(COPY ${UTF8CPP_HEADERS} DESTINATION ${UTF8CPP_INCLUDE_DIR}) # For the SDK
	
	add_library(utf8cpp INTERFACE)
	target_compile_definitions(utf8cpp INTERFACE -DUSE_LIB_UTF8CPP=1) # Now we can check in C++ code if library is available
	# target_sources(glm PRIVATE ${GLM_SOURCES} PUBLIC ${GLM_HEADERS})
	target_include_directories(utf8cpp INTERFACE ${UTF8CPP_INCLUDE_DIR})
	
endif()
