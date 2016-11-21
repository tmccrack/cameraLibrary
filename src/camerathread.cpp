/*
 * Class for reading camera. Requires camera initialization and setup prior to calling.
 * Creates win32 event for Andor callback.
 * Reads camera data into local buffer, copies into image buffer.
 */

#include "camerathread.h"

CameraThread::CameraThread(QObject *parent, int i_size, long *image_buffer) : QThread(parent)
{
    // Create copy data buffer and win32 event handle
    image_size = i_size;
    cam_data = new long[image_size];
    cam_event = CreateEvent(NULL, TRUE, FALSE, NULL);

    //TODO: Ensure imageBuffer is correct size???
    copy_data = image_buffer;
    abort = true;
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
void CameraThread::startCameraThread()
{
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
            GetMostRecentImage(cam_data, image_size);
            std::copy(cam_data, cam_data + (long) image_size, copy_data);
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
    copyData = new long[262144];
    copyControl = new float[6];
    camEvent = CreateEvent(NULL, TRUE, FALSE, NULL);  // Create win32 event handle
    abort = false;
}

ClosedLoopCameraThread::~ClosedLoopCameraThread()
{
    mutex.lock();
    abort = true;
    mutex.unlock();

    wait();  // Wait for run() to finish

    // Free up allocated memory;
    if (camData) delete[] camData;
    if (copyData) delete[] copyData;
    if (copyControl) delete[] copyControl;
}

void ClosedLoopCameraThread::startCameraThread(int xPix, int yPix, float set_x, float set_y, long *imageBuffer, float *controlBuffer)
{
    controlVals.xDim = xPix;
    controlVals.yDim = yPix;

    // Initialize control loop values
    /*
     * TODO: check if set point has been called, if not, set
     *
     */
    controlVals.kp = -0.05;
    controlVals.ki = -0.005;
    controlVals.kd = 0.005;
    controlVals.pre_error_x = 0.0;
    controlVals.pre_error_y = 0.0;
    controlVals._dt = 0.01;
    float ANGLE = 315.0;
    controlVals.x_rot = cos(ANGLE * PI / 180.0);
    controlVals.y_rot = sin(ANGLE * PI / 180.0);
    setTargetCoordinates(set_x, set_y);

    imageSize = controlVals.xDim * controlVals.yDim;
    camData = new long[imageSize];
    copyData = imageBuffer;
    controlVals.caus = controlBuffer;

    // Set abort flag to false and start thread
    mutex.lock();
    abort = false;
    mutex.unlock();
    start();

}


/*
 * Setpoint of control loop
 */
void ClosedLoopCameraThread::setTargetCoordinates(float x, float y)
{
    controlVals.set_point_x = x;
    controlVals.set_point_y = y;
}


/*
 * Reimplementation of run function for closed loop thread
 */
void ClosedLoopCameraThread::run()
{

    SocketClient *sClient = new SocketClient;
    sClient->openConnection("Closed");
    if (!sClient->isConnected())
    {
        mutex.lock();
        abort = true;
        mutex.unlock();
    }

    // Pass Andor API event handle and start acquistion
    and_error = SetDriverEvent(camEvent);
    checkError(and_error, "SetDriverEvent");

    and_error = StartAcquisition();
    checkError(and_error, "StartAcquisition");
    //int ind = 0;

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
            controlLoop();

//            if (controlVals.even)
//            {
//                controlVals.y = 5.0;
//                //sClient->sendData(controlVals.x, controlVals.y);
//                controlVals.even = false;

//            }
//            else
//            {
//                controlVals.x = 0.025;
//                controlVals.y = 0.00;
//                //sClient->sendData(controlVals.x, controlVals.y);
//                controlVals.even = true;
//            }
            sClient->sendData(controlVals.caus[5], controlVals.caus[4]);


            // Copy data into accessible buffers
            std::copy(camData, camData + (long) imageSize, copyData);

            // Update control loop values
            controlVals.pre_error_x = controlVals.caus[4];
            controlVals.pre_error_y = controlVals.caus[5];

            mutex.unlock();
            //qDebug() << ind;
            //ind++;
        }
        else if (win_error == WAIT_TIMEOUT)
        {
            // Timeout, do nothing
            ResetEvent(camEvent);
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

    // Disconnect from client
    if (sClient->isConnected())
    {
        /*
         * TODO: recenter mirror stage on exit???
         */
        //sClient->sendData(0.0, 0.0);  // set to zero voltage
        sClient->closeConnection();
    }
    else
    {
        // Something failed, try to reopen and zero voltage
        sClient->openConnection("Closed");
        //sClient->sendData(0.0, 0.0);
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
    int offs = 0;
    for (int i = 0; i < controlVals.xDim; i++)
    {
        dummy_x = 0;
        dummy_y = 0;
        for (int j = 0; j < controlVals.yDim; j++)
        {
            offs = i * controlVals.xDim;
            sum += imageBuffer[offs + j];
            dummy_x += imageBuffer[i + j * controlVals.yDim];
            dummy_y += imageBuffer[offs + j];
        }
        sum_x += (i + 1) * dummy_x;
        sum_y += (i + 1) * dummy_y;
    }
    controlVals.caus[0] = sum_x / sum;
    controlVals.caus[1] = sum_y / sum;
    //qDebug() << "Cent values: " << controlVals.caus[0] << "\t" << controlVals.caus[1];
}


/*
 * Function to implement PID control loop
 * Includes transformation axes
 */
void ClosedLoopCameraThread::controlLoop()
{
    err_x = controlVals.caus[0] - controlVals.set_point_x;
    err_y = controlVals.caus[1] - controlVals.set_point_y;

    // Transform error
    controlVals.caus[2] = (err_x * controlVals.x_rot - err_y * controlVals.y_rot);
    controlVals.caus[3] = (err_x * controlVals.y_rot + err_y * controlVals.x_rot);

    // PID x
    controlVals.caus[4] = (controlVals.caus[2] * controlVals.kp) +  // Proportional
            (controlVals.caus[2] * controlVals._dt * controlVals.ki) + // Integral
            ((controlVals.caus[2] - controlVals.pre_error_x) * controlVals.kd);  // Derivative

    // PID y
    controlVals.caus[5] = (controlVals.caus[3] * controlVals.kp) +  // Proportional
            (controlVals.caus[3] * controlVals._dt * controlVals.ki) + // Integral
            ((controlVals.caus[3] - controlVals.pre_error_y) * controlVals.kd);  // Derivative
}



/*
 * Function to set abort flag
 */
void ClosedLoopCameraThread::abortCameraThread()
{
    mutex.lock();
    abort = true;
    mutex.unlock();
}


/*
 * Function to handle Andor error flags
 */
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

/*
 * Function to manually move mirror
 *
 */
void moveMirror(float x, float y)
{
    SocketClient *sClient = new SocketClient;
    qDebug() << "Opening connection";
    sClient->openConnection("Single");
    qDebug() << "Sending data";

    sClient->sendData(x, y);
    qDebug() << "Closing connection";

    sClient->closeConnection();
}

