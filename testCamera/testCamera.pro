QT += core\
    network
QT -= gui

CONFIG += c++11

TARGET = testCamera
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app

SOURCES += main.cpp

HEADERS += \
    cameralibrary.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-cameraLibrary/release/ -lcameraLibrary
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-cameraLibrary/debug/ -lcameraLibrary
else:unix: LIBS += -L$$PWD/../build-cameraLibrary/ -lcameraLibrary

INCLUDEPATH += $$PWD/../src\
        $$PWD/../include
DEPENDPATH += $$PWD/../src\
        $$PWD/../include
