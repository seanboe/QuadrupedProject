#ifndef _KINEMATICS_
#define _KINEMATICS_

#include <Arduino.h>
// #include <math.h>

#include "Config.h"

typedef struct {
  uint16_t angleDegrees;
  uint16_t angleMicros;
  uint8_t  calibOffset;
} motor;


class Kinematics {
  
  protected:

    uint16_t _shoulderFootLength;

    uint8_t _legID;

    uint16_t _degreesToMicros(uint8_t inputDegrees, uint8_t calibOffset);

    double _applyConstraints(uint8_t motor, double demandAngle);

  public:

    motor motor1;
    motor motor2;
    motor motor3;

    Kinematics(uint8_t legID, uint16_t motor1CalibOffset, uint8_t motor1StartPos, uint16_t motor2CalibOffset, uint8_t motor2StartPos, uint16_t motor3CalibOffset, uint8_t motor3StartPos);

    // calculates all relevant motor angles (the angles of motors 2 & 3) to achieve vertical translation
    void applyVerticalTranslation(uint16_t controllerInput);

    // calculates M2 angle offset to achieve a foward translation in the x direction
    void applyForwardTranslation(uint16_t controllerInput);

    bool printStatusString();
};

#endif