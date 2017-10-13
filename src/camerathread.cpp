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
    real_cam = false;
    centroids = new float[2];
    updates = new float[2];
    servo = new ImageServo(this, centroids, updates, 512, 512, 0.0);
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
    if (centroids) delete centroids;
    if (updates) delete updates;
    if (client) delete client;
    if (servo) delete servo;
    if (x_err) delete x_err;
    if (y_err) delete y_err;
    if (i_logger) delete i_logger;
    if (s_logger) delete s_logger;
}


/*
 * Set camera thread properties and start the acquistion loop
 */
void CameraThread::startThread(int x, int y, bool r_cam, string filename)
{
    // Create copy data buffer
    xd = (float) x;
    yd = (float) y;
    image_size = (int) xd * yd;
    cam_data = new uint16_t[image_size];
    real_cam = r_cam;
    if (filename.empty())
    {
        setLogInterval(0);
    }
    else
    {
        i_log_file = filename + "-ftt.dat";
        s_log_file = filename + "-servo.dat";
    }

    if (isRunning())
    {
        // Kill it if running
        mutex.lock();
        b_abort = true;
        mutex.unlock();

        wait();
    }

    mutex.lock();
    b_abort = false;
    mutex.unlock();
    start(HighPriority);
}

// Overloaded for callback
void CameraThread::startThread(cb_cam_func cb, void *user_data, int x, int y, bool r_cam, bool s_shot)
{
    // Create copy data buffer
    xd = (float) x;
    yd = (float) y;
    image_size = (int) xd * yd;
    cam_data = new uint16_t[image_size];
    real_cam = r_cam;

    // Set to appropriate callback thread function
    if (s_shot) i_loop_cond = 3;
    else i_loop_cond = 2;
    callback = cb;
    ud = user_data;

    if (isRunning())
    {
        // Kill it if running
        mutex.lock();
        b_abort = true;
        mutex.unlock();

        wait();
    }
    mutex.lock();
    b_abort = false;
    mutex.unlock();
    start(HighPriority);
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

bool CameraThread::setLoopCond(int loopCond)
{
    i_loop_cond = loopCond;
    return i_loop_cond;
}

bool CameraThread::setServoState(bool state)
{
    mutex.lock();
    b_closed = state;
    // Closing loop, zero the servo error structures
    // Zero updates value as server knows current position
    if (b_closed)
    {
        servo->zeroErrors();
        updates[0] = 0;
        updates[1] = 0;
        if (client)
        {
            if (!client->isConnected())
                client->openConnection("Closed");

        }
    }
    mutex.unlock();
    return b_closed;
}

bool CameraThread::setLogState(bool state)
{
    mutex.lock();
    b_log = state;
    mutex.unlock();
    qDebug() << "Log state: " << b_log;
    return b_log;
}

unsigned int CameraThread::getLogInveral()
{
    return i_log_interval;
}


unsigned int CameraThread::setLogInterval(unsigned int frames)
{
    mutex.lock();
    i_log_interval = frames;
    mutex.unlock();
    qDebug() << "Logging every " << frames << "frames";
    return i_log_interval;
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

void CameraThread::getServoData(float *update)
{
    update[0] = updates[0];
    update[1] = updates[1];
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

void CameraThread::getLeakyFactor(float *factor)
{
    factor[0] = servo->getLeakyFactor();
}

void CameraThread::setLeakyFactor(float factor)
{
    servo->setLeakyFactor(factor);
}

float CameraThread::getBackground()
{
    return servo->getBackground();
}

void CameraThread::setBackground(float background)
{
    servo->setBackground(background);
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
 * Instatiate objects associated with camera read here
 * or in called functions to match thread affinity
 * Otherwise use moveToThread() if to be called from
 * camera thread.
 */
void CameraThread::run()
{
    // Setup the loggers
    i_logger = new DataLogger(i_log_file);  // Image logger
    s_logger = new DataLogger(s_log_file);  // Servo logger
    uint i_log_counter = 1;

    if(real_cam)
    {
        // Pass Andor API event handle
        and_error = SetDriverEvent(cam_event);
        checkError(and_error, "SetDriverEvent");
    }

    // Select camera function
    switch(i_loop_cond)
    {
        case 0 :
            qDebug() << "Starting open loop";
            openLoop(i_logger, i_log_counter);
            break;

        case 1 :
            qDebug() << "Starting servo loop";
            servoLoop(i_logger, s_logger, i_log_counter);
            break;

        case 2 :
            qDebug() << "Starting callback loop " << endl;
            callbackLoop();
            break;

        case 3 :
            qDebug() << "Starting single shot";
            singleShot();
            break;

        default :
            b_abort = true;
            break;
    }

    if (real_cam)
    {
        and_error = AbortAcquisition();
        checkError(and_error, "AbortAcquisition");
    }

    printf("Acquistion ended\n");

    // Close loggers
    i_logger->closeFile();
    s_logger->closeFile();
    delete i_logger;
    delete s_logger;
}


/*
 * Function to acquire camera data in video
 */
void CameraThread::openLoop(DataLogger *logger, uint log_counter)
{
    if (real_cam)
    {
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
                if (b_log)
                {
                    log_counter = checkLogCounter(log_counter);
                    if (log_counter == i_log_interval)
                    {
                        logger->append(cam_data, image_size);
                    }
                }

                mutex.unlock();
            }
            else if (win_error == WAIT_TIMEOUT)
            {
                // Timeout, do nothing
                qDebug() << "Camera thread timed out waiting for event";
                b_abort = true;
                break;
//                and_error = GetStatus(status);
//                checkError(and_error, "GetStatus");
//                qDebug() << "Camera status: " << status;
//                ResetEvent(cam_event);

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

    else
    {
        while(!b_abort)
        {
            mutex.lock();
            for (int i=0; i<image_size; i++)
            {
                cam_data[i] = (uint16_t) rand() % 100;

            }
            if (b_log)
            {
                log_counter = checkLogCounter(log_counter);
                if (log_counter == i_log_interval)
                {
                    i_logger->append(cam_data, image_size);
                    s_logger->appendFloat(updates, 2);
                }
            }
            std::copy(cam_data, cam_data + image_size, copy_data);
            mutex.unlock();
            Sleep(100);
        }
    }
}


/*
 * Function to implement servo
 */
void CameraThread::servoLoop(DataLogger *i_logger, DataLogger *s_logger, unsigned int log_counter)
{
    // Setup servo
    setServoDim(xd, yd);
    getServoTargetCoords(&xd, &yd);
    servo->setBuffer(cam_data);

    // Prep the client
    if (real_cam) client = new SocketClient(6666, "fem-rt.lowell.edu");  // Real cam, assume remote server
    else client = new SocketClient();  // Defaults to localhost, 6666

    // ImageServo passed updates pointer, need to get starting value for servo
    // Switch axes, based on experiment
    client->openConnection("Values");
    client->getData(&updates[1], &updates[0]);
    client->closeConnection();

    client->openConnection("Closed");


    if(real_cam)
    {
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
//              qDebug() << "X: " << centroids[0] << " " << x_err->error << " " << updates[0]
//                             << "\tY: " << centroids[1] << " " << y_err->error << "" << updates[1];
                if (b_log)
                {
                    log_counter = checkLogCounter(log_counter);
                    if (log_counter == i_log_interval)
                    {
                        i_logger->append(cam_data, image_size);
                        s_logger->appendFloat(updates, 2);
                    }
                }
                std::copy(cam_data, cam_data + image_size, copy_data);
                mutex.unlock();
            }
            else if (win_error == WAIT_TIMEOUT)
            {
                // Timeout, do nothing
                qDebug() << "Camera thread timed out waiting for event";
                b_abort = true;
                break;
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
    else
    {
        while(!b_abort)
        {
            mutex.lock();
            for (int i=0; i<image_size; i++)
            {
                cam_data[i] = (uint16_t) rand() % 100;

            }
            servo->getUpdate();
            servo->getErrors(x_err, y_err);
                client->sendData(updates[1], updates[0]);
//                qDebug() << "X: " << centroids[0] << " " << x_err->error << " " << updates[0]
//                         << "\tY: " << centroids[1] << " " << y_err->error << "" << updates[1];

            if (b_log)
            {
                log_counter = checkLogCounter(log_counter);
                if (log_counter == i_log_interval)
                {
                    i_logger->append(cam_data, image_size);
                    s_logger->appendFloat(updates, 2);
                }
            }
            std::copy(cam_data, cam_data + image_size, copy_data);
            mutex.unlock();
            Sleep(100);
        }
    }
    client->closeConnection();

}

void CameraThread::callbackLoop()
{
    if (real_cam)
    {
        and_error = StartAcquisition();
        checkError(and_error, "StartAcquistion");

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
                mutex.unlock();

                if (!b_abort) callback(cam_data, image_size, ud);
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
    else
    {
        while(!b_abort)
        {
            for (int i=0; i<image_size; i++)
            {
                cam_data[i] = (uint16_t) rand() % 100;
            }
            callback(cam_data, image_size, ud);
            Sleep(1000);
        }
    }
}

void CameraThread::singleShot()
{
    if (real_cam)
    {
        and_error = StartAcquisition();
        checkError(and_error, "StartAcquisition");

        win_error = WaitForSingleObject(cam_event, 2500);

        // Object triggered, check what happened
        if (win_error == WAIT_OBJECT_0)
        {
            // Camera triggered event, get data
            mutex.lock();
            and_error = GetMostRecentImage16((WORD*) cam_data, image_size);
            checkError(and_error, "GetMostRecentImage16");
            mutex.unlock();
            callback(cam_data, image_size, ud);
        }
        else if (win_error == WAIT_TIMEOUT)
        {
            // Timeout, do nothing
            qDebug() << "Camera thread timed out waiting for event";
        }
        else
        {
            printf("Error in image acquisition\n");
        }
    }
    else
    {
        for (int i=0; i<image_size; i++)
        {
            cam_data[i] = (uint16_t) rand() % 100;
        }
        callback(cam_data, image_size, ud);
    }
}

uint CameraThread::checkLogCounter(uint counter)
{
    if (counter >= i_log_interval) return counter = 1;

    else return counter += 1;
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
