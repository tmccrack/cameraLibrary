#include "camera.h"


Camera::Camera(QObject *parent, string camera_name, char *argv[])
{
    camera_name = camera_name;
    initializeCamera();

}

Camera::startCamera()
{
// start thread, pass array buffer
}

Camera::stopCamera()
{
// check if thread running, abort and delete array
}

Camera::setFrameSize(int hstart, int hend, int vstart, int vend, int hbin, int vbin)
{

}

Camera::ImageDim Camera::getFrameSize()
{

}

Camera::setImageParams(int read_mode, int acq_mode, int dma_images, int dma_accum_time)
{

}

Camera::ReadProperties Camera::getImageParams()
{

}

Camera::setTimingParams(int h_shift, int v_shift)
{

}

Camera::TimingProperties Camera::getTimingParam()
{

}


Camera::getCameraData(long *buffer)
{
    copy(camData, camData + (long) imageDim.size, buffer);
}

Camera::getTemperature(int *temperature)
{

}

Camera::initializeCamera()
{
    char detFile[] = "";
    ui_error = Initialize(detFile);
    checkError(ui_error, "Initialize");
    Initialize(char[] "");

}
