#include <CoprocessorTalk.h>
#include <Wire.h>

#define SLAVE_ADDRESS 8
#define DATA_INDEX_COUNT 17

CoprocessorTalk teensyLC(SLAVE_ADDRESS, DATA_INDEX_COUNT, true);

//#define MODE_COUNT 4
//int robotMode = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();
}

void loop() {
  while(!teensyLC.retrieveData());            // halt while controller isn't connected
  for (int x = 1; x < DATA_INDEX_COUNT; x++) {
    Serial.println(teensyLC.getControllerInput(x));
  }

  /* uncomment for mode switching - don't forget to create a variable int robotMode = 0; 
  switchModes();
  Serial.println(robotMode); */
}

/* uncomment for mode switching - don't forget to create a variable int robotMode = 0; 
void switchModes() {
  int teensyL1 = teensyLC.getControllerInput(L1);
  int teensyR1 = teensyLC.getControllerInput(R1);
  
  if ((teensyL1 == 1) && (teensyLC.controllerInputChanged(L1))) {
    if (robotMode != 0)
      robotMode--;
  }
  
  if ((teensyR1 == 1) && (teensyLC.controllerInputChanged(R1))) {
    if (robotMode != MODE_COUNT-1)
      robotMode++;
  }
} */