#-------------------------------------------------
#
# Project created by QtCreator 2014-05-20T08:49:49
#
#-------------------------------------------------

QT       += core gui

TARGET = plugin_ocpm
TEMPLATE = lib
CONFIG			 += debug plugin


SOURCES += \
    plugin_ocpm.cpp

HEADERS  += \
    plugin_ocpm.h

FORMS    += \
    plugin_ocpm.ui

DESTDIR			 = ./pluginsOPP

LIBS += -L$$PWD/../lib/ -lopp
INCLUDEPATH += $$PWD/../include
