#ifndef _CONFIG_
#define _CONFIG_

// degrees to microseconds sale factor (determined experimentally, MUST be the same for every motor)
#define DEGREES_TO_MICROS 7.5

// Maximum motor speed; millis per 180 degrees factor; NOT DEGREES PER MILLIS I.E. SPEED (determined experimentally) this is 0.6 sec / 180 degrees (actual value is 0.52 sec)
#define MAX_SPEED_INVERSE 4

typedef enum {
  DEGREES, MILLIS
} unitType;


// leg numbering
#define LEG_1 1
#define LEG_2 2
#define LEG_3 3
#define LEG_4 4


// motor enums
typedef enum {
  M1, M2, M3
} motorID;



//********* robot hardware constraints **********

// limb length in millimeters
#define LIMB_1  30
#define LIMB_2  125
#define LIMB_3  125


// shoulder to foot length constraints in mm - determined using max/min angles for motors 2 & 3
#define SHOULDER_FOOT_MAX 230
#define SHOULDER_FOOT_MIN 100


//********* Constants for legs 2 & 3 **********************
// motor angular offsets (0 position is different than what is assumed for triangle calcs)
#define M1_OFFSET 90
#define M2_OFFSET 90
#define M3_OFFSET 0


// motor angular limits (degrees)
#define M1_MAX 135
#define M1_MIN 45
#define M2_MAX 270
#define M2_MIN 0
#define M3_MAX 130     //130
#define M3_MIN 45


#define M3_DEFAULT_ANGLE 90     // this motor has a default, because it's movement isn't direct (desired movement = 90 - demandMovement + 90)

#endif