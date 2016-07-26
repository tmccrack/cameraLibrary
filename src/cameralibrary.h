#ifndef CAMERALIBRARY_H
#define CAMERALIBRARY_H

#include <QtCore/qglobal.h>

#if defined(CAMERALIBRARY_LIBRARY)
//#  define CAMERALIBRARYSHARED_EXPORT Q_DECL_EXPORT
#define CAMERALIBRARYSHARED_EXPORT __declspec(dllexport)
#else
//#  define CAMERALIBRARYSHARED_EXPORT Q_DECL_IMPORT
#  define CAMERALIBRARYSHARED_EXPORT __declspec(dllimport)
#endif

#include <iostream>
#include "ATMCD32D.h"
#include "camerathread.h"



/*
 * Struct for carrying around image dimensions
 */
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


/*
 * Functions for labview interface
 */
extern "C" {
void CAMERALIBRARYSHARED_EXPORT initializeCameraLV();
void CAMERALIBRARYSHARED_EXPORT acquireSingleFullFrameLV(float expTime);
void CAMERALIBRARYSHARED_EXPORT acquireSingleSubFrameLV(float expTime);
void CAMERALIBRARYSHARED_EXPORT acquireFullFrameLV(float expTime);
void CAMERALIBRARYSHARED_EXPORT acquireSubFrameLV(float expTime);
void CAMERALIBRARYSHARED_EXPORT acquireClosedLoopLV(float expTime);
void CAMERALIBRARYSHARED_EXPORT getCameraDataLV(long *dataOut);
void CAMERALIBRARYSHARED_EXPORT setFrameSizeLV(int hstart, int hend, int vstart, int vend, int hbin, int vbin);
void CAMERALIBRARYSHARED_EXPORT abortAcquisitionLV();
void CAMERALIBRARYSHARED_EXPORT getTemperatureLV(int *temperature, unsigned int *status);
void CAMERALIBRARYSHARED_EXPORT setTemperatureLV(int setTemp);
void CAMERALIBRARYSHARED_EXPORT getControlsValueLV(double *controlValsOut);
void CAMERALIBRARYSHARED_EXPORT shutdownCameraLV();
}


/*
 * Internal library functions
 */
void setupCamera();
void checkFrameSize();
void centroid(long *imageBuffer);
bool checkError(unsigned int _ui_error, const char* _cp_func);


/*
 * Internal library variables
 */
unsigned int ui_error;
bool b_gblerrorFlag = false;
bool b_gblAcquireFlag = false;
bool b_gblCoolerPower = false;
static long *camData = new long[262144];
static double *controlData = new double[2];
static CameraThread *camThread;
static ClosedLoopCameraThread *closedThread;
QMutex mutex;

#endif // CAMERALIBRARY_H
