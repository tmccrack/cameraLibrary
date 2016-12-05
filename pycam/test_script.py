# # The below does not work, still requires Qt5Core in the directory
# QT_PATH = "C:\\Qt\\5.7\\msvc2013\\lib"
# import sys
# sys.path.append(QT_PATH)

import pycamera
import numpy as np

def GetInput():
	print "1 - Start camera"
	print "2 - Stop camera"
	print "3 - Get handle"
	print "4 - Get data"
	print "5 - Check if running"
	print "6 - Get image dimension"
	print "Any other key to shutdown"
	val = int(raw_input("What do you want to do?"))
	if val == 1: 
		print "Starting camera"
		ftt.Start()
		return True

	elif val == 2: 
		"Stopping camera"
		ftt.Stop()
		return True

	elif val == 3: 
		"Getting handle"
		a = ftt.Handle()
		print "Handle: " + str(a)
		return True

	elif val == 4:
		"Retrieving data"
		ftt.Data(buffer)
		print buffer[0:200]
		return True

	elif val == 5:
		print "Running? " + str(ftt.Running())
		return True

	elif val == 6:
		ftt.GetImageDimension()
		return True

	else:
		return False	

ftt = pycamera.PyCamera("FTT", True)	
print ftt
buffer = np.ascontiguousarray(np.empty([262144, ], dtype=long))
print "What do you want to do?"
while (GetInput()):
	print "What do you want to do?\n"

print "Shutting down"
ftt.Shutdown()

print "Done"