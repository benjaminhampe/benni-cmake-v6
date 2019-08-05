cmake_minimum_required(VERSION 3.1)

project(benni-lib-open3dgc CXX)

if (TARGET open3dgc)
	message("${BoldYellow}[benni] HEADER_GUARD: ${PROJECT_NAME}${ColourReset}")
else()
	set(OPEN3DGC_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
	set(OPEN3DGC_INCLUDE_DIR ${BENNI_SDK_INCLUDE_DIR}/open3dgc)
	set(OPEN3DGC_HEADERS 
		${OPEN3DGC_SOURCE_DIR}/o3dgcAdjacencyInfo.h
		${OPEN3DGC_SOURCE_DIR}/o3dgcArithmeticCodec.h
		${OPEN3DGC_SOURCE_DIR}/o3dgcBinaryStream.h
		${OPEN3DGC_SOURCE_DIR}/o3dgcCommon.h
		${OPEN3DGC_SOURCE_DIR}/o3dgcDVEncodeParams.h
		${OPEN3DGC_SOURCE_DIR}/o3dgcDynamicVectorDecoder.h
		${OPEN3DGC_SOURCE_DIR}/o3dgcDynamicVectorEncoder.h
		${OPEN3DGC_SOURCE_DIR}/o3dgcDynamicVector.h
		${OPEN3DGC_SOURCE_DIR}/o3dgcFIFO.h
		${OPEN3DGC_SOURCE_DIR}/o3dgcIndexedFaceSet.h
		${OPEN3DGC_SOURCE_DIR}/o3dgcIndexedFaceSet.inl
		${OPEN3DGC_SOURCE_DIR}/o3dgcSC3DMCDecoder.h
		${OPEN3DGC_SOURCE_DIR}/o3dgcSC3DMCDecoder.inl
		${OPEN3DGC_SOURCE_DIR}/o3dgcSC3DMCEncodeParams.h
		${OPEN3DGC_SOURCE_DIR}/o3dgcSC3DMCEncoder.h
		${OPEN3DGC_SOURCE_DIR}/o3dgcSC3DMCEncoder.inl
		${OPEN3DGC_SOURCE_DIR}/o3dgcTimer.h
		${OPEN3DGC_SOURCE_DIR}/o3dgcTriangleFans.h
		${OPEN3DGC_SOURCE_DIR}/o3dgcTriangleListDecoder.h
		${OPEN3DGC_SOURCE_DIR}/o3dgcTriangleListDecoder.inl
		${OPEN3DGC_SOURCE_DIR}/o3dgcTriangleListEncoder.h
		${OPEN3DGC_SOURCE_DIR}/o3dgcTriangleListEncoder.inl
		${OPEN3DGC_SOURCE_DIR}/o3dgcVector.h
		${OPEN3DGC_SOURCE_DIR}/o3dgcVector.inl)
		
	set(OPEN3DGC_SOURCES 
		${OPEN3DGC_SOURCE_DIR}/o3dgcArithmeticCodec.cpp
		${OPEN3DGC_SOURCE_DIR}/o3dgcDynamicVectorDecoder.cpp
		${OPEN3DGC_SOURCE_DIR}/o3dgcDynamicVectorEncoder.cpp
		${OPEN3DGC_SOURCE_DIR}/o3dgcTools.cpp
		${OPEN3DGC_SOURCE_DIR}/o3dgcTriangleFans.cpp)

	# message("===============================================")
	message("${BoldMagenta}LIBRARY_NAME: ${PROJECT_NAME}${ColourReset}")
	# message("===============================================")
	# message("CMAKE_CURRENT_LIST_DIR = ${CMAKE_CURRENT_LIST_DIR}")
	# message("OPEN3DGC_INCLUDE_DIR = ${OPEN3DGC_INCLUDE_DIR}")
	# message("OPEN3DGC_SOURCE_DIR = ${OPEN3DGC_SOURCE_DIR}")	
	# message("OPEN3DGC_HEADERS = ${OPEN3DGC_HEADERS}")
	# message("OPEN3DGC_SOURCES = ${OPEN3DGC_SOURCES}")
	
	add_library(open3dgc STATIC "")
	target_include_directories(open3dgc PRIVATE ${OPEN3DGC_SOURCE_DIR})		
	target_sources(open3dgc PRIVATE ${OPEN3DGC_SOURCES} ${OPEN3DGC_HEADERS})
	target_include_directories(open3dgc PUBLIC ${OPEN3DGC_INCLUDE_DIR}) # For the SDK
	file(COPY ${OPEN3DGC_HEADERS} DESTINATION ${OPEN3DGC_INCLUDE_DIR}) # For the SDK

endif()