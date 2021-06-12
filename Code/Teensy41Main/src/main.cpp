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

// ******************************* LEG STRUCTURE DECLARATION ********************************
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

Kinematics leg2(LEG_2);
Servo L2M1_SERVO;
Servo L2M2_SERVO;
Servo L2M3_SERVO;


void setup() {
  motors[3].angleDegrees = 10;
  Serial.begin(9600);
  while (!Serial)
    delay(10);

  // Activate motors
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
  Serial.println("Relay activated");
  delay(1000);
  Serial.println("Relay engaged");


  // Setup motors and initialize kinematics library
  initMotorVals(motors);
  leg2.init(DEFAULT_X, DEFAULT_Y, DEFAULT_Z, motors);


  L2M1_SERVO.attach(23);
  L2M2_SERVO.attach(22);
  L2M3_SERVO.attach(15);

  // write primary servo positions
  L2M1_SERVO.writeMicroseconds(getPreparedAngles(motors, LEG_2, M1, MILLIS, STATIC));
  L2M2_SERVO.writeMicroseconds(getPreparedAngles(motors, LEG_2, M2, MILLIS, STATIC));
  L2M3_SERVO.writeMicroseconds(getPreparedAngles(motors, LEG_2, M3, MILLIS, STATIC));

  Serial.println();

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

    leg2.setFootEndpoint(amount, 45, DEFAULT_Z);


    leg2.updateDynamicFootPosition();

  L2M1_SERVO.writeMicroseconds(getPreparedAngles(motors, LEG_2, M1, MILLIS, DYNAMIC));
  L2M2_SERVO.writeMicroseconds(getPreparedAngles(motors, LEG_2, M2, MILLIS, DYNAMIC));
  L2M3_SERVO.writeMicroseconds(getPreparedAngles(motors, LEG_2, M3, MILLIS, DYNAMIC));

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