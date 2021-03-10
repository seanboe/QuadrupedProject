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

  public:

    motor motor1;
    motor motor2;
    motor motor3;

    Kinematics(uint8_t legID, uint16_t motor1CalibOffset, uint8_t motor1StartPos, uint16_t motor2CalibOffset, uint8_t motor2StartPos, uint16_t motor3CalibOffset, uint8_t motor3StartPos);

    // calculates all relevant motor angles (the angles of motors 2 & 3) to achieve veritcal translation
    void applyVerticalTranslation(uint8_t controllerInput);

    bool printStatusString();
};

#endif