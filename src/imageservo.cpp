#include "imageservo.h"

ImageServo::ImageServo(QObject *parent, int x_dim, int y_dim)
{
    x_dim = x_dim;
    y_dim = y_dim;
    dimension = x_dim * y_dim;
}

void ImageServo::getUpdate(long *buffer, float *x, float *y)
{
    centroid(buffer, x, y);
    x_servo.getUpdate(x);
    y_servo.getUpdate(y);
    cent[0] = *x;
    cent[1] = *y;
}

void ImageServo::getErrors(float *x, float *y)
{
    x[0] = s_x_error.error;
    x[1] = s_x_error.pre_error;
    x[2] = s_x_error.set_point;

    y[0] = s_y_error.error;
    y[1] = s_y_error.pre_error;
    y[2] = s_y_error.set_point;
}

void ImageServo::setRotation(float x_degrees, float y_degrees)
{
    rotation[0] = PI * x_degrees / 180.0;
    rotation[1] = PI * y_degrees / 180.0;
}

void ImageServo::getTargetCoords(float *x, float *y)
{
    x = x_servo.get
}

void ImageServo::setTargetCoords(float x, float y)
{
    x_servo.setTarget(x);
    y_servo.setTarget(y);
}



void ImageServo::centroid(long *buffer, float *x, float *y)
{
    row_x = 0;
    row_y = 0;
    sum = 0;
    sum_x = 0;
    sum_y = 0;
    int offs = 0;

    for (int i = 0; i < x_dim; i++)
    {
        row_x = 0;
        row_y = 0;
        for (int j = 0; j < y_dim; j++)
        {
            offs = i * x_dim;
            sum += buffer[offs + j];
            row_x += buffer[i + j*y_dim];
            row_y += buffer[offs + j];
        }

        sum_x += (i+1) * row_x;
        sum_y += (i+1) * row_y;
    }

    x = sum_x / sum;
    y = sum_y / sum;
}
