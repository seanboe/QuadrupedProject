## Quadruped Remote Controller

<p align="center"><img src=".../images/quadController.png" alt="QuadrupedProject" width="300"></p>

This is a __temporary__ controller for the quadruped built on a breadboard with two joysticks and two buttons. A future project will be to design a PCB to make a PS4 style, arduino compatible remote, but that's for later. 

If you want to make it, you'll need:
- [5V booster](https://www.adafruit.com/product/4654)
- [RFM69HCW LoRA radio](https://www.adafruit.com/product/4654)
- [Joysticks](https://www.amazon.com/ARCELI-Joystick-Controller-Dual-axis-Breakout/dp/B077Z8QN3S/ref=sr_1_6?dchild=1&keywords=joystick+arduino&qid=1621526560&sr=8-6)
  - *You'll need to modify the headers for this*
- Buttons
- Teensy 4.0
- A 1s Lipo battery

### Pinouts:
| Teensy 4.0    | RFM69HCW || Teensy 4.0| Joystick Left|| Teensy 4.0| Joystick Right|
| ------------- |:-------------:|| ------------- |:-------------:|| ------------- |:-------------:|
| 3V|VIN||GND|GND||GND|GND|
| GND|GND||3V|+5V||3V|+5V|
| NC|EN||15|VRx||16|VRx|
|3|G0||14|VRy||16|VRy|
|13|SCK|
|12|MISO|
|11|MOSI|
|4|CS|
|2|RST|

The left and right buttons are a attached to pins 23 and 22, respectively, on the Teensy. Either use a 10K resistor for pullup, or 
specify both pins as <code>INPUT_PULLUP</code>.

The 5V boost is set up to provide the teensy with 5V for wireless controlling. 

### Library dependencies:
To run my code, you'll need the [RadioHead Library](https://github.com/adafruit/RadioHead). Create an <code>include</code> folder in the 
QuadController directory and place the library in there. 