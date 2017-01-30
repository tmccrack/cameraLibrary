#ifndef CAMERA_H
#define CAMERA_H


#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QDebug>
#include "camerathread.h"
#include "ATMCD32D.H"

using namespace std;

struct TimingProperties{
    int h_shift;
    int v_shift;
    int dma_images;
    float dma_accum_time;
};

struct ReadProperties{
    int read_mode;
    int acq_mode;
    int frame_transfer;
    int output_amp;
};

struct ImageDimension{
    int h_start;
    int h_end;
    int h_bin;
    int v_start;
    int v_end;
    int v_bin;
    int h_dim;
    int v_dim;
    unsigned long size;
};

struct ExposureProperties{
    float exp_time;
    float accum_time;
    float kinetic_time;
    int em_gain;
    int em_gain_high;
    int em_gain_low;
};

struct ShutterProperties{
    int type;
    int mode;
    int open_time;
    int close_time;
};

class Camera
{

public:
    Camera();
    ~Camera();
    void initializeCamera(string cam_name, bool r_cam);
    void shutdownCamera();
    void startCamera();
    void stopCamera();
    bool isCameraRunning();
    void getCameraData(int *buffer);
//    bool cooler(int *state);
//    bool cooler(bool pwr);

    ImageDimension getImageDims();
    void setImageDims(int hstart, int hend, int vstart, int vend, int hbin, int vbin);
    void setImageDims(ImageDimension imageParameters);

    ExposureProperties getExposureParams();
    void setExposureParams(float exposure, int em_gain);
    void setExposureParams(ExposureProperties expParameters);

    ReadProperties getReadParams();
    void setReadParams(int read_mode, int acq_mode, int frame_transfer, int output_amp);
    void setReadParams(ReadProperties readParameters);

    TimingProperties getTimingParams();
    void setTimingParams(int h_shift, int v_shift, int dma_images, int dma_accum_time);
    void setTimingParams(TimingProperties timingParameters);

    ShutterProperties getShutterParams();
    void setShutterParams(int mode, int open, int close);
    void setShutterParams(ShutterProperties shutterParameters);

private:
    void _initializeCamera();
    void _setImageDims();
    void _setExposureParams();
    void _setReadParams();
    void _setTimingParams();
    void _setShutterParams();
    void setCooler(int temperature);
    void _shutdownCamera();
    void _getArrayTemp();
    void _warmArray();

    ImageDimension s_imageDim;
    ExposureProperties s_expProp;
    ReadProperties s_readProp;
    TimingProperties s_timingProp;
    ShutterProperties s_shutterProp;
    CameraThread *t_cam_thread;

    bool checkError(unsigned int _ui_err, const char* _cp_func);

    QString camera_name;
    int *cam_data;
    unsigned int ui_error;
    bool b_gblerrorFlag;
    bool b_gblAcquireFlag;

    int array_temp;
    bool real_cam;
    bool fake_cam_running;


};

#endif // CAMERA_H
