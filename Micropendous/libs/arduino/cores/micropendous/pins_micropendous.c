/*
  pins_micropendous.c - pin definitions for Micropendous-based Arduino-like boards
  Part of LUFAduino / Arduino / Wiring Lite

  Copyright (c) 2005 David A. Mellis

  Adapted for LUFA + Arduino by Opendous Inc. 2009-09
  For more information visit:  www.Micropendous.org/LUFAduino

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

*/
/*  Notes:	Timer1 cannot be used by the larger AVRs (ATmega32U4, AT90USB???6/7)
				as it is used by FreeRTOS.  Timer1 is enabled on the AT90USB162 as that
				IC cannot support FreeRTOS (not enough SRAM).
*/

#include <avr/io.h>
#include "wiring_private.h"
#include "pins_micropendous.h"

#define PA 1
#define PB 2
#define PC 3
#define PD 4
#define PE 5
#define PF 6

#define REPEAT8(x) x, x, x, x, x, x, x, x
#define BV0TO7 _BV(0), _BV(1), _BV(2), _BV(3), _BV(4), _BV(5), _BV(6), _BV(7)
#define BV7TO0 _BV(7), _BV(6), _BV(5), _BV(4), _BV(3), _BV(2), _BV(1), _BV(0)



/** Arduino Definitions for 64-pin USB AVRs */

#if (defined(__AVR_AT90USB1287__) || defined(__AVR_AT90USB647__) ||  \
		defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB646__) ||  \
		defined(__AVR_ATmega32U6__))

#define HWB_PIN	42

const uint16_t PROGMEM port_to_mode_PGM[] = {
	NOT_A_PORT,
	(uint16_t)&DDRA,
	(uint16_t)&DDRB,
	(uint16_t)&DDRC,
	(uint16_t)&DDRD,
	(uint16_t)&DDRE,
	(uint16_t)&DDRF,
};

const uint16_t PROGMEM port_to_output_PGM[] = {
	NOT_A_PORT,
	(uint16_t)&PORTA,
	(uint16_t)&PORTB,
	(uint16_t)&PORTC,
	(uint16_t)&PORTD,
	(uint16_t)&PORTE,
	(uint16_t)&PORTF,
};

const uint16_t PROGMEM port_to_input_PGM[] = {
	NOT_A_PIN,
	(uint16_t)&PINA,
	(uint16_t)&PINB,
	(uint16_t)&PINC,
	(uint16_t)&PIND,
	(uint16_t)&PINE,
	(uint16_t)&PINF,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
	// PORTLIST		
	// -------------------------------------------		
	PD	, // PD 0 ** 0 ** PWM, I2C_SCL, INT0
	PD	, // PD 1 ** 1 ** PWM, I2C_SDA, INT1
	PD	, // PD 2 ** 2 ** UART_RX, INT2
	PD	, // PD 3 ** 3 ** UART_TX, INT3
	PD	, // PD 4 ** 4 ** 
	PD	, // PD 5 ** 5 ** 
	PD	, // PD 6 ** 6 ** 
	PD	, // PD 7 ** 7 ** 
	PB	, // PB 0 ** 8 ** SPI_SS
	PB	, // PB 1 ** 9 ** SPI_CLK
	PB	, // PB 2 ** 10 ** SPI_MOSI
	PB	, // PB 3 ** 11 ** SPI_MISO
	PB	, // PB 4 ** 12 ** PWM
	PB	, // PB 5 ** 13 ** 
	PB	, // PB 6 ** 14 ** 
	PB	, // PB 7 ** 15 ** 
	PF	, // PF 0 ** 16 ** AI0
	PF	, // PF 1 ** 17 ** AI1
	PF	, // PF 2 ** 18 ** AI2
	PF	, // PF 3 ** 19 ** AI3
	PF	, // PF 4 ** 20 ** AI4
	PF	, // PF 5 ** 21 ** AI5
	PF	, // PF 6 ** 22 ** AI6
	PF	, // PF 7 ** 23 ** AI7
	PA	, // PA 0 ** 24 ** 
	PA	, // PA 1 ** 25 ** 
	PA	, // PA 2 ** 26 ** 
	PA	, // PA 3 ** 27 ** 
	PA	, // PA 4 ** 28 ** 
	PA	, // PA 5 ** 29 ** 
	PA	, // PA 6 ** 30 ** 
	PA	, // PA 7 ** 31 ** 
	PC	, // PC 0 ** 32 ** 
	PC	, // PC 1 ** 33 ** 
	PC	, // PC 2 ** 34 ** 
	PC	, // PC 3** 35 ** 
	PC	, // PC 4 ** 36 ** PWM
	PC	, // PC 5 ** 37 ** PWM
	PC	, // PC 6 ** 38 ** PWM
	PC	, // PC 7 ** 39 ** 
	PE	, // PE 0 ** 40 ** SRAM
	PE	, // PE 1 ** 41 ** SRAM
	PE	, // PE 2 ** 42 ** HWB, SRAM
	PE	, // PE 3 ** 43 ** 
	PE	, // PE 4 ** 44 ** INT4
	PE	, // PE 5 ** 45 ** INT5
	PE	, // PE 6 ** 46 ** INT6, SRAM Disable
	PE	, // PE 7 ** 47 ** INT7, SRAM
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
	// PIN IN PORT		
	// -------------------------------------------		
	_BV( 0 )	, // PD 0 ** 0 ** PWM, I2C_SCL, INT0
	_BV( 1 )	, // PD 1 ** 1 ** PWM, I2C_SDA, INT1
	_BV( 2 )	, // PD 2 ** 2 ** UART_RX, INT2
	_BV( 3 )	, // PD 3 ** 3 ** UART_TX, INT3
	_BV( 4 )	, // PD 4 ** 4 ** 
	_BV( 5 )	, // PD 5 ** 5 ** 
	_BV( 6 )	, // PD 6 ** 6 ** 
	_BV( 7 )	, // PD 7 ** 7 ** 
	_BV( 0 )	, // PB 0 ** 8 ** SPI_SS
	_BV( 1 )	, // PB 1 ** 9 ** SPI_CLK
	_BV( 2 )	, // PB 2 ** 10 ** SPI_MOSI
	_BV( 3 )	, // PB 3 ** 11 ** SPI_MISO
	_BV( 4 )	, // PB 4 ** 12 ** PWM
	_BV( 5 )	, // PB 5 ** 13 ** 
	_BV( 6 )	, // PB 6 ** 14 ** 
	_BV( 7 )	, // PB 7 ** 15 ** 
	_BV( 0 )	, // PF 0 ** 16 ** AI0
	_BV( 1 )	, // PF 1 ** 17 ** AI1
	_BV( 2 )	, // PF 2 ** 18 ** AI2
	_BV( 3 )	, // PF 3 ** 19 ** AI3
	_BV( 4 )	, // PF 4 ** 20 ** AI4
	_BV( 5 )	, // PF 5 ** 21 ** AI5
	_BV( 6 )	, // PF 6 ** 22 ** AI6
	_BV( 7 )	, // PF 7 ** 23 ** AI7
	_BV( 0 )	, // PA 0 ** 24 ** 
	_BV( 1 )	, // PA 1 ** 25 ** 
	_BV( 2 )	, // PA 2 ** 26 ** 
	_BV( 3 )	, // PA 3 ** 27 ** 
	_BV( 4 )	, // PA 4 ** 28 ** 
	_BV( 5 )	, // PA 5 ** 29 ** 
	_BV( 6 )	, // PA 6 ** 30 ** 
	_BV( 7 )	, // PA 7 ** 31 ** 
	_BV( 0 )	, // PC 0 ** 32 ** 
	_BV( 1 )	, // PC 1 ** 33 ** 
	_BV( 2 )	, // PC 2 ** 34 ** 
	_BV( 3 )	, // PC 3** 35 ** 
	_BV( 4 )	, // PC 4 ** 36 ** PWM
	_BV( 5 )	, // PC 5 ** 37 ** PWM
	_BV( 6 )	, // PC 6 ** 38 ** PWM
	_BV( 7 )	, // PC 7 ** 39 ** 
	_BV( 0 )	, // PE 0 ** 40 ** SRAM
	_BV( 1 )	, // PE 1 ** 41 ** SRAM
	_BV( 2 )	, // PE 2 ** 42 ** HWB, SRAM
	_BV( 3 )	, // PE 3 ** 43 ** 
	_BV( 4 )	, // PE 4 ** 44 ** INT4
	_BV( 5 )	, // PE 5 ** 45 ** INT5
	_BV( 6 )	, // PE 6 ** 46 ** INT6, SRAM Disable
	_BV( 7 )	, // PE 7 ** 47 ** INT7, SRAM
};                     

const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
	// TIMERS		
	// -------------------------------------------		
	TIMER0B	, // PD 0 ** 0 ** PWM, I2C_SCL, INT0
	TIMER2B	, // PD 1 ** 1 ** PWM, I2C_SDA, INT1
	NOT_ON_TIMER	, // PD 2 ** 2 ** UART_RX, INT2
	NOT_ON_TIMER	, // PD 3 ** 3 ** UART_TX, INT3
	NOT_ON_TIMER	, // PD 4 ** 4 ** 
	NOT_ON_TIMER	, // PD 5 ** 5 ** 
	NOT_ON_TIMER	, // PD 6 ** 6 ** 
	NOT_ON_TIMER	, // PD 7 ** 7 ** 
	NOT_ON_TIMER	, // PB 0 ** 8 ** SPI_SS
	NOT_ON_TIMER	, // PB 1 ** 9 ** SPI_CLK
	NOT_ON_TIMER	, // PB 2 ** 10 ** SPI_MOSI
	NOT_ON_TIMER	, // PB 3 ** 11 ** SPI_MISO
	TIMER2A	, // PB 4 ** 12 ** PWM
	NOT_ON_TIMER	, // PB 5 ** 13 ** 
	NOT_ON_TIMER	, // PB 6 ** 14 ** 
	NOT_ON_TIMER	, // PB 7 ** 15 ** 
	NOT_ON_TIMER	, // PF 0 ** 16 ** AI0
	NOT_ON_TIMER	, // PF 1 ** 17 ** AI1
	NOT_ON_TIMER	, // PF 2 ** 18 ** AI2
	NOT_ON_TIMER	, // PF 3 ** 19 ** AI3
	NOT_ON_TIMER	, // PF 4 ** 20 ** AI4
	NOT_ON_TIMER	, // PF 5 ** 21 ** AI5
	NOT_ON_TIMER	, // PF 6 ** 22 ** AI6
	NOT_ON_TIMER	, // PF 7 ** 23 ** AI7
	NOT_ON_TIMER	, // PA 0 ** 24 ** 
	NOT_ON_TIMER	, // PA 1 ** 25 ** 
	NOT_ON_TIMER	, // PA 2 ** 26 ** 
	NOT_ON_TIMER	, // PA 3 ** 27 ** 
	NOT_ON_TIMER	, // PA 4 ** 28 ** 
	NOT_ON_TIMER	, // PA 5 ** 29 ** 
	NOT_ON_TIMER	, // PA 6 ** 30 ** 
	NOT_ON_TIMER	, // PA 7 ** 31 ** 
	NOT_ON_TIMER	, // PC 0 ** 32 ** 
	NOT_ON_TIMER	, // PC 1 ** 33 ** 
	NOT_ON_TIMER	, // PC 2 ** 34 ** 
	NOT_ON_TIMER	, // PC 3** 35 ** 
	TIMER3C	, // PC 4 ** 36 ** 
	TIMER3B	, // PC 5 ** 37 ** 
	TIMER3A	, // PC 6 ** 38 ** 
	NOT_ON_TIMER	, // PC 7 ** 39 ** 
	NOT_ON_TIMER	, // PE 0 ** 40 ** SRAM
	NOT_ON_TIMER	, // PE 1 ** 41 ** SRAM
	NOT_ON_TIMER	, // PE 2 ** 42 ** HWB, SRAM
	NOT_ON_TIMER	, // PE 3 ** 43 ** 
	NOT_ON_TIMER	, // PE 4 ** 44 ** INT4
	NOT_ON_TIMER	, // PE 5 ** 45 ** INT5
	NOT_ON_TIMER	, // PE 6 ** 46 ** INT6, SRAM Disable
	NOT_ON_TIMER	, // PE 7 ** 47 ** INT7, SRAM
};                                
#endif



/** Arduino Definitions for 44-pin USB AVRs */

#if (defined(__AVR_ATmega16U4__)  || defined(__AVR_ATmega32U4__))

#define HWB_PIN 24

const uint16_t PROGMEM port_to_mode_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t)&DDRB,
	(uint16_t)&DDRC,
	(uint16_t)&DDRD,
	(uint16_t)&DDRE,
	(uint16_t)&DDRF,
};

const uint16_t PROGMEM port_to_output_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t)&PORTB,
	(uint16_t)&PORTC,
	(uint16_t)&PORTD,
	(uint16_t)&PORTE,
	(uint16_t)&PORTF,
};

const uint16_t PROGMEM port_to_input_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t)&PINB,
	(uint16_t)&PINC,
	(uint16_t)&PIND,
	(uint16_t)&PINE,
	(uint16_t)&PINF,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
	PD, // PD 0 ** 0 ** PWM, INT0
	PD, // PD 1 ** 1 ** INT1
	PD, // PD 2 ** 2 ** UART_RX, INT2
	PD, // PD 3 ** 3 ** UART_TX, INT3
	PD, // PD 4 ** 4 ** AI8
	PD, // PD 5 ** 5 ** 
	PD, // PD 6 ** 6 ** !PWMXD, AI9
	PD, // PD 7 ** 7 ** PWMXD, AI10
	PB, // PB 0 ** 8 ** SPI_SS
	PB, // PB 1 ** 9 ** SPI_CLK
	PB, // PB 2 ** 10 ** SPI_MOSI
	PB, // PB 3 ** 11 ** SPI_MISO
	PB, // PB 4 ** 12 ** AI11
	PB, // PB 5 ** 13 ** AI12
	PB, // PB 6 ** 14 ** PWM, AI13
	PB, // PB 7 ** 15 ** PWM
	PF,  // PF 0 ** 16 ** AI0
	PF, // PF 1 ** 17 ** AI1
	PF, // PF 4 ** 18 ** AI4
	PF, // PF 5 ** 19 ** AI5
	PF, // PF 6 ** 20 ** AI6
	PF, // PF 7 ** 21 ** AI7
	PC, // PC 6 ** 22 ** PWM
	PC, // PC 7 ** 23 ** PWM
	PE, // PE 2 ** 24 ** HWB
	PE, // PE 6 ** 25 ** INT6
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
	_BV( 0 )	, // PD 0 ** 0 ** PWM, INT0
	_BV( 1 )	, // PD 1 ** 1 ** INT1
	_BV( 2 )	, // PD 2 ** 2 ** UART_RX, INT2
	_BV( 3 )	, // PD 3 ** 3 ** UART_TX, INT3
	_BV( 4 )	, // PD 4 ** 4 ** AI8
	_BV( 5 )	, // PD 5 ** 5 ** 
	_BV( 6 )	, // PD 6 ** 6 ** !PWMXD, AI9
	_BV( 7 )	, // PD 7 ** 7 ** PWMXD, AI10
	_BV( 0 )	, // PB 0 ** 8 ** SPI_SS
	_BV( 1 )	, // PB 1 ** 9 ** SPI_CLK
	_BV( 2 )	, // PB 2 ** 10 ** SPI_MOSI
	_BV( 3 )	, // PB 3 ** 11 ** SPI_MISO
	_BV( 4 )	, // PB 4 ** 12 ** AI11
	_BV( 5 )	, // PB 5 ** 13 ** AI12
	_BV( 6 )	, // PB 6 ** 14 ** PWM, AI13
	_BV( 7 )	, // PB 7 ** 15 ** PWM
	_BV( 0 )	,  // PF 0 ** 16 ** AI0
	_BV( 1 )	, // PF 1 ** 17 ** AI1
	_BV( 4 )	, // PF 4 ** 18 ** AI4
	_BV( 5 )	, // PF 5 ** 19 ** AI5
	_BV( 6 )	, // PF 6 ** 20 ** AI6
	_BV( 7 )	, // PF 7 ** 21 ** AI7
	_BV( 6 )	, // PC 6 ** 22 ** PWM
	_BV( 7 )	, // PC 7 ** 23 ** PWM
	_BV( 2 )	, // PE 2 ** 24 ** HWB
	_BV( 6 )	, // PE 6 ** 25 ** INT6
};                  

const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
	// TIMERS		
	// -------------------------------------------
	TIMER0B	, // PD 0 ** 0 ** PWM, INT0
	NOT_ON_TIMER	, // PD 1 ** 1 ** INT1
	NOT_ON_TIMER	, // PD 2 ** 2 ** UART_RX, INT2
	NOT_ON_TIMER	, // PD 3 ** 3 ** UART_TX, INT3
	NOT_ON_TIMER	, // PD 4 ** 4 ** AI8
	NOT_ON_TIMER	, // PD 5 ** 5 ** 
	TIMER4D	, // PD 6 ** 6 ** !PWMXD, AI9
	TIMER4D	, // PD 7 ** 7 ** PWMXD, AI10
	NOT_ON_TIMER	, // PB 0 ** 8 ** SPI_SS
	NOT_ON_TIMER	, // PB 1 ** 9 ** SPI_CLK
	NOT_ON_TIMER	, // PB 2 ** 10 ** SPI_MOSI
	NOT_ON_TIMER	, // PB 3 ** 11 ** SPI_MISO
	NOT_ON_TIMER	, // PB 4 ** 12 ** AI11
	NOT_ON_TIMER	, // PB 5 ** 13 ** AI12
	TIMER4B	, // PB 6 ** 14 ** PWMX, AI13
	TIMER0A	, // PB 7 ** 15 ** PWM
	NOT_ON_TIMER	,  // PF 0 ** 16 ** AI0
	NOT_ON_TIMER	, // PF 1 ** 17 ** AI1
	NOT_ON_TIMER	, // PF 4 ** 18 ** AI4
	NOT_ON_TIMER	, // PF 5 ** 19 ** AI5
	NOT_ON_TIMER	, // PF 6 ** 20 ** AI6
	NOT_ON_TIMER	, // PF 7 ** 21 ** AI7
	TIMER3A	, // PC 6 ** 22 ** PWM
	TIMER4A	, // PC 7 ** 23 ** PWM
	NOT_ON_TIMER	, // PE 2 ** 24 ** HWB
    NOT_ON_TIMER	, // PE 6 ** 25 ** INT6
};                            
#endif



/** Arduino Definitions for 32-pin USB AVRs */
/*  */

#if (defined(__AVR_AT90USB162__)  || defined(__AVR_AT90USB82__))

#define HWB_PIN 15

const uint16_t PROGMEM port_to_mode_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t)&DDRB,
	(uint16_t)&DDRC,
	(uint16_t)&DDRD,
};

const uint16_t PROGMEM port_to_output_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t)&PORTB,
	(uint16_t)&PORTC,
	(uint16_t)&PORTD,
};

const uint16_t PROGMEM port_to_input_PGM[] = {
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t)&PINB,
	(uint16_t)&PINC,
	(uint16_t)&PIND,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
	PD, // PD 0 ** 0 ** PWM, INT0
	PD, // PD 1 ** 1 ** INT1
	PD, // PD 2 ** 2 ** UART_RX, INT2
	PD, // PD 3 ** 3 ** UART_TX, INT3
	PD, // PD 4 ** 4 ** INT5
	PD, // PD 5 ** 5 ** 
	PD, // PD 6 ** 6 ** INT6
	PD, // PD 7 ** 7 ** HWB, INT7
	PB, // PB 0 ** 8 ** SPI_SS
	PB, // PB 1 ** 9 ** SPI_CLK
	PB, // PB 2 ** 10 ** SPI_MOSI
	PB, // PB 3 ** 11 ** SPI_MISO
	PB, // PB 4 ** 12 ** INT4
	PB, // PB 5 ** 13 ** 
	PB, // PB 6 ** 14 ** 
	PB, // PB 7 ** 15 ** PWM
	PC, // PC 2 ** 16 ** 
	PC, // PC 4 ** 17 ** 
	PC, // PC 5 ** 18 ** PWM
	PC, // PC 6 ** 19 ** PWM
	PC, // PC 7 ** 20 ** 
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
	_BV(0), // PD 0 ** 0 ** PWM, INT0
	_BV(1), // PD 1 ** 1 ** INT1
	_BV(2), // PD 2 ** 2 ** UART_RX, INT2
	_BV(3), // PD 3 ** 3 ** UART_TX, INT3
	_BV(4), // PD 4 ** 4 ** INT5
	_BV(5), // PD 5 ** 5 ** 
	_BV(6), // PD 6 ** 6 ** INT6
	_BV(7), // PD 7 ** 7 ** HWB, INT7
	_BV(0), // PB 0 ** 8 ** SPI_SS
	_BV(1), // PB 1 ** 9 ** SPI_CLK
	_BV(2), // PB 2 ** 10 ** SPI_MOSI
	_BV(3), // PB 3 ** 11 ** SPI_MISO
	_BV(4), // PB 4 ** 12 ** INT4
	_BV(5), // PB 5 ** 13 ** 
	_BV(6), // PB 6 ** 14 ** 
	_BV(7), // PB 7 ** 15 ** PWM
	_BV(2), // PC 2 ** 16 ** 
	_BV(4), // PC 4 ** 17 ** 
	_BV(5), // PC 5 ** 18 ** PWM
	_BV(6), // PC 6 ** 19 ** PWM
	_BV(7), // PC 7 ** 20 ** 
};               

const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
	// TIMERS		
	// -------------------------------------------
	TIMER0B , // PD 0 ** 0 ** PWM, INT0
	NOT_ON_TIMER	, // PD 1 ** 1 ** INT1
	NOT_ON_TIMER	, // PD 2 ** 2 ** UART_RX, INT2
	NOT_ON_TIMER	, // PD 3 ** 3 ** UART_TX, INT3
	NOT_ON_TIMER	, // PD 4 ** 4 ** INT5
	NOT_ON_TIMER	, // PD 5 ** 5 ** 
	NOT_ON_TIMER	, // PD 6 ** 6 ** INT6
	NOT_ON_TIMER	, // PD 7 ** 7 ** HWB, INT7
	NOT_ON_TIMER	, // PB 0 ** 8 ** SPI_SS
	NOT_ON_TIMER	, // PB 1 ** 9 ** SPI_CLK
	NOT_ON_TIMER	, // PB 2 ** 10 ** SPI_MOSI
	NOT_ON_TIMER	, // PB 3 ** 11 ** SPI_MISO
	NOT_ON_TIMER	, // PB 4 ** 12 ** INT4
	NOT_ON_TIMER	, // PB 5 ** 13 ** 
	NOT_ON_TIMER	, // PB 6 ** 14 ** 
	TIMER0A	, // PB 7 ** 15 ** PWM
	NOT_ON_TIMER	, // PC 2 ** 16 ** 
	NOT_ON_TIMER	, // PC 4 ** 17 ** 
	TIMER1B	, // PC 5 ** 18 ** PWM
	TIMER1A	, // PC 6 ** 19 ** PWM
	NOT_ON_TIMER	, // PC 7 ** 20 ** 
};                               
#endif
