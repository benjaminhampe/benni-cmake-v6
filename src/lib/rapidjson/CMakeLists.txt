cmake_minimum_required(VERSION 3.1)

project(benni-lib-rapidjson)

if (TARGET rapidjson)
	message("${BoldYellow}[benni] HEADER_GUARD: ${PROJECT_NAME}${ColourReset}")
else()
	set(RJSON_HEADER_DIR ${CMAKE_CURRENT_LIST_DIR}/include/rapidjson)
	set(RJSON_INCLUDE_DIR ${BENNI_SDK_INCLUDE_DIR}/rapidjson)
	set(RJSON_HEADERS 
		${RJSON_HEADER_DIR}/allocators.h
		${RJSON_HEADER_DIR}/document.h
		${RJSON_HEADER_DIR}/encodedstream.h
		${RJSON_HEADER_DIR}/encodings.h
		${RJSON_HEADER_DIR}/filereadstream.h
		${RJSON_HEADER_DIR}/filewritestream.h
		${RJSON_HEADER_DIR}/fwd.h
		${RJSON_HEADER_DIR}/istreamwrapper.h
		${RJSON_HEADER_DIR}/memorybuffer.h
		${RJSON_HEADER_DIR}/memorystream.h
		${RJSON_HEADER_DIR}/ostreamwrapper.h
		${RJSON_HEADER_DIR}/pointer.h
		${RJSON_HEADER_DIR}/prettywriter.h
		${RJSON_HEADER_DIR}/rapidjson.h
		${RJSON_HEADER_DIR}/reader.h
		${RJSON_HEADER_DIR}/schema.h
		${RJSON_HEADER_DIR}/stream.h
		${RJSON_HEADER_DIR}/stringbuffer.h
		${RJSON_HEADER_DIR}/writer.h)
	set(RJSON_HEADERS_INTERNAL 		
		${RJSON_HEADER_DIR}/internal/biginteger.h
		${RJSON_HEADER_DIR}/internal/diyfp.h
		${RJSON_HEADER_DIR}/internal/dtoa.h
		${RJSON_HEADER_DIR}/internal/ieee754.h
		${RJSON_HEADER_DIR}/internal/itoa.h
		${RJSON_HEADER_DIR}/internal/meta.h
		${RJSON_HEADER_DIR}/internal/pow10.h
		${RJSON_HEADER_DIR}/internal/regex.h
		${RJSON_HEADER_DIR}/internal/stack.h
		${RJSON_HEADER_DIR}/internal/strfunc.h
		${RJSON_HEADER_DIR}/internal/strtod.h
		${RJSON_HEADER_DIR}/internal/swap.h)
	set(RJSON_HEADERS_ERROR
		${RJSON_HEADER_DIR}/error/en.h
		${RJSON_HEADER_DIR}/error/error.h
	)

	# message("===============================================")
	message("${BoldMagenta}LIBRARY_NAME: ${PROJECT_NAME}${ColourReset}")
	# message("===============================================")
	# message("CMAKE_CURRENT_LIST_DIR = ${CMAKE_CURRENT_LIST_DIR}")
	# message("RJSON_INCLUDE_DIR = ${RJSON_INCLUDE_DIR}")
	# message("RJSON_HEADER_DIR = ${RJSON_HEADER_DIR}")
	# message("RJSON_HEADERS = ${RJSON_HEADERS}")

	file(COPY ${RJSON_HEADERS} DESTINATION ${RJSON_INCLUDE_DIR}) # For the SDK
	file(COPY ${RJSON_HEADERS_INTERNAL} DESTINATION ${RJSON_INCLUDE_DIR}/internal) # For the SDK
	file(COPY ${RJSON_HEADERS_ERROR} DESTINATION ${RJSON_INCLUDE_DIR}/error) # For the SDK
	
	add_library(rapidjson INTERFACE)
	target_compile_definitions(rapidjson INTERFACE -DUSE_LIB_RAPIDJSON=1) # Now we can check in C++ code if library is available
	target_include_directories(rapidjson INTERFACE ${RJSON_INCLUDE_DIR})
	
endif()