# # The below does not work, still requires Qt5Core in the directory
# QT_PATH = "C:\\Qt\\5.7\\msvc2013\\lib"
# import sys
# sys.path.append(QT_PATH)

import pycamera
import numpy as np
import time


#
# TODO: set external trigger mode access
# TODO: set shutter mode
#

def report_data(buffer, buff_len):
	print("Buff len: {}".format(buff_len))
	print("Buff: {}".format(buffer[0:10]))

def GetInput():
	print("1 - Start camera")
	print("2 - Stop camera")
	print("3 - Set read mode")
	print("4 - Single shot")
	print("5 - Set trigger")
	print("6 - Get image dimension")
	print("Any other key to shutdown")
	val = input("What do you want to do?\n")
	if val == '1': 
		print("Starting camera")
		ftt.startExpMeter(report_data)
		return True

	elif val == '2': 
		print("Stopping camera")
		ftt.stop()
		return True

	elif val == '3': 
		a = input("Enter read mode:\n  0 for FVB\n  3 for single track\n  4 for image\n  ")
		temp = ftt.getReadProp()

		if a == '3':
			temp['track_height'] = int(input("Track height: "))
			temp['track_cent'] = int(input("Track center: "))

		temp['read_mode'] = int(a)

		print(ftt.setReadProp(temp))
		print(ftt.getImageDimension())
		return True

	elif val == '4':
		ftt.singleShot(report_data)
		return True

	elif val == '5':
		exttrig = input("External trigger: 0 - false\t 1 - true\n")
		temp = ftt.getExposureProp()
		if exttrig == '0':
			temp['ext_trig'] = False
		else: temp['ext_trig'] = True
		ftt.setExposureProp(temp)
		print(temp)
		return True

	elif val == '6':
		ftt.getImageDimension()
		return True

	else:
		return False	

name = "FTT"
ftt = pycamera.PyCamera(name, False, 0)	
# buffer = np.empty([262144, 1], dtype=int)

while (GetInput()):
	print("What do you want to do?\n")

print("Shutting down")
ftt.shutdown()

print("Done")