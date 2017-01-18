#include "camera.h"

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
//    if (cam_data) delete[] cam_data;
}

void Camera::initializeCamera(string cam_name, bool r_cam)
{
    cam_data = new int[262144];  // Initialize camera data buffer
    camera_name = QString::fromStdString(cam_name);
    real_cam = r_cam;
    if (!real_cam)
    {
        // Generate fake data
        for (int i = 0; i < 262144; i++)
        {
            cam_data[i] = i;
        }
        qDebug() << "Fake camera initialized";
    }
    _initializeCamera();
    b_gblerrorFlag = false;  // Clear error flag
}

void Camera::startCamera()
{
    if (real_cam)
    {
        t_cam_thread = new CameraThread(0, s_imageDim.size, cam_data);
        t_cam_thread->startCameraThread();
    }
    else fake_cam_running = true;
}

bool Camera::isCameraRunning()
{
    if (real_cam)
    {
        if (t_cam_thread) return t_cam_thread->isRunning();
        else return false;
    }
    else return fake_cam_running;
}

void Camera::stopCamera()
{
    if (real_cam)
    {
        // check if thread running, abort and delete array
        if (t_cam_thread->isRunning())
        {
            t_cam_thread->abortCameraThread();
            t_cam_thread->wait();
        }

    }
    else fake_cam_running = false;

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
void Camera::getCameraData(int *buffer)
{
    copy(cam_data, cam_data + (int) s_imageDim.size, buffer);
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
    setImageProperties();
}

void Camera::setImageDims(ImageDimension imageParameters)
{
    s_imageDim = imageParameters;
    setImageProperties();
}


///*
// * Exposure and EM gain parameters
// */
//ExposureProperties Camera::getExposureParams()
//{
//    return s_expProp;
//}

//void Camera::setExposureParams(ExposureProperties expParameters)
//{
//    s_expProp = expParameters;
//    setExposureProperties();
//}

//void Camera::setExposureParams(float exposure, int em_gain)
//{
//    s_expProp.exp_time = exposure;
//    s_expProp.em_gain = em_gain;
//    setExposureProperties();
//}


///*
// * Camera read setup parameters
// */
//ReadProperties Camera::getReadParams()
//{
//    return s_readProp;
//}

//void Camera::setReadParams(ReadProperties readParameters)
//{
//    s_readProp = readParameters;
//    setReadProperties();
//}

//void Camera::setReadParams(int read_mode, int acq_mode, int frame_transfer, int output_amp)
//{
//    s_readProp.read_mode = read_mode;
//    s_readProp.acq_mode = acq_mode;
//    s_readProp.frame_transfer = frame_transfer;
//    s_readProp.output_amp = output_amp;
//    setReadProperties();
//}


///*
// * Camera read timing parameters
// */
//TimingProperties Camera::getTimingParams()
//{
//    return s_timingProp;
//}

//void Camera::setTimingParams(int h_shift, int v_shift, int dma_images, int dma_accum_time)
//{
//    s_timingProp.h_shift = h_shift;
//    s_timingProp.v_shift = v_shift;
//    s_timingProp.dma_images = dma_images;
//    s_timingProp.dma_accum_time = dma_accum_time;
//    setTimingProperties();
//}

//void Camera::setTimingParams(TimingProperties timingParameters)
//{
//    s_timingProp = timingParameters;
//    setTimingProperties();
//}


///*
// *  Shutter parameters
// */
//ShutterProperties Camera::getShutterParams()
//{
//    return s_shutterProp;
//}

//void Camera::setShutterParams(int mode, int open, int close)
//{
//    s_shutterProp.mode = mode;
//    s_shutterProp.open_time = open;
//    s_shutterProp.close_time = close;
//    setShutterProperties();
//}

//void Camera::setShutterParams(ShutterProperties shutterParameters)
//{
//    s_shutterProp = shutterParameters;
//    setShutterProperties();
//}



///*
// * Return array temperature
// */
//void Camera::getArrayTemp(int *temperature)
//{
//    if (real_cam) _getArrayTemp();
//    *temperature = array_temp;
//}



//void Camera::setArrayTemp(int temperature)
//{
//    /*
//     * TODO:
//     * Add callback once temp reached?
//     */
//    if (real_cam) SetTemperature(temperature);
//    else array_temp = temperature;

//}

/*
 * Camera initialization
 */
void Camera::_initializeCamera()
{

    //if ((camera_name == "FTT") || (camera_name == "NULL") || (camera_name == ""))
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
        s_shutterProp.mode = 1;  // Always open
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
        s_expProp.exp_time = 0.1;

    }
    else if (camera_name == "ExpM")
    {
        qDebug() << "Setting to default ExpM values";
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
        s_expProp.exp_time = 0.01;

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
        s_expProp.exp_time = 0.01;

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
    }


    setReadProperties();
    setTimingProperties();
    setShutterProperties();
    setImageProperties();
    setExposureProperties();
    qDebug() << "Intialized" << camera_name << "camera";
}


/*
 * Internal function to set image properties
 */
void Camera::setImageProperties()
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

void Camera::setExposureProperties()
{
    qDebug() << "Setting read properties";
    // Program exposure properites
    if (real_cam)
    {
        // Check gain levels and set
        ui_error = GetEMGainRange(&s_expProp.em_gain_low, &s_expProp.em_gain_high);
        qDebug() << "Allowed EM gain: " << s_expProp.em_gain_low << " to " << s_expProp.em_gain_high;
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


void Camera::setReadProperties()
{
    qDebug() << "Setting read properties";
    // Program camera read properties
    if (real_cam)
    {
        ui_error = SetReadMode(s_readProp.read_mode);
        checkError(ui_error, "SetReadMode");

        ui_error = SetAcquisitionMode(s_readProp.acq_mode);
        checkError(ui_error, "SetAcquisitionMode");

        ui_error = SetFrameTransferMode(s_readProp.frame_transfer);
        checkError(ui_error, "SetFrameTransfer");

        ui_error = SetOutputAmplifier(s_readProp.output_amp);
        checkError(ui_error, "SetOutputAmplifier");
    }
    qDebug() << "Done!";

}

void Camera::setTimingProperties()
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

void Camera::setShutterProperties()
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

void Camera::setCooler(int temperature)
{
    if (real_cam) SetTemperature(temperature);
}

void Camera::_shutdownCamera()
{
    s_shutterProp.mode = 2;  // Set shutter to permanently closed
    setShutterProperties();

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



void Camera::_getArrayTemp()
{
    if (real_cam)
    {
        ui_error = GetTemperature(&array_temp);
        if (ui_error == DRV_TEMPERATURE_OFF)
        {
            array_temp = 15;
        }
        else checkError(ui_error, "GetTemperature");
    }
}


/*
 * Function to warm up array
 * Camera cannot be shutdown while array is cold
 */
void Camera::_warmArray()
{
    setCooler(0);
    _getArrayTemp();
    qDebug() << "Checking array temp\n...";
    if (real_cam)
    {
        qDebug() << "Array at" << array_temp << endl;
        while (array_temp < 0)
        {
            Sleep(5000);  // Wait 5 seconds
            _getArrayTemp();
            qDebug() << "Warming array, array at: " << array_temp << "\n...";
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
