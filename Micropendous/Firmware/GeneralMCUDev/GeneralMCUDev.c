/*  General Purpose MCU Development Demo
    (C) 2010-03-10 By Opendous Inc. - www.Micropendous.org

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
	uint8_t tempByte = 0;

	/* initialize the board for general purpose non-usb AVR development */
	SetupHardware();

	/* TODO - Custom Hardware Initialization */

	/* Initialize IO ports */
	/* For now we will set up half of PortD as input and the other half as output  */
	/* PD4,5,6,7 will be input (DDR=0, PORT=0) and PD0,1,2,3 will be output with a 1 start state (DDR=1, PORT=1) */
	DDRD =  (1 << PD0) | (1 << PD1) | (1 << PD2) | (1 << PD3) | (0 << PD4) | (0 << PD5) | (0 << PD6) | (0 << PD7);
	PORTD = (1 << PD0) | (1 << PD1) | (1 << PD2) | (1 << PD3) | (0 << PD4) | (0 << PD5) | (0 << PD6) | (0 << PD7);


	/* Initialize the HWB Button as input, but after IO Ports to be certain HWB is input */
	BUTTON1_Init();


	/* TODO - this is the main program loop where your code should go */
	while (1) {

        // if Button1 (HWB) is pressed turn on a LED connected to PD0
		if (BUTTON1_GetStatus() > 0) {
			// turn on a LED connected to PORTD Pin 0 - PD0
			PORTD |= (1 << PD0);
		} else {
			// turn off a LED connected to PORTD Pin 0 - PD0
			PORTD &= ~(1 << PD0);
		}

	} // end while(1)

}



/* To use an USB AVR board as a General Purpose Microcontroller Development board */
/*   simply disable the USB system */
void SetupHardware(void)
{
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Disable clock division */
	clock_prescale_set(clock_div_1);

	/* disable USB */
	USBCON = ((0 << USBE) | (1 << FRZCLK));
	PRR1 |= (1 << PRUSB);

	/* disable JTAG to allow corresponding pins to be used - PF4, PF5, PF6, PF7 */
	/* TODO - remove this if you want to use your JTAG debugger to debug this firmware */
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

	/* Hardware Initialization */
	/* enable Ports based on which IC is being used */
	/* For more information look over the corresponding AVR's datasheet in the
		'I/O Ports' Chapter under subheading 'Ports as General Digital I/O' */
	#if (defined(__AVR_AT90USB162__)  || defined(__AVR_AT90USB82__))
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

	/* Hardware Initialization */
	LEDs_Init();

}
