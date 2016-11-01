#include "camera.h"


Camera::Camera(QObject *parent, QString cam_name)
{
    if (cam_name == "")
    {
        realcam = false;
        camera_name = "NULL";
    }
    else
    {
        realcam = true;
        camera_name = cam_name;
    }
    b_gblerrorFlag = false;  // Clear error flag

    initializeCamera();
}

void Camera::startCamera()
{

    t_cam_thread = new CameraThread(this, s_imageDim.size, cam_data);
    t_cam_thread->startCameraThread();
}

void Camera::stopCamera()
{
    // check if thread running, abort and delete array
    if (t_cam_thread->isRunning()) t_cam_thread->abortCameraThread();
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


/*
 * Exposure and EM gain parameters
 */
ExposureProperties Camera::getExposureParams()
{
    return s_expProp;
}

void Camera::setExposureParams(float exposure, int em_gain)
{
    s_expProp.exp_time = exposure;
    s_expProp.em_gain = em_gain;
    setExposureProperties();
}

void Camera::setExposureParams(ExposureProperties expParameters)
{
    s_expProp = expParameters;
    setImageProperties();
}


/*
 * Camera read setup parameters
 */
ReadProperties Camera::getImageParams()
{
    return s_readProp;
}

void Camera::setReadParams(int read_mode, int acq_mode, int frame_transfer, int output_amp)
{
    s_readProp.read_mode = read_mode;
    s_readProp.acq_mode = acq_mode;
    s_readProp.frame_transfer = frame_transfer;
    s_readProp.output_amp = output_amp;
    setReadProperties();
}

void Camera::setReadParams(ReadProperties readParameters)
{
    s_readProp = readParameters;
    setReadProperties();
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
    setTimingProperties();
}

void Camera::setTimingParams(TimingProperties timingParameters)
{
    s_timingProp = timingParameters;
    setTimingProperties();
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
    setShutterProperties();
}

void Camera::setShutterParams(ShutterProperties shutterParameters)
{
    s_shutterProp = shutterParameters;
    setShutterProperties();
}


/*
 * Retrieve current camera data buffer
 */
void Camera::getCameraData(long *buffer)
{
    copy(cam_data, cam_data + (long) s_imageDim.size, buffer);
}


/*
 * Return array temperature
 */
void Camera::getArrayTemp(int *temperature)
{
    *temperature = array_temp;
}



void Camera::setArrayTemp(int temperature)
{
    /*
     * TODO:
     * Add callback once temp reached?
     */
    if (realcam) SetTemperature(temperature);
    else array_temp = temperature;

}

/*
 * Camera initialization
 */
void Camera::initializeCamera()
{
    cam_data = new long[262144];  // Initialize camera data buffer
    char det_file[] = "";
    ui_error = Initialize(det_file);
    checkError(ui_error, "Initialize");

    if (camera_name == "FTT")
    {
        // Set read parameters
        s_readProp.read_mode = 4; //Image
        s_readProp.acq_mode = 0;  // Run till abort
        s_readProp.frame_transfer = 1;  // On
        s_readProp.output_amp = 0;  // Em amplifier
        setReadProperties();

        // Set timing parameters
        s_timingProp.h_shift = 0;
        s_timingProp.v_shift = 0;
        s_timingProp.dma_images = 1;
        s_timingProp.dma_accum_time = float(0.001);
        setTimingProperties();

        // Set shutter parameters
        s_shutterProp.type = 0;  // TTL low
        s_shutterProp.mode = 0;  // Fully auto
        s_shutterProp.open_time = 30;  //
        s_shutterProp.close_time = 30;
        setShutterProperties();

        // Set image properties and dimension
        s_imageDim.h_bin = 1;
        s_imageDim.v_bin = 1;
        s_imageDim.h_start = 1;
        s_imageDim.v_start = 1;
        s_imageDim.h_end = 512;
        s_imageDim.v_end = 512;
        setImageProperties();
    }
    else if (camera_name == "ExpM")
    {

    }

}


/*
 * Internal function to set image properties
 */
void Camera::setImageProperties()
{
    // Calculate frame size
    s_imageDim.h_dim = s_imageDim.h_end - s_imageDim.h_start + 1;
    s_imageDim.v_dim = s_imageDim.v_end - s_imageDim.v_start + 1;

    // Horizontal
    if ((s_imageDim.h_dim > 512) ||
            (s_imageDim.h_start < 1) || (s_imageDim.h_start > 512) ||
            (s_imageDim.h_end < 1) || (s_imageDim.h_end > 512)
        )
    {
        s_imageDim.h_start = 1;
        s_imageDim.h_end = 512;
        s_imageDim.h_dim = 512;
        s_imageDim.h_dim = (s_imageDim.h_end - s_imageDim.h_start + 1) / s_imageDim.h_bin;
    }

    // Vertical
    if ((s_imageDim.v_dim > 512) ||
            (s_imageDim.v_start < 1) || (s_imageDim.v_start > 512) ||
            (s_imageDim.v_end < 1) || (s_imageDim.v_end > 512)
        )
    {
        s_imageDim.v_start = 1;
        s_imageDim.v_end = 512;
        s_imageDim.v_dim = 512;
        s_imageDim.v_dim = (s_imageDim.v_end - s_imageDim.v_start + 1) / s_imageDim.v_bin;
    }

    // Recalculate total size and set flag
    s_imageDim.size = s_imageDim.h_dim * s_imageDim.v_dim;

    if (realcam)
    {
        ui_error = SetImage(s_imageDim.h_bin,
                            s_imageDim.v_bin,
                            s_imageDim.h_start,
                            s_imageDim.h_end,
                            s_imageDim.v_start,
                            s_imageDim.v_end);
        checkError(ui_error, "SetImage");
    }
}

void Camera::setExposureProperties()
{
    if (realcam)
    {
        // Check gain levels and set
        ui_error = GetEMGainRange(s_expProp.em_gain_low, s_expProp.em_gain_high);
        checkError(ui_error, "GetEMGainRange");
        if (s_expProp.em_gain > *(s_expProp.em_gain_high)) s_expProp.em_gain = *(s_expProp.em_gain_high);
        else if (s_expProp.em_gain < *(s_expProp.em_gain_low)) s_expProp.em_gain = *(s_expProp.em_gain_low);
        ui_error = SetEMCCDGain(s_expProp.em_gain);
        checkError(ui_error, "SetEMCCDGain");

        ui_error = SetExposureTime(s_expProp.exp_time);
        checkError(ui_error, "SetExposureTime");
    }
}


void Camera::setReadProperties()
{
    // Program camera read properties
    if (realcam)
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





}

void Camera::setTimingProperties()
{
    // Program camera timing properties
    if (realcam)
    {
        ui_error = SetVSSpeed(s_timingProp.v_shift);  // Fastest vertical shift speed
        checkError(ui_error, "SetVSSpeed");

        ui_error = SetHSSpeed(0, s_timingProp.h_shift);  // Fastest horizontal shift speed
        checkError(ui_error, "SetHSSpeed");

        ui_error = SetNumberAccumulations(s_timingProp.dma_images);  // Do not accumulate images onboard camera
        checkError(ui_error, "SetNumberAccumulations");

        ui_error = SetDMAParameters(1, s_timingProp.dma_accum_time);
        checkError(ui_error, "SetDMAParameters");
    }
}

void Camera::setShutterProperties()
{
    // Program shutter properties
    if (realcam)
    {
        ui_error = SetShutter(s_shutterProp.type,
                              s_shutterProp.mode,
                              s_shutterProp.close_time,
                              s_shutterProp.open_time);
        checkError(ui_error, "SetShutter");
    }
}

void Camera::setCooler(int temperature)
{
    if (realcam) SetTemperature(temperature);
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
