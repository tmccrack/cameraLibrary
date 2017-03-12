import pycamera
import numpy as np
import time

def report_data(buffer, buff_len):
    print("Buff: {}".format(buffer[0:10]))


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
        expm.startExpMeter(report_data)
        return True

    elif val == '2': 
        print("Stopping camera")
        expm.stop()
        return True

    elif val == '3': 
        a = input("Enter read mode:\n  0 for FVB\n  3 for single track\n  4 for image\n  ")
        temp = expm.getReadProp()

        if a == '3':
            temp['track_height'] = int(input("Track height: "))
            temp['track_cent'] = int(input("Track center: "))

        temp['read_mode'] = int(a)

        print(expm.setReadProp(temp))
        print(expm.getImageDimension())
        return True

    elif val == '4':
        expm.singleShot(report_data)
        return True

    elif val == '5':
        exttrig = input("External trigger: 0 - false\t 1 - true\n")
        temp = expm.getExposureProp()
        if exttrig == '0':
            temp['ext_trig'] = False
        else: temp['ext_trig'] = True
        expm.setExposureProp(temp)
        print(temp)
        return True

    elif val == '6':
        expm.getImageDimension()
        return True

    else:
        return False    

expm = pycamera.PyCamera("ExpM", False, 0) 

while (GetInput()):
    print("What do you want to do?\n")

print("Shutting down")
expm.shutdown()

print("Done")