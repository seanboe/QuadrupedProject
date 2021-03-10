#ifndef _CONFIG_
#define _CONFIG_

// leg numbering
#define LEG_1 1
#define LEG_2 2
#define LEG_3 3
#define LEG_4 4


// motor numbering
#define M1 1
#define M2 2
#define M3 3


// limb length in millimeters
#define LIMB_1  30
#define LIMB_2  125
#define LIMB_3  125


// shoulder to foot length constraints in mm - determined using max/min angles for motors 2 & 3
#define SHOULDER_FOOT_MAX 230
#define SHOULDER_FOOT_MIN 100


// motor angular offsets (0 position is different than what is assumed for triangle calcs)
#define M1_OFFSET 90
#define M2_OFFSET 90
#define M3_OFFSET 0


// motor angular limits
#define M1_MAX 135
#define M1_MIN 45
#define M2_MAX 270
#define M2_MIN 0
#define M3_MAX 130
#define M3_MIN 45


#define M3_DEFAULT_ANGLE 90     // this motor has a default, because it's movement isn't direct (desired movement = 90 - demandMovement + 90)


// degrees to microseconds sale factor (determined experimentally, same for every motor)
#define DEGREES_TO_MICROS 7.5



#endif