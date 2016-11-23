# The below does not work, still requires Qt5Core in the directory
QT_PATH = "C:\\Qt\\5.7\\msvc2013\\lib"
import sys
sys.path.append(QT_PATH)

import pycamera

ftt = pycamera.PyCamera("Ftt", True)
a = ftt.getHandle()
print a