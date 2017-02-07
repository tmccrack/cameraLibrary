#ifndef SERVO_H
#define SERVO_H

#include <QtCore/QObject>

struct Gain{
    float kp;
    float ki;
    float kd;
    float dt;
};

struct Error{
    float error;
    float pre_error;
    float set_point;
};

class Servo : public QObject
{
    Q_OBJECT

public:
    explicit Servo(QObject *parent = 0);
    void getGain(Gain *gain);
    void setGain(float kp, float ki, float kd);
    void setSpeed(float dt);
    void getTarget(float *set_point);
    void setTarget(float set_point);
    void getUpdate(float *value, float *update);
    void getError(Error *error);

signals:

public slots:

private:
    Gain s_gain;
    Error s_error;

};

#endif // SERVO_H
