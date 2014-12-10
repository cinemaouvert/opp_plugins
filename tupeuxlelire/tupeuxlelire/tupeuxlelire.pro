#-------------------------------------------------
#
# Project created by QtCreator 2014-05-21T15:23:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tupeuxlelire
TEMPLATE = app


SOURCES += main.cpp\
    Info.cpp \
    ProviderLinux.cpp \
    AbstractProvider.cpp \
    Util.cpp \
    ProviderMac.cpp \
    FenInfo.cpp \
    ProviderWin32.cpp \
    ProviderWMI.cpp \
    QuerySink.cpp

HEADERS  += \
    Info.h \
    ProviderLinux.h \
    AbstractProvider.h \
    Util.h \
    ProviderMac.h \
    FenInfo.h \
    ProviderWin32.h \
    ProviderWMI.h \
    QuerySink.h \
    Environment.h

FORMS    += \
    FenInfo.ui
