#ifndef SERVO_H
#define SERVO_H

#include <QtCore/QObject>
#include <QtCore/QDebug>

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


/*
 * Class implements PID control loop
 */
class Servo : public QObject
{

public:
    explicit Servo(QObject *parent=0);
    Gain getGain();
    void setGain(Gain gain);
    void setGain(float kp, float ki, float kd, float dt);
    float getTarget();
    void setTarget(float set_point);

    float getUpdate();

    Error getError();
    void setError(float err);


private:
    Gain s_gain;
    Error s_error;
    float update;

};

#endif // SERVO_H
