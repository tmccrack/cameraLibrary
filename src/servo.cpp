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


/*
 * Iterate servo loop to update values
 */
void Servo::getUpdate(float *value, float *update)
{
    s_error.error = value - s_error.set_point;
    update = s_error.error * s_gain.kp +  // Proportional
            s_error.error * s_gain.dt * s_gain.ki +  // Integral
            (s_error.error - s_error.pre_error) * s_gain.kd;  // Derivative
    s_error.pre_error = s_error.error;
}


/*
 * Error accessor
 */
void Servo::getError(Error *error)
{
    error = s_error;
}


/*
 * Gain accessor
 */
void Servo::getGain(Gain *gain)
{
    gain = s_gain;
}


/*
 * Gain setter
 */
void Servo::setGain(float kp, float ki, float kd)
{
    s_gain.kp = kp;
    s_gain.ki = ki;
    s_gain.kd = kd;
}


/*
 * Loop speed setter
 */
void Servo::setSpeed(float dt)
{
    s_gain.dt = dt;
}


/*
 * Target setpoint setter
 */
void Servo::getTarget(float *set_point)
{
    set_point = s_error.set_point;
}

void Servo::setTarget(float set_point)
{
    s_error.set_point = set_point;
}




