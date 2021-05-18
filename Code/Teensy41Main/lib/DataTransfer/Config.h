// This is where important radio parameters are defined


// size of your data buffer. This MUST be the same on both devices that are communicating
#define BUFFER_SIZE   4

//Teensy SPI pins
#define RFM69_RST     2   
#define RFM69_CS      4   
#define RFM69_INT     3    

// Radio frequency. You probably shouldn't change this if you are in the US... or you don't want to be arrested
#define RF69_FREQ 915.0

const uint8_t BITS[8] = {0b00000001, 0b00000010, 0b00000100, 0b00001000, 0b00010000, 0b00100000, 0b01000000, 0b10000000};

typedef enum {
  BIT, BYTE
} BIT_OR_BYTE;