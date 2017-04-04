#include "datalogger.h"

using namespace std;
DataLogger::DataLogger(string filename)
{
    file = new QFile(QString::fromStdString(filename));
    file->open(QIODevice::WriteOnly);
    d_out = new QDataStream(file);
//    qDebug() << "Logging to: " << QString::fromStdString(filename);

}

DataLogger::~DataLogger()
{
    if (d_out) delete d_out;
    if (file->isOpen()) file->close();
    if (file) delete file;
}

void DataLogger::newFile(string filename)
{
    file = new QFile(QString::fromStdString(filename));
    file->open(QIODevice::WriteOnly);
    d_out = new QDataStream(file);
    qDebug() << "Opened: " << file->fileName();
}

void DataLogger::closeFile()
{
    if (file->isOpen()) file->close();
    qDebug() << "Closed: " << file->fileName();
}

void DataLogger::header(string head)
{
    if (!file->isOpen()) file->open(QIODevice::WriteOnly);
    d_out->writeBytes(QString::fromStdString(head).toUtf8(), sizeof(QString::fromStdString(head).toUtf8()));
}

void DataLogger::append(uint16_t *data, int sized)
{
    if (!file->isOpen()) file->open(QIODevice::WriteOnly);
    d_out->writeRawData((char *) data, d_size * sized);
}

void DataLogger::appendFloat(float *data, int sized)
{
    if (!file->isOpen()) file->open(QIODevice::WriteOnly);
    d_out->setFloatingPointPrecision(QDataStream::SinglePrecision);
    d_out->writeRawData((char *) data, f_size * sized);
}

