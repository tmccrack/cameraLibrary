# distutils: language = c++
# file: pycamera.pyx

#
# Wrapper class for camera
# Declare functionality 
#
cimport pycamera
cimport numpy as np
import numpy as np

cdef class PyCamera:
	cdef pycamera.Camera pycam
	cdef pycamera.ImageDimension s_imageDim
	cdef pycamera.ExposureProperites s_expProp
	cdef string name
	cdef bint real_cam
	cdef long *phandle
	cdef long buffer[262144]


	def __cinit__(self, name, real_cam):
		self.pycam = pycamera.Camera()
		self.name = name
		self.real_cam = real_cam
		self._Initialize()

	def _Initialize(self):
		self.pycam.initializeCamera(self.name, self.real_cam)
		self.s_imageDim = self.GetImageDimension()

	def Start(self):
		self.pycam.startCamera()
		return self.pycam.isCameraRunning()

	def Stop(self):
		self.pycam.stopCamera()
		return self.pycam.isCameraRunning()

	def Shutdown(self):
		self.pycam.shutdownCamera()

	def Running(self):
		return self.pycam.isCameraRunning()

	def Data(self, np.ndarray d_buff):
		self.pycam.getCameraData(self.buffer)
		for i in range(0, d_buff.shape[0]):
			d_buff[i] = self.buffer[i]

	def GetImageDimension(self):
		self.s_imageDim = self.pycam.getImageDims()
		return (self.s_imageDim.h_start,
			self.s_imageDim.h_dim,
			self.s_imageDim.v_start,
			self.s_imageDim.v_dim)

	def SetImageDimension(self, int x_s, int y_s, int x_size, int y_size):
		# Set array size
		# x_start, y_start, x dimension, y dimension
		self.s_imageDim.h_start = x_s
		self.s_imageDim.v_start = y_s
		self.s_imageDim.h_end = x_s + x_size - 1
		self.s_imageDim.v_end = y_s + y_size - 1
		self.pycam.setImageDims(self.s_imageDim)

	def GetExposureSettings(self):
		self.s_expProp = self.pycam.GetExposureSettings()
		return 

	def SetExposureSettings(self, float exp_time, int em_gain):
		self.s_expProp.exp_time = exp_time
		self.s_expProp.em_gain = em_gain
		self.pycam.SetExposureSettings(self.s_expProp)

	def Handle(self):
		self.pycam.getHandle(self.phandle)
		print "Got handle"
		return self.phandle[0]
