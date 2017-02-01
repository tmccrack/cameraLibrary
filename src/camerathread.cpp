/*
 * Class for reading camera. Requires camera initialization and setup prior to calling.
 * Creates win32 event for Andor callback.
 * Reads camera data into local buffer, copies into image buffer.
 */

#include "camerathread.h"

/*
 * Constructor
 */
CameraThread::CameraThread(QObject *parent, uint16_t *image_buffer) : QThread(parent)
{
    // Create copy data buffer and win32 event handle
    cam_event = CreateEvent(NULL, TRUE, FALSE, NULL);

    //TODO: Ensure imageBuffer is correct size???
    copy_data = image_buffer;
    abort = true;
}

/*
 * Destructor
 */
CameraThread::~CameraThread()
{
    mutex.lock();
    abort = true;
    mutex.unlock();

    wait();  // Wait for run() to finish
    if (cam_data) delete[] cam_data;
}


/*
 * Sets camera thread properties and starts the acquistion loop
 */
void CameraThread::startCameraThread(int i_size)
{
    // Create copy data buffer
    image_size = i_size;
    cam_data = new uint16_t[image_size];
    if (isRunning())
    {
        mutex.lock();
        abort = true;
        mutex.unlock();

        wait();

        mutex.lock();
        abort = false;
        mutex.unlock();
        start();
    }
    else
    {
        mutex.lock();
        abort = false;
        mutex.unlock();
        start();
    }
    qDebug() << "Camera thread started." << endl;
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
    and_error = SetDriverEvent(cam_event);
    checkError(and_error, "SetDriverEvent");

    and_error = StartAcquisition();
    checkError(and_error, "StartAcquisition");

    while (!abort)
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

