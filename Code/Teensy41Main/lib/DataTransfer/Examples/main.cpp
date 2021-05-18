#include <Arduino.h>

#include <DataTransfer.h>

// first, instantiate a radio object
RH_RF69 rf69(RFM69_CS, RFM69_INT);
//then, pass this object to the DataTransfer constructor
Transfer dataBuffer(&rf69, 4);    // My setup requires 2 bytes for determining whether there is relevant data for each respective button/sensor. The other 2 bytes are for holding the analog values of the joysticks.

void setup() {

  Serial.begin(9600);
  while (!Serial);
  dataBuffer.init();
  Serial.println("dklfjlsdkjf");
}

void loop() {

  if (dataBuffer.receiveData());
    Serial.println("received data");
  // dataBuffer.readData();

}

