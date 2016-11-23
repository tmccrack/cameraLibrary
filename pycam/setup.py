try:
    from setuptools import setup
    from setuptools import Extension
except  ImportError:
    from distutils.core import setup
    from distutils.extension import Extension
    
from Cython.Build import cythonize

ext = [
       Extension(name = "pycamera",
                 sources = ["pycamera.pyx",
                  "./../src/camera.cpp", 
                  "./../src/camerathread.cpp"],
                 include_dirs = ["C:/Qt/5.7/msvc2013/include", "./../src", "./../include"],
                 library_dirs = ["C:/Qt/5.7/msvc2013/lib", "./../lib"], 
                 libraries = ["Qt5Core", "atmcd32m"],
                 language = "c++")
       ]

setup(
      name = "Andor camera wrapper",
      ext_modules = cythonize(ext)
)