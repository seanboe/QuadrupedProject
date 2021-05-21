#include "DataTransfer.h"


Transfer::Transfer(RH_RF69 *rf69, int byteCount, uint8_t * key)  {

  _byteCount = byteCount;

  _rf69 = rf69;

  _dataArray = (byte *) malloc(_byteCount);
  for (int x = 0; x < _byteCount; x++)
    _dataArray[x] = 0;

  _key = key;

};

bool Transfer::init() {
  pinMode(RFM69_RST, OUTPUT);
  digitalWrite(RFM69_RST, LOW);

  //Manual radio reset
  digitalWrite(RFM69_RST, HIGH);
  delay(10);
  digitalWrite(RFM69_RST, LOW);
  delay(10);

  if (Serial) 
    Serial.println("LoRA radio reset complete!");

  if (!_rf69->init()) {
    if (Serial)
      Serial.println("RFM69 radio initialization failed");
    return false;
  }

  if (!_rf69->setFrequency(RF69_FREQ)) {
    if (Serial)
      Serial.println("setFrequency failed");
  }

  _rf69->setTxPower(20);
  _rf69->setEncryptionKey(_key);

  return true;
};


void Transfer::write(BIT_OR_BYTE bitOrByte, int index, int value) {
  if (bitOrByte == BIT) {
    switch(value) {
      case 0: 
        if (read(BIT, index)) 
          _dataArray[((int)ceil(((index - (index % 8))/8)))] -= BITS[index % 8];
        break;
      case 1: 
        if (!read(BIT, index))
          _dataArray[((int)ceil(((index - (index % 8))/8)))] += BITS[index % 8];
        break;
    }
  }
  else if (bitOrByte == BYTE) {
    _dataArray[index] = value;
  }
};


byte Transfer::read(BIT_OR_BYTE bitOrByte, int index) {

  // index serves two roles:
  // when returning bits, index represents the index of bits in the array of bytes (_dataArray)
  // when returning bytes, index represents the index of bytes in _dataArray (or just the array index)

  if (bitOrByte == BIT) {
    if ((_dataArray[((int)ceil(((index - (index % 8))/8)))] & BITS[index % 8]) == BITS[index % 8])
      return 1;
    else 
      return 0;
  }

  // else, the user is requesting a Byte
  else
    return _dataArray[index];
};

void Transfer::send(bool resetBuffer) {
  _rf69->send(_dataArray, sizeof(_dataArray));
  _rf69->waitPacketSent();

  if (resetBuffer) {
    for (int x = 0; x < _byteCount; x++) 
      _dataArray[x] = 0;
  }
};

bool Transfer::receive() {
  if (_rf69->available()) {
    uint8_t dataBuffer[RH_RF69_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(dataBuffer);
    if (_rf69->recv(dataBuffer, &len)) {
      if (!len) 
        return false;
      if (len != _byteCount)
        return false;
      dataBuffer[len] = 0;
      for (int x = 0; x < _byteCount; x++)
        _dataArray[x] = dataBuffer[x];
    }
    return true;
  }
  return false;
}