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
#include "socketclient.h"
#include "imageservo.h"
#include "ATMCD32D.h"

#define PI 3.14159265
#define ROTATION 45.0 * PI / 180.0;


/*
 * Class for implementing camera acquisition in a thread
 */
class CameraThread : public QThread
{

public:
    CameraThread(QObject *parent = 0, uint16_t *image_buffer = 0);
    ~CameraThread();
    void startThread(int x = 0, int y = 0);
    void abortThread();

    bool setLoopStatus(bool loop = false);

    Gain getServoGainX();
    Gain getServoGainY();
    void setServoGain(Gain gainx, Gain gainy);
    float getServoRotation();
    void setServoRotation(float rotation);

    void getServoTargetCoords(float *tar_x, float *tar_y);
    void setServoTargetCoords(float tar_x, float tar_y);

    void getServoDim(int *x, int *y);
    void setServoDim(int x = 32, int y = 32);


protected:
    void run() Q_DECL_OVERRIDE;

private:
    bool checkError(unsigned int _ui_error, const char* _cp_func);
    void openLoop();
    void servoLoop();
    bool b_gblerrorFlag;
    unsigned int and_error; // Andor error
    DWORD win_error;  // Windows event error
    QMutex mutex;
    HANDLE cam_event;
    int image_size;
    float xd = 32;
    float yd = 32;
    bool b_abort;
    bool b_servo;
    uint16_t *cam_data;
    uint16_t *copy_data;
    int *status;

    // For servo
    float *centroids;
    float *updates;
    Error *x_err;
    Error *y_err;

    ImageServo *servo;
    SocketClient *client;

};


///*
// * Class for extending camera thread to include closed loop operation
// */
//class ServoThread: public CameraThread
//{

//public:
//    ServoThread(QObject *parent = 0, uint16_t *image_buffer = 0);
//    ~ServoThread();
//    ImageServo *servo;
//    SocketClient *client;

////    struct ControlValues{
////        int xDim;
////        int yDim;
////        float *caus = new float[6];
//////        float x;  caus[0]
//////        float y;  caus[1]
//////        float x_update;  caus[2]
//////        float y_update;  caus[3]
//////        float error_x;  caus[4]
//////        float error_y;  caus[5]
////        float pre_error_x;
////        float pre_error_y;
////        float set_point_x;
////        float set_point_y;
////        float kp;
////        float ki;
////        float kd;
////        float _dt;
////        float x_rot;
////        float y_rot;
////        bool even;

////    } controlVals;

//protected:
//    void run() Q_DECL_OVERRIDE;


//private:
////    int port;
////    QString *host;
////    float *copyControl;
////    float err_x, err_y;

//    /*
//     * Struct for control loop values
//     */


//};


#endif // CAMERATHREAD_H
