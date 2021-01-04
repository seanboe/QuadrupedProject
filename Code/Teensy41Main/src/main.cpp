#include <Arduino.h>

#include <CoprocessorTalk.h>
#include <Wire.h>

#define SLAVE_ADDRESS 8
#define DATA_INDEX_COUNT 17

CoprocessorTalk teensyLC(SLAVE_ADDRESS, DATA_INDEX_COUNT, true);

void setup() {
  Serial.begin(9600);
  Wire.begin();
}

void loop() {
  while(!teensyLC.retrieveData());            // halt while controller isn't connected
  for (int x = 1; x < DATA_INDEX_COUNT; x++) {
    Serial.println(teensyLC.getControllerInput(x));
  }
}

// #include <Arduino.h>

// #include <Wire.h>

// #define SLAVE_ADDRESS 8

// #define I2CBYTES 17

// volatile uint8_t array[I2CBYTES] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

// void setup() {
//   Serial.begin(9600);
//   Wire.begin();
// }

// void loop() {
//   Wire.requestFrom(SLAVE_ADDRESS, I2CBYTES);

//   while (Wire.available() == I2CBYTES) {
//     for (int x = 0; x < I2CBYTES; x++) {
//       array[x] = Wire.read();
//       Serial.println(array[x]);
//     }

//     if (array[0] == 0)
//       Serial.println("NULL received; data not yet acquired on slave, or controller not yet connected");

//     Serial.println("transimission completed");
//   }

//   delay(50);

// }