#include "datalogger.h"

using namespace std;
using namespace CCfits;

DataLogger::DataLogger(string fname, int idim[])
{
    // Data logger constructor, setup image dimensions
    nelements = float(idim[0] *idim[1]);  // elements per image
    naxes[0] = long(idim[0]);
    naxes[1] = long(idim[1]);
    naxes[2] = 0;
    image_ind = 0;  // Image index
    filename = fname;
//    pFits.reset(0);

    try {
//        pFits.reset(new FITS(filename, USHORT_IMG, naxis, naxes));
        pFits = new FITS(filename, USHORT_IMG, naxis, naxes);
    }

    catch (FITS::CantCreate) {
        return;
    }
}

DataLogger::~DataLogger()
{
    if (pFits) delete pFits;
}

//void DataLogger::writeSlice(valarray<uint16_t> data)
void DataLogger::writeSlice(uint16_t *data)
{
    /*!
      writeSlice

      Takes a valarray of size nelements and writes it to the internal
      data array. If the valarray is not the same size as indicated or
      the function does nothing. If the image index excedes the maximum
      depth, the writeFile function is called and logging no longer occurrs.
     */

    fpixel = (naxes[2]*nelements)+1;
    naxes[2] +=1;
//    qDebug() << pFits->fitsPointer();
    fits_resize_img(pFits->fitsPointer(), USHORT_IMG, naxis, naxes, &status);
//    qDebug() << "Resize: " << status;
    fits_write_img(pFits->fitsPointer(), USHORT_IMG, fpixel, nelements, &data[0], &status);
//    qDebug() << "Write: " << status;
}

void DataLogger::getFitsPointer()
{
    /*!
      getFitsPointer()

      retrieve the underlying cfitsio file pointer for use w/ cfitsio routines
      do not use CCfits after this point
     */
//    fptr = pFits->fitsPointer();
//    qDebug() << fptr;
}

void DataLogger::writeFile()
{
    /*!
      writeFile

      Writes the internal data array to file. Will produce a data cube
      of specified size with some depth. Maximum file size is 0.5GB
     */
//    fits_flush_buffer(fptr, 1, &status);
    fits_flush_file(pFits->fitsPointer(), &status);
    qDebug() << "Flush: " << status;
    fits_close_file(pFits->fitsPointer(), &status);
    qDebug() << "Close: " << status;
    return;
}

void DataLogger::setHeaderData(HeaderData data)
{
    s_h_data = data;

    pFits->pHDU().addKey("KINTIME", s_h_data.acc_time, "Total cycle time");
    pFits->pHDU().addKey("EXPTIME", s_h_data.exp_time, "Exposure time");
    pFits->pHDU().addKey("EMGAIN", s_h_data.em_gain, "EM gain");
    pFits->pHDU().addKey("KP", s_h_data.kp, "Proporational gain");
    pFits->pHDU().addKey("KI", s_h_data.ki, "Integral gain");
    pFits->pHDU().addKey("KD", s_h_data.kd, "Derivative gain");
    pFits->pHDU().addKey("ALG", s_h_data.alg, "Algorithm");
    pFits->pHDU().addKey("BACKGRND", s_h_data.background, "Background, auto if -1");
    pFits->pHDU().addKey("TARGX", s_h_data.targx, "Servo x target");
    pFits->pHDU().addKey("TARGY", s_h_data.targy, "Servo y target");
    pFits->pHDU().addKey("ROTATION", s_h_data.rotation, "Servo rotation");
    pFits->flush();
}




