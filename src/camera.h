#ifndef CAMERA_H
#define CAMERA_H

#include <cstdint>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QDebug>
#include "camerathread.h"
#include "ATMCD32D.H"


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
    bool ext_trig;
};

struct ReadProperties{
    int read_mode;
    int acq_mode;
    int frame_transfer;
    int output_amp;
    int track_cent;
    int track_height;
};

struct TimingProperties{
    int h_shift;
    int v_shift;
    int v_amp;
    int dma_images;
    float dma_accum_time;
};

struct ShutterProperties{
    int type;
    int mode;
    int open_time;
    int close_time;
};

struct TemperatureProperties{
    int set_point;
    int array_temp;
    int temp_high;
    int temp_low;
    bool power_state;
    int cooler_state;
};

typedef void (*cb_cam_func)(uint16_t *buffer, int size, void *user_data);

class Camera
{

public:
    Camera();
    ~Camera();
    void initializeCamera(std::string cam_name, bool r_cam);
    void shutdownCamera();
    void startCamera(int loopCond, std::string filename=NULL);
    void startCamera(cb_cam_func cb, void *user_data);
    void singleShot(cb_cam_func cb, void *user_data);
    void stopCamera();
    bool isCameraRunning();
    void getCameraData(uint16_t *buffer);
    void getServoData(float *updates);

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

    TemperatureProperties getTempParams();
    TemperatureProperties getTempArray();
    void setTempParams(int set_point, bool state);
    void setTempParams(TemperatureProperties tempParameters);


    // Servo getters and setters
    Gain getGainX();
    Gain getGainY();
    void setGain(Gain gainx, Gain gainy);

    void getTargetCoords(float *x, float *y);
    void setTargetCoords(float x, float y);

    float getRotation();
    void setRotation(float rot);

    bool setServoState(bool state);

    float getBackground();
    void setBackground(float background);

    float getLeakyFactor();
    void setLeakyFactor(float factor);


    // Logger getters and setters
    unsigned int getLogInterval();
    bool setLogState(bool state);
    unsigned int setLogInterval(unsigned int frames);


private:
    void _initializeCamera();
    void _setImageDims();
    void _setExposureParams();
    void _setReadParams();
    void _setTimingParams();
    void _setShutterParams();
    void _setTempParams();
    void _getTempArray();
    void _shutdownCamera();
    void _warmArray();

    ImageDimension s_imageDim;
    ExposureProperties s_expProp;
    ReadProperties s_readProp;
    TimingProperties s_timingProp;
    ShutterProperties s_shutterProp;
    TemperatureProperties s_tempProp;
    Gain s_gainxProp;
    Gain s_gainyProp;
    CameraThread *t_cam_thread;

    bool checkError(unsigned int _ui_err, const char* _cp_func);

    QString camera_name;
    uint16_t *cam_data;
    unsigned int ui_error;
    bool b_gblerrorFlag;
    bool b_gblAcquireFlag;

    bool real_cam;
    bool fake_cam_running;


};

#endif // CAMERA_H
