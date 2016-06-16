#include "cameralibrary.h"
#include "camerathread.h"
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
    //setFrameSizeLV(1, 512, 1, 512, 1, 1);  // Initialize frame size to full frame

    if (!b_gblerrorFlag) printf("Camera initialized\n");
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
        //long * camData = new long[imageDim.size];
        StartAcquisition();

        ui_error = WaitForAcquisition();
        checkError(ui_error, "WaitForAcquisition");

        ui_error = AbortAcquisition();
        checkError(ui_error, "AbortAcquisition");

        ui_error = GetMostRecentImage(camData, imageDim.size);
        checkError(ui_error, "GetMostRecentImage");

        // Copy data array in image buffer for external use
        //std::copy(camData, camData + imageDim.size, imageBuffer);
    }
    else
    {
        //outString = "Cannot start camera acquisition...";
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
        //long * camData = new long[imageDim.size];
        StartAcquisition();

        ui_error = WaitForAcquisition();
        checkError(ui_error, "WaitForAcquisition");

        ui_error = AbortAcquisition();
        checkError(ui_error, "AbortAcquisition");

        ui_error = GetMostRecentImage(camData, imageDim.size);
        checkError(ui_error, "GetMostRecentImage");
    }

    else
    {
        printf("Cannot start camera acquisition...\n");
    }
}



/*
 * Acquire continuous full frames from the camera.
 * Differs from sub frame as shutter is set to auto.
 */
void acquireFullFrameLV(float expTime)
{
    //clearError();
    ui_error = SetShutter(1, 0, 27, 27);  // Set shutter to auto
    checkError(ui_error, "SetShutter");

    ui_error = SetExposureTime(expTime);
    checkError(ui_error, "SetExposureTime");

    //setFrameSizeLV(int hstart, int hend, int vstart, int vend, int hbin, int vbin)
    setFrameSizeLV(1, 512, 1, 512, 1, 1);  // Set to full camera readout

    // Start camera acquisition thread
    if (!b_gblerrorFlag)
    {
        printf("Starting acquisition...\n");
        camThread = new CameraThread();
        camThread->startCameraThread(imageDim.size, camData);
    }
    else
    {
        printf("Cannot start camera acquisition...\n");
    }
    printf("Returning from wrapper\n");
}


/*
 * Acquire continuous sub frames from the camera.
 * Differs from full frame as shutter is set to always open.
 * Set frame size prior to calling.
 */
void acquireSubFrameLV(float expTime)
{
    //clearError();
    ui_error = SetShutter(1, 1, 27, 27);  // Set to always open
    checkError(ui_error, "SetShutter");

    ui_error = SetExposureTime(expTime);
    checkError(ui_error, "SetExposureTime");

    // Start camera acquisition thread
    if (!b_gblerrorFlag)
    {
        printf("Starting acquisition...\n");
        camThread = new CameraThread();
        camThread->startCameraThread(imageDim.size, camData);
    }
    else
    {
        printf("Cannot start camera acquisition...\n");
    }
}


void acquireClosedLoopLV(float expTime)
{
    ui_error = SetShutter(1, 1, 27, 27);  // Set to always open
    checkError(ui_error, "SetShutter");

    ui_error = SetExposureTime(expTime);
    checkError(ui_error, "SetExposureTime");

    if (!b_gblerrorFlag)
    {
        // Create socket client and connect
        SocketClient sClient;
        sClient.Connect();

        printf("Starting acquisition...\n");
        b_gblstopFlag = false;
        StartAcquisition();

        /*
         * Loop over WaitForAcquisition
         * Sequence aborted by setting stop flag
         */
        controlVals.even = FALSE;
        int counter = 0;
        //while (!b_gblstopFlag)
        while (counter < 1000)
        {
            ui_error = WaitForAcquisition();
            checkError(ui_error, "WaitForAcquisition");

            ui_error = GetMostRecentImage(camData, imageDim.size);
            checkError(ui_error, "GetMostRecentImage");

            // Copy data array in image buffer for external use
            //std::copy(camData, camData + imageDim.size, imageBuffer);

            // Centroid image and send out values
            //centroid(imageBuffer);

            // For testing
            if (controlVals.even)
            {
                sClient.sendData(5.0, 5.0);
                controlVals.even = FALSE;
            }
            else {
                sClient.sendData(0.0, 0.0);
                controlVals.even = TRUE;
            }
            counter += 1;

            // For used
            //sClient.sendData(controlVals.x, controlVals.y);
        }

        // Abort Andor acquisition and check error
        ui_error = AbortAcquisition();
        checkError(ui_error, "AbortAcquisition");

        // Close socket client
        sClient.sendData(0.0, 0.0);
        sClient.Close();
        printf("Closing connection\n");

        //if(camData) delete[] camData;
    }

    else printf("Cannot start camera acquisition...\n");

}


/*
 * Copy latest camera data into array provided from labview
 */
void getCameraDataLV(long *dataOut)
{
    std::copy(camData, camData + (long) imageDim.size, dataOut);
}


/*
 * Set glogal flag to abort acquisition
 */
void abortAcquisitionLV()
{
    if (camThread->isRunning())
    {
        camThread->abortCameraThread();
    }

    b_gblstopFlag = true;
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
    imageDim.size = imageDim.hdim * imageDim.vdim;

    ui_error = SetImage(imageDim.hbin, imageDim.vbin, imageDim.hstart, imageDim.hend, imageDim.vstart, imageDim.vend);
    checkError(ui_error, "SetImage");
}


/*
 * Shutdown camera
 */
void shutdownCameraLV()
{
    ui_error = ShutDown();
    checkError(ui_error, "ShutDown");

    // Free data array memory
    if(camData) delete[] camData;
    //if(imageBuffer) delete[] imageBuffer;
}


/*
 * Program camera settings
 */
void setupCamera()
{
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
 */
void checkFrameSize()
{
    /*
     * TODO: check frame dimensions and set to valid values if invalid
     * TODO: notify user of invalid values
     */
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


/*
 * Find the x, y centroid from the provided image buffer
 * and set the appropriate x, y values in the controlVal struct.
 */
void centroid(long *imageBuffer)
{
    float sum = 0;
    float dummy_x = 0;
    float dummy_y = 0;
    float sum_x = 0;
    float sum_y = 0;
    for (int i = 0; i < imageDim.hdim; i++)
    {
        dummy_x = 0;
        dummy_y = 0;
        for (int j = 0; j < imageDim.vdim; j++)
        {
            sum += imageBuffer[i + j];
            dummy_x += imageBuffer[i + j * imageDim.hdim];
            dummy_y += imageBuffer[i * imageDim.vdim + j];
        }
        sum_x += (i + 1) * dummy_x;
        sum_y += (i + 1) * dummy_y;
    }
    controlVals.x = sum_x / sum;
    controlVals.y = sum_y / sum;
}


