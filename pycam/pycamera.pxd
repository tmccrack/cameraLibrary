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
		void initializeCamera(string cam_name, bool real_cam)
		void startCamera()
		void stopCamera()
		void shutdownCamera()
		void getCameraData(long *buffer)
		void getHandle(long *cam_handle)
		ImageDimension getImageDims()
		void setImageDims(ImageDimension imageDim)
		
		bool isCameraRunning()

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

# 	cdef struct: ExposureProperties


# truct TimingProperties{
#     int h_shift;
#     int v_shift;
#     int dma_images;
#     float dma_accum_time;
# };

# struct ReadProperties{
#     int read_mode;
#     int acq_mode;
#     int frame_transfer;
#     int output_amp;
# };

# struct ImageDimension{
#     int h_start;
#     int h_end;
#     int h_bin;
#     int v_start;
#     int v_end;
#     int v_bin;
#     int h_dim;
#     int v_dim;
#     unsigned long size;
# };

# struct ExposureProperties{
#     float exp_time;
#     int em_gain;
#     int *em_gain_high;
#     int *em_gain_low;
# };

# struct ShutterProperties{
#     int type;
#     int mode;
#     int open_time;
#     int close_time;
# };