#ifndef IMAGELOGGER_H
#define IMAGELOGGER_H

#include <QtCore/QFile>
#include <QtCore/QDataStream>
#include <QtCore/QString>
#include <QtCore/QDebug>

class ImageLogger
{
public:
    ImageLogger(std::string filename);
    ~ImageLogger();
    void newFile(std::string filename);
    void closeFile();
    void header(std::string head);
    void append(uint16_t *data, int sized);

private:
    QFile *file;
    QDataStream *d_out;
    int d_size = sizeof(uint16_t);
};

#endif // IMAGELOGGER_H
