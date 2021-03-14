#include <Arduino.h>

#include <Kinematics.h>

#include <Servo.h>

#include "Ramp.h"
rampInt myRamp;


uint16_t dynamicAngle;
uint16_t demandAngle;

Kinematics leg2(2, 150, 90, 125, 135, 55, 90);
Servo L2M2;
Servo L2M3;

float read();

void setup() {
  Serial.begin(9600);
  while (!Serial)

  pinMode(19, INPUT);

  L2M2.attach(0);
  L2M3.attach(1);


  // write primary positions to servos
  L2M2.writeMicroseconds(leg2.motor2.angleMicros);
  L2M3.writeMicroseconds(leg2.motor3.angleMicros);
  
  delay(3000);

  myRamp.go(0);
  myRamp.go(1000, 1000, LINEAR, FORTHANDBACK);

}

void loop() {
    // leg2.printStatusString();

  // leg2.solveFtShldrLength(analogRead(19));
  // leg2.solveFtShldrLength(myRamp.update());
  // leg2.solveFtShldrLength(1000);
  // leg2.solveFtShldrLength(read());

  if (Serial.available()) {
    unsigned int amount = Serial.parseInt();
    leg2.solveFtShldrLength(amount);
  }


    if (millis() % 10 == 0) {
      dynamicAngle = leg2.getDyamicAngle(M3, DEGREES);
      demandAngle = leg2.motor3.angleDegrees;
      Serial.println(dynamicAngle);
      Serial.print(",");
      Serial.println(demandAngle);
    }
    // L2M2.writeMicroseconds(leg2.motor2.angleMicros);
    // L2M3.writeMicroseconds(leg2.motor3.angleMicros);

  // write calculated data to servos
  L2M2.writeMicroseconds(leg2.getDyamicAngle(M2, MILLIS));
  L2M3.writeMicroseconds(leg2.getDyamicAngle(M3, MILLIS));

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