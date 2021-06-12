// Holds all the stuff specifically for the motors
// Define each motor's characteristicts here! Each motor has a struct object.
// The struct itself is defined in Kinematics.h to keep everything consistent. 
// Here is the definition for reference:
// typedef struct {
//   uint8_t controlPin;

//   // Angle/calculation stuff
//   int16_t angleDegrees;
//   int16_t previousDegrees;     // previous degrees since last call to updateDynamicPositions()
//   int16_t angleMicros;
//   int16_t dynamicDegrees;
//   int16_t dynamicMicros;

//   // Calibration
//   uint16_t calibOffset;         // This is an offset for calibration (to keep the motor accurate)
//   uint8_t maxPos;
//   uint8_t minPos;
//   uint16_t applicationOffset;   // This is an offset for putting the calculated angles in contex.
//                                 // It is likely that the zero positions of the motors isn't where
//                                 // calculations assumes it to be, so you need an offset to make 
//                                 // sure that the angle is correct relative to the motor's zero.
// } Motor;

#include <Arduino.h>

// The struct and various enums are defined inside here
#include <Kinematics.h>

#include <motorSetup.h>

// Called in setup
void initMotorVals(Motor _motors[]) {

  _motors[0] = L1M1;
  _motors[1] = L1M2;
  _motors[2] = L1M3;
  _motors[3] = L2M1;
  _motors[4] = L2M2;
  _motors[5] = L2M3;
  _motors[6] = L3M1;
  _motors[7] = L3M2;
  _motors[8] = L3M3;
  _motors[9] = L4M1;
  _motors[10] = L4M2;
  _motors[11] = L4M3;

  // Leg 2
    _motors[indexOfMotor(LEG_2, M1)].angleDegrees = 23; //TESTESTSETSETSETSLTKJA;FLKJAD;LFKJASD ;OFIJAWO;EIFJEIJKDMVNFBK JHUEIJKDMVBDSJHVUIG O
  _motors[indexOfMotor(LEG_2, M1)].controlPin = 23;
  _motors[indexOfMotor(LEG_2, M1)].calibOffset = 60;
  _motors[indexOfMotor(LEG_2, M1)].maxPos = 120;
  _motors[indexOfMotor(LEG_2, M1)].minPos = 45;
  _motors[indexOfMotor(LEG_2, M1)].applicationOffset = 90;

  _motors[indexOfMotor(LEG_2, M2)].controlPin = 22;
  _motors[indexOfMotor(LEG_2, M2)].calibOffset = 120;
  _motors[indexOfMotor(LEG_2, M2)].maxPos = 270;
  _motors[indexOfMotor(LEG_2, M2)].minPos = 0;
  _motors[indexOfMotor(LEG_2, M2)].applicationOffset = 90;

  _motors[indexOfMotor(LEG_2, M3)].controlPin = 15;
  _motors[indexOfMotor(LEG_2, M3)].calibOffset = 55;
  _motors[indexOfMotor(LEG_2, M3)].maxPos = 130;
  _motors[indexOfMotor(LEG_2, M3)].minPos = 45;
  _motors[indexOfMotor(LEG_2, M3)].applicationOffset = 90;
}



uint16_t indexOfMotor(LegID leg, MotorID motor) {
  return ((leg - 1) * MOTORS_PER_LEG + motor) - 1;
};

int16_t constrainAngles(Motor _motors[], LegID legID, MotorID motorID, int16_t demandAngle) {
  if (demandAngle > _motors[indexOfMotor(legID, motorID)].maxPos)
    return _motors[indexOfMotor(legID, motorID)].maxPos;
  else if (demandAngle < _motors[indexOfMotor(legID, motorID)].minPos)
    return _motors[indexOfMotor(legID, motorID)].minPos;
  return 0;
}

// This applies the offsets for the motors based on CONTEXT.
// The library calculates the angles for the all the JOINTS, 
// which isn't actually the angle each motor must achieve 
// since the 0 position of the motors is different for some.
// This is because they all have a defined zero-degrees 
// position and can turn 270 degrees but the motors must be 
// mirrored to each other on each side of the bot.

int16_t applyContextualOffset(Motor _motors[], LegID legID, MotorID motorID, int16_t demandAngle) {
  ContexType contexType = M1_standard;
  int16_t angle = 0;

  switch (legID) {

    case LEG_1:
      switch (motorID) {
        case M1: contexType = M1_mirrored; break;
        case M2: contexType = M2_mirrored; break;
        case M3: contexType = M3_mirrored; break;
      }
      break;

    case LEG_2:
      switch (motorID) {
        case M1: contexType = M1_standard; break;
        case M2: contexType = M2_standard; break;
        case M3: contexType = M3_standard; break;
      }
      break;

    case LEG_3:
      switch (motorID) {
        case M1: contexType = M1_mirrored; break;
        case M2: contexType = M2_standard; break;
        case M3: contexType = M3_standard; break;
      }
      break;

    case LEG_4:
      switch (motorID) {
        case M1: contexType = M1_standard; break;
        case M2: contexType = M2_mirrored; break;
        case M3: contexType = M3_mirrored; break;
      }
      break;
  }

  switch (contexType) {

    case M1_standard: angle =  _motors[indexOfMotor(legID, motorID)].applicationOffset + demandAngle; break;
    case M1_mirrored: angle =  _motors[indexOfMotor(legID, motorID)].applicationOffset - demandAngle; break;

    case M2_standard: angle =  _motors[indexOfMotor(legID, motorID)].applicationOffset + demandAngle; break;
    case M2_mirrored: angle =  90 - demandAngle; break;

    case M3_standard: angle = (2 * _motors[indexOfMotor(legID, motorID)].applicationOffset) - demandAngle; break;
    case M3_mirrored: angle =  demandAngle; break;
  }

  angle = constrainAngles(_motors, legID, motorID, angle);
  return angle;
};