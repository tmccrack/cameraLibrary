#include "servo.h"

Servo::Servo(QObject *parent)
{
    s_gain.kp = 0;
    s_gain.ki = 0;
    s_gain.kd = 0;
    s_gain.dt = 0;

    s_error.error = 0;
    s_error.pre_error = 0;
    s_error.set_point = 0;
}

Servo::~Servo()
{

}


/*
 * Iterate servo loop to get update
 * Error value must be set previously
 * In case of image servo, value must be transformed prior
 */
float Servo::getUpdate()
{
    update = s_error.error * s_gain.kp +  // Proportional
            s_error.error * s_gain.dt * s_gain.ki +  // Integral
            (s_error.error - s_error.pre_error) * s_gain.kd;  // Derivative
    s_error.pre_error = s_error.error;
    return update;
}


/*
 * Error accessor
 */
Error Servo::getError()
{
    return s_error;
}

void Servo::setError(float err)
{
    s_error.error = err;
}

void Servo::zeroError()
{
    s_error.error = 0;
    s_error.pre_error = 0;
}


/*
 * Gain getter/setter
 */
Gain Servo::getGain()
{
    return s_gain;
}

void Servo::setGain(Gain gain)
{
    s_gain = gain;
}

void Servo::setGain(float kp, float ki, float kd, float dt)
{
    s_gain.kp = kp;
    s_gain.ki = ki;
    s_gain.kd = kd;
}


/*
 * Target setpoint setter
 */
float Servo::getTarget()
{
    return s_error.set_point;
}

void Servo::setTarget(float set_point)
{
    s_error.set_point = set_point;
}




