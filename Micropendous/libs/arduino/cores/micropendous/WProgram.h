#ifndef WProgram_h
#define WProgram_h

#ifdef __cplusplus
extern "C" {
#endif
	
#ifdef __cplusplus
} // extern "C"
#endif

	#include <stdlib.h>

#include <string.h>
#include <math.h>
#include <avr/interrupt.h>

#include "LUFA_Arduino.h"

#include "wiring.h"

#include "binary.h"

#ifdef __cplusplus
extern "C" {
#endif
	#include "USBVirtualSerial.h"
	#include <LUFA/Drivers/Peripheral/SPI.h>
	#include <LUFA/Drivers/Peripheral/ADC.h>
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#include "ArduinoPrint.h"
#include "HardwareSerial.h"
#include "WiringVirtualSerial.h"

#include "EEPROM.h"
#include "Ethernet.h"
#include "Client.h"
#include "Server.h"
#include "Firmata.h"
#include "LiquidCrystal.h"
#include "Matrix.h"
#include "Servo.h"
#include "SoftwareSerial.h"
#include "Sprite.h"
#include "Stepper.h"
#include "Wire.h"

uint16_t makeWord(uint16_t w);
uint16_t makeWord(byte h, byte l);

#define word(...) makeWord(__VA_ARGS__)

unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout = 1000000L);

// WMath prototypes
long random(long);
long random(long, long);
void randomSeed(unsigned int);
long map(long, long, long, long, long);
#endif // __cplusplus

#endif