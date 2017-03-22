#include "imagelogger.h"

using namespace std;
ImageLogger::ImageLogger(string filename)
{
    file = new QFile(QString::fromStdString(filename));
    file->open(QIODevice::WriteOnly);
    d_out = new QDataStream(file);
//    qDebug() << "Logging to: " << QString::fromStdString(filename);

}

ImageLogger::~ImageLogger()
{
    if (d_out) delete d_out;
    if (file->isOpen()) file->close();
    if (file) delete file;
}

void ImageLogger::newFile(string filename)
{
    file = new QFile(QString::fromStdString(filename));
    file->open(QIODevice::WriteOnly);
    d_out = new QDataStream(file);
    qDebug() << "Opened: " << file->fileName();
}

void ImageLogger::closeFile()
{
    if (file->isOpen()) file->close();
    qDebug() << "Closed: " << file->fileName();
}

void ImageLogger::header(string head)
{
    if (!file->isOpen()) file->open(QIODevice::WriteOnly);
    d_out->writeBytes(QString::fromStdString(head).toUtf8(), sizeof(QString::fromStdString(head).toUtf8()));
}

void ImageLogger::append(uint16_t *data, int sized)
{
    if (!file->isOpen()) file->open(QIODevice::WriteOnly);
    d_out->writeRawData((char *) data, d_size * sized);
}

