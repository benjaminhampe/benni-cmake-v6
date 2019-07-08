cmake_minimum_required( VERSION 3.0 )

project(test.qt)

set(CMAKE_INCLUDE_CURRENT_DIR ON ) # Find includes in corresponding build directories
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fext-numeric-literals" ) # Add compiler flags for building executables (-fPIE)

##### find Qt package
set(CMAKE_AUTOMOC ON) # Disable automoc (some files contain qt specific keywords which lead to compile error when processed by the qt pre-compiler)
find_package(Qt5Widgets CONFIG REQUIRED) # Find the QtWidgets library
include_directories(SYSTEM ${Qt5Widgets_INCLUDE_DIRS}) # Add the include directories for the Qt 5 Widgets module to the compile lines.
add_definitions(${Qt5Widgets_DEFINITIONS}) # Use the compile definitions defined in the Qt 5 Widgets module
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}" )
QT5_WRAP_CPP(MOC_FILES_CPP "MainWindow.hpp" "MainWindow.cpp")
# QT5_WRAP_UI( MAPGYVER_UI "ui/mainwindow.ui" )
set(PROJ_SOURCES "${PROJ_SOURCES} ${MOC_FILES_CPP}")
# module_include_directories(PRIVATE ${MODULE_PATH})

##### Add project
add_executable(${PROJECT_NAME} "")

set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

file(GLOB PROJ_SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.[ch]pp)

target_sources(${PROJECT_NAME} PRIVATE ${PROJ_SOURCES})
# target_link_libraries(${PROJECT_NAME} PUBLIC de.hampe.common)

target_link_libraries(${PROJECT_NAME} PUBLIC "Qt5::Widgets")

### copy resource files
# file(COPY ${CMAKE_CURRENT_LIST_DIR}/pcc_logo.png DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})