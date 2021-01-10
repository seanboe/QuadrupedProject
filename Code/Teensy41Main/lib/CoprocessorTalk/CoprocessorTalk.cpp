#include "CoprocessorTalk.h"

#include <Arduino.h>
#include <Wire.h>

// Constructor
CoprocessorTalk::CoprocessorTalk(int slaveAddress, int dataIndexCount, bool serialFeedbackOK) { 
  _slaveAddress = slaveAddress;
  _dataIndexCount = dataIndexCount;
  _serialFeedbackOK = serialFeedbackOK;
  if ((dataIndexCount > _DEFAULT_DATA_INDEX_COUNT) && _serialFeedbackOK) {
    Serial.println("The default index of Coprocessor data buffer is too small for what was specified in constructor. Please edit _DEFAULT_DATA_INDEX_COUNT in CoprocessorTalk.h");
  }
};

// Private Functions
int CoprocessorTalk::_isTypeAnalog(uint8_t inputType) {
  if ((inputType == 1) || (inputType == 2) || (inputType == 3) || (inputType == 4))
    return 1;         // analog input; joystick, which are much less stable/consistent than the triggers and require an offset
  else if ((inputType == 6) || (inputType == 8))
    return 2;         // analog input, but it's a trigger (R2, L2), which are much more stable than Joysticks and consistently return 0 when not pressed
  else
    return 0;         // not an anlog input; a button
}

// Public Functions
bool CoprocessorTalk::retrieveData(void) {        // this is a bool, so that I can halt main when the controller is disconnected
  for (int x = 0; x < _dataIndexCount; x++) {         // update the previous data, so that controllerInputChanged works
    _previousDataArray[x] = _currentDataArray[x];
  }
  
  Wire.requestFrom(_slaveAddress, _dataIndexCount);

  // read the incoming data on the buffer; make sure to wait for all bytes expected and not read prematurely
  while(Wire.available() == _dataIndexCount) {
    for (int x = 0; x < _dataIndexCount; x++) {
      _currentDataArray[x] = Wire.read();
    }
  }

  if (_currentDataArray[0] == 1)
    return true;                         // NULL received; controller isn't connected, or data hasn't been acquired yet on Slave
  else {
    if (_serialFeedbackOK)
      Serial.println("no data recieved: controller not connected, or coprocessor not ready yet; Returned from CoprocessorTalk");
    return false;
  }
};

bool CoprocessorTalk::controllerConnected(void) {
  if (_currentDataArray[0] == 0)
    return false;
  else
    return true;
};

int CoprocessorTalk::getControllerInput(int inputType) {
  return _currentDataArray[inputType];
};

bool CoprocessorTalk::controllerInputChanged(uint8_t inputType) {
  // buttons and triggers first; these have only 2 states so offset isn't necessary
  if ((_currentDataArray[inputType] != _previousDataArray[inputType]) && ((_isTypeAnalog(inputType) == 0)|| _isTypeAnalog(inputType) == 2))
    return true;
  // analog inputs, joysticks only because triggers are much more consistent
  else if ((_isTypeAnalog(inputType) == 1) && (abs(_currentDataArray[inputType] - _previousDataArray[inputType])) > JOYSTICK_OFFSET)
    return true;
  else
    return false;
}

// I don't like the way that this is written... will become messy when there are more modes to set; fix if necessary
void CoprocessorTalk::write(uint8_t message) {
  if (message == SET_MODE_0) {
    Wire.beginTransmission(_slaveAddress);
    Wire.write(0);          // 0 is the ping address that tells the Teensy LC that a mode is being set
    Wire.write(message);    // in this case, it is setting the mode to 0 as well
    Wire.endTransmission();

    if (_serialFeedbackOK) {
      String output = "Mode " + String(message) + " Set";
      Serial.println(output);
    }
  }
}