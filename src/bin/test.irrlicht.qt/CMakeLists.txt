cmake_minimum_required(VERSION 3.0)

project(test.irrlicht.qt CXX)

set(APP_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(APP_INCLUDE_DIR ${BENNI_SDK_INCLUDE_DIR}/irrExt)

set(APP_SOURCES
	${APP_SOURCE_DIR}/irrExt/QIrrlichtWidget2019.cpp
	${APP_SOURCE_DIR}/irrExt/QIrrlichtWidget2019.hpp
	${APP_SOURCE_DIR}/MainWindow.cpp
	${APP_SOURCE_DIR}/MainWindow.hpp
	${APP_SOURCE_DIR}/main.cpp)

### find  package
set(CMAKE_AUTOMOC ON) # Qt5 :: Disable automoc (some files contain qt specific keywords which lead to compile error when processed by the qt pre-compiler)
find_package(Qt5Widgets CONFIG REQUIRED) # Qt5 :: Find the QtWidgets library
include_directories(SYSTEM ${Qt5Widgets_INCLUDE_DIRS}) # Qt5 :: Add the include directories for the Qt 5 Widgets module to the compile lines.
add_definitions(${Qt5Widgets_DEFINITIONS}) # Qt5 :: Use the compile definitions defined in the Qt 5 Widgets module
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}" ) # Qt5 :: 
QT5_WRAP_CPP(MOC_FILES_CPP ${APP_SOURCES}) # Qt5 :: 
### QT5_WRAP_UI( MAPGYVER_UI "ui/mainwindow.ui" )
set(APP_SOURCES ${APP_SOURCES} ${MOC_FILES_CPP})

set(CMAKE_INCLUDE_CURRENT_DIR ON) # Find includes in corresponding build directories
### set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fext-numeric-literals" ) # Add compiler flags for building executables (-fPIE)

##### Add project
add_executable(${PROJECT_NAME} "")
set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
target_sources(${PROJECT_NAME} PRIVATE ${APP_SOURCES})

target_link_libraries(${PROJECT_NAME} PUBLIC "Qt5::Widgets")
target_link_libraries(${PROJECT_NAME} PUBLIC Irrlicht)
# target_link_libraries(${PROJECT_NAME} PUBLIC de.hampe.common)

### copy resource files
### file(COPY ${CMAKE_CURRENT_LIST_DIR}/tux.png DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}) ### copy resource files
# file(COPY ${CMAKE_CURRENT_LIST_DIR}/pcc_logo.png DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})