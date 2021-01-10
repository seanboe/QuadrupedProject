#ifndef _COPROCESSOR_TALK_
#define _COPROCESSOR_TALK_

#include <Arduino.h>
#include <Wire.h>

// Definitions for messages/modes/stuff that needs to be sent to Teensy LC
#define SET_MODE_0 0

// Definitions for buttons/hats on controller
#define LEFT_X 1
#define LEFT_Y 2
#define RIGHT_X 3
#define RIGHT_Y 4
#define L1 5
#define L2 6
#define R1 7
#define R2 8
#define DOWN 9
#define LEFT 10
#define UP 11
#define RIGHT 12
#define CROSS 13
#define SQUARE 14
#define TRIANGLE 15
#define CIRCLE 16

#define JOYSTICK_OFFSET 10          // Offset for analog data (especially joysticks) to remove sensitivity to noise
#define JOYSTICK_MIDDLE_POS 130         // average of what is read when the joysticks aren't being moved.

#define _DEFAULT_DATA_INDEX_COUNT 20          // default byte size of data; I've planned for 17, but 20 is just in case (for now)

class CoprocessorTalk {
  private:
    uint8_t _currentDataArray[_DEFAULT_DATA_INDEX_COUNT] = {0,JOYSTICK_MIDDLE_POS,JOYSTICK_MIDDLE_POS,JOYSTICK_MIDDLE_POS,JOYSTICK_MIDDLE_POS,0,0,0,0,0,0,0,0,0,0,0,0}; /* the 130s are a dirty hack to make sure that on the first 
    .retrieveData, .controllerInputChanged won't trigger because default joystick data is read all of a sudden. If the controller
    data was all 0s instead, then when data is retrived and the joysticks read somewhere around 130 (they are in the middle)
    nothing is triggered. 130, because that's where the middle tends to be (in my observations). */
    uint8_t _previousDataArray[_DEFAULT_DATA_INDEX_COUNT] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    uint8_t _slaveAddress;
    uint8_t _dataIndexCount;          // expected number of indexes for internal array; set in the constructor and used to request bytes
    bool _serialFeedbackOK;
    int _isTypeAnalog(uint8_t inputType);         // returns 0 if not analog, 1 if joystick, 2 if trigger (R2, L2)
  public:
    CoprocessorTalk(int slaveAddress, int dataIndexCount, bool serialFeedbackOK);  // Wire.begin() is NOT DONE IN THIS CONSTRUCTOR, INSTEAD IT MUST BE IN THE MAIN PROGRAM
    bool retrieveData(void);
    bool controllerConnected(void);
    int getControllerInput(int inputType);                // returns state of all controller inputs
    bool controllerInputChanged(uint8_t intputType);          // whether an input changed; returns true once, when there is a change. Probably most useful for buttons, but may work with all inputs
    void write(uint8_t message);
};

#endif