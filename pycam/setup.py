# try:
#     from setuptools import setup
#     from setuptools import Extension
# except  ImportError:
#     from distutils.core import setup
#     from distutils.extension import Extension

from distutils.core import setup
from distutils.extension import Extension
    
from Cython.Build import cythonize
import numpy

ext = [
       Extension(name = "pycamera",
                 sources = ["pycamera.pyx",
                  "./../src/camera.cpp",
                  "./../src/camerathread.cpp",
                  "./../src/socketclient.cpp",
                  "./../src/servo.cpp",
                  "./../src/imageservo.cpp",
                  "./../src/datalogger.cpp"],
                 include_dirs = ["C:/Qt/5.7/msvc2015/include", 
                                "C:/Qt/5.7/msvc2015/include/CCfits", 
                                "C:/Qt/5.7/msvc2015/include/cfitsio", 
                                "./../src", "./../include", 
                                numpy.get_include()
                              ],
                 library_dirs = ["C:/Users/TMM/Anaconda3/Library/lib", "./../lib"],
                 libraries = ["Qt5Core", "Qt5Network", "atmcd32m", "CCfits", "cfitsio"],
                 language = "c++")
       ]

setup(
      name = "Andor camera wrapper",
      include_dirs = [numpy.get_include()],
      ext_modules = cythonize(ext),
)
