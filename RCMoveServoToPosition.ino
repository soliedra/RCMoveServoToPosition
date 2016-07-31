/*
 * Turns the Servo-motor shaft to the position set by the user through an RC control set.
 * The servo will move in the range -45 to 45 with the neutral position in 0.
 *
 * This library uses PID and Encoder feedback to control a DC motor. It's modeled a little bit after the AccelStepper library.
 *
 * Pololu VNH5019 Driver has been used. 
 *
 *
 * DC motor driver connections
 *-----------------------------
 * 
 * Function         | Pin   
 *----------------------------
 * Direcion A (CW)  | D5	 
 *----------------------------
 * Direcion B (CCW) | D7 	  
 *----------------------------
 * PWM	            | D6	  
 *------------------------------------------
 * Brake (GND)      | D5 and D7 set to LOW  
 *---------------------------------------------
 * Current sensing  | A1      
 ------------------------------
 *
 * RC Receiver connections (We use only channel 1)
 *------------------------------------------------
 * Each channel has three pins (Signal, Vdd (power), GND) that
 * have to be connected to Arduino as indicated in the table below
 *
 * Channel 1   |  Arduino
 *-------------------------
 * Signal      | RC_INPUT_1
 *-------------------------
 * Vdd         | Vin (Not needed when the USB cable is used)
 *-------------------------
 * GND         | GND
 *-------------------------
 * 
 * Hardware
 * --------
 * RC control set (emitter and receiver)
 * [Pololu VNH5019 Driver](https://www.pololu.com/product/1451) 
 * Arduino UNO R3
 * A 12V geared DC motor from a battery drill. Output speed aprox 600rpm
 * A DIY Quad encoder made with with two [Pololu QTR-1RC Reflectance sensors](https://www.pololu.com/product/2459) that
 * provides 8CPR.
 *
 * Front view of the encoders location
 *
 *        B
 *       ***
 *      *   * A
 *       ***
 * CW(+)<---> CCW (-)
 * 
 *  Connections:
 *  Encoder A -> D2 (Arduino, not motor shield)
 *  Encoder B -> D3 (Arduino, not motor shield)
 *
 *  Activation sequence
 *                 ______        ______
 *  Encoder A  ___|      |______|      |______
 *                    ______        ______
 *  Encoder B  ______|      |______|      |______
 *             CCW-->(-)                   (+) <--CW
 *
 * 
 * Pins
 * ----
 * The Driver pins have been connected as described above. 
 * The Quad encoder channel B (North side of the motor shaft, front view) connected to pin D3 (external interrupt)
 * The Quad encoder channel B (East side of the motor shaft, front view) connected to pin D2 (external interrupt) 
 * 
 * Modified by Javier Casado July 2016
 */
// Esta librería permite crear conexiones serie usando pines distintos de D0 y D1
#include <SoftwareSerial.h>
 
#define PIN_RXD 		6
#define PIN_TXD 		7
#define INPUT_RC_1      19
#define DATA_BEGIN      '@'

// Variables used to store the value of the RC signal received
unsigned long currentSignal = 0;
unsigned long lastSignal = 0;

int targetPosition = 0;

//Módulo Servo conectado a los pines:
SoftwareSerial theServo(PIN_RXD,PIN_TXD);

void setup() {

  
  //Serial.begin(9600);
  //Serial.println("Target position received: "); 
  
  // start the communication with the servo
  delay(1000);
  theServo.begin(1200); 
}


void loop() {
 
    if(readTargetPositionRC()) writeTargetPosition();
    
    // not to jam the serial port
    delay(50);
    
}

/*
 * Returns true when a new position has been received from the
 * RC set and false otherwise.
 * The value received is stored in the global variable targetPosition.
 */
bool readTargetPositionRC()
{
  // We divide by 50, the number of microseconds isn't accurate. This will
  // give a value between 20 and 40, neutral 30
  currentSignal = pulseIn(INPUT_RC_1,10000)/50;

  // If the value received is a valid one and has changed from the last time we 
  // map it in the range 0 to 90	
  if(currentSignal > 0 && currentSignal != lastSignal) {
    targetPosition = map(currentSignal,20,40,-45,45);
    //Serial.print(currentSignal);Serial.print(" : ");Serial.println(targetPosition);
    lastSignal = currentSignal;
    return true;
  }
  return false;
}

bool readTargetPositionSerial()
{
	// read speed to be set
	while(Serial.available() > 0) {
		//wait for the incomming data to arrive, avoids partial readings
		delay(10);
		targetPosition = Serial.parseInt();
		//Maybe there's an end of string read as a zero, remove it reading again 
		// Maybe there's a problem if two positions are sent together
		//Serial.parseInt();
                
                // Yes, there was a reading
                return true;
	}
   // By default, no reading
   return false;
}

/*
 * sends the targetPosition to the servo through
 * the serial port theServo
 */
 void writeTargetPosition() {
   theServo.print(DATA_BEGIN);
   theServo.println(targetPosition);
 }
