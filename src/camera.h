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
    setFrameSize(int hstart, int hend, int vstart, int vend, int hbin, int vbin);
    ImageDim getFrameSize();
    setImageParams(int read_mode, int acq_mode, int dma_images, int dma_accum_time);
    ReadProperties getImageParams();
    setTimingParams(int h_shift, int v_shift);
    TimingProperties getTimingParam();
    getCameraData(long *buffer);
    getTemperature(int *temperature);
    setTemperature(int temperature);


signals:

public slots:


private:
    initializeCamera();
    setExposureTime();
    setupCamera();
    checkFrameSize();
    bool checkError();
    QString camera_name;
    CameraThread camThread;

    struct TimingProperties{

    };

    struct ReadProperties{

    };

    struct ImageDim{
        int hstart;
        int hend;
        int hbin;
        int vstart;
        int vend;
        int vbin;
        int hdim;
        int vdim;
        unsigned long size;
    } imageDim;

};

#endif // CAMERA_H
