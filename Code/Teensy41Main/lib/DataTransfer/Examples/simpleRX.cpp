#include <Arduino.h>

#include <DataTransfer.h>

// You need a key that is the same on both devices (RX/TX)
uint8_t key[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};

// first, instantiate a radio object
RH_RF69 rf69(RFM69_CS, RFM69_INT);
//then, pass this object to the DataTransfer constructor
Transfer dataBuffer(&rf69, BUFFER_SIZE, key);   // this example involves the transfer of 4 bytes, which can be changed by modifying BUFFER_SIZE in Config.h

// This function is purely for you to inspect the buffer in binary if you wish.
void printBinary(byte inByte);

void setup() {

  Serial.begin(9600);
  // while (!Serial);

  //initialize the data buffer. This also completes the setup of the radio. 
  dataBuffer.init();

}

void loop() {

  if (dataBuffer.receive()) {
    Serial.println(dataBuffer.read(BIT, 4));
    Serial.println(dataBuffer.read(BYTE, 2));
  }

  delay(1000);

  // if you want to see the entire data buffer (in binary), then uncomment this:
  // if (dataBuffer.receive()) {
  //   for (int x = 0; x < 4; x++) {
  //     printBinary(dataBuffer._dataArray[x]);
  //   }
  //   Serial.println();
  // }

}

void printBinary(byte inByte)
{
  for (int b = 7; b >= 0; b--)
  {
    Serial.print(bitRead(inByte, b));
  }
  Serial.print("   ");
}