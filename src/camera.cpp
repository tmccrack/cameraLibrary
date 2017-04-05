#include "camera.h"
using namespace std;

/*
 * Camera constructor
 */
Camera::Camera()
{

}

/*
 * Camera destructor
 */
Camera::~Camera()
{
    if (cam_data) delete[] cam_data;
    if (t_cam_thread) delete t_cam_thread;
}

void Camera::initializeCamera(string cam_name, bool r_cam, int temp)
{
    cam_data = new uint16_t[262144];  // Initialize camera data buffer
    camera_name = QString::fromStdString(cam_name);
    real_cam = r_cam;
    qDebug() << "Real cam: " << real_cam;
    if (!real_cam)
    {
        // Generate fake data
        for (int i = 0; i < 262144; i++)
        {
            cam_data[i] = i;
        }
        qDebug() << "Fake camera initialized";
    }
    _initializeCamera(temp);
    b_gblerrorFlag = false;  // Clear error flag
    qDebug() << "Done initializing";
}


/*
 * Start camera acquisition thread
 */
void Camera::startCamera(int loopCond, string filename)
{
    if (camera_name == "FTT")
    {
        s_shutterProp.mode = 1;  // Always open
        setShutterParams(s_shutterProp);
    }

    // Check and start thread
    if (t_cam_thread->isRunning())
        qDebug() << "Already running";
    else
    {
        // Update servo variables
        int st = t_cam_thread->setLoopCond(loopCond);
        qDebug() << "Loop status: " << st;
//            t_cam_thread->setServoDim(s_imageDim.h_dim, s_imageDim.v_dim);
        t_cam_thread->startThread(s_imageDim.h_dim, s_imageDim.v_dim, real_cam, filename);
    }
}

void Camera::startCamera(cb_cam_func cb, void *user_data)
{
    if (camera_name == "FTT")
    {
        s_shutterProp.mode = 1;  // Always open
        setShutterParams(s_shutterProp);
    }

    // Check and start thread
    if (t_cam_thread->isRunning())
        qDebug() << "Already running";
    else
    {
        t_cam_thread->startThread(cb, user_data, s_imageDim.h_dim, s_imageDim.v_dim, real_cam, false);
    }
}

void Camera::singleShot(cb_cam_func cb, void *user_data)
{
    if (t_cam_thread->isRunning())
        qDebug() << "Already running";
    else
    {
        t_cam_thread->startThread(cb, user_data, s_imageDim.h_dim, s_imageDim.v_dim, real_cam, true);
    }
}

bool Camera::isCameraRunning()
{
    if (t_cam_thread) return t_cam_thread->isRunning();
    else return false;
}


/*
 * Stop camera acquisition thread
 */
void Camera::stopCamera()
{
    // check if thread running, abort and delete array
    if (t_cam_thread->isRunning())
    {
        t_cam_thread->abortThread();
        t_cam_thread->wait();
    }
}


/*
 * Shutdown the camera
 */
void Camera::shutdownCamera()
{
    if (isCameraRunning())
    {
        qDebug() << "Stopping camera thread before shutting down";
        stopCamera();
    }
    _shutdownCamera();
}

///*
// * Get handle of specified camera
// */
//void Camera::getHandle(long *cam_handle)
//{
//    if (real_cam)
//    {
//        ui_error = GetCameraHandle(0, cam_handle);
//        checkError(ui_error, "GetHandle");
//    }
//    else *cam_handle = long(666);
//}

/*
 * Retrieve current camera data buffer
 */
void Camera::getCameraData(uint16_t *buffer)
{
    if (real_cam)
    {
        copy(cam_data, cam_data + s_imageDim.size, buffer);
    }
    else
    {
        for (int i = 0; i < 262144; i++)
        {
            cam_data[i] = qrand() % ((1000));
        }
        copy(cam_data, cam_data + s_imageDim.size, buffer);
    }
}

void Camera::getServoData(float *updates)
{
    t_cam_thread->getServoData(updates);
}


/*
 * Frame dimensions
 */
ImageDimension Camera::getImageDims()
{
    return s_imageDim;
}

void Camera::setImageDims(int hstart, int hend, int vstart, int vend, int hbin, int vbin)
{
    s_imageDim.h_start = hstart;
    s_imageDim.h_end = hend;
    s_imageDim.v_start = vstart;
    s_imageDim.v_end = vend;
    s_imageDim.h_bin = hbin;
    s_imageDim.v_bin = vbin;
    _setImageDims();
}

void Camera::setImageDims(ImageDimension imageParameters)
{
    s_imageDim = imageParameters;
    _setImageDims();
}


/*
 * Exposure and EM gain parameters
 */
ExposureProperties Camera::getExposureParams()
{
    return s_expProp;
}

void Camera::setExposureParams(ExposureProperties expParameters)
{
    s_expProp = expParameters;
    _setExposureParams();
}

void Camera::setExposureParams(float exposure, int em_gain)
{
    s_expProp.exp_time = exposure;
    s_expProp.em_gain = em_gain;
    _setExposureParams();
}


/*
 * Camera read setup parameters
 */
ReadProperties Camera::getReadParams()
{
    return s_readProp;
}

void Camera::setReadParams(ReadProperties readParameters)
{
    s_readProp = readParameters;
    _setReadParams();
}

void Camera::setReadParams(int read_mode, int acq_mode, int frame_transfer, int output_amp)
{
    s_readProp.read_mode = read_mode;
    s_readProp.acq_mode = acq_mode;
    s_readProp.frame_transfer = frame_transfer;
    s_readProp.output_amp = output_amp;
    _setReadParams();
}


/*
 * Camera read timing parameters
 */
TimingProperties Camera::getTimingParams()
{
    return s_timingProp;
}

void Camera::setTimingParams(int h_shift, int v_shift, int dma_images, int dma_accum_time)
{
    s_timingProp.h_shift = h_shift;
    s_timingProp.v_shift = v_shift;
    s_timingProp.dma_images = dma_images;
    s_timingProp.dma_accum_time = dma_accum_time;
    _setTimingParams();
}

void Camera::setTimingParams(TimingProperties timingParameters)
{
    s_timingProp = timingParameters;
    _setTimingParams();
}


/*
 *  Shutter parameters
 */
ShutterProperties Camera::getShutterParams()
{
    return s_shutterProp;
}

void Camera::setShutterParams(int mode, int open, int close)
{
    s_shutterProp.mode = mode;
    s_shutterProp.open_time = open;
    s_shutterProp.close_time = close;
    _setShutterParams();
}

void Camera::setShutterParams(ShutterProperties shutterParameters)
{
    s_shutterProp = shutterParameters;
    _setShutterParams();
}

TemperatureProperties Camera::getTempParams()
{
    return s_tempProp;
}

TemperatureProperties Camera::getTempArray()
{
    _getTempArray();
    return s_tempProp;
}

void Camera::setTempParams(int set_point, bool state)
{
    s_tempProp.set_point = set_point;
    s_tempProp.power_state = state;
    _setTempParams();
}

void Camera::setTempParams(TemperatureProperties tempParameters)
{
    s_tempProp = tempParameters;
    _setTempParams();
}


/*
 * Getter/setters for servo
 */
Gain Camera::getGainX()
{
    s_gainxProp = t_cam_thread->getServoGainX();
    return s_gainxProp;
}

Gain Camera::getGainY()
{
    s_gainyProp = t_cam_thread->getServoGainY();
    return s_gainyProp;
}

void Camera::setGain(Gain gainx, Gain gainy)
{
    s_gainxProp = gainx;
    s_gainyProp = gainy;
    t_cam_thread->setServoGain(s_gainxProp, s_gainyProp);
}

void Camera::getTargetCoords(float *x, float *y)
{
    t_cam_thread->getServoTargetCoords(x, y);
}

void Camera::setTargetCoords(float x, float y)
{
    t_cam_thread->setServoTargetCoords(x, y);
}

float Camera::getRotation()
{
    return t_cam_thread->getServoRotation();
}

void Camera::setRotation(float rot)
{
    t_cam_thread->setServoRotation(rot);
}

bool Camera::setServoState(bool state)
{
    return t_cam_thread->setServoState(state);
}


/*
 * Camera initialization
 */
void Camera::_initializeCamera(int temp)
{

    //if ((camera_name == "FTT") || (camera_name == "NULL") || (camera_name == ""))
    t_cam_thread = new CameraThread(0, cam_data);

    if (camera_name == "FTT")
    {
        qDebug() << "Setting to default FTT values";
        // Set read parameters
        s_readProp.read_mode = 4; //Image
        s_readProp.acq_mode = 5;  // Run till abort
        s_readProp.frame_transfer = 1;  // On
        s_readProp.output_amp = 0;  // Em amplifier

        // Set timing parameters
        s_timingProp.h_shift = 0;  // Fastest horizontal shift speed
        s_timingProp.v_shift = 0;  // Fastest vertical shift speed
        s_timingProp.dma_images = 1;  // Maximum number of images in DMA buffer
        s_timingProp.dma_accum_time = float(0.001);  // Minimum time between hardware interrupts

        // Set shutter parameters
        s_shutterProp.type = 0;  // TTL low
        s_shutterProp.mode = 0;  // Auto
        s_shutterProp.open_time = 30;  //
        s_shutterProp.close_time = 30;

        // Set image properties and dimension
        s_imageDim.h_bin = 1;
        s_imageDim.v_bin = 1;
        s_imageDim.h_start = 1;
        s_imageDim.v_start = 1;
        s_imageDim.h_end = 512;
        s_imageDim.v_end = 512;

        // Set exposure properties
        s_expProp.em_gain = 1;
        s_expProp.exp_time = float(0.1);
        s_expProp.ext_trig = false;

        // Set gain
        s_gainxProp.kp = float(0.01);
        s_gainxProp.ki = float(0.0);
        s_gainxProp.kd = float(0.0);
        s_gainxProp.dt = s_expProp.exp_time;
        s_gainyProp = s_gainxProp;
        setGain(s_gainxProp, s_gainyProp);

    }
    else if (camera_name == "ExpM")
    {
        qDebug() << "Setting to default ExpM values";
        // Set read parameters
        s_readProp.read_mode = 4; //Image
        s_readProp.acq_mode = 5;  // Run till abort
        s_readProp.frame_transfer = 1;  // On
        s_readProp.output_amp = 0;  // Em amplifier
        s_readProp.track_cent = 256;
        s_readProp.track_height = 50;

        // Set timing parameters
        s_timingProp.h_shift = 3;  // Slowest horizontal shift speed
        s_timingProp.v_shift = 4;  // Slowest vertical shift speed
        s_timingProp.dma_images = 1;  // Maximum number of images in DMA buffer
        s_timingProp.dma_accum_time = float(0.001);  // Minimum time between hardware interrupts

        // Set shutter parameters
        s_shutterProp.type = 0;  // TTL low
        s_shutterProp.mode = 0;  // Fully auto
        s_shutterProp.open_time = 30;  //
        s_shutterProp.close_time = 30;

        // Set image properties and dimension
        s_imageDim.h_bin = 1;
        s_imageDim.v_bin = 1;
        s_imageDim.h_start = 1;
        s_imageDim.v_start = 1;
        s_imageDim.h_end = 512;
        s_imageDim.v_end = 512;

        // Set exposure properties
        s_expProp.em_gain = 1;
        s_expProp.exp_time = 1.0;
        s_expProp.ext_trig = false;
    }
    else
    {
        qDebug() << "Setting to full image mode values";
        // Set read parameters
        s_readProp.read_mode = 4; //Image
        s_readProp.acq_mode = 5;  // Run till abort
        s_readProp.frame_transfer = 1;  // On
        s_readProp.output_amp = 0;  // Em amplifier

        // Set timing parameters
        s_timingProp.h_shift = 0;  // Fastest horizontal shift speed
        s_timingProp.v_shift = 0;  // Fastest vertical shift speed
        s_timingProp.dma_images = 1;  // Maximum number of images in DMA buffer
        s_timingProp.dma_accum_time = float(0.001);  // Minimum time between hardware interrupts

        // Set shutter parameters
        s_shutterProp.type = 0;  // TTL low
        s_shutterProp.mode = 0;  // Fully auto
        s_shutterProp.open_time = 30;  //
        s_shutterProp.close_time = 30;

        // Set image properties and dimension
        s_imageDim.h_bin = 1;
        s_imageDim.v_bin = 1;
        s_imageDim.h_start = 1;
        s_imageDim.v_start = 1;
        s_imageDim.h_end = 512;
        s_imageDim.v_end = 512;

        // Set exposure properties
        s_expProp.em_gain = 1;
        s_expProp.exp_time = float(0.01);
        s_expProp.ext_trig = false;

    }

    if (real_cam)
    {
        qDebug() << "Initializing " << camera_name;
        char det_file[] = "";
        ui_error = Initialize(det_file);
        checkError(ui_error, "Initialize");

//        qDebug() << "Checking gain settings";
//        ui_error = SetEMAdvanced(0);  // Disable higher EM gains
//        checkError(ui_error, "SetEMAdvanced");
//        qDebug() << "Disabled high gains";
        ui_error = GetEMGainRange(&s_expProp.em_gain_low, &s_expProp.em_gain_high);
        checkError(ui_error, "GetEMGainRange");

        ui_error = GetTemperatureRange(&s_tempProp.temp_low, &s_tempProp.temp_high);
        checkError(ui_error, "GetTemperatureRange");


    }

    else
    {
        s_tempProp.temp_low = -273;
        s_tempProp.temp_high = 200;
    }

    // Set cooling point and power on cooler
    s_tempProp.set_point = temp;
    s_tempProp.power_state = true;

    _setReadParams();
    _setReadParams();
    _setTimingParams();
    _setShutterParams();
    _setImageDims();
    _setExposureParams();
    _setTempParams();

    qDebug() << "Intialized" << camera_name << "camera";
}


/*
 * Internal function to set image properties
 */
void Camera::_setImageDims()
{
    qDebug() << "Setting image properties";
    // Calculate frame size
    s_imageDim.h_dim = s_imageDim.h_end - s_imageDim.h_start + 1;
    s_imageDim.v_dim = s_imageDim.v_end - s_imageDim.v_start + 1;

    qDebug() << "Checking horizontal dimension";
    // Horizontal
    if ((s_imageDim.h_dim > 512) ||
            (s_imageDim.h_start < 1) || (s_imageDim.h_start > 512) ||
            (s_imageDim.h_end < 1) || (s_imageDim.h_end > 512)
        )
    {
        qDebug() << "Reseting horizontal";
        qDebug() << "HStart: " << s_imageDim.h_start << "    VStart: " << s_imageDim.v_start << endl
                 << "HBin: " << s_imageDim.h_bin << "    VBin: " << s_imageDim.v_bin << endl
                 << "HEnd: " << s_imageDim.h_end << "    VEnd: " << s_imageDim.v_end;
        s_imageDim.h_start = 1;
        s_imageDim.h_end = 512;
        s_imageDim.h_dim = (s_imageDim.h_end - s_imageDim.h_start + 1) / s_imageDim.h_bin;
    }

    qDebug() << "Checking vertical dimension";
    // Vertical
    if ((s_imageDim.v_dim > 512) ||
            (s_imageDim.v_start < 1) || (s_imageDim.v_start > 512) ||
            (s_imageDim.v_end < 1) || (s_imageDim.v_end > 512)
        )
    {
        s_imageDim.v_start = 1;
        s_imageDim.v_end = 512;
        s_imageDim.v_dim = (s_imageDim.v_end - s_imageDim.v_start + 1) / s_imageDim.v_bin;
    }

    qDebug() << "Recalculating total size";
    // Recalculate total size and set flag
    s_imageDim.size = s_imageDim.h_dim * s_imageDim.v_dim;


    if (real_cam)
    {
        qDebug() << "Programming camera";
        ui_error = SetImage(s_imageDim.h_bin,
                            s_imageDim.v_bin,
                            s_imageDim.h_start,
                            s_imageDim.h_end,
                            s_imageDim.v_start,
                            s_imageDim.v_end);
        checkError(ui_error, "SetImage");
    }
    qDebug() << "Done!";
}

void Camera::_setExposureParams()
{
    qDebug() << "Setting read properties";
    // Program exposure properites
    if (real_cam)
    {
        // Check gain levels and set
        ui_error = GetEMGainRange(&s_expProp.em_gain_low, &s_expProp.em_gain_high);
//        qDebug() << "Allowed EM gain: " << s_expProp.em_gain_low << " to " << s_expProp.em_gain_high;
        checkError(ui_error, "GetEMGainRange");

        if (s_expProp.em_gain > (s_expProp.em_gain_high))
        {
            qDebug() << "Specified EM gain too high";
            s_expProp.em_gain = s_expProp.em_gain_high;
        }
        else if (s_expProp.em_gain < (s_expProp.em_gain_low))
        {
            qDebug() << "Specified EM gain too low";
            s_expProp.em_gain = s_expProp.em_gain_low;
        }

        ui_error = SetEMCCDGain(s_expProp.em_gain);
        checkError(ui_error, "SetEMCCDGain");
        qDebug() << "EM gain set to " << s_expProp.em_gain;

        ui_error = SetExposureTime(s_expProp.exp_time);
        checkError(ui_error, "SetExposureTime");

        if (s_expProp.ext_trig)
        {
            qDebug() << "Setting for external trigger";
            ui_error = SetTriggerMode(6);
        }
        else
        {
            qDebug() << "Setting for internal trigger";
            ui_error = SetTriggerMode(0);
        }
        checkError(ui_error, "SetTriggerMode");

        ui_error = GetAcquisitionTimings(&s_expProp.exp_time,
                                         &s_expProp.accum_time,
                                         &s_expProp.kinetic_time);
        checkError(ui_error, "GetAcquisitionTime");
        qDebug() << "Exposure time set to: " << s_expProp.exp_time << endl
                 << "Total accumulation time: " << s_expProp.accum_time << endl
                 << "Kinetic time: " << s_expProp.kinetic_time;

    }
    qDebug() << "Done!";
}


void Camera::_setReadParams()
{
    qDebug() << "Setting read properties";
    // Check if FVB or single track
    if ((s_readProp.read_mode == 3) || (s_readProp.read_mode == 0))
    {
        // Need to adjust image dimension for array size
        s_imageDim.h_start = 1;
        s_imageDim.h_end = 512;
        s_imageDim.v_start = 1;
        s_imageDim.v_end = 1;
        setImageDims(s_imageDim);
    }
    // Program camera read properties
    if (real_cam)
    {
        ui_error = SetReadMode(s_readProp.read_mode);
        checkError(ui_error, "SetReadMode");

        // If single track or FVB, set track
        if ((s_readProp.read_mode == 3) || (s_readProp.read_mode == 0))
        {
            ui_error = SetSingleTrack(s_readProp.track_cent, s_readProp.track_height);
            checkError(ui_error, "SetSingleTrack");
        }

        ui_error = SetAcquisitionMode(s_readProp.acq_mode);
        checkError(ui_error, "SetAcquisitionMode");

        ui_error = SetFrameTransferMode(s_readProp.frame_transfer);
        checkError(ui_error, "SetFrameTransfer");

        ui_error = SetOutputAmplifier(s_readProp.output_amp);
        checkError(ui_error, "SetOutputAmplifier");
    }
    qDebug() << "Done!";

}

void Camera::_setTimingParams()
{
    qDebug() << "Setting timing properties";
    // Program camera timing properties
    if (real_cam)
    {
        ui_error = SetVSSpeed(s_timingProp.v_shift);  // Fastest vertical shift speed
        checkError(ui_error, "SetVSSpeed");

        ui_error = SetHSSpeed(0, s_timingProp.h_shift);  // Fastest horizontal shift speed
        checkError(ui_error, "SetHSSpeed");

        //ui_error = SetNumberAccumulations(s_timingProp.dma_images);  // Do not accumulate images onboard camera
        //checkError(ui_error, "SetNumberAccumulations");

        ui_error = SetDMAParameters(s_timingProp.dma_images, s_timingProp.dma_accum_time);
        checkError(ui_error, "SetDMAParameters");
    }
    qDebug() << "Done!";
}

void Camera::_setShutterParams()
{
    qDebug() << "Setting shutter properties";
    // Program shutter properties
    if (real_cam)
    {
        ui_error = SetShutter(s_shutterProp.type,
                              s_shutterProp.mode,
                              s_shutterProp.close_time,
                              s_shutterProp.open_time);
        checkError(ui_error, "SetShutter");
    }
    qDebug() << "Done!";
}

void Camera::_setTempParams()
{
    qDebug() << "Setting temperature properties";
    if (real_cam)
    {
        // Set point
        if (s_tempProp.set_point > s_tempProp.temp_high)
        {
            qDebug() << "Specified temperature too high";
            s_tempProp.array_temp = s_tempProp.temp_high;
        }
        else if (s_tempProp.set_point < s_tempProp.temp_low)
        {
            qDebug() << "Specified temperature too low";
        }
        ui_error = SetTemperature(s_tempProp.set_point);
        checkError(ui_error, "SetTemperature");
        qDebug() << "Temperature set to " << s_tempProp.set_point;

        // Power state
        if (s_tempProp.power_state)
        {
            CoolerON();
        }
        else
        {
            CoolerOFF();
        }
    }
    else
    {
        s_tempProp.array_temp = s_tempProp.set_point;
    }

    _getTempArray();
    qDebug() << "Array temperaturea at " << s_tempProp.array_temp;
    qDebug() << "Done!";
}

void Camera::_getTempArray()
{
    if (real_cam)
    {
        // Array temp
        ui_error = GetTemperature(&s_tempProp.array_temp);
        if (ui_error == DRV_TEMPERATURE_OFF)
        {
            s_tempProp.array_temp = 15;
            s_tempProp.cooler_state = ui_error;
        }
        else if ((ui_error == DRV_TEMPERATURE_DRIFT) ||
                 (ui_error == DRV_TEMPERATURE_NOT_REACHED) ||
                 (ui_error == DRV_TEMPERATURE_NOT_STABILIZED) ||
                 (ui_error == DRV_TEMPERATURE_STABILIZED) ||
                 (ui_error == DRV_ACQUIRING))
        {
            s_tempProp.cooler_state = ui_error;
        }
        else checkError(ui_error, "GetTemperature");
    }
    else
    {
        s_tempProp.array_temp = s_tempProp.set_point;
        s_tempProp.cooler_state = 0;

    }

}

void Camera::_shutdownCamera()
{
    s_shutterProp.mode = 2;  // Set shutter to permanently closed
    _setShutterParams();

    if (real_cam)
    {
        // Cannot shutdown camera with cold array
        _warmArray();
        ui_error = ShutDown();
        checkError(ui_error, "ShutDown");
    }

    qDebug() << camera_name << "successfully shutdown";
    // Free data array memory
    if(cam_data) delete[] cam_data;
}


/*
 * Function to warm up array
 * Camera cannot be shutdown while array is cold
 */
void Camera::_warmArray()
{
    qDebug() << "Checking array temp\n...";

    _getTempArray();
    if ((s_tempProp.array_temp < 0) || (s_tempProp.set_point < 0))
    {
        s_tempProp.set_point = 0;
        _setTempParams();
    }

    if (real_cam)
    {
        qDebug() << "Array at" << s_tempProp.array_temp << endl;
        while (s_tempProp.array_temp < 0)
        {
            Sleep(5000);  // Wait 5 seconds
            _getTempArray();
            qDebug() << "Warming array, array at: " << s_tempProp.array_temp << "\n...";
        }
    }
}

/*
 * Error handler for camera API
 */
bool Camera::checkError(unsigned int _ui_err, const char* _cp_func)
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
