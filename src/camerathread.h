#ifndef CAMERATHREAD_H
#define CAMERATHREAD_H

#include <cstdint>
#include <cstdlib>

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
#include "datalogger.h"
#include "ATMCD32D.h"

#define PI 3.14159265
#define ROTATION 45.0 * PI / 180.0;

typedef void (*cb_cam_func)(uint16_t *data, int length, void *user_data);

/*
 * Class for implementing camera acquisition in a thread
 */
class CameraThread : public QThread
{

public:
    CameraThread(QObject *parent = 0, uint16_t *image_buffer = 0);
    ~CameraThread();
    void startThread(int x = 0, int y = 0, bool r_cam = false, std::string filename = "");  // Must get data manually
    void startThread(cb_cam_func cb = NULL,
                     void *user_data = NULL,
                     int x = 0, int y = 0, bool r_cam = false, bool s_shot = false);  // Supplied callabck
    void abortThread();

    bool setLoopCond(int loopCond = 0);
    bool setServoState(bool state);

    bool setLogState(bool state);
    unsigned int getLogInveral();
    unsigned int setLogInterval(unsigned int frames);

    // Servo getters and setters
    Gain getServoGainX();
    Gain getServoGainY();
    void setServoGain(Gain gainx, Gain gainy);
    void getLeakyFactor(float *factor);
    void setLeakyFactor(float factor);

    float getServoRotation();
    void setServoRotation(float rotation);

    void getServoData(float *update);

    void getServoTargetCoords(float *tar_x, float *tar_y);
    void setServoTargetCoords(float tar_x, float tar_y);

    void getServoDim(int *x, int *y);
    void setServoDim(int x = 32, int y = 32);

    float getBackground();
    void setBackground(float background);

protected:
    void run() Q_DECL_OVERRIDE;

private:
    bool checkError(unsigned int _ui_error, const char* _cp_func);

    void openLoop(DataLogger *logger, unsigned int log_counter);
    void servoLoop(DataLogger *i_logger, DataLogger *s_logger, unsigned int log_counter);
    void callbackLoop();
    void singleShot();
    uint checkLogCounter(uint counter);

    bool b_gblerrorFlag;
    unsigned int and_error; // Andor error
    DWORD win_error;  // Windows event error
    QMutex mutex;
    HANDLE cam_event;
    int image_size;
    float xd = 32;
    float yd = 32;
    bool b_abort;
    bool b_closed;
    bool b_log;
    unsigned int i_log_interval = 0;
    bool real_cam;
    std::string i_log_file;  // image log file
    std::string s_log_file;  // servo log file
    int i_loop_cond = 0;
    uint16_t *cam_data;
    uint16_t *copy_data;
    int *status;


    cb_cam_func callback;
    void *ud;


    // For servo
    float *centroids;
    float *updates;
    Error *x_err;
    Error *y_err;

    ImageServo *servo;
    DataLogger *i_logger;
    DataLogger *s_logger;
    SocketClient *client;

};



#endif // CAMERATHREAD_H
