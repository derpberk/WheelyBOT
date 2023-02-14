// PID.h
#ifndef PID_h
#define PID_h

#include <Arduino.h>

class PID {

  private:

    float y; // this is the input of the PID controller
    float y_prev; // this is the input of the PID controller in t-1
    float u; // this is the output of the PID controller
    float e; // this is the error of the PID controller
    float e_integral; // this is the integral of the error of the PID controller
    float Kp; // this is the proportional gain of the PID controller
    float Ki; // this is the integral gain of the PID controller
    float Kd; // this is the derivative gain of the PID controller

    int PWM_pin; // this is the pin where the PWM signal is sent to the motor
    int PWM_direction_pin; // this is the pin where the direction of the motor is set

    float PWM_max; // this is the maximum value of the PWM signal
    float PWM_min; // this is the minimum value of the PWM signal
    
  public:

    // Here we define the methods of the class PID
    // This is the constructor of the class PID
    PID(float Kp, float Ki, float Kd, int PWM_pin, int PWM_direction_pin, float PWM_max, float PWM_min); // Constructor of the class PID

    // This is the method that updates the PID controller
    void update(float y, float y_ref);
    

    



    
};

#endif