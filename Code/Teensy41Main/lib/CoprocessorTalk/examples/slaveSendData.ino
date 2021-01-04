#include <Arduino.h>

#include <PS3BT.h>
#include <usbhub.h>

#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

#include <Wire.h>
#define SLAVE_ADDRESS  8

USB Usb;
BTD Btd(&Usb); 
PS3BT PS3(&Btd, 0x5C, 0xF3, 0x70, 0x9D, 0x84, 0x81);  // actual bluetooth address from dongle - set ENABLE_UHS_DEBUGGING to 1 in settings.h

#define I2CBYTES  17

volatile uint8_t array[I2CBYTES] = {0};

void masterRequestHandler();

void setup() {
  Serial.begin(9600);
// #if !defined(__MIPSEL__)
//   while (!Serial);
// #endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nPS3 Bluetooth Library Started"));

  Wire.begin(SLAVE_ADDRESS);
  Wire.onRequest(masterRequestHandler);

}
void loop() {
  Usb.Task();

  array[0] = (PS3.PS3Connected ? 1 : 0);

  if (PS3.PS3Connected) {
      
    if (PS3.getButtonClick(PS))
      PS3.disconnect();

    array[1] = PS3.getAnalogHat(LeftHatX);
    array[2] = PS3.getAnalogHat(LeftHatY);
    array[3] = PS3.getAnalogHat(RightHatX);
    array[4] = PS3.getAnalogHat(RightHatY);

    // L1, L2, R1, R2
    /* PS3.getButtonClick returns a boolean*/
    array[5] = (PS3.getButtonPress(L1) ? 1 : 0);
    array[6] = PS3.getAnalogButton(L2);
    array[7] = (PS3.getButtonPress(R1) ? 1 : 0);
    array[8] = PS3.getAnalogButton(R2);

    // D-pad buttons
    array[9] = (PS3.getButtonPress(DOWN) ? 1 : 0);
    array[10] = (PS3.getButtonPress(LEFT) ? 1 : 0);
    array[11] = (PS3.getButtonPress(UP) ? 1 : 0);
    array[12] = (PS3.getButtonPress(RIGHT) ? 1 : 0);

    // action buttons (cross, square, triangle, circle)
    array[13] = (PS3.getButtonPress(CROSS) ? 1 : 0);
    array[14] = (PS3.getButtonPress(SQUARE) ? 1 : 0);
    array[15] = (PS3.getButtonPress(TRIANGLE) ? 1 : 0);
    array[16] = (PS3.getButtonPress(CIRCLE) ? 1 : 0);
    
    // for (int x = 0; x < I2CBYTES; x++) {
    //   Serial.print(array[x]);
    // }
    // Serial.println();

  }
}

void masterRequestHandler() {
  Wire.write((const uint8_t*) array, I2CBYTES);
}

