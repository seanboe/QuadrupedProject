#include <Arduino.h>

//graphics libraries, for tft display
#include <Adafruit_GFX.h>    
#include <Adafruit_ST7735.h>

// definitions for SPI pins for display
#define TFT_CS        6
#define TFT_RST       2
#define TFT_DC        3

//usb host libraries for interfacing with ps3 controller
#include <PS3BT.h>
#include <usbhub.h>

#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif

// SPI: for tft display and usb host libraries, Wire: for coms with teensy 4.1
#include <SPI.h>
#include <Wire.h>

// display object initlizaation, as shown in the example code
Adafruit_ST7735 tft = Adafruit_ST7735(&SPI1, TFT_CS, TFT_DC, TFT_RST);

// usb object initialization, as shown in the example code
USB Usb;
BTD Btd(&Usb); 
PS3BT PS3(&Btd, 0x5C, 0xF3, 0x70, 0x9D, 0x84, 0x81);  // actual bluetooth address from dongle - set ENABLE_UHS_DEBUGGING to 1 in settings.h

#define SLAVE_ADDRESS  8    // this device's address, for I2C

#define I2CBYTES  17

// the array that stores controller data
volatile uint8_t array[I2CBYTES] = {0};

// function forward declarations
void masterRequestHandler();
void displayContollerStatus();

void setup() {
  Serial.begin(9600);

  // usb library initialization and dongle intialization
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nPS3 Bluetooth Library Started"));

  // join the I2C bus with the teensy 4.1 and set request handler
  Wire.begin(SLAVE_ADDRESS);
  Wire.onRequest(masterRequestHandler);

  // initialize the display
  tft.initR(INITR_BLACKTAB);      // Init ST7735S chip, black tab
  tft.setRotation(1);             // rotate the screen to "landscape"
  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(10, 10);
  tft.print("PS3 connected: ");
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

    /* this is test code to check if stuff is being read correctly*/
    // for (int x = 0; x < I2CBYTES; x++) {
    //   Serial.print(array[x]);
    // }
    // Serial.println();

  }
  
  displayContollerStatus();
  
}

void masterRequestHandler() {
  Wire.write((const uint8_t *) array, I2CBYTES);
}


void displayContollerStatus() {
  static int previousConnectionStatus = 1;        // set to 1, so that this function is run once when controller isn't connected yet
  
  if ((millis() % 500 == 0) && previousConnectionStatus != array[0]) {
    
    tft.setCursor(100, 10);
    tft.fillRect(100, 10, 35, 7, ST7735_BLACK);
    previousConnectionStatus = array[0];
    
    if (array[0] == 1) {
      tft.setTextColor(ST7735_GREEN);
      tft.print("True");
    }
    else if (array[0] == 0) {
      tft.setTextColor(ST7735_RED);
      tft.print("False");
    }
    
  }
}

