#include "PID.h"
#include <Arduino.h>


PID::PID(float Kp, float Ki, float Kd, int PWM_pin, int PWM_direction_pin, float PWM_max, float PWM_min)
{
    /* Here we initialize the variables of the class PID-Controller */

    this->Kp = Kp;
    this->Ki = Ki;
    this->Kd = Kd;

    this->PWM_pin = PWM_pin;
    this->PWM_direction_pin = PWM_direction_pin;

    this->PWM_max = PWM_max;
    this->PWM_min = PWM_min;

    this->y = 0;
    this->y_prev = 0;
    this->e = 0;
    this->e_integral = 0;
    this->u = 0;

    // Set the pinout
    pinMode(PWM_pin, OUTPUT);
    pinMode(PWM_direction_pin, OUTPUT);

    // Set the initial direction of the motor
    digitalWrite(PWM_direction_pin, HIGH);
    
}

void PID::update(float y, float y_ref)
{
    /* Here starts the Update of the PID */
    /* ... */


}
