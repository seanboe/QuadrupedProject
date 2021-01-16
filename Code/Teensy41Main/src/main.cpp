#include <Arduino.h>

#include <CoprocessorTalk.h>
#include <Wire.h>

#define SLAVE_ADDRESS 8
#define DATA_INDEX_COUNT 17

CoprocessorTalk teensyLC(SLAVE_ADDRESS, DATA_INDEX_COUNT, true);

void setup() {
  Serial.begin(9600);
  while (!Serial)
  Wire.begin();

  while(!teensyLC.retrieveData());            // halt while controller isn't connected
  teensyLC.write(SET_MODE_0);      

  teensyLC.joinTriggers();    
}

void loop() {
  while(!teensyLC.retrieveData());            // halt while controller isn't connected
  // for (int x = 0; x < DATA_INDEX_COUNT; x++) {
  //   Serial.println(teensyLC.getControllerInput(x));
  // }
  Serial.println(teensyLC.getControllerInput(TRIGGERS));          // left trigger (going down) not working
}