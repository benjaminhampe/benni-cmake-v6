cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

project(benni-lib-png C)

if (TARGET png)
	message("${BoldYellow}[benni] HEADER_GUARD: ${PROJECT_NAME}${ColourReset}")
else()
	set(PNG_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
	set(PNG_INCLUDE_DIR ${BENNI_SDK_INCLUDE_DIR}/png)

	set(PNG_HEADERS 
		${PNG_SOURCE_DIR}/png.h
		${PNG_SOURCE_DIR}/pngconf.h
		${PNG_SOURCE_DIR}/pnglibconf.h
		${PNG_SOURCE_DIR}/pngdebug.h
		${PNG_SOURCE_DIR}/pnginfo.h
		${PNG_SOURCE_DIR}/pngstruct.h)

	set(PNG_PRIVATE_HEADERS 
		${PNG_SOURCE_DIR}/pngpriv.h)
		
	set(PNG_SOURCES
		${PNG_SOURCE_DIR}/png.c
		${PNG_SOURCE_DIR}/pngerror.c
		${PNG_SOURCE_DIR}/pngget.c
		${PNG_SOURCE_DIR}/pngmem.c
		${PNG_SOURCE_DIR}/pngpread.c
		${PNG_SOURCE_DIR}/pngread.c
		${PNG_SOURCE_DIR}/pngrio.c
		${PNG_SOURCE_DIR}/pngrtran.c
		${PNG_SOURCE_DIR}/pngrutil.c
		${PNG_SOURCE_DIR}/pngset.c
		${PNG_SOURCE_DIR}/pngtrans.c
		${PNG_SOURCE_DIR}/pngwio.c
		${PNG_SOURCE_DIR}/pngwrite.c
		${PNG_SOURCE_DIR}/pngwtran.c
		${PNG_SOURCE_DIR}/pngwutil.c)
	
	if (BENNI_SDK_SHARED)
		message("${Blue}SHARED_LIBRARY: ${PROJECT_NAME}${ColourReset}")
		add_library(png SHARED "")	
	else()
		message("${Blue}STATIC_LIBRARY: ${PROJECT_NAME}${ColourReset}")
		add_library(png STATIC "")
	endif()
	
	target_include_directories(png PRIVATE ${PNG_SOURCE_DIR})			
	target_sources(png PRIVATE ${PNG_SOURCES} ${PNG_HEADERS} ${PNG_PRIVATE_HEADERS} )
	target_link_libraries(png PUBLIC z bzip2 m)
	target_include_directories(png PUBLIC ${PNG_INCLUDE_DIR}) # For the SDK
	file(COPY ${PNG_HEADERS} DESTINATION ${PNG_INCLUDE_DIR}) # For the SDK

endif()
