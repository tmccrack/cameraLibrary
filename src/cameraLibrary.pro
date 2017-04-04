
#-------------------------------------------------
#
# Project created by QtCreator 2016-05-06T13:43:55
#
#-------------------------------------------------

QT       -= gui
QT       += network

CONFIG += c++11

TARGET = camera
TEMPLATE = lib

DEFINES += CAMERALIBRARY_LIBRARY

SOURCES += \
    camera.cpp \
    camerathread.cpp \
    socketclient.cpp \
    imageservo.cpp \
    servo.cpp \
    datalogger.cpp

HEADERS +=\
    camera.h \
    camerathread.h \
    socketclient.h \
    imageservo.h \
    servo.h \
    datalogger.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

unix|win32: LIBS += -L$$PWD/../lib/ -latmcd32m

win32: LIBS += -lAdvapi32 -lgdi32 -luser32 -lshell32
#win32:CONFIG(release): DLLDESTDIR = "C:\Program Files (x86)\National Instruments\LabVIEW 2016\user.lib\cameraLibrary"

INCLUDEPATH += $$PWD/../include
DEPENDPATH += $$PWD/../include

FORMS += \
    fttMainWindow.ui \
    fttgainwindow.ui
