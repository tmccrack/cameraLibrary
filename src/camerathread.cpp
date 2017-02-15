/*
 * Class for reading camera. Requires camera initialization and setup prior to calling.
 * Creates win32 event for Andor callback.
 * Reads camera data into local buffer, copies into image buffer.
 */

#include "camerathread.h"
using namespace std;

/*
 * Constructor
 */
CameraThread::CameraThread(QObject *parent, uint16_t *image_buffer) : QThread(parent)
{
    // Create copy data buffer and win32 event handle
    cam_event = CreateEvent(NULL, TRUE, FALSE, NULL);

    //TODO: Ensure imageBuffer is correct size???
    copy_data = image_buffer;
    b_abort = true;
    b_servo = false;
    centroids = new float[2];
    updates = new float[2];
    servo = new ImageServo(this, centroids, updates, 512, 512);
    x_err = new Error;
    y_err = new Error;
}

/*
 * Destructor
 */
CameraThread::~CameraThread()
{
    mutex.lock();
    b_abort = true;
    mutex.unlock();

    wait();  // Wait for run() to finish
    if (cam_data) delete[] cam_data;
}


/*
 * Set camera thread properties and start the acquistion loop
 */
void CameraThread::startThread(int x, int y)
{
    // Create copy data buffer
    xd = (float) x;
    yd = (float) y;
    image_size = (int) xd * yd;
    cam_data = new uint16_t[image_size];
    if (isRunning())
    {
        mutex.lock();
        b_abort = true;
        mutex.unlock();

        wait();

        mutex.lock();
        b_abort = false;
        mutex.unlock();
        start();
    }
    else
    {
        mutex.lock();
        b_abort = false;
        mutex.unlock();
        start();
    }
}


/*
 * Set abort flag and stop acquisition
 */
void CameraThread::abortThread()
{
    mutex.lock();
    b_abort = true;
    mutex.unlock();
}

bool CameraThread::setLoopStatus(bool loop)
{
    b_servo = loop;
    return b_servo;
}

/*
 * Getter and setter for servo gain
 */
Gain CameraThread::getServoGainX()
{
    return servo->getGainsX();
}

Gain CameraThread::getServoGainY()
{
    return servo->getGainsY();
}

void CameraThread::setServoGain(Gain gainx, Gain gainy)
{
    servo->setGainsX(gainx);
    servo->setGainsY(gainy);
}


/*
 * Getter and setter for servo rotation parameter
 */
float CameraThread::getServoRotation()
{
    return servo->getRotation();
}

void CameraThread::setServoRotation(float rotation)
{
    servo->setRotation(rotation);
}


/*
 * Getter and setter for servo dimensions
 */
void CameraThread::getServoDim(int *x, int *y)
{
    servo->getImageDim(x, y);
}

void CameraThread::setServoDim(int x, int y)
{
    servo->setImageDim(x, y);
}


/*
 * Getter and setter for servo target coordinates
 */
void CameraThread::getServoTargetCoords(float *tar_x, float *tar_y)
{
    servo->getTargetCoords(tar_x, tar_y);
}
void CameraThread::setServoTargetCoords(float tar_x, float tar_y)
{
    servo->setTargetCoords(tar_x, tar_y);
}


/*
 * Main function for camera thread.
 * Retrieves camera data when event set.
 */
void CameraThread::run()
{
    // Pass Andor API event handle and start acquistion
    and_error = SetDriverEvent(cam_event);
    checkError(and_error, "SetDriverEvent");

    if (b_servo)
    {
        servoLoop();
    }
    else
    {
        openLoop();
    }

    and_error = AbortAcquisition();
    checkError(and_error, "AbortAcquisition");

    printf("Acquistion ended\n");
}


/*
 * Function to acquire camera data in video
 */
void CameraThread::openLoop()
{
    qDebug() << "Starting camera acquistion";

    and_error = StartAcquisition();
    checkError(and_error, "StartAcquisition");

    while (!b_abort)
    {
        win_error = WaitForSingleObject(cam_event, 2500);

        // Object triggered, check what happened
        if (win_error == WAIT_OBJECT_0)
        {
            // Camera triggered event, get data
            mutex.lock();
            ResetEvent(cam_event);
            and_error = GetMostRecentImage16((WORD*) cam_data, image_size);
            checkError(and_error, "GetMostRecentImage16");
            std::copy(cam_data, cam_data + image_size, copy_data);
            mutex.unlock();
        }
        else if (win_error == WAIT_TIMEOUT)
        {
            // Timeout, do nothing
            qDebug() << "Camera thread timed out waiting for event";
            /*
            GetStatus(status);
            qDebug() << "Camera status: " << *status;
            checkError(and_error, "GetStatus");
            */
            ResetEvent(cam_event);
        }
        else
        {
            // Error, abort acquisition loop
            mutex.lock();
            b_abort = true;
            mutex.unlock();
            printf("Error in image acquisition\n");
        }
    }
}


/*
 * Function to implement servo
 */
void CameraThread::servoLoop()
{
    qDebug() << "Starting camera servo";

    setServoDim(xd, yd);
    setServoTargetCoords(xd/2.0, yd/2.0);
    qDebug() << "Set too: " << xd << " " << yd;
    getServoTargetCoords(&xd, &yd);
    qDebug() << "Actual: " << xd << " " << yd;
    servo->setBuffer(cam_data);

    client = new SocketClient();
    client->openConnection("Values");
    client->getData(&updates[0], &updates[1]);  // Assign starting values
//    updates[0] = 0;
//    updates[1] = 0;
    client->closeConnection();

//    qDebug() << "Starting values: " << updates[0] << " " << updates[1];

    client->openConnection("Closed");

    and_error = StartAcquisition();
    checkError(and_error, "StartAcquisition");

    while (!b_abort)
    {
        win_error = WaitForSingleObject(cam_event, 2500);

        // Object triggered, check what happened
        if (win_error == WAIT_OBJECT_0)
        {
            // Camera triggered event, get data
            mutex.lock();
            ResetEvent(cam_event);
            and_error = GetMostRecentImage16((WORD*) cam_data, image_size);
            checkError(and_error, "GetMostRecentImage16");

            // Pass current data to servo
            // Servo updates pointers passed to constructor
            servo->getUpdate();
            servo->getErrors(x_err, y_err);
            client->sendData(updates[1], updates[0]);
            qDebug() << "X: " << centroids[0] << " " << x_err->error << " " << updates[0]
                     << "\tY: " << centroids[1] << " " << y_err->error << "" << updates[1];
            std::copy(cam_data, cam_data + image_size, copy_data);
            mutex.unlock();
        }
        else if (win_error == WAIT_TIMEOUT)
        {
            // Timeout, do nothing
            qDebug() << "Camera thread timed out waiting for event";
            /*
            GetStatus(status);
            qDebug() << "Camera status: " << *status;
            checkError(and_error, "GetStatus");
            */
            ResetEvent(cam_event);
        }
        else
        {
            // Error, abort acquisition loop
            mutex.lock();
            b_abort = true;
            mutex.unlock();
            printf("Error in image acquisition\n");
        }
    }
//    client->sendData(5.0,5.0);
    client->closeConnection();

}

/*
 * Check Andor error code
 */
bool CameraThread::checkError(unsigned int _ui_err, const char* _cp_func)
{
  bool b_ret;

  if(_ui_err == DRV_SUCCESS) {
    b_ret = true;
  }
  else {
      printf("ERROR - %s -- %i\n\n",_cp_func,_ui_err);
      b_gblerrorFlag = true;
      b_ret = false;
  }
  return b_ret;
}
