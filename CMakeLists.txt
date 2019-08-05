cmake_minimum_required(VERSION 3.10)

project(benni-cmake-v6.6)

if(NOT MSVC)
  string(ASCII 27 Esc)
  set(ColourReset "${Esc}[m")
  set(ColourBold  "${Esc}[1m")
  set(Red         "${Esc}[31m")
  set(Green       "${Esc}[32m")
  set(Yellow      "${Esc}[33m")
  set(Blue        "${Esc}[34m")
  set(Magenta     "${Esc}[35m")
  set(Cyan        "${Esc}[36m")
  set(White       "${Esc}[37m")
  set(BoldRed     "${Esc}[1;31m")
  set(BoldGreen   "${Esc}[1;32m")
  set(BoldYellow  "${Esc}[1;33m")
  set(BoldBlue    "${Esc}[1;34m")
  set(BoldMagenta "${Esc}[1;35m")
  set(BoldCyan    "${Esc}[1;36m")
  set(BoldWhite   "${Esc}[1;37m")
endif()

set(CMAKE_VERBOSE_MAKEFILE 0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_REQUIRED_DEFINITIONS ${CMAKE_REQUIRED_DEFINITIONS} -D_FILE_OFFSET_BITS=64) # enable fseeko64()
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wfatal-errors") # -Wc++11-extensions
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wfatal-errors")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fext-numeric-literals" ) # Add compiler flags for building executables (-fPIE)
# set(CMAKE_LINKER_FLAGS "-pthread" CACHE STRING "Linker Flags" FORCE)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fpic" ) # Add compiler flags for building executables (-fPIE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpic" ) # Add compiler flags for building executables (-fPIE)

if (BENNI_SDK_SHARED)
   message("${BoldYellow}[benni] Build shared SDK${ColourReset}")
   # set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++ -static-libgcc")
else()
   message("${BoldYellow}[benni] Build static SDK${ColourReset}")
   # set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++ -static-libgcc")
endif()

# to distinguish between debug and release lib
set(CMAKE_DEBUG_POSTFIX "d")

### =============================================================
###
### Detect host operating system HOST_OS
###
### =============================================================

message("${BoldYellow}[benni] Detect OS...${ColourReset}")

if (APPLE)
   SET(BENNI_HOST_OS "macosx")
elseif (WIN32)
   SET(BENNI_HOST_OS "win")
elseif (UNIX)
   SET(BENNI_HOST_OS "linux")
endif()

### =============================================================
###
### Detect address size 32|64-Bit of the host arch
###
### =============================================================

message("${BoldYellow}[benni] Detect address size 32|64 bits...${ColourReset}")

set(BENNI_HOST_ARCH "${CMAKE_HOST_SYSTEM_PROCESSOR}")

# We are on a 64 bit host system toolchain amd64|arm64|...
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
   set(BENNI_HOST_SIZE 64)
   set(BENNI_HOST_64 1)
# We are on a 32 bit host system toolchain x86|arm32|...
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
   set(BENNI_HOST_SIZE 32)
   set(BENNI_HOST_64 0)
endif()

### =============================================================
###
##### Detect CMAKE_BUILD_TYPE Debug|Release|MinSizeRel=default
###
### =============================================================

message("${BoldYellow}[benni] Detect build configuration...${ColourReset}")

### Set default configuration MinSizeRel
if (CMAKE_BUILD_TYPE STREQUAL "")
   message("CMAKE_BUILD_TYPE not set, fallback to MinSizeRel")
   set(CMAKE_BUILD_TYPE "MinSizeRel")
endif()

### Settings for Debug configuration
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
   add_definitions(-D_DEBUG=1)
   add_definitions(-DDEBUG=1)
### Settings for Release|MinSizeRel configuration
else()
   add_definitions(-DNDEBUG=1)
endif()

### =============================================================
###
### Combine os name + address size 32|64
###
### =============================================================

set(BENNI_HOST "${BENNI_HOST_OS}${BENNI_HOST_SIZE}") # linux32/64, win32/64
# if (MINGW OR CYGWIN OR MSYS)
# 	set(BENNI_HOST "${BENNI_HOST}-gcc") # win32-gcc
# elseif (MSVC)
# 	set(BENNI_HOST "${BENNI_HOST}-vc") # win32-vcxx
# endif()

### TODO Should be controllable somehow
set(BENNI_TARGET_OS ${BENNI_HOST_OS})
set(BENNI_TARGET_ARCH "${BENNI_HOST_ARCH}")
set(BENNI_TARGET_SIZE ${BENNI_HOST_SIZE})
set(BENNI_TARGET_64 ${BENNI_HOST_64})
set(BENNI_TARGET "${BENNI_TARGET_OS}${BENNI_TARGET_SIZE}_${CMAKE_BUILD_TYPE}") # e.g. win64_Debug, linux64_MinSizeRel, etc...
if (BENNI_SDK_SHARED)
   set(BENNI_TARGET "${BENNI_TARGET}_shared")
else()
   set(BENNI_TARGET "${BENNI_TARGET}_static")
endif()
set(BENNI_SDK_DIR "${CMAKE_CURRENT_LIST_DIR}/sdk/${BENNI_TARGET}") 				 # e.g. ./sdk_win64_Debug, ./sdk_linux64_MinSizeRel, etc...
set(BENNI_SDK_INCLUDE_DIR "${BENNI_SDK_DIR}/include")									 # e.g. ./sdk_win64_Debug/include
set(BENNI_SDK_LIBRARY_DIR "${BENNI_SDK_DIR}/lib")										 # e.g. ./sdk_win64_Debug/lib

# Enable our cmake scripts ( if any )
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/src/lib/cmake")

# Tell cmake where to output sdk/lib and app files
set(CMAKE_INSTALL_PREFIX "${BENNI_SDK_LIBRARY_DIR}") # ?
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/bin/${BENNI_TARGET}") # Where our generated programs end...
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BENNI_SDK_LIBRARY_DIR}") 				# ? Where cmake finds our generated (*.a) libs, where add_library(STATIC) ends...
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BENNI_SDK_LIBRARY_DIR}") 				# ? Where cmake finds our generated (*.a) libs, where add_library(STATIC) ends...
set(CMAKE_LIBRARY_PATH "${CMAKE_LIBRARY_PATH} ${BENNI_SDK_LIBRARY_DIR}")	# ? Where cmake finds our generated (*.a) libs, where add_library(STATIC) ends...

include(PrintVars) # Script

##### DETECT MSYS on Windows with MinGW toolchain, cmake -G "MSYS Makefile"
# if(WIN32)
# if("$ENV{OSTYPE}" STREQUAL "msys")
    # set(CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "MSYS default install path")
# endif()
# endif()

if (WIN32)
   add_definitions(-DWIN32_LEAN_AND_MEAN)
   add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
   add_definitions(-D_CRT_SECURE_NO_WARNINGS) 	# Shut up about unsafe stuff
   add_definitions(-D_CRT_NONSTDC_NO_WARNINGS)
endif()

# if(TREAT_WARNINGS_AS_ERRORS)
#   if(MSVC)
#     set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /WX")
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
#   elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
#     set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror")
# 		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
#   endif()
# endif(TREAT_WARNINGS_AS_ERRORS)

# if( WIN32 AND NOT CYGWIN )
#   set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc" )  # Force to always compile with W4
#   if( CMAKE_CXX_FLAGS MATCHES "/W[0-4]" )
#     string( REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" )
#   else()
#     set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4" )
#   endif()
# elseif( CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX )
#   # Update if necessary
#   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic ${OPENDDL_CXXFLAGS}")
# elseif ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" )
#   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic ${OPENDDL_CXXFLAGS} -Wwrite-strings")
# endif()

# if (COVERALLS)
#     include(Coveralls)
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
#     set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
# endif()

# Include the doc component.
# FIND_PACKAGE( doxygen )
# IF ( DOXYGEN_FOUND )
#     CONFIGURE_FILE( doc/openddlparser_doc.in doc/doxygenfile @ONLY )
#     ADD_CUSTOM_TARGET( doc ALL ${DOXYGEN_EXECUTABLE} doc/doxygenfile
#         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#         COMMENT "Generating API documentation with Doxygen" VERBATIM )
# ENDIF ( DOXYGEN_FOUND )

execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
                WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                OUTPUT_VARIABLE PACKAGE_GIT_VERSION
                ERROR_QUIET
                OUTPUT_STRIP_TRAILING_WHITESPACE)

# CREATE_DOXYGEN_DOC(${MODULE_PATH}/src)

#add_subdirectory(src/lib/fontawesome)
#add_subdirectory(src/bin/test.fontawesome)

add_subdirectory(src/lib/tinyxml2)
#add_subdirectory(src/bin/test.tinyxml2)

add_subdirectory(src/lib/glm)
add_subdirectory(src/bin/test.glm)

add_subdirectory(src/lib/z)
# add_subdirectory(src/bin/test.z)

add_subdirectory(src/lib/bzip2)
# add_subdirectory(src/bin/test.bzip2)

add_subdirectory(src/lib/jpeg)
add_subdirectory(src/bin/test.jpeg)

add_subdirectory(src/lib/png)
add_subdirectory(src/bin/test.png)

add_subdirectory(src/lib/irrlicht-1.8.4)
add_subdirectory(src/bin/test.irrlicht.fpscube)
# add_subdirectory(src/bin/test.irrlicht.qt)

add_subdirectory(src/lib/freetype-2.10.0)
add_subdirectory(src/lib/irrExt)

add_subdirectory(src/lib/irrXML) # New:
add_subdirectory(src/lib/clipper) # New:
add_subdirectory(src/lib/minizip) # New:
add_subdirectory(src/lib/unzip) # New:
add_subdirectory(src/lib/openddlparser) # New:
add_subdirectory(src/lib/open3dgc) # New:
add_subdirectory(src/lib/poly2tri) # New:
add_subdirectory(src/lib/rapidjson) # New:
add_subdirectory(src/lib/utf8cpp) # New:
add_subdirectory(src/lib/assimp-4.1.0) # New: uses clipper open3dgc, openddlparser, rapidjson, z, irrXML ( in irrlicht ), minizip, unzip, RT?, others
add_subdirectory(src/bin/test.assimp) # New:

add_subdirectory(src/lib/fltk-1.3.5) # New:
add_subdirectory(src/bin/test.fltk) # New:

# add_subdirectory(src/lib/fontconfig-2.13.91) # New:
# add_subdirectory(src/lib/freeglut-3.0.0) # New:
# add_subdirectory(src/lib/glui-2.37) # New:
# add_subdirectory(src/lib/glfw-3.3) # New:
# add_subdirectory(src/lib/wxWidgets-3.1.2) # New:

# add_subdirectory(src/lib/Xft) # New:
# add_subdirectory(src/lib/cairo) # New:


# add_subdirectory(src/lib/de.hampe.common)
# add_subdirectory(src/bin/sysinfo)
# add_subdirectory(src/bin/meshview)
# add_subdirectory(src/bin/wohnung)
# add_subdirectory(src/bin/polyline)
# add_subdirectory(src/bin/pioneers3d)
# add_subdirectory(src/bin/test.qt)
# add_subdirectory(src/bin/FontView)
