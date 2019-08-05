cmake_minimum_required(VERSION 3.1)

project(benni-lib-IrrXML CXX)

if (TARGET irrXML)
   message("${BoldYellow}[benni] HEADER_GUARD: ${PROJECT_NAME}${ColourReset}")
else()
   set(IRRXML_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
   set(IRRXML_INCLUDE_DIR ${BENNI_SDK_INCLUDE_DIR}/irrXML)
   set(IRRXML_HEADERS
      ${IRRXML_SOURCE_DIR}/CXMLReaderImpl.h
      ${IRRXML_SOURCE_DIR}/heapsort.h
      ${IRRXML_SOURCE_DIR}/irrArray.h
      ${IRRXML_SOURCE_DIR}/irrString.h
      ${IRRXML_SOURCE_DIR}/irrTypes.h
      ${IRRXML_SOURCE_DIR}/irrXML.h
      ${IRRXML_SOURCE_DIR}/fast_atof.h
      ${IRRXML_SOURCE_DIR}/pstdint.h)
   set(IRRXML_SOURCES
      ${IRRXML_SOURCE_DIR}/irrXML.cpp)

   # message("===============================================")
   message("${BoldMagenta}LIBRARY_NAME: ${PROJECT_NAME}${ColourReset}")
   # message("===============================================")
   # message("CMAKE_CURRENT_LIST_DIR = ${CMAKE_CURRENT_LIST_DIR}")
   # message("IRRXML_INCLUDE_DIR = ${IRRXML_INCLUDE_DIR}")
   # message("IRRXML_SOURCE_DIR = ${IRRXML_SOURCE_DIR}")
   # message("IRRXML_HEADERS = ${IRRXML_HEADERS}")
   # message("IRRXML_SOURCES = ${IRRXML_SOURCES}")

   add_library(irrXML STATIC "")
   target_include_directories(irrXML PRIVATE ${IRRXML_SOURCE_DIR})
   target_sources(irrXML PRIVATE ${IRRXML_SOURCES} ${IRRXML_HEADERS})
   target_include_directories(irrXML PUBLIC ${IRRXML_INCLUDE_DIR})
   file(COPY ${IRRXML_HEADERS} DESTINATION ${IRRXML_INCLUDE_DIR}) # For the SDK

endif()
