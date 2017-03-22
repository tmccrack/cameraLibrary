#ifndef IMAGESERVO_H
#define IMAGESERVO_H

#include <cstdint>
#include <QtCore/QObject>
#include <QtCore/QDebug>
#include "servo.h"

#define PI 3.14159265

class ImageServo : public QObject
{

public:
    ImageServo(QObject *parent = 0, float *centroids=0, float *updates=0, int x_dim=32, int y_dim=32);
    ~ImageServo();

    void setBuffer(uint16_t *cont_buffer);

    void getUpdate();
    void getErrors(Error *x, Error *y);
    void zeroErrors();

    void getImageDim(int *x_dim, int *y_dim);
    void setImageDim(int x_dim, int y_dim);

    float getRotation();
    void setRotation(float degrees);

    void getTargetCoords(float *x, float *y);
    void setTargetCoords(float x, float y);

    Gain getGainsX();
    Gain getGainsY();
    void setGainsX(Gain gain);
    void setGainsY(Gain gain);

private:
    void centroid();

    float sum, row_x, row_y, sum_x, sum_y = 0;

    int *dim = new int[3];  // x, y, total
    float *cent = new float[2];
    float *update = new float[2];
    float *rotation = new float[2];
    float rot_deg;
    uint16_t *buffer;
    float *errs;

    Servo *x_servo;
    Servo *y_servo;

    Error s_x_error;
    Error s_y_error;

};

#endif // IMAGESERVO_H
