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
#define TOLERANCE       2
#define JOYSTICK_CENTER 128 // mapped value; 0-1023 to 0-255

//Radio Encryption Key
const byte radioKey[6] = "00001";

Transfer dataBuffer(21, 20, BUFFER_SIZE);

uint8_t filterAnalog(uint8_t value, uint8_t tolerance);


void setup() {

  // Pin setup
  pinMode(JOYL_X_PIN, INPUT);
  pinMode(JOYL_Y_PIN, INPUT);
  pinMode(JOYR_X_PIN, INPUT);
  pinMode(JOYR_Y_PIN, INPUT);
  pinMode(BUTTON_LEFT_PIN, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT_PIN, INPUT_PULLUP);

  Serial.begin(9600);
  dataBuffer.init(0, TRANSMITTER, radioKey);

}

void loop() {

  // I'm organizing my data buffer in the first byte as such: BUTTON_LEFT, BUTTON_RIGHT, JOYL_X, JOYL_Y, JOYR_X, JOYR_Y reading from RIGHT TO LEFT
  //(if each button/joystick is active, then the bit will read 1)

  dataBuffer.write(BIT, BUTTON_LEFT, (!digitalRead(BUTTON_LEFT_PIN) ? 1 : 0));
  dataBuffer.write(BIT, BUTTON_RIGHT, (!digitalRead(BUTTON_RIGHT_PIN) ? 1 : 0));
  dataBuffer.write(BIT, JOYL_X, ((filterAnalog(map(analogRead(JOYL_X_PIN), 0, 1023, 255, 0), TOLERANCE) != JOYSTICK_CENTER) ? 1 : 0 ));
  dataBuffer.write(BIT, JOYL_Y, ((filterAnalog(map(analogRead(JOYL_Y_PIN), 0, 1023, 255, 0), TOLERANCE) != JOYSTICK_CENTER) ? 1 : 0 ));
  dataBuffer.write(BIT, JOYR_X, ((filterAnalog(map(analogRead(JOYR_X_PIN), 0, 1023, 255, 0), TOLERANCE) != JOYSTICK_CENTER) ? 1 : 0 ));
  dataBuffer.write(BIT, JOYR_Y, ((filterAnalog(map(analogRead(JOYR_Y_PIN), 0, 1023, 255, 0), TOLERANCE) != JOYSTICK_CENTER) ? 1 : 0 ));


  // The next 4 bytes are for holding analog joystick data (in that order) if their respective bits are high

  dataBuffer.write(BYTE, JOYL_X_ANALOG, filterAnalog(map(analogRead(JOYL_X_PIN), 0, 1023, 255, 0), TOLERANCE));
  dataBuffer.write(BYTE, JOYL_Y_ANALOG, filterAnalog(map(analogRead(JOYL_Y_PIN), 0, 1023, 255, 0), TOLERANCE));
  dataBuffer.write(BYTE, JOYR_X_ANALOG, filterAnalog(map(analogRead(JOYR_X_PIN), 0, 1023, 255, 0), TOLERANCE));
  dataBuffer.write(BYTE, JOYR_Y_ANALOG, filterAnalog(map(analogRead(JOYR_Y_PIN), 0, 1023, 255, 0), TOLERANCE));

  dataBuffer.send();

  delay(100);

}

uint8_t filterAnalog(uint8_t value, uint8_t tolerance) {
  if (abs(value - JOYSTICK_CENTER) > TOLERANCE)
    return value;
  else 
    return JOYSTICK_CENTER;
}
