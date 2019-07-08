TARGET = Pioneers3D
TEMPLATE = app
DEFINES += USE_WINDOWS
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += console c++17
# QT += core gui widgets
#DEFINES += QT_DEPRECATED_WARNINGS
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# if (NOT MSVC)
##### Link statically with libstdc++ and libc
QMAKE_LDFLAGS += -static-libstdc++
QMAKE_LDFLAGS += -static-libgcc
##### Link dynamically with a libstdc++.so in exe path $ORIGIN, or other path $ORIGIN/../other/path/relative/to/origin
# QMAKE_LDFLAGS += '-Wl,-rpath,$ORIGIN/../lib'
#endif()

win32 {
   TARGET_OS = win
#   QMAKE_LFLAGS_WINDOWS +=
#   !contains(QMAKE_TARGET.arch, x86_64) {
#      message( "Building for 32 bit")
#      TARGET_OS = $${TARGET_OS}32
#   } else {
#      message( "Building for 64 bit")
#      TARGET_OS = $${TARGET_OS}64
#   }
}
unix:!macx {
   TARGET_OS = linux

}
macx: {
   TARGET_OS = macosx
}

contains(QT_ARCH, i386) {
   message( "Building for 32 bit")
   TARGET_OS = $${TARGET_OS}32
} else {
   message( "Building for 64 bit")
   TARGET_OS = $${TARGET_OS}64
}


DESTDIR = $$PWD/../../bin/$${TARGET_OS}

CONFIG(debug, debug|release) {
   TARGET = $${TARGET}.debug
   DEFINES += _DEBUG
   DEFINES += DEBUG
#   OBJECTS_DIR = $$PWD/obj/$$TARGET/debug
#   MOC_DIR = $$PWD/obj/$$TARGET/de.hampe.commondebug/moc
#   RCC_DIR = $$PWD/obj/$$TARGET/debug/qrc
#   UI_DIR = $$PWD/obj/$$TARGET/debug/ui
}
CONFIG(release, debug|release) {
   DEFINES += NDEBUG
#   OBJECTS_DIR = $$PWD/obj/$$TARGET/release
#   MOC_DIR = $$PWD/obj/$$TARGET/release/moc
#   RCC_DIR = $$PWD/obj/$$TARGET/release/qrc
#   UI_DIR = $$PWD/obj/$$TARGET/release/ui
}

QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CXXFLAGS += -Wno-unused-variable
QMAKE_CXXFLAGS += -Wno-return-type
QMAKE_CXXFLAGS += -Wno-comment

# include ( src/pioneers3d/app/app.pri )
include ( ../../lib/lib-awesome/lib-awesome.pri )
include ( ../../lib/lib-tinyxml2/lib-tinyxml2.pri )
include ( ../../lib/lib-glm/lib-glm.pri )
include ( ../../lib/lib-freetype-2.9-static/lib-freetype-2.9-static.pri )
include ( ../../lib/lib-irrlicht-1.8.4-static/lib-irrlicht-1.8.4-static.pri )
include ( ../irrExt/irrExt.pri )
include ( ../de.hampe.common/de.hampe.common.pri )
include ( ../DarkGDK/DarkGDK.pri )
include ( pioneers3d.pri )

