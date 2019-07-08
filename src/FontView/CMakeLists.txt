cmake_minimum_required( VERSION 3.0 )

project(fontview)

set(FV_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(FV_SOURCES
	"${FV_SOURCE_DIR}/main.cpp"
	"${FV_SOURCE_DIR}/FontGlyphInfoWidget.hpp"
	"${FV_SOURCE_DIR}/FontGlyphInfoWidget.cpp"
	) # Sources without moc for qt?

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fext-numeric-literals" ) # Add compiler flags for building executables (-fPIE)

##### find Qt package
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

message("===============================================")
message("${BoldBlue}PROJECT_NAME: ${PROJECT_NAME}${ColourReset}")
message("===============================================")
message("CMAKE_CURRENT_LIST_DIR = ${CMAKE_CURRENT_LIST_DIR}")
message("FV_SOURCE_DIR = ${FV_SOURCE_DIR}")
message("FV_SOURCE_DIR = ${FV_SOURCE_DIR}")
message("FV_INCLUDE_DIR = ${FV_INCLUDE_DIR}")

##### Add project
add_executable(fontview "")

set_target_properties(fontview PROPERTIES POSITION_INDEPENDENT_CODE TRUE) # -fPic

target_include_directories(fontview PRIVATE ${FV_SOURCE_DIR})

target_include_directories(fontview PUBLIC ${PROJECT_BINARY_DIR}) # For QT ?

target_sources(fontview PRIVATE ${FV_SOURCES})

target_link_libraries(fontview 
	PUBLIC 
	Qt5::Widgets
    Qt5::Core
    Qt5::Gui) #     Qt5::OpenGL

# file(COPY ${CMAKE_CURRENT_LIST_DIR}/themes/darcula.css DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}) ### copy resource files