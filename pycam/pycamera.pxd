# distutils: language = c++
# distutils: sources = ./../src/camera.cpp
# file: pycamera.pxd

#
# Wrapper class for camera
# Declare functions to be wrapped
#

from libcpp cimport bool
from libcpp cimport int
from libcpp.string cimport string


cdef extern from "./../src/camera.h":
	cdef cppclass Camera:
		Camera() except +
		void initializeCamera(string cam_name, bool real_cam)
		void startCamera()
		bool isCameraRunning()
		void stopCamera()
		void shutdownCamera()
		void getCameraData(int *buffer)

		ImageDimension getImageDims()
		void setImageDims(ImageDimension imageDim)

		ExposureProperties getExposureParams()
		void setExposureParams(ExposureProperties expProps)

	cdef struct ImageDimension:
		int h_start
		int h_end
		int h_bin
		int v_start
		int v_end
		int v_bin
		int h_dim
		int v_dim
		unsigned long size

	cdef struct ExposureProperties:
		float exp_time
		int em_gain

	# cdef struct ShutterProperties:
	# 	int type
	# 	int mode
	# 	int open_time
	# 	int close_time

	# cdef struct ReadProperties:
	# 	int read_mode
	# 	int acq_mode
	# 	int frame_transfer
	# 	int output_amp

	# cdef struct TimingProperties
	# 	int h_shift
	# 	int v_shift
	# 	int dma_images
	# 	float dma_accum_time