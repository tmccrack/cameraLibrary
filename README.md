# cameraLibrary
Windows based project for interfacing to an Andor iXon897 Ultra EMCCD through a pyqt gui and custom c++ library.
The library uses win32 events to handle the data coming from the camera in video mode. Data is read from the camera and copied to a buffer for user access.

To start the gui, ensure 32bit Python3 (tested with Anaconda3, 4.2), Qt5 (tested with 5.7, msvc2015) and the visual studio 2015 build tools are installed.
Download the repository.
Edit the repo/pycam/setup.py file:
  library_dirs should point to pyqt library path (Anaconda_install_dir/Library/lib)
  include_dirs should point to the Qt include path (Qt_install_dir/msvc2015/include)
  
Build the library and cython module:
$ python setup.py build_ext --inplace
  
Edit the repo/pycam/fttGui.py file:
  to test with a fake camera, change pycamera instantiation to 'False' flag
  eg line215: self.camera = pycamera.PyCamera(name, False, temp=17)
  
The cython camera interface (pycamera.pyx) uses dictionaries to translate between the python gui and the underlying c structures. It is easiest to initialize each dictionary after the camera is instantiated. The dictionaries lump together similar functions (array size, how the array is read, timing for the array, etc.) Dictionary keys are listed in the cython pxd file (pycamera.pxd). The key name should translate to the appropriate parameter in the Andor SDK. A not necessarily up-to-date list follows:

ImageDimension:
		int h_start,
		int h_end,
		int h_bin,
		int v_start,
		int v_end,
		int v_bin,
		int h_dim,
		int v_dim,
		unsigned long size
    
ExposureProperties:
		float exp_time,
		int em_gain,
		int em_gain_high,
		int em_gain_low,
    
ReadProperties:
		int read_mode,
		int acq_mode,
		int frame_transfer,
		int output_amp,
		int track_cent,
		int track_height,
    
TimingProperties:
		int h_shift,
		int v_shift,
		int dma_images,
		float dma_accum_time,
    
ShutterProperties:
		int type,
		int mode,
		int open_time,
		int close_time,
    
TemperatureProperties:
		int set_point,
		int array_temp,
		int temp_high,
		int temp_low,
		bool power_state,
		unsigned int cooler_state
