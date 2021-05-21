#include <Arduino.h>

#include <DataTransfer.h>

// You need a key that is the same on both devices (RX/TX)
uint8_t key[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};

// first, instantiate a radio object
RH_RF69 rf69(RFM69_CS, RFM69_INT);
//then, pass this object to the DataTransfer constructor
Transfer dataBuffer(&rf69, BUFFER_SIZE, key);   // this example involves the transfer of 4 bytes, which can be changed by modifying BUFFER_SIZE in Config.h

void setup() {

  Serial.begin(9600);
  // while (!Serial);

  //initialize the data buffer. This also completes the setup of the radio. 
  dataBuffer.init();


}

void loop() {

  // Time to write data! You can choose to either write a bit or a byte. The buffer syntax is up to you, so stay organized!
  // This example sets 4 in byte 0 to 1. Don't forget that bits are read right to left; the following function does this: 00010000
  dataBuffer.write(BIT, 4);

  // This writes the value 100 to byte 2 in the array
  dataBuffer.write(BYTE, 2, 100);

  // Finally, send your data! By default, the buffer will be cleared (HIGHLY RECOMMENDED), but simply pass false to the function to overwrite this.
  dataBuffer.send();

}

// future edits: change name writeData to write
// change name sendData to send
// make the value term in write be optional