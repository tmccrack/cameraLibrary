# distutils: language = c++
# file: pycamera.pyx

#
# Wrapper class for camera
# Declare functionality 
#
cimport pycamera
cimport numpy as np
import numpy as np

cdef np.uint16_t buffer[262144]
cdef class PyCamera:
    cdef pycamera.Camera pycam
    cdef string name
    cdef bint real_cam
    cdef np.uint16_t buffer[262144]
    cdef float updates[2]

    # Declarations for used by camera library
    cdef pycamera.ImageDimension s_imageDim
    cdef pycamera.ExposureProperties s_expProp
    cdef pycamera.ReadProperties s_readProp
    cdef pycamera.TimingProperties s_timingProp
    cdef pycamera.ShutterProperties s_shutterProp
    cdef pycamera.TemperatureProperties s_tempProp
    cdef pycamera.Gain s_gainx
    cdef pycamera.Gain s_gainy

    def __cinit__(self, name = "", real_cam = False, temp=-65):
        self.pycam = pycamera.Camera()
        self.name = name.encode('utf-8')
        self.real_cam = <bint> real_cam
        self.pycam.initializeCamera(self.name, self.real_cam, temp)
        self.buffer = np.ascontiguousarray(np.empty(shape=262144, dtype=np.uint16))

    def __dealloc__(self):
        self.stop()
        self.shutdown()
        self.updates
        self.buffer
        self.real_cam
        self.name
        self.s_imageDim
        self.s_expProp
        self.s_readProp
        self.s_timingProp
        self.s_shutterProp
        self.s_tempProp
        self.s_gainx
        self.s_gainy
        self.pycam

    #
    # General camera methods
    def start(self, loopCond, filename = ""):
        print(filename)
        self.pycam.startCamera(<int>loopCond, <string>filename.encode('utf-8'))
        return self.running()

    def stop(self):
        # Stop camera acquisition and delete data array
        self.pycam.stopCamera()
        return self.running()

    def shutdown(self):
        self.pycam.shutdownCamera()

    def running(self):
        return self.pycam.isCameraRunning()

    def data(self):
        self.pycam.getCameraData(self.buffer)
        return np.asarray(self.buffer, dtype=np.uint16)

    def servoData(self):
        self.pycam.getServoData(self.updates)
        return np.asarray(self.updates, dtype=np.float)

    #
    # Exposure meter methods
    def arm(self, arm_state):
        if (arm_state):
            self.s_expProp.ext_trig = True
            self.pycam.setExposureParams(self.s_expProp)
        else:
            # Software trigger
            self.s_expProp.ext_trig = False
            self.pycam.setExposureParams(self.s_expProp)
        return self.getExposureProp()

    def startExpMeter(self, f):
        self.pycam.startCamera(<pycamera.cb_cam_func> PyCamera.expmfunction, <void*>f)
        return self.running()

    def singleShot(self, f):
        self.pycam.singleShot(<pycamera.cb_cam_func> PyCamera.expmfunction, <void*> f)

    @staticmethod
    cdef void expmfunction(np.uint16_t *buff, int buff_len, void *f) with gil:
        for i in range(0,buff_len):
            buffer[i] = buff[i]
        (<object>f)(<object>buffer, <object>buff_len)

    #
    # Image dimension getter/setter
    def getImageDimension(self):
        self.s_imageDim = self.pycam.getImageDims()
        return self.s_imageDim

    def setImageDimension(self, imageDim):
        # Sets image dimensions as perscribed, program camera, return actual dimensions
        self.s_imageDim.h_start = imageDim['h_start']
        self.s_imageDim.v_start = imageDim['v_start']
        self.s_imageDim.h_end = imageDim['h_end']
        self.s_imageDim.v_end = imageDim['v_end']
        self.s_imageDim.h_bin = imageDim['h_bin']
        self.s_imageDim.v_bin = imageDim['v_bin']
        self.pycam.setImageDims(self.s_imageDim)
        return self.getImageDimension()

    #
    # Exposure properites getter/setter
    def getExposureProp(self):
        self.s_expProp = self.pycam.getExposureParams()
        return self.s_expProp

    def setExposureProp(self, expProp):
        self.s_expProp.exp_time = expProp['exp_time']
        self.s_expProp.em_gain = expProp['em_gain']
        self.s_expProp.ext_trig = expProp['ext_trig']
        self.pycam.setExposureParams(self.s_expProp)
        return self.getExposureProp()

    #
    # Read properties getter/setter
    def getReadProp(self):
        self.s_readProp = self.pycam.getReadParams()
        return self.s_readProp

    def setReadProp(self, readProp):
        self.s_readProp.read_mode = readProp['read_mode']
        self.s_readProp.acq_mode = readProp['acq_mode']
        self.s_readProp.frame_transfer = readProp['frame_transfer']
        self.s_readProp.output_amp = readProp['output_amp']
        self.s_readProp.track_cent = readProp['track_cent']
        self.s_readProp.track_height = readProp['track_height']
        self.pycam.setReadParams(self.s_readProp)
        return self.getReadProp()

    #
    # Timing properties getter/setter
    def getTimingProp(self):
        self.s_timingProp = self.pycam.getTimingParams()
        return self.s_timingProp

    def setTimingProp(self, timingProp):
        self.s_timingProp.h_shift = timingProp['h_shift']
        self.s_timingProp.v_shift = timingProp['v_shift']
        self.s_timingProp.v_amp = timingProp['v_amp']
        self.s_timingProp.dma_images = timingProp['dma_images']
        self.s_timingProp.dma_accum_time = timingProp['dma_accum_time']
        self.pycam.setTimingParams(self.s_timingProp)
        return self.getTimingProp()

    #
    # Shutter properties getter/setter
    def getShutterProp(self):
        self.s_shutterProp = self.pycam.getShutterParams()
        return self.s_shutterProp

    def setShutterProp(self, shutterProp):
        self.s_shutterProp.type = shutterProp['type']
        self.s_shutterProp.mode = shutterProp['mode']
        self.s_shutterProp.open_time = shutterProp['open_time']
        self.s_shutterProp.close_time = shutterProp['close_time']
        return self.getShutterProp()

    #
    # Temperature properties getter/setter
    def getTempProp(self):
        self.s_tempProp = self.pycam.getTempParams()
        return self.s_tempProp

    def getTempArray(self):
        self.s_tempProp = self.pycam.getTempArray()
        return self.s_tempProp

    def setTempProp(self, tempProp):
        self.s_tempProp.set_point = tempProp['set_point']
        self.s_tempProp.power_state = tempProp['power_state']
        self.pycam.setTempParams(self.s_tempProp)
        return self.getTempProp()

 
    #
    # Servo properties getter/setter
    def getGainX(self):
        self.s_gainx = self.pycam.getGainX()
        return self.s_gainx

    def getGainY(self):
        self.s_gainy = self.pycam.getGainY()
        return self._s_gainy

    def setGain(self, gain, rotation):
        self.s_gainx.kp = gain['kp']
        self.s_gainx.ki = gain['ki']
        self.s_gainx.kd = gain['kd']
        self.s_gainx.dt = gain['dt']
        self.s_gainy = self.s_gainx
        self.pycam.setGain(self.s_gainx, self.s_gainy)
        self.pycam.setRotation(rotation)
        return self.getGainX()

    def getRotation(self):
        return self.pycam.getRotation()

    def getTargetCoords(self):
        cdef float x[1]
        cdef float y[1]
        self.pycam.getTargetCoords(x, y)
        return (x[0], y[0])

    def setTargetCoords(self, x, y):
        self.pycam.setTargetCoords(x, y)
        return self.getTargetCoords()

    def setServoState(self, state):
        return self.pycam.setServoState(state)

    def getBackground(self):
        return self.pycam.getBackground()

    def setBackground(self, background):
        self.pycam.setBackground(background)
        return self.getBackground()

 
    #
    # Logging
    def getLogInterval(self):
        return self.pycam.getLogInterval()
        
    def setLogInterval(self, frames):
        return self.pycam.setLogInterval(<np.npy_uint32>frames)

    def setLogState(self, state):
        return self.pycam.setLogState(<bool> state)

