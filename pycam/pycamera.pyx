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
		self._setImageDimension()

	cpdef _setImageDimension(self):
		self.pycam.setImageDims(self.s_imageDim)
		self.getImageDimension()

	cpdef _calcDimension(self):
		self.x_dim = self.s_imageDim.h_end - self.s_imageDim.h_start + 1
		self.y_dim = self.s_imageDim.v_end - self.s_imageDim.v_start + 1

	def getExposureSettings(self):
		self.s_expProp = self.pycam.getExposureParams()
		return self.s_expProp

	def setExposureSettings(self):
		self.pycam.setExposureParams(self.s_expProp)
		self.getExposureSettings()

	# def Handle(self):
	# 	self.pycam.getHandle(self.phandle)
	# 	print("Got handle")
	# 	return self.phandle[0]
