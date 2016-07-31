# RCMoveServoToPosition
An arduino Board is connected to the channel 1 (could be any other) of an standard RC receiver of the type used
in RC models. 

The channel 1 signal (a pwm pulse with a width between 1000 and 2000 us that repeats at a rate of 50Hz approx) is mapped to into the valid range of positions of a DIY DC Servo Motor (not a Hobby one)
with a quad encoder that does 32 CPR. I'm using the range -45 to 45 for the DIY servo motor, but any other range can be defined in the mapping process.
The main advantage over using an standard Hobby servo, is that my DIY Servo Motor is really powerful 
(it's mainly a portable drill with an quad encoder that consumes 34 Amps at the start), and can drive very high loads at incredible speeds, and more important than this, the range of positions can expand over
more than one turn whereas a Hobby servo only turns 270 degrees.

Once the channel 1 signal has been translated into a target position for my DIY Servo Motor (a number between -45 and 45), the target position
is sent over RS232 (Not the default one in Arduinos over pins D0 and D1) to the Arduino board that controls the DIY Servo Motor, to drive the servo motor to its target position.

The Arduino board that controls the DIY servo motor has to be running the sketch [SerialMoveServoToPosition](https://github.com/soliedra/SerialMoveServoToPosition) and connected properly
to the Motor Driver and the Quad encoder.

It's extremely important to use an Arduino Board exclusively to control the DIY Servo Motor, it uses interruptions intensively, and not much cpu time is left for anything else. That's the
reason why I've used another one to deal with the RC receiver signal.

It's important also to use a slow speed (1200 bauds) to communicate both Arduino boards over RS232, in order not to burden the DIY servo motor controller with more interruption cycles
from the USART. The priority here is that the DIY servo motor encoder doesn't miss any step, otherwise it would be useless.

The channel 1 connector has two more cables, in addition to the already descibed signal cable (white or orange):
- GND (black or brown): That has to be connected along with the rest of GNDs
- Vdd (Red): Positive terminal intended to power a Hobby servo straight from the RC receiver battery, it's going to be used
  to power both the Arduinos and therefore has to be connected to their Vin pin.
  
##Libraries used:

- [SoftwareSerial] (https://www.arduino.cc/en/Reference/SoftwareSerial) to implement an RS232 stack onto two digital pins different from D0 and D1 (one for transmission and another for reception)

##Hardware:

- Arduino Uno board
- RC receiver with a rechargable battery with 6 Ni-Mh elements and a switch to switch the logic on and off (this battery will power also both Arduino boards).   

##Connections:

```
                                    |------------------------|
                                    |                        |
 -------------------------------    |  ------------------    |     ----------------------------
 |                             |    |  |    Arduino -1  |    |----|-GND       Arduino -2       |
 |  RC          Channel 1 GND -|----|--|-GND     Tx  D7-|---------|-D0 Rx DIY servo controller |------> To the motor driver
 | Receiver     Channel 1 Vdd -|--|----|-Vin     Rx  D6-|---------|-D1 Tx                      |
 |           Channel 1 Signal -|--|----|-D19 (A5)       |         |                            |<------ From the Quad encoder
 |-----------------------------|  |    |----------------|      |--|-Vin                        |
      |      |                    |----------------------------|  |----------------------------|
    + |    - |
    ------------
    | Battery  |
    |          |
    ------------
    
```

Author: Javier Casado
July 2016
License: CC BY
