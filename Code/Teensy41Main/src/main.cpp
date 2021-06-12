#include <Arduino.h>

#include <Kinematics.h>
#include <motorSetup.h> // Holds the stuff for constraining, offsetting, and initializing the motor values

#include <Servo.h>

#include "Ramp.h"


// default axis lengths (the 'safe' position for all the motors)
#define DEFAULT_X 0
#define DEFAULT_Y 45    // This is the same as LIMB_1; I want the foot to be directly under the shoulder ie straight, not under the bearing
#define DEFAULT_Z 177   // This is the foot-shoulder length when the leg makes a 45-45-90 triangle

#define RELAY_PIN   33


// Kinematics leg2(2, 60, 90, 120, 135, 55, 90); // This sets the initial leg position too
Kinematics leg2(LEG_2, DEFAULT_X, DEFAULT_Y, DEFAULT_Z, motors);
Servo L2M1_SERVO;
Servo L2M2_SERVO;
Servo L2M3_SERVO;


// ******************************* LEG STRUCTURE DECLRATION ********************************
//Leg 1:
Motor L1M1;
Motor L1M2;
Motor L1M3;

//Leg 2:
Motor L2M1;
Motor L2M2;
Motor L2M3;

//Leg 2:
Motor L3M1;
Motor L3M2;
Motor L3M3;

//Leg 2:
Motor L4M1;
Motor L4M2;
Motor L4M3;

Motor motors[ROBOT_LEG_COUNT * MOTORS_PER_LEG];


void setup() {
  Serial.begin(9600);
  while (!Serial)
    delay(10);

  // Activate motors
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
  Serial.println("Relay activated");
  delay(5000);
  Serial.println("Relay engaged");

  initMotorVals(motors);

  L2M1_SERVO.attach(23);
  L2M2_SERVO.attach(22);
  L2M3_SERVO.attach(15);

  // write primary servo positions
  // L2M1_SERVO.writeMicroseconds(applyContextualOffset(motors, LEG_2, M1, motors[indexOfMotor(LEG_2, M1)].angleDegrees));
  // L2M2_SERVO.writeMicroseconds(applyContextualOffset(motors, LEG_2, M2, motors[indexOfMotor(LEG_2, M2)].angleDegrees));
  // L2M3_SERVO.writeMicroseconds(applyContextualOffset(motors, LEG_2, M3, motors[indexOfMotor(LEG_2, M3)].angleDegrees));

  Serial.println(motors[indexOfMotor(LEG_2, M1)].angleDegrees);
  Serial.println(motors[indexOfMotor(LEG_2, M2)].angleDegrees);
  Serial.println(motors[indexOfMotor(LEG_2, M3)].angleDegrees);

  Serial.println(applyContextualOffset(motors, LEG_2, M1, motors[indexOfMotor(LEG_2, M1)].angleDegrees));
  Serial.println(applyContextualOffset(motors, LEG_2, M2, motors[indexOfMotor(LEG_2, M2)].angleDegrees));
  Serial.println(applyContextualOffset(motors, LEG_2, M3, motors[indexOfMotor(LEG_2, M3)].angleDegrees));

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

  //   leg2.setFootEndpoint(amount, 45, DEFAULT_Z);


  //   leg2.updateDynamicFootPosition();

  // L2M1_SERVO.writeMicroseconds(applyContextualOffset(motors, LEG_2, M1, motors[indexOfMotor(LEG_2, M1)].dynamicMicros));
  // L2M2_SERVO.writeMicroseconds(applyContextualOffset(motors, LEG_2, M2, motors[indexOfMotor(LEG_2, M2)].dynamicMicros));
  // L2M3_SERVO.writeMicroseconds(applyContextualOffset(motors, LEG_2, M3, motors[indexOfMotor(LEG_2, M3)].dynamicMicros));

}











// ///  
// Kinematics leg1(1, motorList)
// leg1.setFootEndpoint(x, y, z)

// Kinematics legs(motorList)
// for leg in legs:
//   leg.setFootEndPoints()

// Kine::Kine(motors) {
//   int length = sizeof()
//   for (i = 0; i < length; i++)
//     motors[i].angleDegrees = 10;
// }


// ///