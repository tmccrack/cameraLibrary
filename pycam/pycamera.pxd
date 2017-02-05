# distutils: language = c++
# distutils: sources = ./../src/camera.cpp
# file: pycamera.pxd

#
# Wrapper class for camera
# Declare functions to be wrapped
#

from libcpp cimport bool
from libcpp cimport int
from libcpp cimport float
from libc.stdint cimport uint16_t
from libcpp.string cimport string

cdef extern from "./../src/socketclient.h":
	cdef cppclass SocketClient:
		SocketClient() except+ 
		bool openConnection(string conn_type)
		bool isConnected()
		void closeConnection()
		void sendData(float x, float y)
		void getData(float *x, float *y)


cdef extern from "./../src/camera.h":
	cdef cppclass Camera:
		Camera() except +
		void initializeCamera(string cam_name, bool real_cam, int temp)
		void startCamera()
		bool isCameraRunning()
		void stopCamera()
		void shutdownCamera()
		void getCameraData(uint16_t *buffer)

		ImageDimension getImageDims()
		void setImageDims(ImageDimension imageDim)

		ExposureProperties getExposureParams()
		void setExposureParams(ExposureProperties expProp)

		TemperatureProperties getTempParams()
		TemperatureProperties getTempArray()
		void setTempParams(TemperatureProperties tempProp)

		ShutterProperties getShutterParams()
		void setShutterParams(ShutterProperties shutProp)
		

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
		int em_gain_high
		int em_gain_low

	cdef struct TemperatureProperties:
		int set_point
		int array_temp
		int temp_high
		int temp_low
		bool power_state
		unsigned int cooler_state

	cdef struct ShutterProperties:
		int type
		int mode
		int open_time
		int close_time

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


