#include <Arduino.h>

#include <Kinematics.h>

#include <Servo.h>

#include <Ramp.h>

#define TIME_ANGLE_GRADIENT 

Kinematics leg2(2, 0, 90, 75, 135, 55, 90);
Servo L2M2;
Servo L2M3;

void setup() {
  Serial.begin(9600);
  // while (!Serial)

  pinMode(19, INPUT);

  L2M2.attach(0);
  L2M3.attach(1);


  // write primary positions to servos
  L2M2.writeMicroseconds(leg2.motor2.angleMicros);
  L2M3.writeMicroseconds(leg2.motor3.angleMicros);
  
  delay(3000);

}

void loop() {
  if (millis() % 100 == 0)
    // leg2.printStatusString();

  leg2.applyVerticalTranslation(analogRead(19));

  // write calculated data to servos
  L2M2.writeMicroseconds(leg2.motor2.angleMicros);
  L2M3.writeMicroseconds(leg2.motor3.angleMicros);


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