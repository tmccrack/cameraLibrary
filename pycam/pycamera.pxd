# distutils: language = c++
# distutils: sources = ./../src/camera.cpp
# file: pycamera.pxd

#
# Wrapper class for camera
# Declare functions to be wrapped
#

from libcpp cimport bool
from libcpp cimport long
from libcpp.string cimport string


cdef extern from "./../src/camera.h":
	cdef cppclass Camera:
		Camera() except +
		Camera(string cam_name, bool real_cam) except +
		void startCamera()
		void stopCamera()
		bool isCameraRunning()
		long getHandle()

