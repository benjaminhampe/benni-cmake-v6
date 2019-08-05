cmake_minimum_required(VERSION 3.1)

project(benni-lib-irrExt)

if (TARGET irrExt)
   message("${BoldYellow}[benni] HEADER_GUARD: ${PROJECT_NAME}${ColourReset}")
else()

   set(IRREXT_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/src/irrExt")
   set(IRREXT_INCLUDE_DIR "${BENNI_SDK_INCLUDE_DIR}/irrExt")
   set(IRREXT_SOURCES
      ${IRREXT_SOURCE_DIR}/AutoMeshBuffer.cpp
      ${IRREXT_SOURCE_DIR}/AutoMeshBufferUtils.cpp
      ${IRREXT_SOURCE_DIR}/AutoSceneNode.cpp
      ${IRREXT_SOURCE_DIR}/BaseEventReceiver.cpp
      ${IRREXT_SOURCE_DIR}/BaseWindow.cpp
      ${IRREXT_SOURCE_DIR}/Camera.cpp
      ${IRREXT_SOURCE_DIR}/CylinderCamera.cpp
      ${IRREXT_SOURCE_DIR}/CGUITTFont.cpp
      ${IRREXT_SOURCE_DIR}/CGUIWindow.cpp
      ${IRREXT_SOURCE_DIR}/NullDevice.cpp
      ${IRREXT_SOURCE_DIR}/OpenGLDevice.cpp
      ${IRREXT_SOURCE_DIR}/createCircle.cpp
      ${IRREXT_SOURCE_DIR}/createRoundRect.cpp
      ${IRREXT_SOURCE_DIR}/CXMLWriterUTF8.cpp
      ${IRREXT_SOURCE_DIR}/FileSystem.cpp
      ${IRREXT_SOURCE_DIR}/FontUtils.cpp
      ${IRREXT_SOURCE_DIR}/GlmUtils.cpp
      ${IRREXT_SOURCE_DIR}/ImageUtils.cpp
      ${IRREXT_SOURCE_DIR}/irrExt.cpp
      ${IRREXT_SOURCE_DIR}/libFFT.cpp
      ${IRREXT_SOURCE_DIR}/MeshLoader.cpp
      ${IRREXT_SOURCE_DIR}/PerfTimer.cpp
      ${IRREXT_SOURCE_DIR}/StringUtils.cpp
      ${IRREXT_SOURCE_DIR}/Types.cpp)

   set(IRREXT_HEADERS
      ${IRREXT_SOURCE_DIR}/AutoMeshBuffer.hpp
      ${IRREXT_SOURCE_DIR}/AutoMeshBufferUtils.hpp
      ${IRREXT_SOURCE_DIR}/AutoSceneNode.hpp
      ${IRREXT_SOURCE_DIR}/BaseEventReceiver.hpp
      ${IRREXT_SOURCE_DIR}/BaseWindow.hpp
      ${IRREXT_SOURCE_DIR}/Camera.hpp
      ${IRREXT_SOURCE_DIR}/CylinderCamera.hpp
      ${IRREXT_SOURCE_DIR}/CGUITTFont.hpp
      ${IRREXT_SOURCE_DIR}/CGUIWindow.hpp
      ${IRREXT_SOURCE_DIR}/NullDevice.hpp
      ${IRREXT_SOURCE_DIR}/OpenGLDevice.hpp
      ${IRREXT_SOURCE_DIR}/createCircle.hpp
      ${IRREXT_SOURCE_DIR}/createRoundRect.hpp
      ${IRREXT_SOURCE_DIR}/CXMLWriterUTF8.hpp
      ${IRREXT_SOURCE_DIR}/FileSystem.hpp
      ${IRREXT_SOURCE_DIR}/FontUtils.hpp
      ${IRREXT_SOURCE_DIR}/GlmUtils.hpp
      ${IRREXT_SOURCE_DIR}/ImageUtils.hpp
      ${IRREXT_SOURCE_DIR}/irrExt.hpp
      ${IRREXT_SOURCE_DIR}/libFFT.hpp
      ${IRREXT_SOURCE_DIR}/MeshLoader.hpp
      ${IRREXT_SOURCE_DIR}/PerfTimer.hpp
      ${IRREXT_SOURCE_DIR}/sinCosTable.hpp
      ${IRREXT_SOURCE_DIR}/StringUtils.hpp
      ${IRREXT_SOURCE_DIR}/ustring.hpp
      ${IRREXT_SOURCE_DIR}/Types.hpp)

	if (BENNI_SDK_SHARED)
		message("${Red}SHARED_LIBRARY: ${PROJECT_NAME}${ColourReset}")
		add_library(irrExt SHARED "")	
	else()
		message("${Red}STATIC_LIBRARY: ${PROJECT_NAME}${ColourReset}")
		add_library(irrExt STATIC "")
	endif()
	   
   target_sources(irrExt PRIVATE ${IRREXT_SOURCES} ${IRREXT_HEADERS})
   target_include_directories(irrExt PRIVATE ${IRREXT_SOURCE_DIR}/..) # go up -> now we can do <irrExt/xxx.hpp>
   target_include_directories(irrExt PRIVATE ${BENNI_SDK_INCLUDE_DIR}) # go up -> now we can do <irrExt/xxx.hpp>
   target_link_libraries(irrExt PUBLIC freetype Irrlicht glm png jpeg bzip2 z m stdc++fs)

   target_include_directories(irrExt PUBLIC ${IRREXT_INCLUDE_DIR}/..) # For the SDK
   file(COPY ${IRREXT_HEADERS} DESTINATION ${IRREXT_INCLUDE_DIR}) # For the SDK

endif()
