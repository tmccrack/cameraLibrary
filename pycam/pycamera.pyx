# distutils: language = c++
# file: pycamera.pyx

#
# Wrapper class for camera
# Declare functionality 
#
cimport pycamera

cdef class PyCamera:
	cdef pycamera.Camera pycam
	cdef long *phandle
	cdef string name
	cdef bint real_cam

	def __cinit__(self, name, real_cam):
		self.name = name
		self.real_cam = real_cam
		self.pycam = pycamera.Camera(self.name, self.real_cam)

	def Start(self):
		self.pycam.startCamera()
		return self.pycam.isCameraRunning()

	def Stop(self):
		self.pycam.stopCamera()
		return self.pycam.isCameraRunning()

	def Running(self):
		return self.pycam.isCameraRunning()

	def getHandle(self ):
		return self.pycam.getHandle()