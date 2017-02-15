QT += core\
    network
QT -= gui

CONFIG += c++11

TARGET = testCamera
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app

SOURCES += main.cpp\
    $$PWD/../src/camera.cpp\
    $$PWD/../src/camerathread.cpp\
#    $$PWD/../src/imageservo.cpp\
#    $$PWD/../src/servo.cpp\
    $$PWD/../src/socketclient.cpp\


HEADERS += \
    $$PWD/../src/camera.h\
    $$PWD/../src/camerathread.h\
#    $$PWD/../src/imageservo.h\
#    $$PWD/../src/servo.h\
    $$PWD/../src/socketclient.h\

#win32:CONFIG(release): LIBS += -L$$PWD/../build-cameraLibrary/release/ -lcameraLibrary
#else:win32:CONFIG(debug): LIBS += -L$$PWD/../build-cameraLibrary/debug/ -lcameraLibrary
#else:unix: LIBS += -L$$PWD/../build-cameraLibrary/release/ -lcameraLibrary

win32|unix: LIBS += -L$$PWD/../lib/ -latmcd32m

INCLUDEPATH += $$PWD/../src\
        $$PWD/../include
DEPENDPATH += $$PWD/../src\
        $$PWD/../include
