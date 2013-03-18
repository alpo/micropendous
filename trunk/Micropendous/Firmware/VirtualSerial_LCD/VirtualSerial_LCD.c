/*
             LUFA Library
     Copyright (C) Dean Camera, 2013.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2013  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Updated for Loopback by Opendous Inc. 2013-03-16
  www.Micropendous.org/VirtualSerial

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaims all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

/** \file
 *
 *  Main source file for the VirtualSerial demo. This file contains the main tasks of
 *  the demo and is responsible for the initial application hardware configuration.
 */

#include "VirtualSerial_LCD.h"


// Global buffer for use with STDIO functions
volatile char buffer[CDC_TXRX_EPSIZE];


/** LUFA CDC Class driver interface configuration and state information. This structure is
 *  passed to all CDC Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_CDC_Device_t VirtualSerial_CDC_Interface =
	{
		.Config =
			{
				.ControlInterfaceNumber   = 0,
				.DataINEndpoint           =
					{
						.Address          = CDC_TX_EPADDR,
						.Size             = CDC_TXRX_EPSIZE,
						.Banks            = 1,
					},
				.DataOUTEndpoint =
					{
						.Address          = CDC_RX_EPADDR,
						.Size             = CDC_TXRX_EPSIZE,
						.Banks            = 1,
					},
				.NotificationEndpoint =
					{
						.Address          = CDC_NOTIFICATION_EPADDR,
						.Size             = CDC_NOTIFICATION_EPSIZE,
						.Banks            = 1,
					},
			},
	};

/** Standard file stream for the CDC interface when set up, so that the virtual CDC COM port can be
 *  used like any regular character stream in the C APIs
 */
static FILE USBSerialStream;

// global variable for use with HD44780 LCD display
volatile uint8_t lcd_character_count;
volatile uint8_t lcd_current_line;

/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */
int main(void)
{
	SetupHardware();

	/* Create a regular character stream for the interface so that it can be used with the stdio.h functions */
	CDC_Device_CreateStream(&VirtualSerial_CDC_Interface, &USBSerialStream);

	GlobalInterruptEnable();

	for (;;)
	{
		MainTask();

		/* Must throw away unused bytes from the host, or it will lock up while waiting for the device */
		//CDC_Device_ReceiveByte(&VirtualSerial_CDC_Interface);

		CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
		USB_USBTask();
	}
}

/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware(void)
{
	// Disable watchdog if enabled by bootloader/fuses
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	// Disable clock division
	clock_prescale_set(clock_div_1);

	// Disable JTAG; allows upper nibble of Port F to be used as GPIO
	#if (	defined(__AVR_AT90USB1287__) || defined(__AVR_AT90USB647__) ||  \
			defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB646__) ||  \
			defined(__AVR_ATmega16U4__)  || defined(__AVR_ATmega32U4__) ||  \
			defined(__AVR_ATmega32U6__)	)
		JTAG_DISABLE();
	#endif


	// Hardware Initialization
	// 16-bit Timer1 Initialization
	TCCR1A = 0; //start the timer
	//TCCR1B = (1 << CS10); // no Timer1 prescaling, use CLK
	TCCR1B = (1 << CS12); // prescale Timer1 by CLK/256
	// 8000000 / 256 = 31250 ticks per second
	// 16-bit = 2^16 = 65536 maximum ticks for Timer1
	// 65536 / 31250 =~ 2.1
	// so Timer1 will overflow back to 0 about every 2 seconds
	uint16_t Timer1Val = TCNT1; // get current Timer1 value
	Timer1Val = 345;

	// AVRlib HD44780 LCD initialization
	lcdInit();
	rprintfInit(lcdDataWrite); // direct rprintf to LCD
	lcdClear(); // clear the LCD
	rprintf("GO AVRlib!");

	// Hardware Initialization
	LEDs_Init();
	LEDs_TurnOnLEDs(LEDS_LED1);
	DISABLE_VOLTAGE_TXRX(); // used on Micropendous REV1/2 boards
	DISABLE_EXT_SRAM(); // used on Micropendous REV1/2 boards
	SELECT_USB_B(); // needed for Micropendous REV1/2 boards
	USB_Init();
}

/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{
	LEDs_SetAllLEDs(LEDMASK_USB_ENUMERATING);
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{
	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	bool ConfigSuccess = true;

	ConfigSuccess &= CDC_Device_ConfigureEndpoints(&VirtualSerial_CDC_Interface);

	LEDs_SetAllLEDs(ConfigSuccess ? LEDMASK_USB_READY : LEDMASK_USB_ERROR);
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
	CDC_Device_ProcessControlRequest(&VirtualSerial_CDC_Interface);
}


uint16_t firstNumber = 12345;

void MainTask(void)
{
	int     tempInt  = 0;
	uint8_t tempChar = 0;

	// example usage
	if (firstNumber == 12345) {
		lcdClear();
		rprintfChar('#'); // print a single character to the LCD
		// print a value to the LCD in Base-10 using 5 digits.  It is FALSE that the
		// number can be negative (so it is positive) and use '0' to pad to 5 digits
		rprintfNum(10, 5, FALSE, '0', firstNumber);
		rprintf(" ABC"); // print some text to the LCD
		lcdGotoXY(0, 1); // go to the first character of the second line (counting starts at 0)
		lcdProgressBar(100, 255, 16); // display a progress bar of 100/255 using 16 characters from current XY position
		lcdGotoXY(0, 0); // return to start position
		firstNumber = 0; // do not run this if (...) code again
	}

	// while there are characters in the USB stream, write them to LCD
	// Each character is written sequentially,
	// first to the top line, then to the next, etc...
	while ((tempInt = fgetc(&USBSerialStream)) > 0)
	{
		tempChar = (uint8_t)tempInt;
		// send char back for echo'ing on terminal
		fputc(tempChar, &USBSerialStream);

		lcd_character_count++;
		rprintfChar(tempChar); // print char to the LCD
		// once we hit the end of a single line, loop to next line
		if (lcd_character_count >= LCD_LINE_LENGTH)
		{
			lcd_character_count = 0;
			lcd_current_line++;
			// once we hit the end of the last line, loop to the first line
			if (lcd_current_line >= LCD_LINES)
			{
				lcd_current_line = 0;
			}

			lcdGotoXY(0, lcd_current_line);
		}
	}


	// If HWB Button is pressed then send formatted strings over VirtualSerial and to LCD
	if (Buttons_GetStatus()) {
		fprintf_P(&USBSerialStream, PSTR("\r\nHWB has been pressed!\r\n")); // send a constant string stored in FLASH
		fprintf(&USBSerialStream, "PORTB = %3x\r\n", PINB); // send a string that is dynamic and stored in SRAM
		rprintf("PORTB=0x");
		// print a value to the LCD in Base-16 (Hex) using 2 digits.  It is FALSE that the
		// number can be negative (so it is positive) and use '0' to pad to 2 digits
		rprintfNum(16, 2, FALSE, '0', PINB);
	}

}
