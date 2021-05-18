### Data Transfer library

Meant to efficiently store and send data. It essentially "sits on top of" the RadioHead library that interfaces with the rf69 LoRA radio and allows me to very easily specify 
bits to turn on/off. In total, I think I can send all the controller data for my quadruped with 4 bytes... which should be SUPERRR fast! I may consider making this a standalone class(this is currently only useful for those using the same radio as me) but we shall see. 

#### How To Use?

First, you need to instantiate an RH_RF69 radio object. The arguments specify SPI pins, and can be modified in Config.h
<code>RH_RF69 rf69(RFM69_CS, RFM69_INT);</code>

This needs to be passed to a 'Transfer</code> object as such:
<code>Transfer dataBuffer(&rf69, BUFFER_SIZE, key);</code>

<code>BUFFER _SIZE</code> is the size of the buffer array to be sent across devices, in bytes (can be modified in Config.h)
</code>key</code> is a 16 byte array for data encryption

###### Functions

<code>init()</code>
Initializes the dataBuffer and performs radio setup. This includes a manual reset of the radio via an interrupt pin on G0; this pin can be changed in Config.h


<code>write(BIT_OR_BYTE bitOrByte, int index, int value)</code>
Allows you to write data to the local <code>Transfer</code> object

Where:
<code>bitOrByte</code> specifies whether data is written to a bit or a byte in the buffer; choose between 'BIT' or 'BYTE</code>
<code>index</code> is the bit or byte you want to modify
  - If you wish to modify a <code>BIT</code>, index allows you to select the number bit you wish to change. Indexes larger than 8 will shift to the next byte, and the bit's index in the byte is determined by <code>index % 8</code>, counting from right to left and starting from 0 in each byte. 
  - If you wish to modify a <code>BYTE</code>, then you may select the index in the buffer to modify, starting with 0, obviously.
<code>value</code> is required only for bytes; this is simply the value you want to write the selected byte. 


<code>read(BIT_OR_BYTE bitOrByte, int index)</code>
Allows you to read data in the local <code>Transfer</code> object

Where: 
<code>bitOrByte</code> specifies whether data is written to a bit or a byte in the buffer; choose between <code>BIT</code> or <code>BYTE</code>
<code>index</code> is the bit or byte you want to read
  - If you wish to read a <code>BIT</code>, index allows you to select the number bit you wish to read. Indexes larger than 8 will shift to the next byte, and the bit's index in the byte is determined by <code>index % 8</code>, counting from right to left and starting from 0 in each byte. 
  - If you wish to read a <code>BYTE</code>, then you may select the index in the buffer to read, starting with 0, obviously.

This is recommended to be used after a call to <code>receive()</code>, which handles the reception of a buffer sent fro manother device.


<code>send(bool resetBuffer)</code>
Sends a local buffer to a different device. Defaults to resetting the local buffer, but this can be overwritten by specifying false in the argument.

<i> Note that this will wait for the buffer to be sent, which may cause issues </i>

<code>receive()</code>
Receives a buffer from a different device. Returns true if successful, false if not. 


###### Common use:

For transmitting data, it makes sense to use <code>write()</code> and <code>send</code>. Send will reset the local buffer to 0 for future calls to <code>write()</code>. 

For receiving data, it makes sense to use <code>receive()</code>, and upon a return of <code>true</code>, to 'read()</code> relevant data. <code>
