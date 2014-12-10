TEMPLATE = app
QT = core
CONFIG += console
CONFIG -= app_bundle

SOURCES += main.cpp \
    QueryWMI.cpp \
    QuerySink.cpp \
    AbstractQuery.cpp

HEADERS += \
    QueryWMI.h \
    QuerySink.h \
    AbstractQuery.h

