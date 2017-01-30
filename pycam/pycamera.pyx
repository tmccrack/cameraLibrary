# distutils: language = c++
# file: pycamera.pyx

#
# Wrapper class for camera
# Declare functionality 
#
cimport pycamera
cimport numpy as np
import numpy as np
import sys

cdef class PyCamera:
	cdef pycamera.Camera pycam
	cdef string name
	cdef bint real_cam
	cdef int buffer[262144]

	# These are 'public' attributes
	cdef pycamera.ImageDimension s_imageDim
	cdef pycamera.ExposureProperties s_expProp
	cpdef public int x_dim, y_dim


	def __cinit__(self, name = "", real_cam = False):
		self.pycam = pycamera.Camera()
		self.name = name.encode('utf-8')
		self.real_cam = real_cam
		self._Initialize()

	cpdef _Initialize(self):
		self.pycam.initializeCamera(self.name, self.real_cam)
		self.stop()
		self.getImageDimension()

	def start(self):
		self.pycam.startCamera()
		return self.pycam.isCameraRunning()

	def stop(self):
		self.pycam.stopCamera()
		return self.pycam.isCameraRunning()

	def shutdown(self):
		self.pycam.shutdownCamera()

	def running(self):
		return self.pycam.isCameraRunning()

	def data(self, buffer):
		self.pycam.getCameraData(self.buffer)
		for i in range(len(buffer)-1):
			buffer[i] = self.buffer[i]

	def getImageDimension(self):
		self.s_imageDim = self.pycam.getImageDims()
		self._calcDimension()
		return self.s_imageDim

	def setImageDimension(self, imageDim):
		self.s_imageDim.h_start = imageDim['h_start']
		self.s_imageDim.v_start = imageDim['v_start']
		self.s_imageDim.h_end = imageDim['h_end']
		self.s_imageDim.v_end = imageDim['v_end']
		self.s_imageDim.h_bin = imageDim['h_bin']
		self.s_imageDim.v_bin = imageDim['v_bin']
		self.pycam.setImageDims(self.s_imageDim)
		return self.getImageDimension()

	cpdef _calcDimension(self):
		self.x_dim = self.s_imageDim.h_end - self.s_imageDim.h_start + 1
		self.y_dim = self.s_imageDim.v_end - self.s_imageDim.v_start + 1

	def getExposureProp(self):
		self.s_expProp = self.pycam.getExposureParams()
		return self.s_expProp

	def setExposureProp(self, expProp):
		self.s_expProp.exp_time = expProp['exp_time']
		self.s_expProp.em_gain = expProp['em_gain']
		self.pycam.setExposureParams(self.s_expProp)
		return self.getExposureProp()

	# def Handle(self):
	# 	self.pycam.getHandle(self.phandle)
	# 	print("Got handle")
	# 	return self.phandle[0]

# class Temperature(object):
# 	"""Manages the camera cooler. 
  
#  	Default temperature setpoint is -65C.
#  	"""
#  	def __init__(self, cam):
# 		self._cam = cam
# 		self._setpoint = -65

# 	@property
# 	def range(self):
# 	"""Return the valid range of temperatures in centigrade to which the detector can be cooled."""
# 		cdef int tmin, tmax
# 		andorError(sdk.GetTemperatureRange(&tmin, &tmax))
# 		return (tmin, tmax)
  
#   @property
#   def precision(self):
#     """Return the number of decimal places to which the sensor temperature can be returned.""" 
#     cdef int precision
#     andorError(sdk.GetTemperaturePrecision(&precision))#, ignore = (sdk.DRV_NOT_SUPPORTED,))
#     return precision
    
#   @property
#   def setpoint(self):
#     """Return the current setpoint."""
#     return self._setpoint
  
#   @setpoint.setter
#   def setpoint(self, value):
#     """Change the setpoint."""
#     andorError(sdk.SetTemperature(value))
#     self._setpoint = value
    
#   @property
#   def read(self):
#     """Returns the temperature of the detector to the nearest degree, and the status of cooling process."""
#     cdef int value
#     error_code = sdk.GetTemperature(&value)
#     andorError(error_code, ignore={ERROR_CODE[k] for k in TEMPERATURE_MESSAGES})
#     return {"temperature": value, "status": ERROR_CODE[error_code]}

#   @property
#   def cooler(self):
#     """Query or set the state of the TEC cooler (True: ON, False: OFF)."""
#     cdef bint state
#     andorError(sdk.IsCoolerOn(&state)) # returns 1 if on, 0 if off
#     return state
#   @cooler.setter  
#   def cooler(self, state):
#     if state:
#       andorError(sdk.CoolerON())
#     else:
#       andorError(sdk.CoolerOFF())
      
#   def __repr__(self):
#     return "Current temperature: " + str(self.read) + ", cooler: "+ ("ON" if str(self.cooler) else "OFF") + ", setpoint: " + str(self.setpoint)+"." 
