#include <Arduino.h>
#include "utils/PID.h"

//vel max 35.7 mc/s

// Define the target and current values
float y_ref = 30.0;
float y = 0.0;

// Define the control signal
float u = 0; 

// Define the sampling time
unsigned long sample_time = 100; // in milliseconds

// Define the variables used to compute the sample time
unsigned long prev_time = 0;
unsigned long current_time = 0;
unsigned long time_change = 0;

//Define the variables used to obtain the wheel speed
volatile int enc_time,enc_end,enc_start;
volatile long enc_count = 0;
 
String encdir ="";

PID PID1(25, 0.003, 1, 6, 7, 255, 0, sample_time);
void enc_isr();
void speed_data();

void setup() {
  // put your setup code here, to run once:
  // Initialize the serial communication
  Serial.begin(115200);
  
  // Set the pin mode

  pinMode(26, INPUT_PULLUP); //CLK_enc
  pinMode(27, INPUT_PULLUP); //DT_enc

  pinMode(8, OUTPUT); //PWM motor 1
  pinMode(9, OUTPUT); //Pin dir motor

  pinMode(28, INPUT_PULLUP); //CLK_enc
  pinMode(29, INPUT_PULLUP); //DT_enc

  attachInterrupt(digitalPinToInterrupt(26),enc_isr,CHANGE);
}

void loop() {
  // put your main code here, to run repeatedly:

  // Get the current speed
    if(enc_count>=400 || enc_count<=-400){
    enc_count=0;

    enc_end=millis();
    enc_time=enc_end-enc_start;
    y=(11.25*M_PI*1000/(30*enc_time)); //cm/s
    enc_start=enc_end;  
    
  }

  // Get the current time
  current_time = millis();
  time_change = current_time - prev_time;
  if(millis()>=10000) y_ref=10;

  // Wait for the sampling time
  if (time_change >= sample_time) {
    PID1.update(y,y_ref);
    u = PID1.getU();

    // Update the previous time
    prev_time = current_time;

    // Print the desired data
    speed_data();
  }



}

void enc_isr(){
// If the inputDT state is different than the inputCLK state then
// the encoder is rotating clockwise
  if (digitalRead(26) != digitalRead(27)) {
    enc_count ++;
    encdir ="CW";    
   } else {
    // Encoder is rotating counterclockwise
    enc_count --;
    encdir ="CCW";     
   }

}

void speed_data(){
  Serial.print(time_change);
  Serial.print(" ");
  Serial.print(y_ref);
  Serial.print(" ");
  Serial.print(y);
  Serial.print(" ");
  Serial.print(u);
  Serial.println(" ");
}