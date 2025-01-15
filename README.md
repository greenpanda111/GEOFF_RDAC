# GEOFF
 3rd Year Engineering Module

 GEOFF is a 2 wheeled skid steered maze solving robot consisting of:
 2 motors with an ecoder each
 The encoders are wired to the ARB board using the expected pins as well as spare GPIO for the ultrasonics.
 
 4 IR sensors
 Two on the front for wall alignment using triganometry to calculate the angle to turn and for a wider field of view.
 One on either side of the robot for measuring the distance to the left and right of the robot.

 2 Ultrasonic sensors
 These are not currently utilised but my CAD design allows for them to be freely rotated so could be added for improved mapping capabilities in future.
 
 4 LED NeoPixel panels
 These form a 16x16 LED matrix to display live mapping of obstacles and route travelled along with current location and heading.
 The graphics library (All three NeoPixel libraries are created by Adafruit) used can allow for easy debugging. For example: writing the current angle onto the screen to test if it is being updated correctly as the robot turns.

 The Neopixel matrix can be controlled with one 5V signal pin. To achieve this, I had to design a level shifter to control it with the arduinos 3V3 signal

 2 Microswitches attached to the front corners of the robot to create a front bumper
 These prevent the robot from crashing into obstacles that the front IRs did not detect or catching on corners