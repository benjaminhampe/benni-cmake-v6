INCLUDEPATH += $$PWD

HEADERS +=   \
   $$PWD/de/hampe/PerfTimer.hpp \
   $$PWD/de/hampe/StringUtil.hpp \
   $$PWD/de/hampe/Types.hpp \
   $$PWD/de/hampe/image/Color.hpp \
   $$PWD/de/hampe/image/ColorFormat.hpp \
   $$PWD/de/hampe/image/Image.hpp \
   $$PWD/de/hampe/image/Types.hpp \
   $$PWD/de/hampe/logging/LogLevel.hpp \
   $$PWD/de/hampe/logging/Logger.hpp \
   $$PWD/de/hampe/logging/StdOutLogger.hpp \
   $$PWD/de/hampe/logging/Types.hpp \
   $$PWD/de/hampe/math/Types.hpp \
   $$PWD/de/hampe/math/sinCosTable.hpp \
   $$PWD/de/hampe/os/FileFinderSTL.hpp \
   $$PWD/de/hampe/os/FileMagic.hpp \
   $$PWD/de/hampe/os/FileSystemSTL.hpp \
   $$PWD/de/hampe/os/FileType.hpp \
   $$PWD/de/hampe/os/SysInfo.hpp \
   $$PWD/de/hampe/os/Types.hpp \
   $$PWD/de/hampe/ostreamGLM.hpp \
   $$PWD/de/hampe/sprintf.hpp
SOURCES +=   \
   $$PWD/de/hampe/PerfTimer.cpp \
   $$PWD/de/hampe/StringUtil.cpp \
   $$PWD/de/hampe/image/Color.cpp \
   $$PWD/de/hampe/image/Image.cpp \
   $$PWD/de/hampe/logging/StdOutLogger.cpp \
   $$PWD/de/hampe/os/FileFinderSTL.cpp \
   $$PWD/de/hampe/os/FileMagic.cpp \
   $$PWD/de/hampe/os/FileSystemSTL.cpp \
   $$PWD/de/hampe/os/FileType.cpp \
   $$PWD/de/hampe/os/SysInfo.hpp
   
LIBS += -lstdc++fs
