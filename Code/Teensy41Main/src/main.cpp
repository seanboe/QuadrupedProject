#include <Arduino.h>

#include <Kinematics.h>

#include <Servo.h>

#include "Ramp.h"
rampInt myRamp;

#include <DataTransfer.h>
#include "dataEnums.h"

#define RELAY_PIN   33



//Radio Encryption Key
uint8_t key[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};


RH_RF69 rf69(RFM69_CS, RFM69_INT);
Transfer dataBuffer(&rf69, BUFFER_SIZE, key);

uint16_t dynamicAngle;
uint16_t demandAngle;


// Kinematics leg2(2, 60, 90, 120, 135, 55, 90); // This sets the initial leg position too
Kinematics leg2(2, DEFAULT_X, DEFAULT_Y, DEFAULT_Z, 60, 120, 55);
Servo L2M1;
Servo L2M2;
Servo L2M3;


uint8_t parseControllerData(BIT_OR_BYTE bitOrByte, int index);

void calculateGait();
float sinusoidGaits(int16_t xIn, int type);

void setup() {
  Serial.begin(9600);
  while (!Serial)
    delay(10);

  // dataBuffer.init();
  // Serial.println("initialization complete");

  // Activate motors
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
  Serial.println("Relay activated");
  delay(5000);
  Serial.println("Relay engaged");


  L2M1.attach(23);
  L2M2.attach(22);
  L2M3.attach(15);

  // write primary servo positions
  L2M1.writeMicroseconds(leg2.motor1.angleMicros);
  L2M2.writeMicroseconds(leg2.motor2.angleMicros);
  L2M3.writeMicroseconds(leg2.motor3.angleMicros);

  delay(3000);

}

void loop() {

  // calculateGait();

  static unsigned int amount = 0;
  if (Serial.available()) {
    amount = Serial.parseInt();
    // leg2.solveFootPosition(0, amount, 177);        // below
    // leg2.setFootEndpoint(amount, 45, DEFAULT_Z);
  } 
      // leg2.solveFootPosition(0, amount, 177);

      leg2.setFootEndpoint(amount, 45, DEFAULT_Z);


    leg2.updateDynamicFootPosition();

    L2M1.writeMicroseconds(leg2.motor1.dynamicMicros);
    L2M2.writeMicroseconds(leg2.motor2.dynamicMicros);
    L2M3.writeMicroseconds(leg2.motor3.dynamicMicros);

}


// void calculateGait() {
//   static int16_t xIn = -100;
//   static bool forwards = true;
//   int gait = 0;
//   if (micros() % 5000 == 0) {
//     if (forwards == true) {
//       // gait = (75*cos((PI * xIn)/200));
//       gait = (int)sinusoidGaits(xIn, 4);
//       Serial.println(gait);
//       leg2.solveFootPosition(xIn, 0, (177 - gait));
//       xIn += 1; 
//       if (xIn == 101)
//         forwards = false; 
//     }
//     else if (forwards == false) {
//       xIn -= 1;
//       leg2.solveFootPosition(xIn, 0, 177);
//       if (xIn == -100)
//         forwards = true;
//     }
//   }
// }


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


uint8_t parseControllerData(BIT_OR_BYTE bitOrByte, int index) {
  dataBuffer.receive();
  return dataBuffer.read(bitOrByte, index);
}