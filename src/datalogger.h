#ifndef DATALOGGER_H
#define DATALOGGER_H

#include <QtCore/QFile>
#include <QtCore/QDataStream>
#include <QtCore/QString>
#include <QtCore/QDebug>

class DataLogger
{
public:
    DataLogger(std::string filename);
    ~DataLogger();
    void newFile(std::string filename);
    void closeFile();
    void header(std::string head);
    void append(uint16_t *data, int sized);
    void appendFloat(float *data, int sized);

private:
    QFile *file;
    QDataStream *d_out;
    int d_size = sizeof(uint16_t);
    int f_size = sizeof(float);
    bool isFileEmpty();
};

#endif // DATALOGGER_H
