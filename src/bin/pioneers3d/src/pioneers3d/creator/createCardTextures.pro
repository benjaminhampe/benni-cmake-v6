TARGET = Pioneers3D
TEMPLATE = app
CONFIG += console c++14
QT += core gui widgets
DEFINES += USE_IRRLICHT
DEFINES += USE_WINDOWS

### Release version
CONFIG(release, debug|release) {
   OBJECTS_DIR = $$PWD/obj/$$TARGET/release
   MOC_DIR = $$PWD/obj/$$TARGET/release/moc
   RCC_DIR = $$PWD/obj/$$TARGET/release/qrc
   UI_DIR = $$PWD/obj/$$TARGET/release/ui
   DESTDIR = $$PWD/bin/win32-gcc
   TARGET = $${TARGET}
}

### Debug version
CONFIG(debug, debug|release) {
   OBJECTS_DIR = $$PWD/obj/$$TARGET/debug
   MOC_DIR = $$PWD/obj/$$TARGET/debug/moc
   RCC_DIR = $$PWD/obj/$$TARGET/debug/qrc
   UI_DIR = $$PWD/obj/$$TARGET/debug/ui
   DESTDIR = $$PWD/bin/win32-gcc
   TARGET = $${TARGET}_debug
   DEFINES += _DEBUG
}

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include ( lib/res/pioneers3d.pri )
include ( lib/lib-awesome/lib-awesome.pri )
include ( lib/lib-tinyxml2/lib-tinyxml2.pri )
include ( lib/lib-irrlicht-1.8.4/lib-irrlicht-1.8.4-mingw530.pri )
include ( lib/lib-glm/lib-glm.pri )

# Sources:
INCLUDEPATH += $$PWD/src

HEADERS	+= \
	src/common/AutoMeshBuffer.hpp \
	src/common/AutoSceneNode.hpp \
	src/common/Camera.hpp \
	src/common/createBox.hpp \
	src/common/createCircle.hpp \
	src/common/createCylinder.hpp \
	src/common/createDevice.hpp \
	src/common/createHexagon.hpp \
	src/common/createRect.hpp \
	src/common/createRoundRect.hpp \
	src/common/createTriangle.hpp \
	src/common/ImageUtils.hpp \
	src/common/sinCosTable.hpp \
	src/common/StringUtils.hpp \
	src/common/TextureManager.hpp \
	src/common/Types.hpp \
    src/pioneers3d/Board.hpp \
    src/pioneers3d/BoardSceneNode.hpp \
    src/pioneers3d/createTileImage.hpp \
    src/pioneers3d/Game.hpp \
    src/pioneers3d/Player.hpp \
    src/pioneers3d/Tile.hpp \
    src/pioneers3d/TileSceneNode.hpp \
    src/pioneers3d/Types.hpp \
    src/pioneers3d/Waypoint.hpp \
    src/pioneers3d/Textures.hpp

SOURCES += \
	src/common/AutoMeshBuffer.cpp \
	src/common/AutoSceneNode.cpp \
	src/common/Camera.cpp \
	src/common/createBox.cpp \
	src/common/createCircle.cpp \
	src/common/createCylinder.cpp \
	src/common/createDevice.cpp \
	src/common/createHexagon.cpp \
	src/common/createRect.cpp \
	src/common/createRoundRect.cpp \
	src/common/createTriangle.cpp \
	src/common/ImageUtils.cpp \
	src/common/StringUtils.cpp \
	src/common/TextureManager.cpp \
	src/common/Types.cpp \
    src/pioneers3d/Board.cpp \
    src/pioneers3d/BoardSceneNode.cpp \
    src/pioneers3d/createTileImage.cpp \
    src/pioneers3d/Game.cpp \
    src/pioneers3d/main_Pioneers3D.cpp \
    src/pioneers3d/Player.cpp \
    src/pioneers3d/Tile.cpp \
    src/pioneers3d/TileSceneNode.cpp \
    src/pioneers3d/Types.cpp \
    src/pioneers3d/Waypoint.cpp \
    src/pioneers3d/Textures.cpp
