/*  General Purpose MCU Development Demo
    (C) 2011-02-25 By Opendous Inc. - www.Micropendous.org

  Look for TODO comments for implementation hints.

  Permission to use, copy, modify, distribute, and sell this 
  software and its documentation for any purpose is hereby granted
  without fee, provided that the name of the author not be used in 
  advertising or publicity pertaining to distribution of the 
  software without specific, written prior permission.

  The author disclaim all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

#include "GeneralMCUDev.h"


int main(void)
{
	uint16_t timer1val = 0;

	// initialize the board for general purpose non-usb AVR development
	SetupHardware();

	// enable pin PD6 as output
	DDRD |= (1 << PD6);
	PORTD &= ~(1 << PD6);

	//start the 16-bit timer
	TCCR1A = 0;
	TCCR1B = ((1 << CS02) | (0 << CS01) | (0 << CS00)); // prescale Timer0 clock by CLK/512
	timer1val = TCNT1; // read Timer1 value to start the timer

	// TODO - this is the main program loop where your code should go
	while (1) {
		// if Timer1 passes 16000 then set PD6 high, else leave it low to flash an LED connected to PD6
		timer1val = TCNT1;
		if (timer1val > 16000) {
			PORTD &= ~(1 << PD6); // set PD6 low
		} else {
			PORTD |= (1 << PD6); // set PD6 high
		}
	}

}



// To use an USB AVR board as a General Purpose Microcontroller Development board
//   simply disable the USB system
void SetupHardware(void)
{
	// Disable watchdog if enabled by bootloader/fuses
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	// Disable clock division
	clock_prescale_set(clock_div_1);

	// disable USB
	USBCON = ((0 << USBE) | (1 << FRZCLK));
	PRR1 |= (1 << PRUSB);

	// disable JTAG to allow corresponding pins to be used - PF4, PF5, PF6, PF7
	// TODO - remove this if you want to use your JTAG debugger to debug this firmware
	#if ((defined(__AVR_AT90USB1287__) || defined(__AVR_AT90USB647__) ||  \
			defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB646__) ||  \
			defined(__AVR_ATmega16U4__)  || defined(__AVR_ATmega32U4__) ||  \
			defined(__AVR_ATmega32U6__)))
		// note the JTD bit must be written twice within 4 clock cycles to disable JTAG
		// you must also set the IVSEL bit at the same time, which requires IVCE to be set first
		// port pull-up resistors are enabled - PUD(Pull Up Disable) = 0
		MCUCR = (1 << JTD) | (1 << IVCE) | (0 << PUD);
		MCUCR = (1 << JTD) | (0 << IVSEL) | (0 << IVCE) | (0 << PUD);
	#endif

	// Hardware Initialization
	// enable Ports based on which IC is being used
	// For more information look over the corresponding AVR's datasheet in the
	//	'I/O Ports' Chapter under subheading 'Ports as General Digital I/O'
	#if (defined(__AVR_AT90USB162__) || defined(__AVR_AT90USB82__) || \
			defined(__AVR_ATmega16U2__) || defined(__AVR_ATmega32U2__))
		DDRD = 0;
		PORTD = 0;
		DDRB = 0;
		PORTB = 0;
		DDRC = 0;
		PORTC |= (0 << PC2) | (0 << PC4) | (0 << PC5) | (0 << PC6) | (0 << PC7); //only PC2,4,5,6,7 are pins
		// be careful using PortC as PC0 is used for the Crystal and PC1 is RESET
	#endif

	#if (defined(__AVR_ATmega16U4__)  || defined(__AVR_ATmega32U4__))
		DDRD = 0;
		PORTD = 0;
		DDRB = 0;
		PORTB = 0;
		DDRC = 0;
		PORTC = (0 << PC6) | (0 << PC7); //only PC6,7 are pins
		DDRE = 0;
		PORTE = (0 << PE2) | (0 << PE6); //only PE2,6 are pins
		DDRF = 0;
		PORTF = (0 << PF0) | (0 << PF1) | (0 << PF4) | (0 << PF5) | (0 << PF6) | (0 << PF7); // only PF0,1,4,5,6,7 are pins
	#endif

	#if (defined(__AVR_AT90USB1287__) || defined(__AVR_AT90USB647__) ||  \
			defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB646__) ||  \
			defined(__AVR_ATmega32U6__))
		DDRA = 0;
		PORTA = 0;
		DDRB = 0;
		PORTB = 0;
		DDRC = 0;
		PORTC = 0;
		DDRD = 0;
		PORTD = 0;
		DDRE = ((1 << PE4) | (1 << PE6));	// set PE4, PE6 to HIGH to disable external SRAM, if connected
		PORTE = ((1 << PE4) | (1 << PE6));	// set PE4, PE6 to HIGH to disable external SRAM, if connected
		DDRF = 0;
		PORTF = 0;
	#endif

	// Hardware Initialization
	LEDs_Init();

}
