#ifndef DATALOGGER_H
#define DATALOGGER_H

#include <QtCore/QFile>
#include <QtCore/QDataStream>
#include <QtCore/QString>
#include <QtCore/QDebug>
#include <CCfits/CCfits>

#define MAX_ELEMENTS 256000000

//class HeaderData
//{
//public:
//    long exp_time;
//    long acc_time;
//    int em_gain;
//    float kp;
//    float ki;
//    float kd;
//    float rotation;
//    float targx;
//    float targy;
//    int background;
//    std::string alg;
//};

struct HeaderData{
    float exp_time;
    float acc_time;
    int em_gain;
    float kp;
    float ki;
    float kd;
    float rotation;
    float targx;
    float targy;
    float background;
    std::string alg;
};


class DataLogger
{
public:
    DataLogger(std::string filename, int idim[2]);
    ~DataLogger();

    void writeSlice(std::uint16_t *data);
    void writeFile();
    void writeHeader(HeaderData *data);
    void setHeaderData(HeaderData data);
    void getFitsPointer();
//    void writeHeader(int acc_time, int exp_time, int em_gain);
//    void writeServoHeader(float kp, float ki, float kd,
//                          float rotation, float targx, float targy,
//                          int background, std::string alg);


private:
//    QFile *file;
//    QDataStream *d_out;
//    int d_size = sizeof(uint16_t);
//    int f_size = sizeof(float);
//    bool isFileEmpty();
    void setServoHeaderData();

//    std::unique_ptr<CCfits::FITS> pFits;
    CCfits::FITS *pFits;
    std::string filename;
    int naxis = 3;
    long naxes[3];
    long cfitsaxes[3];
    long nelements;
    long image_ind;
    unsigned long long fpixel;
    HeaderData s_h_data;
    fitsfile *fptr;
    int status;
};

#endif // DATALOGGER_H
