DEFINES += USE_LIB_GLM

win32 {
   INCLUDEPATH += $$PWD/include
}
unix:!macx {
   # INCLUDEPATH += /usr/include
}
macx: {
   # INCLUDEPATH += /usr/include
}

# HEADERS += $$PWD/include/glm/glm.hpp
# HEADERS += $$PWD/include/GLM/gtc/*.hpp
# HEADERS += $$PWD/include/GLM/gtx/*.hpp
