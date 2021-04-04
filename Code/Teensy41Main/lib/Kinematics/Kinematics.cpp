#include "Kinematics.h"

#include <Arduino.h>

Kinematics::Kinematics(uint8_t legID, uint16_t motor1CalibOffset, uint16_t motor1StartPos, uint16_t motor2CalibOffset, uint16_t motor2StartPos, uint16_t motor3CalibOffset, uint16_t motor3StartPos) {
  _legID = legID;

  // motor defintions
  motor1.angleDegrees = motor1StartPos;
  motor1.previousDegrees = 360;         // 0 is a magic number. It just must be different than the start positions so that a call to updateDynamicPositions() works
  motor1.angleMicros = _degreesToMicros(motor1StartPos, motor1CalibOffset);
  motor1.calibOffset = motor1CalibOffset;

  motor2.angleDegrees = motor2StartPos;
  motor2.previousDegrees = 360;
  motor2.angleMicros = _degreesToMicros(motor2StartPos, motor2CalibOffset);
  motor2.calibOffset = motor2CalibOffset;

  motor3.angleDegrees = motor3StartPos;
  motor2.previousDegrees = 360;
  motor3.angleMicros = _degreesToMicros(motor3StartPos, motor3CalibOffset);
  motor3.calibOffset = motor3CalibOffset;


  // set initial dyanamic angles to start positions
  dynamicMotor1Angle.go(motor1StartPos);

  dynamicMotor2Angle.go(motor2StartPos);

  dynamicMotor3Angle.go(motor3StartPos);

};


// *****************Private Functions*****************

uint16_t Kinematics::_degreesToMicros(uint8_t inputDegrees, uint8_t calibOffset) {
  int microsecondsInput = ((DEGREES_TO_MICROS * inputDegrees) + 500 + calibOffset);    // 500 is a "magic number" of micros for the motors; before that they do nothing
  return microsecondsInput;
};


// *****************Public Functions*****************

void Kinematics::updateDynamicEndpoint() {

  uint16_t motor1AngleDelta = abs(motor1.angleDegrees - motor1.previousDegrees);
  uint16_t motor2AngleDelta = abs(motor2.angleDegrees - motor2.previousDegrees);
  uint16_t motor3AngleDelta = abs(motor3.angleDegrees - motor3.previousDegrees);
  uint16_t demandTime = lrint(MAX_SPEED_INVERSE * max(max(motor1AngleDelta, motor2AngleDelta), motor3AngleDelta));


    // determine whether motor angles have been updated i.e. new end angle, and update final positions accordingly
  if (motor1.previousDegrees != motor1.angleDegrees) {
    motor1.previousDegrees = motor1.angleDegrees;
    dynamicMotor1Angle.go(motor1.angleDegrees, demandTime, LINEAR, ONCEFORWARD);
  }
  if (motor2.previousDegrees != motor2.angleDegrees) {
    motor2.previousDegrees = motor2.angleDegrees;
    dynamicMotor2Angle.go(motor2.angleDegrees, demandTime, LINEAR, ONCEFORWARD);
  }
  if (motor3.previousDegrees != motor3.angleDegrees) {
    motor3.previousDegrees = motor3.angleDegrees;
    dynamicMotor3Angle.go(motor3.angleDegrees, demandTime, LINEAR, ONCEFORWARD);
  }
};



uint16_t Kinematics::getDyamicAngle(motorID motorID, unitType unit) {
  
  // update the dynamic endpoint in case there is a new demand endpoint
  updateDynamicEndpoint();

  if (motorID == M1) {
      if (unit == DEGREES)
        return dynamicMotor1Angle.update();
      else if (unit == MILLIS)
        return _degreesToMicros(dynamicMotor1Angle.update(), motor1.calibOffset);
  }
  else if (motorID == M2) {
    if (unit == DEGREES)
      return dynamicMotor2Angle.update();
    else if (unit == MILLIS)
      return _degreesToMicros(dynamicMotor2Angle.update(), motor2.calibOffset);
  }
  else if (motorID == M3) {
    if (unit == DEGREES)
      return dynamicMotor3Angle.update();
    else if (unit == MILLIS)
      return _degreesToMicros(dynamicMotor3Angle.update(), motor3.calibOffset);
  }
  return _degreesToMicros(90, 0);       // this should never happen, but if necessary, return 90 degrees (safe value for all motors)
};



void Kinematics::solveFtShldrLength(float demandFtShldr, float *demandAngle2, float *demandAngle3) {

    // Use the Law of Cosines to solve for the angles of motor 3 and convert to degrees
    float _demandAngle3 = acos( ( pow(demandFtShldr, 2) - pow(LIMB_2, 2) - pow(LIMB_3, 2) ) / (-2 * LIMB_2 * LIMB_3) ); // demand angle for position 3 (operated by M3)
    _demandAngle3 = ((_demandAngle3 * 180) / PI);   //convert to degrees

    // Use demandAngle3 to calculate for demandAngle2 (angle for M2)
    float _demandAngle2 = ((180 - _demandAngle3) / 2 );

    *demandAngle2 += _demandAngle2;
    *demandAngle3 += _demandAngle3;
};



void  Kinematics::solveForwardFootMove(int16_t inputX, int16_t inputZ, float *demandAngle2) {

  *demandAngle2 = ((atan((float)abs(inputX)/(float)abs(inputZ))*180) / PI);

  if (inputX > 0)
    *demandAngle2 *= -1;
};



void Kinematics::solveFootPosition(int16_t inputX, int16_t inputY, int16_t inputZ) {
  // float demandAngle1 = 0; // this angle isn't necessary (yet) for the currect calculations
  float demandAngle2 = 0;
  float demandAngle3 = 0;

  //calculate the demand shoulder-foot length and determine whether it is possible to achieve
  float demandFtShldrLength = sqrt(pow(abs(inputZ), 2) + pow(abs(inputX), 2));
  if (demandFtShldrLength > SHOULDER_FOOT_MAX) 
    demandFtShldrLength = SHOULDER_FOOT_MAX;
  else if (demandFtShldrLength < SHOULDER_FOOT_MIN)
    demandFtShldrLength = SHOULDER_FOOT_MIN;

  solveForwardFootMove(inputX, inputZ, &demandAngle2);

  solveFtShldrLength(demandFtShldrLength, &demandAngle2, &demandAngle3);

  // Round off demand angles
  demandAngle2 = lrint(demandAngle2);
  demandAngle3 = lrint(demandAngle3);

  // Calculate final demand angles suited to motors by applying necessary offsets
  demandAngle2 += M2_OFFSET;
  demandAngle3 = (M3_OFFSET - demandAngle3) + M3_OFFSET;

  // Constrain motor angles
  demandAngle2 = _applyConstraints(2, demandAngle2);
  demandAngle3 = _applyConstraints(3, demandAngle3);

  // Set live motor angles to the newly calculated ones

  // motor 2:
  motor2.angleDegrees = demandAngle2;
  motor2.angleMicros = _degreesToMicros(motor2.angleDegrees, motor2.calibOffset);

  // motor 3:
  motor3.angleDegrees = demandAngle3;
  motor3.angleMicros = _degreesToMicros(motor3.angleDegrees, motor3.calibOffset);

};



float Kinematics::_applyConstraints(uint8_t motor, float demandAngle) {
  if (motor == 2) {
    if (demandAngle > M2_MAX){
      return M2_MAX;
    }
    else if (demandAngle < M2_MIN){
      return M2_MIN;
    }
    else
      return demandAngle;
  }
  else if (motor == 3) {
    if (demandAngle > M3_MAX) {
      return M3_MAX;
    }
    else if (demandAngle < M3_MIN) {
      return M3_MIN;
    }
    else 
      return demandAngle;
  }
  else
    if (Serial)
      Serial.println("Motor argument in _apply constraints wrong, so constraints can't be applied! Terminating program."); //need a better way to report error
    while(1);         // terminate the program to avoid unpredictable movement (which could break stuff)
    return demandAngle;
};



bool Kinematics::printStatusString() {
  if (Serial) {
    char statusString[200];
    sprintf(statusString, "Motor 1: calibOffset = %d, angleDegrees = %d, angleMicros = %d", motor1.calibOffset, motor1.angleDegrees, motor1.angleMicros);
    Serial.println(statusString);
    sprintf(statusString, "Motor 2: calibOffset = %d, angleDegrees = %d, angleMicros = %d", motor2.calibOffset, motor2.angleDegrees, motor2.angleMicros);
    Serial.println(statusString);
    sprintf(statusString, "Motor 3: calibOffset = %d, angleDegrees = %d, angleMicros = %d", motor3.calibOffset, motor3.angleDegrees, motor3.angleMicros);
    Serial.println(statusString);
    Serial.println();
    
    return true;
  }
  else
    return false;
};

