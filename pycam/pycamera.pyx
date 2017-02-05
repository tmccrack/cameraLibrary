# distutils: language = c++
# file: pycamera.pyx

#
# Wrapper class for camera
# Declare functionality 
#
cimport pycamera
cimport numpy as np
import numpy as np

cdef class SClient:
	cdef pycamera.SocketClient client
	cdef string conn_type

	def __cinit__(self, conn_type="Single"):
		self.client = pycamera.SocketClient()
		self.conn_type = conn_type.encode('utf-8')

	def sendData(self, x, y):
		if self.conn_type == 'Single':
			if (self.client.openConnection(self.conn_type)):
				self.client.sendData(x, y)
				self.client.closeConnection()
				return True
			else:
				return False
		else:
			print("Connection type not supported")
			return False

	cdef _getData(self):
		cdef float *x
		cdef float *y
		self.client.openConnection("Values".encode('utf-8'))
		if self.client.isConnected():
			self.client.getData(x, y)
			self.client.closeConnection()
			print("X: {}\tY: {}".format(x[0], y[0]))
		else:
			pass
		
		return np.asarray([x[0], y[0]], dtype=np.float32)

	def getData(self):
		arr = self._getData()
		return arr


cdef class PyCamera:
	cdef pycamera.Camera pycam
	cdef string name
	cdef bint real_cam
	cdef np.uint16_t buffer[262144]

	# These are 'public' attributes
	cdef pycamera.ImageDimension s_imageDim
	cdef pycamera.ExposureProperties s_expProp
	cdef pycamera.TemperatureProperties s_tempProp

	def __cinit__(self, name = "", real_cam = False, temp=-65):
		self.pycam = pycamera.Camera()
		self.name = name.encode('utf-8')
		self.real_cam = real_cam
		self.pycam.initializeCamera(self.name, self.real_cam, temp)
		self.buffer = np.ascontiguousarray(np.empty(shape=262144, dtype=np.uint16))

	def __dealloc__(self):
		self.stop()
		self.shutdown()
		self.buffer

	def start(self):
		self.pycam.startCamera()
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

	def getExposureProp(self):
		self.s_expProp = self.pycam.getExposureParams()
		return self.s_expProp

	def setExposureProp(self, expProp):
		self.s_expProp.exp_time = expProp['exp_time']
		self.s_expProp.em_gain = expProp['em_gain']
		self.pycam.setExposureParams(self.s_expProp)
		return self.getExposureProp()

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

	# def Handle(self):
	# 	self.pycam.getHandle(self.phandle)
	# 	print("Got handle")
	# 	return self.phandle[0]

 

