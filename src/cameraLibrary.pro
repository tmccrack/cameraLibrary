#-------------------------------------------------
#
# Project created by QtCreator 2016-05-06T13:43:55
#
#-------------------------------------------------

QT       -= gui
QT       += network

CONFIG += c++11

TARGET = cameraLibrary
TEMPLATE = lib

DEFINES += CAMERALIBRARY_LIBRARY

SOURCES += cameralibrary.cpp \
    socketclient.cpp

HEADERS += cameralibrary.h\
        #CImg.h
    socketclient.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

unix|win32: LIBS += -L$$PWD/../lib/ -latmcd32m

win32: LIBS += -lAdvapi32 -lgdi32 -luser32 -lshell32

INCLUDEPATH += $$PWD/../include
DEPENDPATH += $$PWD/../include
