#ifndef CAMERATHREAD_H
#define CAMERATHREAD_H

#include <QtCore>
#include <QObject>
#include <QThread>
#include <windows.h>
#include <iostream>
#include "socketclient.h"
#include "ATMCD32D.h"


/*
 * Class for implementing camera acquisition in a thread
 */
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

};


/*
 * Class for extending camera thread to include closed loop operation
 */
class ClosedLoopCameraThread: public CameraThread
{
    Q_OBJECT

public:
    ClosedLoopCameraThread(QObject *parent = 0);
    ~ClosedLoopCameraThread();
    void startCameraThread(int xPix, int yPix, long *imageBuffer, float *x, float *y);
    void abortCameraThread();

protected:
    void run() Q_DECL_OVERRIDE;

private:
    bool checkError(unsigned int _ui_error, const char* _cp_func);
    void centroid(long *imageBuffer);
    bool b_gblerrorFlag;
    unsigned int and_error; // Andor error
    DWORD win_error;  // Windows event error
    int port;
    QString *host;
    QMutex mutex;
    HANDLE camEvent;
    bool abort;
    int imageSize;
    long *camData;
    long *copyData;
    float *copyX, *copyY;

    /*
     * Struct for control loop values
     */
    struct ControlValues{
        int xDim;
        int yDim;
        float x;
        float y;
        float kp;
        float ki;
        float kd;
        float gain;
        bool even;
    } controlVals;

};

#endif // CAMERATHREAD_H
