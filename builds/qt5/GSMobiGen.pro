TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    ../../source/GSMobiHelper.h \
    ../../source/GSMobiHelper.cpp \
    ../../source/GSMobiPacker.h \
    ../../source/GSMobiPacker.cpp \
    ../../source/GSPdbHelper.h \
    ../../source/GSPdbHelper.cpp \
    ../../source/GSPdbPacker.h \
    ../../source/GSPdbPacker.cpp \
    ../../main.cpp

HEADERS += \
    ../../include/GSMobiDef.h \
    ../../include/IGSMobiPacker.h
