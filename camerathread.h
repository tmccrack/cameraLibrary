#ifndef CAMERATHREAD_H
#define CAMERATHREAD_H

#include <QtCore>
#include <QObject>
#include <QThread>
#include <windows.h>
#include <iostream>
#include "ATMCD32D.h"

class CameraThread : public QThread
{
    Q_OBJECT
public:
    CameraThread(QObject *parent = 0);
    ~CameraThread();
    void startCameraThread(int imageSize, long *imageBuffer);
    void abortCameraThread();

protected:
    void run() Q_DECL_OVERRIDE;

private:
    bool checkError(unsigned int _ui_error, const char* _cp_func);
    bool b_gblerrorFlag;
    unsigned int and_error; // Andor error
    DWORD win_error;  // Windows event error
    QMutex mutex;
    HANDLE camEvent;
    bool abort;
    int imageSize;
    long *camData;
    long *copyData;

private slots:
};

#endif // CAMERATHREAD_H
