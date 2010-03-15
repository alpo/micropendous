/*  AVR Firmware to test SPI Communication
    (C) 2010-03-13 By Opendous Inc. - www.Micropendous.org

  Connect your AVR programmed with this firmware to a
  Micropendous (or other USB AVR based board) programmed
  with USBVirtualSerial-SPI firmware and run:
      python Test_SPI_AVRISP_Test_Communication.py COM5
  Change COM5 to the port your USB AVR enumerated to.

  This firmware continually sends 3bytes of data over its
  USI port in SPI mode.  A 0xAA first byte, then the upper
  byte of its Timer0 value, then the lower byte.  There is no
  SPI Chip Select (nCS/SS).  The data register is always
  ready to send data.

  Permission to use, copy, modify, distribute, and sell this 
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in 
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting 
  documentation, and that the name of the author not be used in 
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

#include "SPI_AVRISP_Test.h"

/* global variables */
uint16_t timer0val = 0;
uint8_t spiWhichByte = 3;
uint8_t firstRun = 1;


int main(void)
{
	uint8_t tempByte = 0;

	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Disable clock division */
	clock_prescale_set(clock_div_4);

	/* Initialize IO ports */
	/* Set up IO Pins for SPI - PB2(SCK), PB0(MOSI), PB3(nCS) are input and PB1(MISO) is output */
	DDRB = ((0 << PB0) | (1 << PB1) | (0 << PB2) | (0 << PB3));
	PORTB = ((0 << PB0) | (0 << PB1) | (0 << PB2) | (0 << PB3));

	/* Initialize 16-bit Timer0 */
	TCCR0A = (1 << TCW0); //start the 16-bit timer
	TCCR0B = ((1 << CS02) | (0 << CS01) | (1 << CS00)); // prescale Timer0 clock by CLK/1024
	timer0val = TCNT0L; // read Timer0 value to make sure it has started
	timer0val = ((uint8_t)TCNT0H << 8);

	/* enable interrupts */
	sei();

	/* Set up USI for Three-Wire (SPI) Mode, Externally Clocked (SPI-Mode0), Enable Transfer Complete Interrupt*/
	USICR = ((1 << USIOIE) | (0 << USIWM1) | (1 << USIWM0) | (1 << USICS1) | (0 << USICS0) | (0 << USICLK));
	USIPP = (0 << USIPOS); // USI Pin Position is 0: PB0,PB1,PB2
	tempByte = USISR; // read USI Status Register to enable USI
	USIDR = 0xAA; // USIDR needs initial data to start Transfer Complete Interrupt (USI_OVF)

	/* main program loop */
	while (1) {
		// nothing to do as everything is handled in the interrupt
		tempByte++;
	} // end while(1)

}


/* USI (SPI-mode) Transfer Complete Interrupt */
ISR(USI_OVF_vect)
{
	uint8_t sreg = 0;

	// Want to send current Timer0 value over USI in SPI Mode
	// This interrupt sets up for the next transfer
	if (spiWhichByte == 1) {
		spiWhichByte = 2;
		USIDR = ((uint8_t)(timer0val)); // send upper byte
	} else if (spiWhichByte == 2) {
		spiWhichByte = 3;
		USIDR = ((uint8_t)(timer0val >> 8)); // send lower byte
	} else { // (spiWhichByte == 3)
		sreg = SREG;
		cli();
		timer0val = TCNT0L; // capture current Timer0 value
		timer0val = ((uint8_t)TCNT0H << 8);
		SREG = sreg;
		spiWhichByte = 1;
		if (firstRun == 1) {
			// do nothing, data already placed in USIDR in main()
			firstRun = 0;
		} else {
			USIDR = 0xAA;
		}
	}
}
