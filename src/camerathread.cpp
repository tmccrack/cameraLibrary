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
CameraThread::CameraThread(QObject *parent, std::uint16_t *buffer) : QThread(parent)
{
    // Create copy data buffer and win32 event handle
    cam_event = CreateEvent(NULL, TRUE, FALSE, NULL);

    //TODO: Ensure imageBuffer is correct size???
    copy_data = buffer;
    b_abort = true;
    real_cam = false;
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
        i_log_file = filename + "-ftt.fits";
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

bool CameraThread::getLogState()
{
    return b_log;
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

void CameraThread::setHeaderData(float exp_time, float acc_time, int em_gain)
{
    s_h_data.exp_time = exp_time;
    s_h_data.acc_time = acc_time;
    s_h_data.em_gain = em_gain;
}

void CameraThread::setServoHeaderData()
{
    /*!
      setServoHeaderData

      private function to set the servo data in the FITS header
      pull the data from the servo instance
     */
    Gain temp_gain = servo->getGainsX();
    s_h_data.kp = temp_gain.kp;
    s_h_data.ki = temp_gain.ki;
    s_h_data.kd = temp_gain.kd;

    s_h_data.rotation = servo->getRotation();
    s_h_data.background = servo->getBackground();
    servo->getTargetCoords(&s_h_data.targx, &s_h_data.targy);
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

string CameraThread::getServoAlg()
{
    return servo->getAlgorithm();
}

void CameraThread::setServoAlg(string alg)
{
    servo->setAlgorithm(alg);
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


void CameraThread::run()
{
    /*!
      run()

     * Camera thread method
     * Instatiate objects associated with camera read here
     * or in called functions to match thread affinity
     * Otherwise use moveToThread() if to be called from
     * camera thread.
     */

    // Setup the loggers
    int axes[2] = {int(xd), int(yd)};
    i_logger = new DataLogger(i_log_file, axes);  // Image logger

    mutex.lock();
    // A little weird, cam thread knows about servo params but not image acquisition
    // Image params set before thread is started, servo params updated here
    // Header then written here
    setServoHeaderData();
    i_logger->setHeaderData(s_h_data);
//    i_logger->getFitsPointer();
    mutex.unlock();

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
            servoLoop(i_logger, i_log_counter);
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

    qDebug() << "Acquistion ended";

    // Close loggers
    if (b_log) i_logger->writeFile();
    delete i_logger;
}


void CameraThread::openLoop(DataLogger *logger, uint log_counter)
{
    /*!
     openLoop()

     Runs the camera in open loop
     */
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
                copy(&cam_data[0], &cam_data[image_size], copy_data);
                log_counter = checkLog(log_counter, logger);
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
            // Fake camera, generate data, copy it, log it
            mutex.lock();
            uint16_t r = rand();
            for (int i=0; i<image_size; i++)
            {
                cam_data[i] = (uint16_t) i + r;
//                cam_data[i] = (uint16_t) rand() % 100;

            }
            copy(&cam_data[0], &cam_data[image_size], copy_data);
            log_counter = checkLog(log_counter, logger);
            mutex.unlock();
            Sleep(100);
        }
    }
}


/*
 * Function to implement servo
 */
void CameraThread::servoLoop(DataLogger *logger, unsigned int log_counter)
{
    // Setup servo
    setServoDim(xd, yd);
    getServoTargetCoords(&xd, &yd);
    servo->setBuffer(cam_data);

    // Prep the client
    if (real_cam) client = new SocketClient(6666, "172.28.139.52");  // Real cam, assume remote server
    else client = new SocketClient();  // Defaults to localhost, 6666
    qDebug() << "Client created";

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
//                             << "\tY: " << centroids[1] << " " << y_err->error << "" << updates[1]; copy(&cam_data[0], &cam_data[image_size], begin(cam_array));

                copy(&cam_data[0], &cam_data[image_size], copy_data);
                log_counter = checkLog(log_counter, logger);
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
//            qDebug() << "X: " << centroids[0] << " " << x_err->error << " " << updates[0]
//                         << "\tY: " << centroids[1] << " " << y_err->error << "" << updates[1];

            copy(&cam_data[0], &cam_data[image_size], copy_data);
            log_counter = checkLog(log_counter, logger);
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
                callback(cam_data, image_size, ud);
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

uint CameraThread::checkLog(uint counter, DataLogger *logger)
{
    if (b_log)
    {
        if (counter >= i_log_interval){
            i_logger->writeSlice(copy_data);

            //                    i_logger->append(cam_data, image_size);
            //                    s_logger->appendFloat(updates, 2);
            counter = 1;
        }

        else counter += 1;
    }
    return counter;
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
