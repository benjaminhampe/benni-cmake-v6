TARGET = de.hampe.svg.polyline
TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -Werror=return-type
QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CXXFLAGS += -Wno-unused-variable
QMAKE_CXXFLAGS += -Wno-comment

DESTDIR = $$PWD/bin
# MOC_DIR = .moc
# OBJECTS_DIR = .obj

### Release version
CONFIG(release, debug|release) {
#   OBJECTS_DIR = $$PWD/obj/$$TARGET/release
#   MOC_DIR = $$PWD/obj/$$TARGET/release/moc
#   RCC_DIR = $$PWD/obj/$$TARGET/release/qrc
#   UI_DIR = $$PWD/obj/$$TARGET/release/ui
#   TARGET = $${TARGET}
}

### Debug version
CONFIG(debug, debug|release) {
#   OBJECTS_DIR = $$PWD/obj/$$TARGET/debug
#   MOC_DIR = $$PWD/obj/$$TARGET/debug/moc
#   RCC_DIR = $$PWD/obj/$$TARGET/debug/qrc
#   UI_DIR = $$PWD/obj/$$TARGET/debug/ui
    TARGET = $${TARGET}_debug
    DEFINES += _DEBUG
}

# include ( lib/lib-awesome/lib-awesome.pri )
# include ( lib/lib-tinyxml2/lib-tinyxml2.pri )
include ( lib/lib-freetype-2.9-static/libFreeType-dev.pri )
include ( lib/lib-irrlicht-1.8.4-static/libIrrlicht-dev.pri )
include ( lib/lib-glm/lib-glm.pri )

DEFINES += USE_WINDOWS

include ( include/de.hampe.pri )
include ( include/de.hampe.svg.pri )
# include ( include/de.hampe.image.pri )

SOURCES += main_polyline.cpp \
    EventReceiver.cpp

HEADERS += \
    EventReceiver.hpp
