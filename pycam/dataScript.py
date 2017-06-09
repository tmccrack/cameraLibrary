import sys
import re
import pathlib
import struct
import numpy as np
from os import listdir
from os import remove
from astropy.io import fits

data_dir = './../data/'
reduced_dir = './../reduce/'
files = listdir(data_dir)
log_sheet = './../log/example.log'

def fttWrite(fd, file):
    for line in fd:
        if re.search(file.split('-ftt')[0], line):
            if re.search('image dim ', line):
                im_size = line.split('image dim ')[1].split(',')
                im_size = [int(im_size[0]), int(im_size[1])]

                # read the data
                with open(data_dir + file, 'rb') as binary_file:
                    bdata = binary_file.read()
                # convert the data
                try:
                    data = np.zeros((1, int(len(bdata)/2)))
                    for i in range(0, int(len(bdata)/2)):
                        data[0,i] = int.from_bytes(bdata[i*2:(i+1)*2], byteorder='little')
                except MemoryError:
                    print("File {} is to big, still need to fix that".format(file))
                    return 1

                # reshape data and save as fits
                data = data.reshape( int(data.size/(im_size[0]*im_size[1])), im_size[0], im_size[1] )
                hdu = fits.PrimaryHDU(data)
                hdulist = fits.HDUList([hdu])
                filename = file.split('-ftt')[0] + '-ftt.fits'
                try:
                    hdulist.writeto(reduced_dir + filename)
                    print("{} written".format(filename))
                    remove(file)
                except OSError:
                    print("{} already exsists, deleting {}".format(filename, data_dir+file))
                    remove(data_dir + file)
                except MemoryError:
                    print("Cannot write such a big file: {} is {}x{} and {} deep".format(filename, im_size[0], im_size[1], data.shape[0]))

def writeFitsFile(data, filename):
    pass

def servoWrite(fd, file):
    try:
        # read the data
        with open(data_dir + file, 'rb') as binary_file:
            bdata = binary_file.read()
        # convert the data
        data = np.zeros((1, int(len(bdata)/4)))
        for i in range(0, data.size):
            # data[0,i] = float.from_bytes(bdata[i*4:(i+1)*4], byteorder='little')
            data[0,i] = struct.unpack('f', bdata[i*4:(i+1)*4])[0]

        data = data.reshape( 2, int(data.size/2), order='F')
        hdu = fits.PrimaryHDU(data)
        hdulist = fits.HDUList([hdu])
        filename = file.split('-servo')[0] + '-servo.fits'
        try:
            hdulist.writeto(reduced_dir + filename)
            print("{} written".format(filename))
        except OSError:
            print("{} already exsists, deleting {}".format(filename, data_dir+file))
            remove(data_dir + file)
    except OSError:
        print("Cannot open {}".format(file))


fd = open(log_sheet, 'r')
for file in files:
    # search log sheet for image size
    fd.seek(0)
    if (file.find('-ftt') is not -1):
        fttWrite(fd, file)
    elif (file.find('-servo') is not -1):
        servoWrite(fd, file)
    
fd.close()







