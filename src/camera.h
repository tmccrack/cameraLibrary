#ifndef CAMERA_H
#define CAMERA_H


#include <QObject>
#include <QString>
#include <QDebug>
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
    int em_gain;
    int *em_gain_high;
    int *em_gain_low;
};

struct ShutterProperties{
    int type;
    int mode;
    int open_time;
    int close_time;
};

class Camera : public QObject
{
    Q_OBJECT

public:
    explicit Camera(QObject *parent = 0, QString cam_name = "" , bool r_cam = false);
    void startCamera();
    bool isCameraRunning();
    void stopCamera();
    void getArrayTemp(int *temperature);
    void setArrayTemp(int temperature);
    void getCameraData(long *buffer);
    void shutdownCamera();

    ImageDimension getImageDims();
    void setImageDims(int hstart, int hend, int vstart, int vend, int hbin, int vbin);
    void setImageDims(ImageDimension imageParameters);
    ExposureProperties getExposureParams();
    void setExposureParams(float exposure, int em_gain);
    void setExposureParams(ExposureProperties expParameters);
    ReadProperties getImageParams();
    void setReadParams(int read_mode, int acq_mode, int frame_transfer, int output_amp);
    void setReadParams(ReadProperties readParameters);
    TimingProperties getTimingParams();
    void setTimingParams(int h_shift, int v_shift, int dma_images, int dma_accum_time);
    void setTimingParams(TimingProperties timingParameters);
    ShutterProperties getShutterParams();
    void setShutterParams(int mode, int open, int close);
    void setShutterParams(ShutterProperties shutterParameters);

signals:

public slots:

private:
    void _initializeCamera();
    void setImageProperties();
    void setExposureProperties();
    void setReadProperties();
    void setTimingProperties();
    void setShutterProperties();
    void _shutdownCamera();
    void _getArrayTemp();
    void _warmArray();

    ImageDimension s_imageDim;
    ExposureProperties s_expProp;
    ReadProperties s_readProp;
    TimingProperties s_timingProp;
    ShutterProperties s_shutterProp;
    CameraThread *t_cam_thread;

    void setCooler(int temperature);
    bool checkError(unsigned int _ui_err, const char* _cp_func);

    QString camera_name;
    long *cam_data;
    unsigned int ui_error;
    bool b_gblerrorFlag;
    bool b_gblAcquireFlag;

    int array_temp;
    bool real_cam;
    bool fake_cam_running;

};

#endif // CAMERA_H
