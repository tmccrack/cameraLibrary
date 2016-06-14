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


/*
 * Comment out when running labview import wizard
 * Wizard cannot manage cimg but doesn't need to know about it
 */
//#include "CImg.h"
//using namespace cimg_library;


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
 * Struct for control loop values
 */
struct ControlValues{
    float x;
    float y;
    float kp;
    float ki;
    float kd;
    float gain;
    bool even;
} controlVals;

/*
 * Functions for labview interface
 */
extern "C" {
void CAMERALIBRARYSHARED_EXPORT initializeCameraLV();
void CAMERALIBRARYSHARED_EXPORT acquireSingleFullFrameLV(float expTime);
void CAMERALIBRARYSHARED_EXPORT acquireSingleSubFrameLV(float expTime, char *outString);
void CAMERALIBRARYSHARED_EXPORT acquireFullFrameLV(float expTime);
void CAMERALIBRARYSHARED_EXPORT acquireSubFrameLV(float expTime);
void CAMERALIBRARYSHARED_EXPORT acquireClosedLoopLV(float expTime);
void CAMERALIBRARYSHARED_EXPORT getCameraDataLV(long *dataOut);
void CAMERALIBRARYSHARED_EXPORT setFrameSizeLV(int hstart, int hend, int vstart, int vend, int hbin, int vbin);
void CAMERALIBRARYSHARED_EXPORT abortAcquisitionLV();
void CAMERALIBRARYSHARED_EXPORT shutdownCameraLV();
}


/*
 * Internal library functions
 */
void setupCamera();
void checkFrameSize();
void coolerPower();
void centroid(long *imageBuffer);
bool checkError(unsigned int _ui_error, const char* _cp_func);


/*
 * Internal library variables
 */
unsigned int ui_error;
bool b_gblerrorFlag = false;
bool b_gblstopFlag = false;
static long *camData;
static long *imageBuffer;

#endif // CAMERALIBRARY_H
