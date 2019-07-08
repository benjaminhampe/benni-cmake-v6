cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

project(benni-lib-irrExt)

set(IRREXT_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/src/irrExt")
set(IRREXT_INCLUDE_DIR "${BENNI_SDK_INCLUDE_DIR}/irrExt")

### file(GLOB_RECURSE IRREXT_HEADERS "${IRREXT_INCLUDE_DIR}/*.hpp")
### file(GLOB_RECURSE IRREXT_SOURCES "${IRREXT_SOURCE_DIR}/*.cpp")

set(IRREXT_SOURCES
	${IRREXT_SOURCE_DIR}/AutoMeshBuffer.cpp
	${IRREXT_SOURCE_DIR}/AutoMeshBufferUtils.cpp
	${IRREXT_SOURCE_DIR}/BaseEventReceiver.cpp
	${IRREXT_SOURCE_DIR}/BaseWindow.cpp
	${IRREXT_SOURCE_DIR}/Camera.cpp
	${IRREXT_SOURCE_DIR}/CGUITTFont.cpp
	${IRREXT_SOURCE_DIR}/CGUIWindow.cpp
	${IRREXT_SOURCE_DIR}/createDevice.cpp
	${IRREXT_SOURCE_DIR}/createRoundRect.cpp
	${IRREXT_SOURCE_DIR}/CXMLWriterUTF8.cpp
	${IRREXT_SOURCE_DIR}/FontUtils.cpp
	${IRREXT_SOURCE_DIR}/ImageUtils.cpp
	${IRREXT_SOURCE_DIR}/irrExt.cpp
	${IRREXT_SOURCE_DIR}/libFreeType.cpp
	${IRREXT_SOURCE_DIR}/StringUtils.cpp
	${IRREXT_SOURCE_DIR}/MeshLoader.cpp)

set(IRREXT_HEADERS
	${IRREXT_SOURCE_DIR}/AutoMeshBuffer.hpp
	${IRREXT_SOURCE_DIR}/AutoMeshBufferUtils.hpp
	${IRREXT_SOURCE_DIR}/BaseEventReceiver.hpp
	${IRREXT_SOURCE_DIR}/BaseWindow.hpp
	${IRREXT_SOURCE_DIR}/Camera.hpp
	${IRREXT_SOURCE_DIR}/CGUITTFont.hpp
	${IRREXT_SOURCE_DIR}/CGUIWindow.hpp
	${IRREXT_SOURCE_DIR}/createDevice.hpp
	${IRREXT_SOURCE_DIR}/createRoundRect.hpp
	${IRREXT_SOURCE_DIR}/CXMLWriterUTF8.hpp
	${IRREXT_SOURCE_DIR}/FontUtils.hpp
	${IRREXT_SOURCE_DIR}/ImageUtils.hpp
	${IRREXT_SOURCE_DIR}/irrExt.hpp
	${IRREXT_SOURCE_DIR}/libFreeType.hpp
	${IRREXT_SOURCE_DIR}/sinCosTable.hpp
	${IRREXT_SOURCE_DIR}/StringUtils.hpp
	${IRREXT_SOURCE_DIR}/ustring.hpp
	${IRREXT_SOURCE_DIR}/MeshLoader.hpp)

message("===============================================")
message("${BoldYellow}PROJECT_NAME: ${PROJECT_NAME}${ColourReset}")
message("===============================================")
message("CMAKE_CURRENT_LIST_DIR = ${CMAKE_CURRENT_LIST_DIR}")
message("PROJECT_SOURCE_DIR = ${PROJECT_SOURCE_DIR}")
message("IRREXT_INCLUDE_DIR = ${IRREXT_INCLUDE_DIR}")
message("IRREXT_SOURCE_DIR = ${IRREXT_SOURCE_DIR}")

add_library(irrExt STATIC "")

target_sources(irrExt PRIVATE ${IRREXT_SOURCES} ${IRREXT_HEADERS})

target_include_directories(irrExt PRIVATE ${IRREXT_SOURCE_DIR}/..) # go up -> now we can do <irrExt/xxx.hpp>

target_include_directories(irrExt PRIVATE ${BENNI_SDK_INCLUDE_DIR}) # go up -> now we can do <irrExt/xxx.hpp>

target_link_libraries(irrExt PUBLIC freetype Irrlicht glm png jpeg bzip2 z m)

target_include_directories(irrExt PUBLIC ${IRREXT_INCLUDE_DIR}/..) # For the SDK

file(COPY ${IRREXT_HEADERS} DESTINATION ${IRREXT_INCLUDE_DIR}) # For the SDK

### include(CopyFiles) # For the SDK

### copy_sdk_header_files(${IRREXT_INCLUDE_DIR} ${IRREXT_SOURCE_DIR} ${IRREXT_HEADERS}) # For the SDK
