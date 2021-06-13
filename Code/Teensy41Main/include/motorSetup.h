#ifndef _MOTOR_SETUP_
#define _MOTOR_SETUP_

#include <Arduino.h>

// The motor struct definition and various enums are defined inside here
#include <Kinematics.h>

// degrees to microseconds sale factor (determined experimentally, MUST be the same for every motor)
#define DEGREES_TO_MICROS 7.5

//Leg 1:
extern Motor L1M1;
extern Motor L1M2;
extern Motor L1M3;

//Leg 2:
extern Motor L2M1;
extern Motor L2M2;
extern Motor L2M3;

//Leg 2:
extern Motor L3M1;
extern Motor L3M2;
extern Motor L3M3;

//Leg 2:
extern Motor L4M1;
extern Motor L4M2;
extern Motor L4M3;

extern Motor motors[ROBOT_LEG_COUNT * MOTORS_PER_LEG];

typedef enum {
  M1_standard, M1_mirrored,
  M2_standard, M2_mirrored,
  M3_standard, M3_mirrored
} ContexType;

typedef enum {
  DEGREES, MILLIS
} unitType;

typedef enum {
  DYNAMIC, STATIC
} ActivityType;

// Function definitions
void initMotorVals(Motor _motors[]);
uint16_t indexOfMotor(LegID leg, MotorID motor);
int16_t constrainAngle(Motor _motors[], LegID legID, MotorID motorID, int16_t demandAngle);
int16_t applyContextualOffset(Motor _motors[], LegID legID, MotorID motorID, int16_t demandAngle);
uint16_t degreesToMicros(uint8_t inputDegrees, uint8_t calibOffset);
int16_t getPreparedAngles(Motor _motors[], LegID legID, MotorID motorID, unitType angleUnitType, ActivityType activityType);


#endif