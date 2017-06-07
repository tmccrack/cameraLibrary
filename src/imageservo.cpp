#include "imageservo.h"

ImageServo::ImageServo(QObject *parent, float *centroids, float *updates, int x_dim, int y_dim)
{
    dim[0] = x_dim;
    dim[1] = y_dim;
    dim[2] = x_dim * y_dim;
    cent = centroids;
    update = updates;
    errs = new float[2];  // For transformed errors
    bg = 0;  // Background

    // Set up servos
    x_servo = new Servo(this);
    y_servo = new Servo(this);

    setRotation(45);
    setTargetCoords(256, 256);
}

ImageServo::~ImageServo()
{
    if (errs) delete errs;
    if (x_servo) delete x_servo;
    if (y_servo) delete y_servo;
}


/*
 * Centroid image, find error and rotate to mirror coordinate system
 * Assign updates to pointers from instantiation
 */
void ImageServo::getUpdate()
{
    centroid();

    errs[0] = cent[0] - x_servo->getTarget();
    errs[1] = cent[1] - y_servo->getTarget();

    // Transform errors for servo
    x_servo->setError(errs[0]*rotation[0] - errs[1]*rotation[1]);
    y_servo->setError(errs[0]*rotation[1] + errs[1]*rotation[0]);

    // Assign the update
    update[0] += x_servo->getUpdate();
    update[1] += y_servo->getUpdate();
}

/*
 * Assign the image buffer
 */
void ImageServo::setBuffer(uint16_t *cont_buffer)
{
    buffer = cont_buffer;
}


/*
 * Get error structures
 */
void ImageServo::getErrors(Error *x, Error *y)
{
    *x = x_servo->getError();
    *y = y_servo->getError();
}

void ImageServo::zeroErrors()
{
    x_servo->zeroError();
    y_servo->zeroError();
}


/*
 * Getter/setters for image dimension
 */
void ImageServo::getImageDim(int *x_dim, int *y_dim)
{
    *x_dim = dim[0];
    *y_dim = dim[1];
}

void ImageServo::setImageDim(int x_dim, int y_dim)
{
    dim[0] = x_dim;
    dim[1] = y_dim;
    dim[2] = x_dim * y_dim;
    qDebug() << "ImageServo: imagedim: " << dim[0] << " " << dim[1] << " " << dim[2];
}


/*
 * Set camera rotation for transforming camera axes to stage axes
 */
float ImageServo::getRotation()
{
    return rot_deg;
}

void ImageServo::setRotation(float degrees)
{
    rotation[0] = cos(PI * degrees / 180.0);  // X rotation
    rotation[1] = sin(PI * degrees / 180.0);  // Y rotation
    rot_deg = degrees;
}


/*
 * Getter/setter for target coordinates
 */
void ImageServo::getTargetCoords(float *x, float *y)
{
    *x = x_servo->getTarget();
    *y = y_servo->getTarget();
}

void ImageServo::setTargetCoords(float x, float y)
{
    qDebug() << "ImageServo: targetcoords: " << x << " " << y;
    x_servo->setTarget(x);
    y_servo->setTarget(y);
}


/*
 * Getter/setter for servo gains
 */
Gain ImageServo::getGainsX()
{
    return x_servo->getGain();
}

Gain ImageServo::getGainsY()
{
    return y_servo->getGain();
}

void ImageServo::setGainsX(Gain gain)
{
    x_servo->setGain(gain);
}

void ImageServo::setGainsY(Gain gain)
{
    y_servo->setGain(gain);
}

float ImageServo::getBackground()
{
    return bg;
}

void ImageServo::setBackground(float background)
{
    bg = background;
}


/*
 * Barycenter algorithm
 */
void ImageServo::centroid()
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
            sum += buffer[offs + j] - bg;
            row_x += buffer[i + j*dim[1]] - bg;
            row_y += buffer[offs + j] - bg;
        }

        sum_x += (i+1) * row_x;
        sum_y += (i+1) * row_y;
    }

    cent[0] = sum_x / sum;
    cent[1] = sum_y / sum;
}


