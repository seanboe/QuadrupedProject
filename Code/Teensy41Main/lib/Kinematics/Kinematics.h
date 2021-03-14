#ifndef _KINEMATICS_
#define _KINEMATICS_

#include <Arduino.h>

#include "Config.h"

#include <Ramp.h>

typedef struct {
  uint16_t angleDegrees;
  uint16_t previousDegrees;     // previous degrees since last call to updateDynamicPositions()
  uint16_t angleMicros;
  uint16_t calibOffset;
} motor;


class Kinematics {
  
  private:

    uint16_t _shoulderFootLength;

    uint8_t _legID;

    uint16_t _degreesToMicros(uint8_t inputDegrees, uint8_t calibOffset);

    double _applyConstraints(uint8_t motor, double demandAngle);

  public:

    motor motor1;
    motor motor2;
    motor motor3;

    rampInt dynamicMotor1Angle;
    rampInt dynamicMotor2Angle;
    rampInt dynamicMotor3Angle;

    Kinematics(uint8_t legID, uint16_t motor1CalibOffset, uint16_t motor1StartPos, uint16_t motor2CalibOffset, uint16_t motor2StartPos, uint16_t motor3CalibOffset, uint16_t motor3StartPos);

    // This updates the interpolation objects to make sure that the motor angles are being interpolated correctly even with updated demands
    void updateDynamicEndpoint();

    uint16_t getDyamicAngle(motorID motorID, unitType unit);

    // calculates all relevant motor angles (the angles of motors 2 & 3) to achieve vertical translation
    void solveFtShldrLength(uint16_t controllerInput);

    // calculates M2 angle offset to achieve a foward translation in the x direction
    void applyForwardTranslation(uint16_t controllerInput);

    bool printStatusString();
};

#endif