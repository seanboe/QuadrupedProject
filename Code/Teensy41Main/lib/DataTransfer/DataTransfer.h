#ifndef DATA_TRANSFER_H
#define DATA_TRANSFER_H

#include <Arduino.h>
#include <SPI.h>
#include <RH_RF69.h>

#include "Config.h"

class Transfer {
  public:
    Transfer(RH_RF69 *rf69, int byteCount, uint8_t * key);

    bool init();

    void write(BIT_OR_BYTE bitOrByte, int index, int value = 1);
    byte read(BIT_OR_BYTE bitOrByte, int index);  // true = bit, false = byte

    void send(bool resetBuffer = true);  // It is HIGHLY recommended that resetBuffer stays true; otherwise you will will continue to add bits and make your data nonsensical
    bool receive();   // returns true when data is received, false if otherwise

    uint8_t * _dataArray;  // buffer for all the data. This must be determined correctly; the first few bytes are for determining which sensors have data, and the bytes thereafter hold relevant data that require their own bytes. 

  private:
    RH_RF69 * _rf69;

    int _byteCount;

    // uint8_t * _dataArray;  // buffer for all the data. This must be determined correctly; the first few bytes are for determining which sensors have data, and the bytes thereafter hold relevant data that require their own bytes. 
    uint8_t * _key; 

};

#endif