/*
 LUFAduino
 
 Demonstrates Virtual USB Serial Port loopback using Arduino code
 Also flashes and fades a LED connected to pin 5 - PD0
 Demonstrates multi-tasking ability of LUFAduino by using two loop functions and delays
 Loop2 is created using duinOS
 
 The circuit:
 Any USB AVR with > 2kbytes SRAM - ATmega32U4, ATmega32U6, AT90USB???6/7
 with a LED connected to pin 5 - PD0
 On Micropendous boards, you can simply connect a wire from Pin 5 to 13 (the LED pin)
 
 created created 2009-10-03 by Opendous Inc.
 last edited 2010-12-13 by Opendous Inc.
 
 http://www.Micropendous.org/LUFAduino
*/

#include "EEPROM.h"
//#include "Ethernet.h"
//#include "Client.h"
//#include "Server.h"
//#include "Firmata.h"
#include "LiquidCrystal.h"
#include "Matrix.h"
#include "Servo.h"
#include "SoftwareSerial.h"
#include "Sprite.h"
#include "Stepper.h"
#include "Wire.h"

int ledPin =  5;	// connect an LED to pin 5 - PD0 which is a PWM pin

// example of using duinOS
declareTaskLoop(loop2);

// example of using duinOS, loop2() is a duinOS task
// and must be declared before it is created with createTaskLoop
taskLoop(loop2)
{
	digitalWrite(ledPin, HIGH);  // turn the LED on
	delay(500);                  // wait for half a second
	digitalWrite(ledPin, LOW);   // turn the LED off
	delay(500);                  // wait for half a second

	// fade in from min to max in increments of 5 points:
	for(int fadeValue = 0 ; fadeValue <= 255; fadeValue +=5) { 
		// sets the value (range from 0 to 255):
		analogWrite(ledPin, fadeValue);         
		// wait for 50 milliseconds to see the dimming effect    
		delay(50);                            
	}

	// fade out from max to min in increments of 5 points:
	for(int fadeValue = 255 ; fadeValue >= 0; fadeValue -=5) { 
		// sets the value (range from 0 to 255):
		analogWrite(ledPin, fadeValue);         
		// wait for 50 milliseconds to see the dimming effect    
		delay(50);                            
	}

	// duinOS - yield to the next task
	nextTask();
}


// loop() is a FreeRTOS task with priority = NORMAL_PRIORITY
void loop(void)
{
	if ((Serial.enumerated() > 0) && (Serial.available() > 0))
	{
		// Echo back recevied characters to the host
		Serial.write(Serial.read());
	}

	nextTask();
}


void setup(void)
{
	// Wiring Interface Initialization
	Serial.begin(115200);

	pinMode(ledPin, OUTPUT);

	// example of using duinOS
	createTaskLoop(loop2, NORMAL_PRIORITY);
}
