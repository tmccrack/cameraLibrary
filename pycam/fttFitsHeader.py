from astropy.io import fits
import numpy as np

"""
A hacked together class for FTT fits headers.
"""

class HeaderData:
    def __init__(self, filename, exposure, gain, background, leaky, intv, servo, targ_loc):
        hdu = fits.PrimaryHDU(np.uint16(0))
        hdu.header['EXPTIME'] = exposure['exp_time']
        hdu.header['KINTIME'] = exposure['kinetic_time']
        hdu.header['EMGAIN'] = exposure['em_gain']

        hdu.header['KP'] = gain['kp']
        hdu.header['KI'] = gain['ki']
        hdu.header['KD'] = gain['kd']

        hdu.header['BCKGRND'] = background
        hdu.header['LEAKFCTR'] = leaky

        hdu.header['INTRVL'] = intv
        hdu.header['SERVO'] = servo
        hdu.header['TARGX'] = targ_loc[0]
        hdu.header['TARGY'] = targ_loc[1]
        hdu.writeto(filename)

        # hdulist = fits.HDUList([hdu])
        # hdulist.writeto(filename)

if __name__ == "__main__":

    exp = {"exp_time": 1.0, "kinetic_time":1.1, "em_gain":200}
    gain = {"kp":0.001, "ki":0.001, "kd":0.001}
    HeaderData("test.fits", exp, gain, -1, 0.1, 100)

