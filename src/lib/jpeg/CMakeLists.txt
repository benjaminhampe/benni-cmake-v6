cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

project(benni-lib-jpeg C)

if (TARGET jpeg)
	message("${BoldYellow}[benni] HEADER_GUARD: ${PROJECT_NAME}${ColourReset}")
else()
	set(JPEG_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
	set(JPEG_INCLUDE_DIR ${BENNI_SDK_INCLUDE_DIR}/jpeg)

	set(JPEG_HEADERS 
		${JPEG_SOURCE_DIR}/jconfig.h
		${JPEG_SOURCE_DIR}/cderror.h
		# ${JPEG_SOURCE_DIR}/jchuff.h
		${JPEG_SOURCE_DIR}/jdct.h
		# ${JPEG_SOURCE_DIR}/jdhuff.h
		${JPEG_SOURCE_DIR}/jerror.h
		${JPEG_SOURCE_DIR}/jinclude.h
		${JPEG_SOURCE_DIR}/jmemsys.h
		${JPEG_SOURCE_DIR}/jmorecfg.h
		${JPEG_SOURCE_DIR}/jpegint.h
		${JPEG_SOURCE_DIR}/jpeglib.h
		${JPEG_SOURCE_DIR}/jversion.h)
	
	set(JPEG_SOURCES
		${JPEG_SOURCE_DIR}/jaricom.c
		${JPEG_SOURCE_DIR}/jcapimin.c
		${JPEG_SOURCE_DIR}/jcapistd.c
		${JPEG_SOURCE_DIR}/jcarith.c
		${JPEG_SOURCE_DIR}/jccoefct.c
		${JPEG_SOURCE_DIR}/jccolor.c
		${JPEG_SOURCE_DIR}/jcdctmgr.c
		${JPEG_SOURCE_DIR}/jchuff.c
		${JPEG_SOURCE_DIR}/jcinit.c
		${JPEG_SOURCE_DIR}/jcmainct.c
		${JPEG_SOURCE_DIR}/jcmarker.c
		${JPEG_SOURCE_DIR}/jcmaster.c
		${JPEG_SOURCE_DIR}/jcomapi.c
		${JPEG_SOURCE_DIR}/jcparam.c
		${JPEG_SOURCE_DIR}/jcprepct.c
		${JPEG_SOURCE_DIR}/jcsample.c
		${JPEG_SOURCE_DIR}/jctrans.c
		${JPEG_SOURCE_DIR}/jdapimin.c
		${JPEG_SOURCE_DIR}/jdapistd.c
		${JPEG_SOURCE_DIR}/jdarith.c
		${JPEG_SOURCE_DIR}/jdatadst.c
		${JPEG_SOURCE_DIR}/jdatasrc.c
		${JPEG_SOURCE_DIR}/jdcoefct.c
		${JPEG_SOURCE_DIR}/jdcolor.c
		${JPEG_SOURCE_DIR}/jddctmgr.c
		${JPEG_SOURCE_DIR}/jdhuff.c
		${JPEG_SOURCE_DIR}/jdinput.c
		${JPEG_SOURCE_DIR}/jdmainct.c
		${JPEG_SOURCE_DIR}/jdmarker.c
		${JPEG_SOURCE_DIR}/jdmaster.c
		${JPEG_SOURCE_DIR}/jdmerge.c
		${JPEG_SOURCE_DIR}/jdpostct.c
		${JPEG_SOURCE_DIR}/jdsample.c
		${JPEG_SOURCE_DIR}/jdtrans.c
		${JPEG_SOURCE_DIR}/jerror.c
		${JPEG_SOURCE_DIR}/jfdctflt.c
		${JPEG_SOURCE_DIR}/jfdctfst.c
		${JPEG_SOURCE_DIR}/jfdctint.c
		${JPEG_SOURCE_DIR}/jidctflt.c
		${JPEG_SOURCE_DIR}/jidctfst.c
		${JPEG_SOURCE_DIR}/jidctint.c
		${JPEG_SOURCE_DIR}/jmemmgr.c
		${JPEG_SOURCE_DIR}/jmemnobs.c
		${JPEG_SOURCE_DIR}/jquant1.c
		${JPEG_SOURCE_DIR}/jquant2.c
		${JPEG_SOURCE_DIR}/jutils.c)

	# memmgr back ends: compile only one of these into a working library
	### SYSDEPSOURCES= 
	### ${JPEG_SOURCE_DIR}/jmemansi.c
	### ${JPEG_SOURCE_DIR}/jmemname.c
	### ${JPEG_SOURCE_DIR}/jmemnobs.c
	### ${JPEG_SOURCE_DIR}/jmemdos.c
	### ${JPEG_SOURCE_DIR}/jmemmac.c

	if (BENNI_SDK_SHARED)
		message("${Blue}SHARED_LIBRARY: ${PROJECT_NAME}${ColourReset}")
		add_library(jpeg SHARED "")	
	else()
		message("${Blue}STATIC_LIBRARY: ${PROJECT_NAME}${ColourReset}")
		add_library(jpeg STATIC "")
	endif()

	target_include_directories(jpeg PRIVATE ${JPEG_SOURCE_DIR})					
	target_sources(jpeg PRIVATE ${JPEG_SOURCES} ${JPEG_HEADERS})
	target_include_directories(jpeg PUBLIC ${JPEG_INCLUDE_DIR}) # For the SDK
	file(COPY ${JPEG_HEADERS} DESTINATION ${JPEG_INCLUDE_DIR}) # For the SDK

endif()
