cmake_minimum_required(VERSION 3.10)

project(benni-cmake-v6.12)

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
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fext-numeric-literals" ) # Add compiler flags for building executables (-fPIE)
# set(CMAKE_LINKER_FLAGS "-pthread" CACHE STRING "Linker Flags" FORCE)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++ -static-libgcc")

# to distinguish between debug and release lib
set(CMAKE_DEBUG_POSTFIX "d")

### =============================================================
####
##### Detect host operating system HOST_OS
####
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
####
##### Detect address size 32|64-Bit of the host arch
####
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
####
##### Detect CMAKE_BUILD_TYPE Debug|Release|MinSizeRel=default
####
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

set(BENNI_CONFIG "${CMAKE_BUILD_TYPE}")

### =============================================================
####
##### Combine os name + address size 32|64
####
### =============================================================

set(BENNI_HOST "${BENNI_HOST_OS}${BENNI_HOST_SIZE}") # linux32/64, win32/64
# if (MINGW OR CYGWIN OR MSYS)
# 	set(BENNI_HOST "${BENNI_HOST}-gcc") # win32-gcc
# elseif (MSVC)
# 	set(BENNI_HOST "${BENNI_HOST}-vc") # win32-vcxx
# endif()

# TODO: Should be controllable somehow
set(BENNI_TARGET_OS ${BENNI_HOST_OS})
set(BENNI_TARGET_ARCH "${BENNI_HOST_ARCH}") 
set(BENNI_TARGET_SIZE ${BENNI_HOST_SIZE})
set(BENNI_TARGET_64 ${BENNI_HOST_64})
set(BENNI_TARGET "${BENNI_TARGET_OS}${BENNI_TARGET_SIZE}")
set(BENNI_SDK_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/sdk/include")
set(BENNI_SDK_LIBRARY_DIR "${CMAKE_CURRENT_LIST_DIR}/sdk/lib/${BENNI_TARGET}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/bin/${BENNI_TARGET}") ### Where add_executable() ends...
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BENNI_SDK_LIBRARY_DIR}") ### Where add_library(STATIC) ends...
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BENNI_SDK_LIBRARY_DIR}") ### Where add_library(STATIC) ends...
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/src/cmake) # Enable our scripts
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_LIST_DIR}/sdk) # Enable our scripts
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${CMAKE_CURRENT_LIST_DIR}/sdk/lib/${BENNI_TARGET}")

include(PrintVars) # Script

##### DETECT MSYS on Windows with MinGW toolchain, cmake -G "MSYS Makefile"
# if(WIN32)
# if("$ENV{OSTYPE}" STREQUAL "msys")
    # set(CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "MSYS default install path")
# endif()
# endif()

if(MSVC)
	add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS) 	# Shut up about unsafe stuff
	add_definitions(-D_CRT_NONSTDC_NO_WARNINGS)
endif()

if(TREAT_WARNINGS_AS_ERRORS)
  if(MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /WX")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
  elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
  endif()
endif(TREAT_WARNINGS_AS_ERRORS)

execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
                WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                OUTPUT_VARIABLE PACKAGE_GIT_VERSION
                ERROR_QUIET
                OUTPUT_STRIP_TRAILING_WHITESPACE)

# CREATE_DOXYGEN_DOC(${MODULE_PATH}/src)

#add_subdirectory(src/font-awesome)
#add_subdirectory(src/lib-tinyxml2)
add_subdirectory(src/test.qt)

add_subdirectory(src/lib-glm)
add_subdirectory(src/test.glm)

add_subdirectory(src/lib-z)
# add_subdirectory(src/test.z)

add_subdirectory(src/lib-bzip2)
# add_subdirectory(src/test.bzip2)

add_subdirectory(src/lib-jpeg)
add_subdirectory(src/test.jpeg)

add_subdirectory(src/lib-png)
add_subdirectory(src/test.png)

add_subdirectory(src/lib-irrlicht-1.8.4)
add_subdirectory(src/test.irrlicht.fpscube)

add_subdirectory(src/lib-freetype-2.10.0)
add_subdirectory(src/irrExt)
add_subdirectory(src/de.hampe.common)
add_subdirectory(src/pioneers3d)

add_subdirectory(src/polyline-standalone)

add_subdirectory(src/FontView)
