cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

project(pioneers3d)

set(PIONEERS3D_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/src/pioneers3d")
set(PIONEERS3D_INCLUDE_DIR "${BENNI_SDK_INCLUDE_DIR}/pioneers3d")

set(PIONEERS3D_SOURCES
	${PIONEERS3D_SOURCE_DIR}/types/Action.cpp
	${PIONEERS3D_SOURCE_DIR}/types/Bank.cpp
	${PIONEERS3D_SOURCE_DIR}/types/Board.cpp
	${PIONEERS3D_SOURCE_DIR}/types/BoardObjects.cpp
	${PIONEERS3D_SOURCE_DIR}/types/Card.cpp
	${PIONEERS3D_SOURCE_DIR}/types/Chat.cpp
	${PIONEERS3D_SOURCE_DIR}/types/Common.cpp
	${PIONEERS3D_SOURCE_DIR}/types/CylinderCamera.cpp
	${PIONEERS3D_SOURCE_DIR}/types/Dice.cpp
	${PIONEERS3D_SOURCE_DIR}/types/EventReceiver.cpp
	${PIONEERS3D_SOURCE_DIR}/types/Font.cpp
	${PIONEERS3D_SOURCE_DIR}/types/Game.cpp
	${PIONEERS3D_SOURCE_DIR}/types/GameState.cpp
	${PIONEERS3D_SOURCE_DIR}/types/GameType.cpp
	${PIONEERS3D_SOURCE_DIR}/types/Marquee.cpp
	${PIONEERS3D_SOURCE_DIR}/types/PlaceObjectType.cpp
	${PIONEERS3D_SOURCE_DIR}/types/Player.cpp
	${PIONEERS3D_SOURCE_DIR}/types/Texture.cpp
	${PIONEERS3D_SOURCE_DIR}/types/TextureType.cpp
	${PIONEERS3D_SOURCE_DIR}/types/Tile.cpp
	${PIONEERS3D_SOURCE_DIR}/types/TileType.cpp
	${PIONEERS3D_SOURCE_DIR}/types/UI.cpp
	${PIONEERS3D_SOURCE_DIR}/types/Waypoint.cpp
	${PIONEERS3D_SOURCE_DIR}/scene/TileSceneNode.cpp
	${PIONEERS3D_SOURCE_DIR}/scene/BoardSceneNode.cpp
	${PIONEERS3D_SOURCE_DIR}/scene/PolyLineSceneNode.cpp
	${PIONEERS3D_SOURCE_DIR}/StandardGame.cpp
	${PIONEERS3D_SOURCE_DIR}/main_Pioneers3D.cpp)

set(PIONEERS3D_HEADERS
	${PIONEERS3D_SOURCE_DIR}/types/Action.hpp
	${PIONEERS3D_SOURCE_DIR}/types/Bank.hpp
	${PIONEERS3D_SOURCE_DIR}/types/Board.hpp
	${PIONEERS3D_SOURCE_DIR}/types/BoardObjects.hpp
	${PIONEERS3D_SOURCE_DIR}/types/Card.hpp
	${PIONEERS3D_SOURCE_DIR}/types/Chat.hpp
	${PIONEERS3D_SOURCE_DIR}/types/Common.hpp
	${PIONEERS3D_SOURCE_DIR}/types/CylinderCamera.hpp
	${PIONEERS3D_SOURCE_DIR}/types/Dice.hpp
	${PIONEERS3D_SOURCE_DIR}/types/EventReceiver.hpp
	${PIONEERS3D_SOURCE_DIR}/types/Font.hpp
	${PIONEERS3D_SOURCE_DIR}/types/Game.hpp
	${PIONEERS3D_SOURCE_DIR}/types/GameState.hpp
	${PIONEERS3D_SOURCE_DIR}/types/GameType.hpp
	${PIONEERS3D_SOURCE_DIR}/types/Marquee.hpp
	${PIONEERS3D_SOURCE_DIR}/types/PlaceObjectType.hpp
	${PIONEERS3D_SOURCE_DIR}/types/Player.hpp
	${PIONEERS3D_SOURCE_DIR}/types/Texture.hpp
	${PIONEERS3D_SOURCE_DIR}/types/TextureType.hpp
	${PIONEERS3D_SOURCE_DIR}/types/Tile.hpp
	${PIONEERS3D_SOURCE_DIR}/types/TileType.hpp
	${PIONEERS3D_SOURCE_DIR}/types/UI.hpp
	${PIONEERS3D_SOURCE_DIR}/types/Waypoint.hpp
	${PIONEERS3D_SOURCE_DIR}/scene/TileSceneNode.hpp
	${PIONEERS3D_SOURCE_DIR}/scene/PolyLineSceneNode.hpp
	${PIONEERS3D_SOURCE_DIR}/scene/BoardSceneNode.hpp
	${PIONEERS3D_SOURCE_DIR}/StandardGame.hpp)

message("===============================================")
message("${BoldBlue}PROJECT_NAME: ${PROJECT_NAME}${ColourReset}")
message("===============================================")
message("CMAKE_CURRENT_LIST_DIR = ${CMAKE_CURRENT_LIST_DIR}")
message("PROJ_SOURCE_DIR = ${PROJ_SOURCE_DIR}")
message("PIONEERS3D_SOURCE_DIR = ${PIONEERS3D_SOURCE_DIR}")
message("PIONEERS3D_INCLUDE_DIR = ${PIONEERS3D_INCLUDE_DIR}")

add_executable(pioneers3d "")

target_include_directories(pioneers3d PRIVATE ${PIONEERS3D_SOURCE_DIR}/..)

target_sources(pioneers3d PRIVATE ${PIONEERS3D_SOURCES} ${PIONEERS3D_HEADERS})

target_link_libraries(pioneers3d PUBLIC becom irrExt freetype Irrlicht png jpeg bzip2 z glm m)