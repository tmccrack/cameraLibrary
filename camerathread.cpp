#include "camerathread.h"

CameraThread::CameraThread(QObject *parent) : QThread(parent)
{
    // Create win32 event handle
    this->camEvent = CreateEvent(NULL, TRUE, FALSE, NULL);  // Create win32 event handle
    this->abort = false;
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
void CameraThread::startCameraThread(int imageSize)
{
    this->imageSize = imageSize;
    this->camData2 = new long[this->imageSize];
    this->start();
}


/*
 * Function to set abort flag
 */
void CameraThread::abortCameraThread()
{
    mutex.lock();
    this->abort = true;
    mutex.unlock();
}


/*
 * Main function for camera thread.
 * Retrieves camera data when event set.
 */
void CameraThread::run()
{
    // Pass Andor API event handle and start acquistion
    and_error = SetDriverEvent(this->camEvent);
    checkError(and_error, "SetDriverEvent");
    and_error = StartAcquisition();
    checkError(and_error, "StartAcquisition");

    while (!this->abort)
    {
        win_error = WaitForSingleObject(this->camEvent, 1000);

        // Object triggered, check what happened
        if (win_error == WAIT_OBJECT_0)
        {
            // Camera triggered event, get data
            mutex.lock();
            GetMostRecentImage(camData2, this->imageSize);
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
