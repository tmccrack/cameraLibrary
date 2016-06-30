#include "cameralibrary.h"
#include "camerathread.h"
#include "socketclient.h"
#include <stdio.h>

/*
 * Initiliaze camera and data structures
 */
void initializeCameraLV()
{
    //clearError();  // Reset global error flag

    char detFile[] = "";
    ui_error = Initialize(detFile);
    checkError(ui_error, "Initialize");

    setupCamera();
    // Initialize camera thread
    camThread = new CameraThread();
    closedThread = new ClosedLoopCameraThread();

    if (!b_gblerrorFlag)
    {
        printf("Camera initialized...\n");
        mutex.lock();
        b_gblAcquireFlag = false;
        mutex.unlock();
    }
}


/*
 * Acquire a single frame from the camera.
 */
void acquireSingleFullFrameLV(float expTime)
{
    ui_error = SetShutter(1, 0, 27, 27);  // Set shutter to auto
    checkError(ui_error, "SetShutter");

    ui_error = SetExposureTime(expTime);
    checkError(ui_error, "SetExposureTime");

    setFrameSizeLV(1, 512, 1, 512, 1, 1);

    /*
     * Wait for the an event, then cancel acquistion
     * Get data and display.
     */
    if (!b_gblerrorFlag)
    {
        printf("Starting acquisition...\n");
        b_gblAcquireFlag = true;
        StartAcquisition();

        ui_error = WaitForAcquisition();
        checkError(ui_error, "WaitForAcquisition");

        ui_error = AbortAcquisition();
        checkError(ui_error, "AbortAcquisition");
        b_gblAcquireFlag = false;

        ui_error = GetMostRecentImage(camData, imageDim.size);
        checkError(ui_error, "GetMostRecentImage");
    }
    else
    {
        printf("Cannot start camera acquisition...\n");
    }
}


/*
 * Acquire a single sub frame from the camera.
 */
void acquireSingleSubFrameLV(float expTime)
{
    //clearError();
    ui_error = SetShutter(1, 0, 27, 27);  // Set shutter to auto
    checkError(ui_error, "SetShutter");

    ui_error = SetExposureTime(expTime);
    checkError(ui_error, "SetExposureTime");

    //setFrameSizeLV(int hstart, int hend, int vstart, int vend, int hbin, int vbin)
    setFrameSizeLV(1, 32, 1, 32, 1, 1);


    /*
     * Create the data buffer and start acquisition
     * Wait for the an event, then cancel acquistion
     * Get data and display.
     */
    if (!b_gblerrorFlag)
    {
        printf("Starting acquisition...\n");
        b_gblAcquireFlag = true;
        StartAcquisition();

        ui_error = WaitForAcquisition();
        checkError(ui_error, "WaitForAcquisition");

        ui_error = AbortAcquisition();
        checkError(ui_error, "AbortAcquisition");
        b_gblAcquireFlag = false;

        ui_error = GetMostRecentImage(camData, imageDim.size);
        checkError(ui_error, "GetMostRecentImage");
    }

    else
    {
        printf("Cannot start camera acquisition...\n");
        b_gblAcquireFlag = false;
    }
}



/*
 * Acquire continuous full frames from the camera.
 * Differs from sub frame as shutter is set to auto.
 */
void acquireFullFrameLV(float expTime)
{
    // Verify camera is not acquiring first
    if (b_gblAcquireFlag)
    {
        abortAcquisitionLV();
    }

    ui_error = SetExposureTime(expTime);
    checkError(ui_error, "SetExposureTime");

    ui_error = SetShutter(1, 0, 27, 27);  // Set shutter to auto
    checkError(ui_error, "SetShutter");    

    //setFrameSizeLV(int hstart, int hend, int vstart, int vend, int hbin, int vbin)
    setFrameSizeLV(1, 512, 1, 512, 1, 1);  // Set to full camera readout

    // Start camera acquisition thread
    if (!b_gblerrorFlag)
    {
        printf("Starting acquisition...\n");
        mutex.lock();
        b_gblAcquireFlag = true;
        mutex.unlock();

        camThread->startCameraThread(imageDim.size, camData);
    }
    else
    {
        printf("Cannot start camera acquisition...\n");
        mutex.lock();
        b_gblAcquireFlag = false;
        mutex.unlock();
    }
}


/*
 * Acquire continuous sub frames from the camera.
 * Differs from full frame as shutter is set to always open.
 * Set frame size prior to calling.
 */
void acquireSubFrameLV(float expTime)
{
    // Verify camera is not acquiring first
    if (b_gblAcquireFlag)
    {
        abortAcquisitionLV();
    }

    ui_error = SetExposureTime(expTime);
    checkError(ui_error, "SetExposureTime");

    ui_error = SetShutter(1, 1, 27, 27);  // Set to always open
    checkError(ui_error, "SetShutter");

    // Start camera acquisition thread
    if (!b_gblerrorFlag)
    {
        printf("Starting acquisition...\n");
        mutex.lock();
        b_gblAcquireFlag = true;
        mutex.unlock();

        camThread->startCameraThread(imageDim.size, camData);
    }
    else
    {
        printf("Cannot start camera acquisition...\n");
        mutex.lock();
        b_gblAcquireFlag = false;
        mutex.unlock();
    }
}


void acquireClosedLoopLV(float expTime)
{
    // Verify camera is not acquiring first
    if (b_gblAcquireFlag)
    {
        abortAcquisitionLV();
    }

    ui_error = SetExposureTime(expTime);
    checkError(ui_error, "SetExposureTime");

    ui_error = SetShutter(1, 1, 27, 27);  // Set to always open
    checkError(ui_error, "SetShutter");    

    if (!b_gblerrorFlag)
    {
        printf("Starting closed loop acquisition...\n");
        mutex.lock();
        b_gblAcquireFlag = true;
        mutex.unlock();

        closedThread->startCameraThread(imageDim.hdim, imageDim.vdim, camData);
    }
    else
    {
        printf("Cannot start camera acquisition...\n");
        mutex.lock();
        b_gblAcquireFlag = false;
        mutex.unlock();
    }

}


/*
 * Copy latest camera data into array provided from labview if acquiring
 * If not acquiring, do nothing
 */
void getCameraDataLV(long *dataOut)
{
    if (b_gblAcquireFlag)
    {
        mutex.lock();
        std::copy(camData, camData + (long) imageDim.size, dataOut);
        mutex.unlock();

    }
}


/*
 * Set glogal flag to abort acquisition
 */
void abortAcquisitionLV()
{
    if (camThread->isRunning())
    {
        camThread->abortCameraThread();
        camThread->wait();
    }

    if (closedThread->isRunning())
    {
        closedThread->abortCameraThread();
        closedThread->wait();
    }

    mutex.lock();
    b_gblAcquireFlag = false;
    mutex.unlock();

    ui_error = SetShutter(1, 2, 27, 27);  // Close shutter
    checkError(ui_error, "SetShutter");
}


/*
 * Set camera frame
 */
void setFrameSizeLV(int hstart, int hend, int vstart, int vend, int hbin, int vbin)
{
    imageDim.hstart = hstart;
    imageDim.hend = hend;
    imageDim.vstart = vstart;
    imageDim.vend = vend;
    imageDim.hbin = hbin;
    imageDim.vbin = vbin;
    imageDim.hdim = (imageDim.hend - imageDim.hstart + 1) / imageDim.hbin;
    imageDim.vdim = (imageDim.vend - imageDim.vstart + 1) / imageDim.vbin;

    // Check that provided frame size is valid
    // Calculate total image size
    checkFrameSize();

    // Check if acquiring before setting new image dimensions
    if (b_gblAcquireFlag)
    {
        abortAcquisitionLV();
    }
    ui_error = SetImage(imageDim.hbin, imageDim.vbin, imageDim.hstart, imageDim.hend, imageDim.vstart, imageDim.vend);
    checkError(ui_error, "SetImage");
}

/*
 * TODO: Implement getter to check frame size in case it changes from specified.
 */


/*
 * Shutdown camera
 */
void shutdownCameraLV()
{
    if (b_gblAcquireFlag)
    {
        if (camThread->isRunning())
        {
            camThread->abortCameraThread();
            camThread->wait();
        }

        mutex.lock();
        b_gblAcquireFlag = false;
        mutex.unlock();
    }

    ui_error = SetShutter(1, 2, 27, 27);  // Close shutter
    checkError(ui_error, "SetShutter");

    ui_error = ShutDown();
    checkError(ui_error, "ShutDown");

    // Free data array memory
    if(camData) delete[] camData;
}


/*
 * Program camera settings
 */
void setupCamera()
{
    printf("Setting up camera...\n");
    ui_error = SetReadMode(4);  // Image
    checkError(ui_error, "SetReadMode");

    ui_error = SetAcquisitionMode(5); // Run till abort
    checkError(ui_error, "SetAcquisitionMode");

    ui_error = SetFrameTransferMode(1);  // Frame transfer on
    checkError(ui_error, "SetFrameTransferMode");

    ui_error = SetOutputAmplifier(0);  // EM amplifier
    checkError(ui_error, "SetOutputAmplifier");

    ui_error = SetVSSpeed(0);  // Fastest vertical shift speed
    checkError(ui_error, "SetVSSpeed");

    ui_error = SetHSSpeed(0, 0);  // Fastest horizontal shift speed
    checkError(ui_error, "SetHSSpeed");

    ui_error = SetNumberAccumulations(1);  // Do not accumulate images onboard camera
    checkError(ui_error, "SetNumberAccumulations");

    ui_error = SetDMAParameters(1, 0.001f);
    checkError(ui_error, "SetDMAParameters");
}


/*
 * Error handler for camera API
 */
bool checkError(unsigned int _ui_err, const char* _cp_func)
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


///*
// * Clear global error flag
// */
//void clearError()
//{
//    if (b_gblerrorFlag)
//    {
//        printf("Error encountered... \t restarting camera\n");
//        shutdownCameraLV();
//        initializeCameraLV();
//        b_gblerrorFlag = false;
//    }
//}




/*
 * Check that provided frame size is valid for camera
 * If frame dimensions invalid, set that direction to full width of chip
 */
void checkFrameSize()
{
    // Horizontal
    if ((imageDim.hdim > 512) ||
            (imageDim.hstart < 1) || (imageDim.hstart > 512) ||
            (imageDim.hend < 1) || (imageDim.hend > 512)
        )
    {
        imageDim.hstart = 1;
        imageDim.hend = 512;
        imageDim.hdim = 512;
        imageDim.hdim = (imageDim.hend - imageDim.hstart + 1) / imageDim.hbin;
    }

    // Vertical
    if ((imageDim.vdim > 512) ||
            (imageDim.vstart < 1) || (imageDim.vstart > 512) ||
            (imageDim.vend < 1) || (imageDim.vend > 512)
        )
    {
        imageDim.vstart = 1;
        imageDim.vend = 512;
        imageDim.vdim = 512;
        imageDim.vdim = (imageDim.vend - imageDim.vstart + 1) / imageDim.vbin;
    }

    // Recalculate total size and set flag
    imageDim.size = imageDim.hdim * imageDim.vdim;
}


/*
 * Check the TEC on the Andor camera
 */
void coolerPower()
{
    /*
     * TODO: program coolerPower()
     */
}
