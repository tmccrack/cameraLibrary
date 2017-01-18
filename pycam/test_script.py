# # The below does not work, still requires Qt5Core in the directory
# QT_PATH = "C:\\Qt\\5.7\\msvc2013\\lib"
# import sys
# sys.path.append(QT_PATH)

import pycamera
import numpy as np
import time

def GetInput():
	print("1 - Start camera")
	print("2 - Stop camera")
	print("3 - Get handle")
	print("4 - Get data")
	print("5 - Check if running")
	print("6 - Get image dimension")
	print("Any other key to shutdown")
	val = input("What do you want to do?\n")
	if val == '1': 
		print("Starting camera")
		ftt.Start()
		return True

	elif val == '2': 
		print("Stopping camera")
		ftt.Stop()
		return True

	# elif val == 3: 
	# 	print("Getting handle")
	# 	a = ftt.Handle()
	# 	print("Handle: {}".format(a))
	# 	return True

	elif val == '4':
		print("Retrieving data")
		ftt.Data(buffer)
		print("{}".format(buffer[0:200]))
		return True

	elif val == '5':
		print("Running? {}".format(ftt.Running()))
		return True

	elif val == '6':
		ftt.GetImageDimension()
		return True

	else:
		return False	

name = "FTT"
ftt = pycamera.PyCamera(name.encode('utf-8'), False)	
buffer = np.empty([262144, 1], dtype=int)

while (GetInput()):
	print("What do you want to do?\n")

print("Shutting down")
ftt.Shutdown()

print("Done")