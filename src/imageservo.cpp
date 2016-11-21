#include "imageservo.h"

ImageServo::ImageServo(QObject *parent, int x_dim, int y_dim)
{
    dim[0] = x_dim;
    dim[1] = y_dim;
    dim[2] = x_dim * y_dim;
}


/*
 * Centroid image, assign updates to pointers passed in
 */
void ImageServo::getUpdate(long *buffer, float *x, float *y)
{
    centroid(buffer);
    x_servo.getUpdate(&cent[0], &updates[0]);
    y_servo.getUpdate(&cent[1], &updates[1]);
    updates[0] = *x;
    updates[1] = *y;
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


/*
 * Set camera rotation for transforming camera axes to stage axes
 */
void ImageServo::setRotation(float x_degrees, float y_degrees)
{
    rotation[0] = PI * x_degrees / 180.0;
    rotation[1] = PI * y_degrees / 180.0;
}


/*
 * Get the current target coordinates
 */
void ImageServo::getTargetCoords(float *x, float *y)
{
    x_servo.getTarget(x);
    y_servo.getTarget(y);
}


/*
 * Set the servo loop target values
 */
void ImageServo::setTargetCoords(float x, float y)
{
    x_servo.setTarget(x);
    y_servo.setTarget(y);
}



void ImageServo::centroid(long *buffer)
{
    row_x = 0;
    row_y = 0;
    sum = 0;
    sum_x = 0;
    sum_y = 0;
    int offs = 0;

    for (int i = 0; i < dim[0]; i++)
    {
        row_x = 0;
        row_y = 0;
        for (int j = 0; j < dim[1]; j++)
        {
            offs = i * dim[0];
            sum += buffer[offs + j];
            row_x += buffer[i + j*dim[1]];
            row_y += buffer[offs + j];
        }

        sum_x += (i+1) * row_x;
        sum_y += (i+1) * row_y;
    }

    cent[0] = sum_x / sum;
    cent[1] = sum_y / sum;
}
