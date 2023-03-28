#include <Arduino.h>
#include "utils/PID.h"

// Define the target and current values
float y_ref = 50.0;
float y = 0.0;

// Define the control signal
float u = 0; 

// Define the sampling time
unsigned long sample_time = 100; // in milliseconds

// Define the variables used to compute the sample time
unsigned long prev_time = 0;
unsigned long current_time = 0;

//Define the variables used to obtain the wheel speed
volatile int enc_time,enc_end,enc_start;
volatile long enc_count = 0;

PID PID1(5, 0.003, 1, 3, 2, 4095, 0, sample_time);
void enc_isr();
void speed_data();

void setup() {
  // put your setup code here, to run once:
  // Initialize the serial communication
  Serial.begin(115200);
  
  // Set the pin mode
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(26, INPUT_PULLUP);
  pinMode(27, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(52),enc_isr,CHANGE);
  attachInterrupt(digitalPinToInterrupt(53),enc_isr,CHANGE);
}

void loop() {
  // put your main code here, to run repeatedly:

  // Get the current speed
    if(enc_count>=4 || enc_count<=-4){
    enc_count=0;

    enc_end=millis();
    enc_time=enc_end-enc_start;
    y=(0.0575*M_PI*1000/(30*enc_time));
    enc_start=enc_end;  
    
  }

  // Get the current time
  current_time = millis();
  if(millis()>10000) y_ref=15;

  // Wait for the sampling time
  while ((current_time - prev_time) < sample_time) {
    PID1.update(y,y_ref);
    u = PID1.getU();
  }

  // Send the PWM signal to the motor controller
  analogWrite(3,u);

  // Print the desired data
  speed_data();

  // Update the previous time
  prev_time = current_time;

}

void enc_isr(){
  static int8_t lookup_table[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};
    static uint8_t enc_val = 0;
    static uint8_t PIND = REG_PIOD_PDSR & 0xFF; //reads the 8 lower bits of port D
    
    enc_val = enc_val << 2;
    enc_val = enc_val | ((PIND & 0b1100) >> 2);
 
    enc_count = enc_count + lookup_table[enc_val & 0b1111];
}

void speed_data(){
  Serial.print(current_time - prev_time);
  Serial.print(" ");
  Serial.print(y_ref);
  Serial.print(" ");
  Serial.print(y);
  Serial.print(" ");
  Serial.print(u);
  Serial.print(" ");
}