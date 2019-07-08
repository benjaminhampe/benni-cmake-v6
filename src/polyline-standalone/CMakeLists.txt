cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

project(polyline-standalone)

set(BESVG_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/src")
set(BESVG_INCLUDE_DIR "${BENNI_SDK_INCLUDE_DIR}/de.hampe.polyline")

set(BESVG_SOURCES
	${BESVG_SOURCE_DIR}/de/hampe/svg/addTextBillboard.cpp
	${BESVG_SOURCE_DIR}/de/hampe/svg/IntersectionTool2D.cpp
	${BESVG_SOURCE_DIR}/de/hampe/svg/LineCap.cpp
	${BESVG_SOURCE_DIR}/de/hampe/svg/LineCapGenerator.cpp
	${BESVG_SOURCE_DIR}/de/hampe/svg/LineJoin.cpp
	${BESVG_SOURCE_DIR}/de/hampe/svg/LineJoinGenerator.cpp
	${BESVG_SOURCE_DIR}/de/hampe/svg/Math.cpp
	${BESVG_SOURCE_DIR}/de/hampe/svg/MiterPolyLine.cpp
	${BESVG_SOURCE_DIR}/de/hampe/svg/PolyLine.cpp
	${BESVG_SOURCE_DIR}/de/hampe/svg/PrimitiveRenderer.cpp
	${BESVG_SOURCE_DIR}/de/hampe/svg/TexturedMiterPolyLine.cpp
	${BESVG_SOURCE_DIR}/PointGenerator.cpp
	${BESVG_SOURCE_DIR}/PolyLineTest.cpp
	${BESVG_SOURCE_DIR}/EventReceiver.cpp	
	${BESVG_SOURCE_DIR}/main_polyline.cpp)

set(BESVG_HEADERS
	${BESVG_SOURCE_DIR}/de/hampe/svg/addTextBillboard.hpp
	${BESVG_SOURCE_DIR}/de/hampe/svg/IntersectionTool2D.hpp
	${BESVG_SOURCE_DIR}/de/hampe/svg/LineCapGenerator.hpp
	${BESVG_SOURCE_DIR}/de/hampe/svg/LineCap.hpp
	${BESVG_SOURCE_DIR}/de/hampe/svg/LineJoinGenerator.hpp
	${BESVG_SOURCE_DIR}/de/hampe/svg/LineJoin.hpp
	${BESVG_SOURCE_DIR}/de/hampe/svg/Math.hpp
	${BESVG_SOURCE_DIR}/de/hampe/svg/MiterPolyLine.hpp
	${BESVG_SOURCE_DIR}/de/hampe/svg/PolyLine.hpp
	${BESVG_SOURCE_DIR}/de/hampe/svg/PrimitiveRenderer.hpp
	${BESVG_SOURCE_DIR}/de/hampe/svg/TexturedMiterPolyLine.hpp
	${BESVG_SOURCE_DIR}/PointGenerator.hpp
	${BESVG_SOURCE_DIR}/PolyLineTest.hpp
	${BESVG_SOURCE_DIR}/EventReceiver.hpp)

message("===============================================")
message("${BoldBlue}PROJECT_NAME: ${PROJECT_NAME}${ColourReset}")
message("===============================================")
message("CMAKE_CURRENT_LIST_DIR = ${CMAKE_CURRENT_LIST_DIR}")
message("POLYLINE_SOURCE_DIR = ${BESVG_SOURCE_DIR}")
message("POLYLINE_INCLUDE_DIR = ${BESVG_INCLUDE_DIR}")

add_executable(polyline-standalone "")

target_include_directories(polyline-standalone PRIVATE ${BESVG_SOURCE_DIR})

target_sources(polyline-standalone PRIVATE ${BESVG_SOURCES} ${BESVG_HEADERS})

target_link_libraries(polyline-standalone PUBLIC becom irrExt freetype Irrlicht png jpeg bzip2 z glm m)