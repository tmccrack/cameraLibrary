/*
 * Class for reading camera. Requires camera initialization and setup prior to calling.
 * Creates win32 event for Andor callback.
 * Reads camera data into local buffer, copies into image buffer.
 */

#include "camerathread.h"

CameraThread::CameraThread(QObject *parent) : QThread(parent)
{
    // Create copy data buffer and win32 event handle
    copyData = new long[262144];
    camEvent = CreateEvent(NULL, TRUE, FALSE, NULL);  // Create win32 event handle
    abort = false;
}


CameraThread::~CameraThread()
{
    mutex.lock();
    abort = true;
    mutex.unlock();

    wait();  // Wait for run() to finish
}


/*
 * Sets camera thread properties and starts the acquistion loop
 */
void CameraThread::startCameraThread(int imageSize, long *imageBuffer)
{
    this->imageSize = imageSize;
    camData = new long[this->imageSize];

    //TODO: Ensure imageBuffer is correct size???
    copyData = imageBuffer;

    // Set abort flag to false and start thread
    mutex.lock();
    abort = false;
    mutex.unlock();
    start();
}


/*
 * Function to set abort flag
 */
void CameraThread::abortCameraThread()
{
    mutex.lock();
    abort = true;
    mutex.unlock();
}


/*
 * Main function for camera thread.
 * Retrieves camera data when event set.
 */
void CameraThread::run()
{
    // Pass Andor API event handle and start acquistion
    and_error = SetDriverEvent(camEvent);
    checkError(and_error, "SetDriverEvent");

    and_error = StartAcquisition();
    checkError(and_error, "StartAcquisition");

    while (!abort)
    {
        win_error = WaitForSingleObject(this->camEvent, 2500);

        // Object triggered, check what happened
        if (win_error == WAIT_OBJECT_0)
        {
            // Camera triggered event, get data
            mutex.lock();
            ResetEvent(camEvent);
            GetMostRecentImage(camData, imageSize);
            std::copy(camData, camData + (long) imageSize, copyData);
            mutex.unlock();
        }
        else if (win_error == WAIT_TIMEOUT)
        {
            // Timeout, do nothing
        }
        else
        {
            // Error, abort acquisition loop
            mutex.lock();
            abort = true;
            mutex.unlock();
            printf("Error in image acquisition\n");
        }
    }

    and_error = AbortAcquisition();
    checkError(and_error, "AbortAcquisition");

    printf("Acquistion ended\n");
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


/*
 * Class definition for closed loop operation
 * Extends camera thread
 */
ClosedLoopCameraThread::ClosedLoopCameraThread(QObject *parent) : CameraThread(parent)
{
    // Create socket client, copy data buffer, and win32 event handle
    sClient = new SocketClient(this);
    copyData = new long[262144];
    camEvent = CreateEvent(NULL, TRUE, FALSE, NULL);  // Create win32 event handle
    abort = false;
    //controlVals = new ControlValues;
}

ClosedLoopCameraThread::~ClosedLoopCameraThread()
{
    mutex.lock();
    abort = true;
    mutex.unlock();

    wait();  // Wait for run() to finish
}

void ClosedLoopCameraThread::startCameraThread(int xPix, int yPix, long *imageBuffer)
{
    controlVals.xDim = xPix;
    controlVals.yDim = yPix;

    imageSize = controlVals.xDim * controlVals.yDim;
    camData = new long[imageSize];
    copyData = imageBuffer;

    // Set abort flag to false and start thread
    mutex.lock();
    abort = false;
    mutex.unlock();
    start();

}


/*
 * Reimplementation of run function for closed loop thread
 */
void ClosedLoopCameraThread::run()
{

    // Pass Andor API event handle and start acquistion
    and_error = SetDriverEvent(camEvent);
    checkError(and_error, "SetDriverEvent");

    and_error = StartAcquisition();
    checkError(and_error, "StartAcquisition");

    sClient->openConnection();

    /*
     * TODO: Connect SocketClient signals to ClosedLoopThread slots indicating connection issues
     */

    while (!abort)
    {
        win_error = WaitForSingleObject(camEvent, 2500);

        // Object triggered, check what happened
        if (win_error == WAIT_OBJECT_0)
        {
            // Camera triggered event, get data
            mutex.lock();
            ResetEvent(camEvent);
            GetMostRecentImage(camData, imageSize);
            centroid(camData);
            if (controlVals.even) sClient->sendData(5.0, 5.0);
            else sClient->sendData(0.0, 0.0);
            //sClient->sendData(controlVals.x, controlVals.y);
            std::copy(camData, camData + (long) imageSize, copyData);
            mutex.unlock();
        }
        else if (win_error == WAIT_TIMEOUT)
        {
            // Timeout, do nothing
        }
        else
        {
            // Error, abort acquisition loop
            mutex.lock();
            this->abort = true;
            mutex.unlock();
            printf("Error in image acquisition\n");
        }
    }

    and_error = AbortAcquisition();
    checkError(and_error, "AbortAcquisition");

    // Disconnect from client
    if (sClient->isConnected())
    {
        /*
         * TODO: recenter mirror stage on exit???
         */
        sClient->sendData(0.0, 0.0);  // set to zero voltage
        sClient->closeConnection();
    }
    else
    {
        // Something failed, try to reopen and zero voltage
        sClient->openConnection();
        sClient->sendData(0.0, 0.0);
        sClient->closeConnection();
    }


    printf("Acquistion ended\n");
}


/*
 * Find the x, y centroid from the provided image buffer
 * and set the appropriate x, y values in the controlVal struct.
 */
void ClosedLoopCameraThread::centroid(long *imageBuffer)
{
    float sum = 0;
    float dummy_x = 0;
    float dummy_y = 0;
    float sum_x = 0;
    float sum_y = 0;
    for (int i = 0; i < controlVals.xDim; i++)
    {
        dummy_x = 0;
        dummy_y = 0;
        for (int j = 0; j < controlVals.yDim; j++)
        {
            sum += imageBuffer[i + j];
            dummy_x += imageBuffer[i + j * controlVals.xDim];
            dummy_y += imageBuffer[i * controlVals.yDim + j];
        }
        sum_x += (i + 1) * dummy_x;
        sum_y += (i + 1) * dummy_y;
    }
    controlVals.x = sum_x / sum;
    controlVals.y = sum_y / sum;
}



bool ClosedLoopCameraThread::checkError(unsigned int _ui_err, const char* _cp_func)
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
