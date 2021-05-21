#include <Arduino.h>

#include <DataTransfer.h>

#include "dataEnums.h"

// Joystick pins
#define JOYL_X_PIN  15
#define JOYL_Y_PIN  14
#define JOYR_X_PIN  16
#define JOYR_Y_PIN  17

// Button pins
#define BUTTON_LEFT_PIN   23
#define BUTTON_RIGHT_PIN  22

// Filter parameters
#define TOLERANCE 2
#define JOYSTICK_ZERO 128   // mapped value; 0-1023 to 0-255

//Radio Encryption Key
uint8_t key[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};


// Instantiate Radio Object
RH_RF69 rf69(RFM69_CS, RFM69_INT);
// Instantiate Buffer Object
Transfer dataBuffer(&rf69, BUFFER_SIZE, key);

uint8_t filterAnalog(uint8_t value, int tolerance);


void setup() {

  // Pin setup
  pinMode(JOYL_X_PIN, INPUT);
  pinMode(JOYL_Y_PIN, INPUT);
  pinMode(JOYR_X_PIN, INPUT);
  pinMode(JOYR_Y_PIN, INPUT);
  pinMode(BUTTON_LEFT_PIN, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT_PIN, INPUT_PULLUP);

  Serial.begin(9600);
  dataBuffer.init();

}

void loop() {

  // I'm organizing my data buffer in the first byte as such: BUTTON_LEFT, BUTTON_RIGHT, JOYL_X, JOYL_Y, JOYR_X, JOYR_Y reading from RIGHT TO LEFT
  //(if each button/joystick is active, then the bit will read 1)

  dataBuffer.write(BIT, BUTTON_LEFT, (!digitalRead(BUTTON_LEFT_PIN) ? 1 : 0));
  dataBuffer.write(BIT, BUTTON_RIGHT, (!digitalRead(BUTTON_RIGHT_PIN) ? 1 : 0));
  dataBuffer.write(BIT, JOYL_X, ((filterAnalog(map(analogRead(JOYL_X_PIN), 0, 1023, 0, 255), TOLERANCE) != 128) ? 1 : 0 ));
  dataBuffer.write(BIT, JOYL_Y, ((filterAnalog(map(analogRead(JOYL_Y_PIN), 0, 1023, 0, 255), TOLERANCE) != 128) ? 1 : 0 ));
  dataBuffer.write(BIT, JOYR_X, ((filterAnalog(map(analogRead(JOYR_X_PIN), 0, 1023, 0, 255), TOLERANCE) != 128) ? 1 : 0 ));
  dataBuffer.write(BIT, JOYR_Y, ((filterAnalog(map(analogRead(JOYR_Y_PIN), 0, 1023, 0, 255), TOLERANCE) != 128) ? 1 : 0 ));


  // The next 4 bytes are for holding analog joystick data (in that order) if their respective bits are high

  dataBuffer.write(BYTE, JOYL_X_ANALOG, filterAnalog(map(analogRead(JOYL_X_PIN), 0, 1023, 0, 255), TOLERANCE));
  dataBuffer.write(BYTE, JOYL_Y_ANALOG, filterAnalog(map(analogRead(JOYL_Y_PIN), 0, 1023, 0, 255), TOLERANCE));
  dataBuffer.write(BYTE, JOYR_X_ANALOG, filterAnalog(map(analogRead(JOYR_X_PIN), 0, 1023, 0, 255), TOLERANCE));
  dataBuffer.write(BYTE, JOYR_Y_ANALOG, filterAnalog(map(analogRead(JOYR_Y_PIN), 0, 1023, 0, 255), TOLERANCE));

}

uint8_t filterAnalog(uint8_t value, int tolerance) {
  if (abs(value - JOYSTICK_ZERO) > TOLERANCE)
    return value;
  else 
    return JOYSTICK_ZERO;
}
