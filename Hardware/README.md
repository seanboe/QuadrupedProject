# Hardware

This is where all my custom hardware is posted. Gerbers for control board V1.0 is already available, and CAD files will be released shortly.

## Quadruped Control Board V1.0

It's done! Note that I haven't had the chance to test it yet (the board hasn't arrived) and a BOM will be coming shortly.

<img src="PCBV1-0/layoutV1-0.png" height="500">

#### Features:
- 2 layer board
- Includes 12 breakouts for all servos
- On-board IMU data (via an [LSM6DSOX](https://www.st.com/resource/en/datasheet/lsm6dsox.pdf))
- Battery capacity monitoring (via an [LC709203F](https://www.onsemi.com/pdf/datasheet/lc709203f-d.pdf))
- LoRA radio 
- Motor power shutoff capability
- Controlled by a Teensy 4.1
- Battery power/USB power selection for Teensy
- 6 breakout GPIO, including an I2C bus and associated power/ground lines

*Known issues with v1.0* 😭
- Holes for header pins are too small to allow them to comfortably fit. You will need to "jam" them in or solder on top of the hole.
- xt-60 connector +/- is flipped (silkscreen outline is wrong). Simply flip the connector on the board, or solder the wires reverse for the male connector that goes onto the board. 
