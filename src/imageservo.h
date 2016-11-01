#ifndef IMAGESERVO_H
#define IMAGESERVO_H

#include <QObject>

class ImageServo : public QObject
{
    Q_OBJECT
public:
    explicit ImageServo(QObject *parent = 0);

signals:

public slots:
};

#endif // IMAGESERVO_H