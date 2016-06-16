/*
 * Class for reading camera. Requires camera initialization and setup prior to calling.
 * Creates win32 event for Andor callback.
 * Reads camera data into local buffer, copies into image buffer.
 */

#include "camerathread.h"

CameraThread::CameraThread(QObject *parent) : QThread(parent)
{
    // Create copy data buffer and win32 event handle
    this->copyData = new long[262144];
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
void CameraThread::startCameraThread(int imageSize, long *imageBuffer)
{
    this->imageSize = imageSize;
    this->camData = new long[this->imageSize];

    //TODO: Ensure imageBuffer is correct size???
    this->copyData = imageBuffer;
    this->start();  // Starts thread
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
        win_error = WaitForSingleObject(this->camEvent, 2500);

        // Object triggered, check what happened
        if (win_error == WAIT_OBJECT_0)
        {
            // Camera triggered event, get data
            mutex.lock();
            ResetEvent(this->camEvent);
            GetMostRecentImage(camData, this->imageSize);
            std::copy(camData, camData + (long) this->imageSize, copyData);
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
