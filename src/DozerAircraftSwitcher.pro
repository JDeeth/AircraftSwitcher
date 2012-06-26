# Shared library without any Qt functionality
TEMPLATE = lib
QT -= gui core

CONFIG += warn_on plugin release
CONFIG -= thread qt rtti debug

VERSION = 1.0.0

INCLUDEPATH += ../tools/SDK/CHeaders/XPLM
INCLUDEPATH += ../tools/SDK/CHeaders/Wrappers
INCLUDEPATH += ../tools/SDK/CHeaders/Widgets
INCLUDEPATH += ../tools/boost_1_49_0
INCLUDEPATH += ../tools/OpenAL/src
INCLUDEPATH += ../tools/Dozer/src
INCLUDEPATH += ../tools/ppl/src
INCLUDEPATH += ../tools/SimpleIni/src
LIBS += -L../tools/ppl/libDozerAircraftSwitcher -lppl

# Defined to use X-Plane SDK 2.0 capabilities - no backward compatibility before 9.0
DEFINES += XPLM200
DEFINES += PRIVATENAMESPACE=DozerAircraftSwitcher

win32 {
    DEFINES += APL=0 IBM=1 LIN=0
    LIBS += -L../tools/SDK/Libraries/Win
    LIBS += -lXPLM -lXPWidgets
    TARGET = win.xpl
}

unix:!macx {
    DEFINES += APL=0 IBM=0 LIN=1
    TARGET = lin.xpl
    # WARNING! This requires the latest version of the X-SDK !!!!
    QMAKE_CXXFLAGS += -fvisibility=hidden
}

macx {
    DEFINES += APL=1 IBM=0 LIN=0
    TARGET = mac.xpl
    QMAKE_LFLAGS += -flat_namespace -undefined suppress

    # Build for multiple architectures.
    # The following line is only needed to build universal on PPC architectures.
    # QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.4u.sdk
    # The following line defines for which architectures we build.
    CONFIG += x86 ppc
}

HEADERS += \ 


SOURCES += \
	DozerAircraftSwitcher.cpp
