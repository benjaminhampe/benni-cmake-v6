cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

project(de.hampe.common)

set(BECOM_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/include")
set(BECOM_INCLUDE_DIR "${BENNI_SDK_INCLUDE_DIR}/de.hampe.common")

set(BECOM_SOURCES
	${BECOM_SOURCE_DIR}/de/hampe/color/Color.cpp
	${BECOM_SOURCE_DIR}/de/hampe/color/RainbowColor.cpp
	${BECOM_SOURCE_DIR}/de/hampe/logging/StdOutLogger.cpp
	${BECOM_SOURCE_DIR}/de/hampe/os/FileMagic.cpp
	${BECOM_SOURCE_DIR}/de/hampe/os/FileType.cpp
	${BECOM_SOURCE_DIR}/de/hampe/os/FileSystemSTL.cpp
	${BECOM_SOURCE_DIR}/de/hampe/os/FileFinderSTL.cpp
	${BECOM_SOURCE_DIR}/de/hampe/PerfTimer.cpp
	${BECOM_SOURCE_DIR}/de/hampe/StringUtil.cpp)

set(BECOM_HEADERS
	${BECOM_SOURCE_DIR}/de/hampe/ostreamGLM.hpp
	${BECOM_SOURCE_DIR}/de/hampe/PerfTimer.hpp
	${BECOM_SOURCE_DIR}/de/hampe/sprintf.hpp
	${BECOM_SOURCE_DIR}/de/hampe/StringUtil.hpp
	${BECOM_SOURCE_DIR}/de/hampe/Types.hpp)

set(BECOM_COLOR_HEADERS
	${BECOM_SOURCE_DIR}/de/hampe/color/Color.hpp
	${BECOM_SOURCE_DIR}/de/hampe/color/ColorFormat.hpp
	${BECOM_SOURCE_DIR}/de/hampe/color/RainbowColor.hpp)
	
set(BECOM_MATH_HEADERS
	${BECOM_SOURCE_DIR}/de/hampe/math/sinCosTable.hpp)

set(BECOM_OS_HEADERS
	${BECOM_SOURCE_DIR}/de/hampe/os/FileSystemSTL.hpp
	${BECOM_SOURCE_DIR}/de/hampe/os/FileType.hpp
	${BECOM_SOURCE_DIR}/de/hampe/os/FileFinderSTL.hpp
	${BECOM_SOURCE_DIR}/de/hampe/os/FileMagic.hpp)
	
set(BECOM_LOGGING_HEADERS	
	${BECOM_SOURCE_DIR}/de/hampe/logging/LogLevel.hpp
	${BECOM_SOURCE_DIR}/de/hampe/logging/Logger.hpp
	${BECOM_SOURCE_DIR}/de/hampe/logging/StdOutLogger.hpp)

message("===============================================")
message("${BoldYellow}PROJECT_NAME: ${PROJECT_NAME}${ColourReset}")
message("===============================================")
message("CMAKE_CURRENT_LIST_DIR = ${CMAKE_CURRENT_LIST_DIR}")
message("BECOM_SOURCE_DIR = ${BECOM_SOURCE_DIR}")
message("BECOM_INCLUDE_DIR = ${BECOM_INCLUDE_DIR}")

add_library(becom STATIC "")

target_sources(becom PRIVATE 
	${BECOM_SOURCES} 
	${BECOM_HEADERS}
	${BECOM_COLOR_HEADERS} 
	${BECOM_MATH_HEADERS} 
	${BECOM_OS_HEADERS}
	${BECOM_LOGGING_HEADERS})

target_include_directories(becom PRIVATE ${BECOM_SOURCE_DIR})

target_link_libraries(becom PUBLIC irrExt stdc++fs)

target_include_directories(becom PUBLIC ${BECOM_INCLUDE_DIR}) # For the SDK

file(COPY ${BECOM_HEADERS} DESTINATION ${BECOM_INCLUDE_DIR}/de/hampe) # For the SDK
file(COPY ${BECOM_COLOR_HEADERS} DESTINATION ${BECOM_INCLUDE_DIR}/de/hampe/color) # For the SDK
file(COPY ${BECOM_MATH_HEADERS} DESTINATION ${BECOM_INCLUDE_DIR}/de/hampe/math) # For the SDK
file(COPY ${BECOM_OS_HEADERS} DESTINATION ${BECOM_INCLUDE_DIR}/de/hampe/os) # For the SDK
file(COPY ${BECOM_LOGGING_HEADERS} DESTINATION ${BECOM_INCLUDE_DIR}/de/hampe/logging) # For the SDK

### include(CopyFiles) # For the SDK

### copy_sdk_header_files(${BECOM_INCLUDE_DIR} ${BECOM_SOURCE_DIR} ${BECOM_HEADERS}) # For the SDK
