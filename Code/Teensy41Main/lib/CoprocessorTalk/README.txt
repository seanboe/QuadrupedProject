Basically, this library condenses the process  of retrieving data from the coprocessor. It uses
.retrieveData() to request and read data from the coprocessor into a private buffer, which can 
then be accessed using .getControllerInput. 

While it doesn't do too much, I've created this in the hopes of reducing the main code for the 
teensy 4.1. 

This library uses Wire.h, which must be initialized using .begin() before any use of this library,  
other than the constructor. This is so that main code will (hopefully) be more readable.

Serial is optional, defined in the constuctor as serialFeedbackOK = true or false.