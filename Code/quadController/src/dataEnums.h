//   THIS SHOULD BE IDENTICAL ON BOTH DEVICES


// These are enums I'm using for each corresponding piece of data to make parsing data more readable
// i.e. the controller uses bit BUTTON_LEFT, and the quadruped reads BUTTON_LEFT, as opposed to 0 and 0. More easily read. 
// This file should be IDENTICAL for the devices receiving data from here.

typedef enum {
  BUTTON_LEFT = 0, BUTTON_RIGHT, JOYL_X, JOYL_Y, JOYR_X, JOYR_Y
} BIT_INPUT_TYPE;

typedef enum {
  JOYL_X_ANALOG = 1, JOYL_Y_ANALOG, JOYR_X_ANALOG, JOYR_Y_ANALOG
} BYTE_INPUT_TYPE;