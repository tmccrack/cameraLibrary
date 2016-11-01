#ifndef IMAGESERVO_H
#define IMAGESERVO_H

#include <QObject>
#include "servo.h"

#define PI 3.14159265

class ImageServo : public QObject
{
    Q_OBJECT
public:
    explicit ImageServo(QObject *parent = 0, int x_dim = 32, int y_dim = 32);
    void getUpdate(long *buffer, float *x, float *y);
    void getErrors(float *x, float *y);
    void setRotation(float x_degrees, float y_degrees);
    void getTargetCoords(float *x, float *y);
    void setTargetCoords(float x, float y);

signals:

public slots:

private:
    void centroid(long *buffer, float *x, float *y);

    float sum, row_x, row_y, sum_x, sum_y = 0;

    int *dim = new int[3];
    float *cent = new float[2];
    float *rotation = new float[2];


    Servo x_servo;
    Servo y_servo;

    Error s_x_error;
    Error s_y_error;

};

#endif // IMAGESERVO_H
