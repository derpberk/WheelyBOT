#include <Arduino.h>
#include "utils/PID.h"

//vel max 35.7 mc/s

// Define the target and current values
float y_ref = 30.0;
float y[4] = {0,0,0,0};

// Define the control signal
float u[4]; 

// Define the sampling time
unsigned long sample_time = 100; // in milliseconds

// Define the variables used to compute the sample time
unsigned long prev_time = 0;
unsigned long current_time = 0;
unsigned long time_change = 0;

//Define the variables used to obtain the wheel speed
volatile int enc_time[4],enc_end[4],enc_start[4];
volatile long enc_count[4] = {0,0,0,0};
 
String encdir ="";

PID PIDs[4]={PID(5, 0.03, 1, 8, 9, 255, 0, sample_time), //mot 1 
             PID(5, 0.03, 1, 6, 7, 255, 0, sample_time), //mot 2
             PID(5, 0.03, 1, 4, 5, 255, 0, sample_time), //mot 3
             PID(5, 0.03, 1, 2, 3, 255, 0, sample_time)  //mot 4
              };

void enc_isr1();
void enc_isr2();
void enc_isr3();
void enc_isr4();

void speed_data();

void setup() {
  // put your setup code here, to run once:
  // Initialize the serial communication
  Serial.begin(115200);
  
  // Set the pin mode

  pinMode(26, INPUT_PULLUP); //CLK_enc mot1
  pinMode(27, INPUT_PULLUP); //DT_enc

  pinMode(28, INPUT_PULLUP); //CLK_enc mot2
  pinMode(29, INPUT_PULLUP); //DT_enc

  //pinMode(30, INPUT_PULLUP); //CLK_enc mot3
  //pinMode(31, INPUT_PULLUP); //DT_enc

  //pinMode(32, INPUT_PULLUP); //CLK_enc mot4
  //pinMode(33, INPUT_PULLUP); //DT_enc

  attachInterrupt(digitalPinToInterrupt(26),enc_isr1,CHANGE);
  attachInterrupt(digitalPinToInterrupt(28),enc_isr2,CHANGE);
  //attachInterrupt(digitalPinToInterrupt(30),enc_isr3,CHANGE);
  //attachInterrupt(digitalPinToInterrupt(32),enc_isr4,CHANGE);
}

void loop() {
  // put your main code here, to run repeatedly:

  // Get the current speed
  for(int i=0; i<4; i++){
    if(enc_count[i]>=400 || enc_count[i]<=-400){
      enc_count[i]=0;

      enc_end[i]=millis();
      enc_time[i]=enc_end[i]-enc_start[i];
      y[i]=(11.25*M_PI*1000/(30*enc_time[i])); //cm/s
      enc_start[i]=enc_end[i];
    
    }
  }
    
  // Get the current time
  current_time = millis();
  time_change = current_time - prev_time;

  if(millis()>=10000) y_ref=10;

  // Wait for the sampling time
  if (time_change >= sample_time) {
    for(int i=0; i<4; i++){
      PIDs[i].update(y[i],y_ref);
      u[i] = PIDs[i].getU();
    }

    // Update the previous time
    prev_time = current_time;

    // Print the desired data
    speed_data();
  }

}

void enc_isr1(){
// If the inputDT state is different than the inputCLK state then
// the encoder is rotating clockwise
  if (digitalRead(26) != digitalRead(27)) {
    enc_count[0] ++;
   } else {
    // Encoder is rotating counterclockwise
    enc_count[0] --;
   }

}
void enc_isr2(){
// If the inputDT state is different than the inputCLK state then
// the encoder is rotating clockwise
  if (digitalRead(28) != digitalRead(29)) {
    enc_count[1] ++;
   } else {
    // Encoder is rotating counterclockwise
    enc_count[1] --;
   }

}
void enc_isr3(){
// If the inputDT state is different than the inputCLK state then
// the encoder is rotating clockwise
  if (digitalRead(30) != digitalRead(31)) {
    enc_count[2] ++;
   } else {
    // Encoder is rotating counterclockwise
    enc_count[2] --;
   }

}
void enc_isr4(){
// If the inputDT state is different than the inputCLK state then
// the encoder is rotating clockwise
  if (digitalRead(32) != digitalRead(33)) {
    enc_count[3] ++;
   } else {
    // Encoder is rotating counterclockwise
    enc_count[3] --;
   }

}

void speed_data(){
  Serial.print(time_change);
  Serial.print(" ");
  Serial.print(y_ref);
  Serial.print(" ");
  Serial.print(y[0]);
  Serial.print(" ");
  Serial.print(y[1]);
  Serial.print(" ");
  Serial.print(y[2]);
  Serial.print(" ");
  Serial.print(y[3]);
  Serial.print(" ");
  Serial.print(u[0]);
  Serial.print(" ");
  Serial.print(u[1]);
  Serial.print(" ");
  Serial.print(u[2]);
  Serial.print(" ");
  Serial.print(u[3]);
  Serial.println(" ");
}