#ifndef CAMERATHREAD_H
#define CAMERATHREAD_H

#include <cstdint>
#include <QtCore/QtCore>
#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtCore/QMutex>
#include <QtCore/QString>
#include <QtCore/QDebug>
#include <windows.h>
#include <math.h>
#include <iostream>
#include "ATMCD32D.h"

#define PI 3.14159265
#define ROTATION 45.0 * PI / 180.0;


/*
 * Class for implementing camera acquisition in a thread
 */
class CameraThread : public QThread
{

public:
    CameraThread(QObject *parent = 0, int i_size = 0, uint16_t *image_buffer = 0);
    ~CameraThread();
    void startCameraThread();
    void abortCameraThread();

protected:
    void run() Q_DECL_OVERRIDE;

private:
    bool checkError(unsigned int _ui_error, const char* _cp_func);
    bool b_gblerrorFlag;
    unsigned int and_error; // Andor error
    DWORD win_error;  // Windows event error
    QMutex mutex;
    HANDLE cam_event;
    bool abort;
    int image_size;
    uint16_t *cam_data;
    uint16_t *copy_data;
    int *status;
};


/*
// * Class for extending camera thread to include closed loop operation
// */
//class ClosedLoopCameraThread: public CameraThread
//{
//    Q_OBJECT

//public:
//    ClosedLoopCameraThread(QObject *parent = 0);
//    ~ClosedLoopCameraThread();
//    void startCameraThread(int xPix, int yPix, float set_x, float set_y, long *imageBuffer, float *controlBuffer);
//    void setTargetCoordinates(float x, float y);
//    void abortCameraThread();

//protected:
//    void run() Q_DECL_OVERRIDE;

//private:
//    bool checkError(unsigned int _ui_error, const char* _cp_func);
//    void centroid(long *imageBuffer);
//    void controlLoop();
//    bool b_gblerrorFlag;
//    unsigned int and_error; // Andor error
//    DWORD win_error;  // Windows event error
//    int port;
//    QString *host;
//    QMutex mutex;
//    HANDLE camEvent;
//    bool abort;
//    int imageSize;
//    long *camData;
//    long *copyData;
//    float *copyControl;
//    float err_x, err_y;

//    /*
//     * Struct for control loop values
//     */
//    struct ControlValues{
//        int xDim;
//        int yDim;
//        float *caus = new float[6];
////        float x;  caus[0]
////        float y;  caus[1]
////        float x_update;  caus[2]
////        float y_update;  caus[3]
////        float error_x;  caus[4]
////        float error_y;  caus[5]
//        float pre_error_x;
//        float pre_error_y;
//        float set_point_x;
//        float set_point_y;
//        float kp;
//        float ki;
//        float kd;
//        float _dt;
//        float x_rot;
//        float y_rot;
//        bool even;

//    } controlVals;

//};

//void moveMirror(float x, float y);

#endif // CAMERATHREAD_H
