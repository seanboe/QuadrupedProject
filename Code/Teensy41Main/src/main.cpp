#include <Arduino.h>

#include <Kinematics.h>

#include <Servo.h>

#include "Ramp.h"
rampInt myRamp;


uint16_t dynamicAngle;
uint16_t demandAngle;

Kinematics leg2(2, 60, 90, 120, 135, 55, 90);
Servo L2M1;
Servo L2M2;
Servo L2M3;

float read();
void calculateGait();
float sinusoidGaits(int16_t xIn, int type);

void setup() {
  Serial.begin(9600);
  while (!Serial)

  pinMode(19, INPUT);

  L2M1.attach(0);
  L2M2.attach(1);
  L2M3.attach(2);


  // write primary positions to servos
  // leg2.solveFootPosition(100, 0, 177);  
  // L2M2.writeMicroseconds(leg2.motor2.angleMicros);
  // L2M3.writeMicroseconds(leg2.motor3.angleMicros);
  // while (1);

  L2M1.writeMicroseconds(leg2.motor1.angleMicros);
  L2M2.writeMicroseconds(leg2.motor2.angleMicros);
  L2M3.writeMicroseconds(leg2.motor3.angleMicros);

  delay(3000);

  myRamp.go(-30);
  myRamp.go(125, 1500, LINEAR, FORTHANDBACK);

}

void loop() {

  // calculateGait();

  static unsigned int amount = 0;
  if (Serial.available()) {
    amount = Serial.parseInt();
    // leg2.solveFootPosition(0, amount, 177);
  }
    // leg2.solveFootPosition(amount, myRamp.update(), 177);
    leg2.solveFootPosition(0, amount, 177);


    // if (millis() % 10 == 0) {
    //   dynamicAngle = leg2.getDyamicAngle(M3, DEGREES);
    //   demandAngle = leg2.motor3.angleDegrees;
    //   Serial.println(dynamicAngle);
    //   Serial.print(",");
    //   Serial.println(demandAngle);
    // }

    // L2M1.writeMicroseconds(leg2.motor1.angleMicros);
    // L2M2.writeMicroseconds(leg2.motor2.angleMicros);
    // L2M3.writeMicroseconds(leg2.motor3.angleMicros);

  // write calculated data to servos
  L2M1.writeMicroseconds(leg2.getDyamicAngle(M1, MILLIS));
  L2M2.writeMicroseconds(leg2.getDyamicAngle(M2, MILLIS));
  L2M3.writeMicroseconds(leg2.getDyamicAngle(M3, MILLIS));

}


void calculateGait() {
  static int16_t xIn = -100;
  static bool forwards = true;
  int gait = 0;
  if (micros() % 5000 == 0) {
    if (forwards == true) {
      // gait = (75*cos((PI * xIn)/200));
      gait = (int)sinusoidGaits(xIn, 4);
      Serial.println(gait);
      leg2.solveFootPosition(xIn, 0, (177 - gait));
      xIn += 1; 
      if (xIn == 101)
        forwards = false; 
    }
    else if (forwards == false) {
      xIn -= 1;
      leg2.solveFootPosition(xIn, 0, 177);
      if (xIn == -100)
        forwards = true;
    }
  }
}


float sinusoidGaits(int16_t xIn, int type) {
  switch (type) {
    case 1:
      return (75*cos((PI * xIn)/200));
    case 2: 
      return (-0.0075*(pow(xIn, 2)-10000));
    case 3: 
      return (-0.002*(pow(xIn, 2)-10000));
    case 4: 
      return (30*cos((PI * xIn)/200));
    default:
      return 0.00;
  }
}


float read() {
  int read1 = analogRead(19);
  delay(1);
  int read2 = analogRead(19);
  delay(1);
  int read3 = analogRead(19);
  delay(1);

  float result = ((float)read1 + (float)read2 + (float)read3)/3.0;

  return result;

}


























// #include <Arduino.h>

// #include <CoprocessorTalk.h>
// #include <Wire.h>

// #include <Kinematics.h>

// #include <Servo.h>

// #define SLAVE_ADDRESS 8
// #define DATA_INDEX_COUNT 17

// CoprocessorTalk teensyLC(SLAVE_ADDRESS, DATA_INDEX_COUNT, true);

// Kinematics leg2(2, 0, 90, 75, 135, 25, 90);
// Servo L2M2;
// Servo L2M3;

// void setup() {
//   Serial.begin(9600);
//   while (!Serial)
//   Wire.begin();

//   L2M2.attach(0);
//   L2M3.attach(1);

//   // write primary positions to servos
//   L2M2.writeMicroseconds(leg2.motor2.angleMicros);
//   L2M3.writeMicroseconds(leg2.motor3.angleMicros);

//   while(!teensyLC.retrieveData());            // halt while controller isn't connected 
//   Serial.println("Data recieved");
  
//   delay(3000);

//   // set controller mechanics
//   teensyLC.joinTriggers();   
//   teensyLC.write(SET_MODE_0); 
// }

// void loop() {
//   while(!teensyLC.retrieveData());            // halt while controller isn't connected
//   // Serial.println(teensyLC.getControllerInput(TRIGGERS));  
//   leg2.printStatusString();

//   leg2.applyVerticalTranslation(teensyLC.getControllerInput(TRIGGERS));

//   // write calculated data to servos
//   L2M2.writeMicroseconds(leg2.motor2.angleMicros);
//   L2M3.writeMicroseconds(leg2.motor3.angleMicros);
// }