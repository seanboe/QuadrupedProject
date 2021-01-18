#include "Kinematics.h"

#include <Arduino.h>

Kinematics::Kinematics(uint8_t legID, uint16_t motor1CalibOffset, uint8_t motor1StartPos, uint16_t motor2CalibOffset, uint8_t motor2StartPos, uint16_t motor3CalibOffset, uint8_t motor3StartPos) {
  _legID = legID;
  _shoulderFootLength = sqrt( (LIMB_2^2) + (LIMB_3^2) );   // default leg length

  // motor defintions
  motor1.angleDegrees = motor1StartPos;
  motor1.angleMicros = _degreesToMicros(motor1StartPos, motor1CalibOffset);
  motor1.calibOffset = motor1CalibOffset;

  motor2.angleDegrees = motor2StartPos;
  motor2.angleMicros = _degreesToMicros(motor2StartPos, motor2CalibOffset);
  motor2.calibOffset = motor2CalibOffset;

  motor3.angleDegrees = motor3StartPos;
  motor3.angleMicros = _degreesToMicros(motor3StartPos, motor3CalibOffset);
  motor3.calibOffset = motor3CalibOffset;

};


// *****************Private Functions*****************

uint16_t Kinematics::_degreesToMicros(uint8_t inputDegrees, uint8_t calibOffset) {
  int microsecondsInput = ((DEGREES_TO_MICROS * inputDegrees) + 500 + calibOffset);    // 500 is a "magic number" of micros for the motors; before that they do nothing
  return microsecondsInput;
};


// *****************Public Functions*****************

void Kinematics::applyVerticalTranslation(uint8_t controllerInput) {
  
  if ((_legID == LEG_2) || (_legID == LEG_3)) {

    // Step 1: Map input to demand shoulder-foot length in cm
    uint16_t demandShoulderToFoot = map(controllerInput, 0, 255, SHOULDER_FOOT_MIN, SHOULDER_FOOT_MAX);


    // Step 2: use the Law of Cosines to solve for the angles of motor 3 and convert to degrees
    double demandAngle3 = acos( ( pow(demandShoulderToFoot, 2) - pow(LIMB_2, 2) - pow(LIMB_3, 2) ) / (-2 * LIMB_2 * LIMB_3) ); // demand angle for position 3 (operated by M3)
    demandAngle3 = lrint( (demandAngle3 * 180) / PI);

    // Step 3: use demandAngle3 to calculate for demandAngle2 (angle for M2)

    double demandAngle2 = lrint( (180 - demandAngle3) / 2 );


    // Step 4: calculate final demand angles suited to motors

    demandAngle2 += M2_OFFSET;
    demandAngle3 = (M3_DEFAULT_ANGLE - demandAngle3) + M3_DEFAULT_ANGLE + M3_OFFSET;


    // Step 5: apply motor angular constraints

    // motor 2:
    if (demandAngle2 > M2_MAX)
      demandAngle2 = M2_MAX;
    else if (demandAngle2 < M2_MIN)
      demandAngle2 = M2_MIN;
    
    // motor 3:
    if (demandAngle3 > M3_MAX)
      demandAngle3 = M3_MAX;
    else if (demandAngle3 < M3_MIN) 
      demandAngle3 = M3_MIN;

    // Step 6: set live motor angles to the newly calculated ones

    // motor 2:
    motor2.angleDegrees = demandAngle2;
    motor2.angleMicros = _degreesToMicros(motor2.angleDegrees, motor2.calibOffset);

    // motor 3:
    motor3.angleDegrees = demandAngle3;
    motor3.angleMicros = _degreesToMicros(motor3.angleDegrees, motor3.calibOffset);

  }

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

