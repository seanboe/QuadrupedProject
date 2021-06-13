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

Kinematics leg1(LEG_1);
Kinematics leg2(LEG_2);
Kinematics leg3(LEG_3);
Kinematics leg4(LEG_4);

Servo L1M1_SERVO;
Servo L1M2_SERVO;
Servo L1M3_SERVO;

Servo L2M1_SERVO;
Servo L2M2_SERVO;
Servo L2M3_SERVO;

Servo L3M1_SERVO;
Servo L3M2_SERVO;
Servo L3M3_SERVO;

Servo L4M1_SERVO;
Servo L4M2_SERVO;
Servo L4M3_SERVO;

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
  leg1.init(DEFAULT_X, DEFAULT_Y, DEFAULT_Z, motors);
  leg2.init(DEFAULT_X, DEFAULT_Y, DEFAULT_Z, motors);
  leg3.init(DEFAULT_X, DEFAULT_Y, DEFAULT_Z, motors);
  leg4.init(DEFAULT_X, DEFAULT_Y, DEFAULT_Z, motors);

  L1M1_SERVO.attach(motors[indexOfMotor(LEG_1, M1)].controlPin);
  L1M2_SERVO.attach(motors[indexOfMotor(LEG_1, M2)].controlPin);
  L1M3_SERVO.attach(motors[indexOfMotor(LEG_1, M3)].controlPin);

  L2M1_SERVO.attach(motors[indexOfMotor(LEG_2, M1)].controlPin);
  L2M2_SERVO.attach(motors[indexOfMotor(LEG_2, M2)].controlPin);
  L2M3_SERVO.attach(motors[indexOfMotor(LEG_2, M3)].controlPin);

  L3M1_SERVO.attach(motors[indexOfMotor(LEG_3, M1)].controlPin);
  L3M2_SERVO.attach(motors[indexOfMotor(LEG_3, M2)].controlPin);
  L3M3_SERVO.attach(motors[indexOfMotor(LEG_3, M3)].controlPin);

  L4M1_SERVO.attach(motors[indexOfMotor(LEG_4, M1)].controlPin);
  L4M2_SERVO.attach(motors[indexOfMotor(LEG_4, M2)].controlPin);
  L4M3_SERVO.attach(motors[indexOfMotor(LEG_4, M3)].controlPin);

  // write primary servo positions
  L1M1_SERVO.writeMicroseconds(getPreparedAngles(motors, LEG_1, M1, MILLIS, STATIC));
  L1M2_SERVO.writeMicroseconds(getPreparedAngles(motors, LEG_1, M2, MILLIS, STATIC));
  L1M3_SERVO.writeMicroseconds(getPreparedAngles(motors, LEG_1, M3, MILLIS, STATIC));

  L2M1_SERVO.writeMicroseconds(getPreparedAngles(motors, LEG_2, M1, MILLIS, STATIC));
  L2M2_SERVO.writeMicroseconds(getPreparedAngles(motors, LEG_2, M2, MILLIS, STATIC));
  L2M3_SERVO.writeMicroseconds(getPreparedAngles(motors, LEG_2, M3, MILLIS, STATIC));

  L3M1_SERVO.writeMicroseconds(getPreparedAngles(motors, LEG_3, M1, MILLIS, STATIC));
  L3M2_SERVO.writeMicroseconds(getPreparedAngles(motors, LEG_3, M2, MILLIS, STATIC));
  L3M3_SERVO.writeMicroseconds(getPreparedAngles(motors, LEG_3, M3, MILLIS, STATIC));

  L4M1_SERVO.writeMicroseconds(getPreparedAngles(motors, LEG_4, M1, MILLIS, STATIC));
  L4M2_SERVO.writeMicroseconds(getPreparedAngles(motors, LEG_4, M2, MILLIS, STATIC));
  L4M3_SERVO.writeMicroseconds(getPreparedAngles(motors, LEG_4, M3, MILLIS, STATIC));

  Serial.println();

  // Serial.println(applyContextualOffset(motors, LEG_1, M1, motors[indexOfMotor(LEG_1, M1)].angleDegrees));
  // Serial.println(applyContextualOffset(motors, LEG_1, M2, motors[indexOfMotor(LEG_1, M2)].angleDegrees));
  // Serial.println(applyContextualOffset(motors, LEG_1, M3, motors[indexOfMotor(LEG_1, M3)].angleDegrees));

  // Serial.println(applyContextualOffset(motors, LEG_2, M1, motors[indexOfMotor(LEG_2, M1)].angleDegrees));
  // Serial.println(applyContextualOffset(motors, LEG_2, M2, motors[indexOfMotor(LEG_2, M2)].angleDegrees));
  // Serial.println(applyContextualOffset(motors, LEG_2, M3, motors[indexOfMotor(LEG_2, M3)].angleDegrees));

  delay(3000);

}

void loop() {

  // calculateGait();

  static int amount = 177;
  if (Serial.available()) {
    amount = Serial.parseInt();
    // leg2.solveFootPosition(0, amount, 177);        // below
    // leg2.setFootEndpoint(amount, 45, DEFAULT_Z);
  } 

  leg1.setFootEndpoint(DEFAULT_X-25, DEFAULT_Y, amount);
  leg2.setFootEndpoint(DEFAULT_X-25, DEFAULT_Y, amount);
  leg3.setFootEndpoint(DEFAULT_X-25, DEFAULT_Y, amount);
  leg4.setFootEndpoint(DEFAULT_X-25, DEFAULT_Y, amount);

  leg1.updateDynamicFootPosition();
  leg2.updateDynamicFootPosition();
  leg3.updateDynamicFootPosition();
  leg4.updateDynamicFootPosition();

  L1M1_SERVO.writeMicroseconds(getPreparedAngles(motors, LEG_1, M1, MILLIS, DYNAMIC));
  L1M2_SERVO.writeMicroseconds(getPreparedAngles(motors, LEG_1, M2, MILLIS, DYNAMIC));
  L1M3_SERVO.writeMicroseconds(getPreparedAngles(motors, LEG_1, M3, MILLIS, DYNAMIC));

  L2M1_SERVO.writeMicroseconds(getPreparedAngles(motors, LEG_2, M1, MILLIS, DYNAMIC));
  L2M2_SERVO.writeMicroseconds(getPreparedAngles(motors, LEG_2, M2, MILLIS, DYNAMIC));
  L2M3_SERVO.writeMicroseconds(getPreparedAngles(motors, LEG_2, M3, MILLIS, DYNAMIC));

  L3M1_SERVO.writeMicroseconds(getPreparedAngles(motors, LEG_3, M1, MILLIS, DYNAMIC));
  L3M2_SERVO.writeMicroseconds(getPreparedAngles(motors, LEG_3, M2, MILLIS, DYNAMIC));
  L3M3_SERVO.writeMicroseconds(getPreparedAngles(motors, LEG_3, M3, MILLIS, DYNAMIC));

  L4M1_SERVO.writeMicroseconds(getPreparedAngles(motors, LEG_4, M1, MILLIS, DYNAMIC));
  L4M2_SERVO.writeMicroseconds(getPreparedAngles(motors, LEG_4, M2, MILLIS, DYNAMIC));
  L4M3_SERVO.writeMicroseconds(getPreparedAngles(motors, LEG_4, M3, MILLIS, DYNAMIC));
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