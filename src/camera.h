#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QString>
#include "camerathread.h"
#include "ATMCD32D.H"

using namespace std;

class Camera : public QObject
{
    Q_OBJECT
public:
    explicit Camera(QObject *parent = 0, string camera_name, char *argv[]);
    startCamera();
    stopCamera();
    getTemperature(int *temperature);
    setTemperature(int temperature);
    getCameraData(long *buffer);

    ImageDimension getImageDims();
    setImageDims(int hstart, int hend, int vstart, int vend, int hbin, int vbin);
    setImageDims(ImageDim imageParameters);
    ExposureProperties getExposureParams();
    setExposureParams(float exposure, int em_gain);
    setExposureParams(ExposureProperties expParameters);
    ReadProperties getImageParams();
    setReadParams(int read_mode, int acq_mode, int frame_transfer, int output_amp);
    setReadParams(ReadProperties readParameters);
    TimingProperties getTimingParams();
    setTimingParams(int h_shift, int v_shift, int dma_images, int dma_accum_time);
    setTimingParams(TimingProperties timingParameters);
    ShutterProperties getShutterParams();
    setShutterParams(int mode, int open, int close);
    setShutterParams(ShutterProperties shutterParameters);


signals:

public slots:

private:
    initializeCamera();
    setImageProperties();
    setExposureProperties();
    setReadProperties();
    setTimingProperties();
    setShutterProperties();

    ImageDimension s_imageDim;
    ExposureProperties s_expProp;
    ReadProperties s_readProp;
    TimingProperties s_timingProp;
    ShutterProperties s_shutterProp;



    setCooler(int temperature);

    QString camera_name;
    CameraThread t_camThread;
    unsigned int ui_error;
    int array_temp;
    bool realcam;

};

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
};

struct ShutterProperties{
    int type;
    int mode;
    int open_time;
    int close_time;
};

#endif // CAMERA_H
