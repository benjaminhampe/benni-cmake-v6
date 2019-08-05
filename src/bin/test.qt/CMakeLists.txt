cmake_minimum_required( VERSION 3.0 )

project(test.qt)

set(TEST_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)

set(TEST_SOURCES
	 ${TEST_SOURCE_DIR}/MainWindow.cpp
	 ${TEST_SOURCE_DIR}/MainWindow.hpp
	 ${TEST_SOURCE_DIR}/main.cpp)

set(CMAKE_INCLUDE_CURRENT_DIR ON) # Find includes in corresponding build directories
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fext-numeric-literals" ) # Add compiler flags for building executables (-fPIE)

### find Qt package
set(CMAKE_AUTOMOC ON) # Disable automoc (some files contain qt specific keywords which lead to compile error when processed by the qt pre-compiler)
find_package(Qt5Core CONFIG REQUIRED) # Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED) # Find the QtWidgets library
find_package(Qt5Gui CONFIG REQUIRED) # Find the QtWidgets library
include_directories(SYSTEM ${Qt5Core_INCLUDE_DIRS}) # Add the include directories for the Qt 5 Widgets module to the compile lines.
include_directories(SYSTEM ${Qt5Widgets_INCLUDE_DIRS}) # Add the include directories for the Qt 5 Widgets module to the compile lines.
include_directories(SYSTEM ${Qt5Gui_INCLUDE_DIRS}) # Add the include directories for the Qt 5 Widgets module to the compile lines.
add_definitions(${Qt5Core_DEFINITIONS}) # Use the compile definitions defined in the Qt 5 Widgets module
add_definitions(${Qt5Widgets_DEFINITIONS}) # Use the compile definitions defined in the Qt 5 Widgets module
add_definitions(${Qt5Gui_DEFINITIONS}) # Use the compile definitions defined in the Qt 5 Widgets module
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Core_EXECUTABLE_COMPILE_FLAGS}" )
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}" )
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Gui_EXECUTABLE_COMPILE_FLAGS}" )

# QT5_WRAP_CPP(MOC_FILES
# 			"${FV_SOURCE_DIR}/FontGlyphInfoWidget.hpp" 
# 			"${FV_SOURCE_DIR}/FontGlyphInfoWidget.cpp")
# QT5_WRAP_UI( FONTVIEW_UI "${FV_SOURCE_DIR}/FontGlyphInfoWidget.ui" )

QT5_WRAP_CPP(MOC_FILES ${TEST_SOURCES})
# set(${TEST_SOURCES} ${TEST_SOURCES} ${MOC_FILES_CPP})
# QT5_WRAP_UI( MAPGYVER_UI "ui/mainwindow.ui" )
# set(TEST_SOURCES ${TEST_SOURCES} ${MOC_FILES_CPP})
# module_include_directories(PRIVATE ${MODULE_PATH})

message("===============================================")
message("${BoldBlue}PROJECT_NAME: ${PROJECT_NAME}${ColourReset}")
message("===============================================")
message("CMAKE_CURRENT_LIST_DIR = ${CMAKE_CURRENT_LIST_DIR}")
message("TEST_SOURCE_DIR = ${TEST_SOURCE_DIR}")
message("MOC_FILES = ${MOC_FILES}")
message("MOC_FILES_CPP = ${MOC_FILES_CPP}")

### Add project
add_executable(test.qt "")

set_target_properties(test.qt PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

target_include_directories(test.qt PRIVATE ${TEST_SOURCE_DIR})

target_sources(test.qt PUBLIC ${MOC_FILES_CPP})

target_link_libraries(test.qt PUBLIC
	Qt5::Widgets
    Qt5::Core
    Qt5::Gui
	 Irrlicht) #     Qt5::OpenGL

### copy resource files
# file(COPY ${CMAKE_CURRENT_LIST_DIR}/pcc_logo.png DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})