/*
             LUFA Library
     Copyright (C) Dean Camera, 2010.
              
  dean [at] fourwalledcubicle [dot] com
      www.fourwalledcubicle.com
*/

/*
  USB Virtual Serial to HD44780-based LCD Firmware

  Copyright 2010  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Altered for USBVirtualSerial-LCD by Opendous Inc. 2010-03
  For more information visit:  www.Micropendous.org

  Look for TODO statements in the code for implementation hints

  Use stdio functions (getchar, putchar, printf) for communicating
  over the virtual serial port and use avrlib's rprintf functions
  for printing text to the LCD.

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

/** \file
 *
 *  Main source file for the USBtoSerial project. This file contains the main tasks of
 *  the demo and is responsible for the initial application hardware configuration.
 */

#include "USBVirtualSerial-LCD.h"

/* The AT90USBXX2/ATmegaXXU2 do not support multiply operations which lcdProgressBar requires */
#if (defined(__AVR_AT90USB162__) || defined(__AVR_AT90USB82__))
#define lcdProgressBar(x, y, z)		__asm__ __volatile__ ("nop");
#endif

/** Circular buffer to hold data from the host before it is processed by MainTask using standard IO functions */
RingBuff_t Host_to_Device_Buffer;

/** Circular buffer to hold data before it is sent to the host. */
RingBuff_t Device_to_Host_Buffer;

/** Global 16-bit integer to store the current value of Timer1 */
volatile uint16_t Timer1Val = 0;

/* some global variables used for LCD code */
volatile uint8_t character_count = 0;
volatile uint8_t current_line = 0;

/** LUFA CDC Class driver interface configuration and state information. This structure is
 *  passed to all CDC Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_CDC_Device_t VirtualSerial_CDC_Interface =
	{
		.Config = 
			{
				.ControlInterfaceNumber         = 0,

				.DataINEndpointNumber           = CDC_TX_EPNUM,
				.DataINEndpointSize             = CDC_TXRX_EPSIZE,
				.DataINEndpointDoubleBank       = false,

				.DataOUTEndpointNumber          = CDC_RX_EPNUM,
				.DataOUTEndpointSize            = CDC_TXRX_EPSIZE,
				.DataOUTEndpointDoubleBank      = false,

				.NotificationEndpointNumber     = CDC_NOTIFICATION_EPNUM,
				.NotificationEndpointSize       = CDC_NOTIFICATION_EPSIZE,
				.NotificationEndpointDoubleBank = false,
			},
	};

/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */
int main(void)
{
	SetupHardware();
	
	Buffer_Initialize(&Host_to_Device_Buffer);
	Buffer_Initialize(&Device_to_Host_Buffer);

	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);

	for (;;)
	{
		/* Read bytes from the USB OUT endpoint into the local buffer */
		for (uint8_t DataBytesRem = CDC_Device_BytesReceived(&VirtualSerial_CDC_Interface); DataBytesRem != 0; DataBytesRem--)
		{
			if (!(BUFF_STATICSIZE - Host_to_Device_Buffer.Elements))
				break;
			Buffer_StoreElement(&Host_to_Device_Buffer, CDC_Device_ReceiveByte(&VirtualSerial_CDC_Interface));
		}

		/* Read bytes from the data buffer into the USB IN endpoint */
		while (Device_to_Host_Buffer.Elements)
			CDC_Device_SendByte(&VirtualSerial_CDC_Interface, Buffer_GetElement(&Device_to_Host_Buffer));

		CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
		USB_USBTask();

		MainTask();
	}
}

/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware(void)
{
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Disable clock division */
	clock_prescale_set(clock_div_1);

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

	/* Initialize stdout and stdin for printf and scanf */
	fdevopen(sendData, getData);

	/* 16-bit Timer1 Initialization */
	TCCR1A = 0; //start the timer
	//TCCR1B = (1 << CS10); // no Timer1 prescaling, use CLK
	TCCR1B = (1 << CS12); // prescale Timer1 by CLK/256
	// 8000000 / 256 = 31250 ticks per second
	// 16-bit = 2^16 = 65536 maximum ticks for Timer1
	// 65536 / 31250 =~ 2.1
	// so Timer1 will overflow back to 0 about every 2 seconds
	Timer1Val = TCNT1; // get current Timer1 value

	/* initialize avrlib LCD functionality */
	lcdInit();
	rprintfInit(lcdDataWrite); // direct rprintf to LCD
	lcdClear();
	rprintf("GO AVRlib!");

	/* Hardware Initialization */
	LEDs_Init();
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
	LEDs_SetAllLEDs(LEDMASK_USB_READY);

	if (!(CDC_Device_ConfigureEndpoints(&VirtualSerial_CDC_Interface)))
	  LEDs_SetAllLEDs(LEDMASK_USB_ERROR);
}

/** Event handler for the library USB Unhandled Control Request event. */
void EVENT_USB_Device_UnhandledControlRequest(void)
{
	CDC_Device_ProcessControlRequest(&VirtualSerial_CDC_Interface);
}

/** Event handler for the CDC Class driver Line Encoding Changed event.
 *
 *  \param[in] CDCInterfaceInfo  Pointer to the CDC class interface configuration structure being referenced
 */
void EVENT_CDC_Device_LineEncodingChanged(USB_ClassInfo_CDC_Device_t* const CDCInterfaceInfo)
{
	/* TODO - Use Virtual Serial Port settings to control your application.
		Think of this as another data channel.  Use it for control/status messaging.
		Leave the data channel (MainTask putchar/getchar) for data only.
	*/

	switch (CDCInterfaceInfo->State.LineEncoding.ParityType)
	{
		case CDC_PARITY_Odd:
			// do something here
			break;
		case CDC_PARITY_Even:
			// and/or maybe here
			break;
		case CDC_PARITY_None:
			// maybe something here
			break;
		case CDC_PARITY_Mark:
			// something here could work
			break;
		case CDC_PARITY_Space:
			// you guessed it, something could go here
			break;
	}


	switch (CDCInterfaceInfo->State.LineEncoding.CharFormat)
	{
		case CDC_LINEENCODING_OneStopBit:
			// do something here
			break;
		case CDC_LINEENCODING_OneAndAHalfStopBits:
			// and/or maybe do something here
			break;
		case CDC_LINEENCODING_TwoStopBits:
			// something here could work
			break;
	}


	switch (CDCInterfaceInfo->State.LineEncoding.DataBits)
	{
		case 6:
			// possibly something
			break;
		case 7:
			// possibly something
			break;
		case 8:
			// possibly something
			break;
	}


	switch (CDCInterfaceInfo->State.LineEncoding.BaudRateBPS)
	{
		case 9600:
			// possibly something
			break;
		case 14400:
			// possibly something
			break;
		case 19200:
			// possibly something
			break;
		case 38400:
			// possibly something
			break;
		case 57600:
			// possibly something
			break;
		case 115200:
			// possibly something
			break;
	}

}



/** CDC class driver event for a control line state change on a CDC interface. This event fires each time the host requests a
 *  control line state change (containing the virtual serial control line states, such as DTR). The new control line states
 *  are available in the ControlLineStates.HostToDevice value inside the CDC interface structure passed as a parameter, set as
 *  a mask of CDC_CONTROL_LINE_OUT_* masks.  1 is for 'Set'(Low) and 0 is for 'Clear'(High) as these are active low signals.
 *  \param[in,out] CDCInterfaceInfo  Pointer to a structure containing a CDC Class configuration and state
*/
void EVENT_CDC_Device_ControLineStateChanged(USB_ClassInfo_CDC_Device_t* const CDCInterfaceInfo)
{
	/* TODO - Use Virtual Serial Port settings to control your application.
		Think of this as another data channel.  Use it for control/status messaging.
		Leave the data channel (MainTask putchar/getchar) for data only.
	*/

	if ((CDCInterfaceInfo->State.ControlLineStates.HostToDevice) & CDC_CONTROL_LINE_OUT_RTS) {
		// Host has set the RTS line
	} else {
		// Host has cleared the RTS line
	}

	if ((CDCInterfaceInfo->State.ControlLineStates.HostToDevice) & CDC_CONTROL_LINE_OUT_DTR) {
		// Host has set the DTR line
	} else {
		// Host has cleared the DTR line
	}
}


/* Use this function to make sure data exists.
	Need this function as RingBuff will crash if getData is called on
	an empty buffer
*/
uint8_t haveData(void) {
	return (uint8_t)(Host_to_Device_Buffer.Elements);
}

/* In order to use printf functions, need a function that will send data over
	the USB Virtual Serial Port link
	return 0 on success, else failure and ensure binary compatibility
*/
static int sendData(char c, FILE *stream) {
	// most terminals require \r\n
	// however, do not include this conversion as it will break binary transfers 
	//if (c == '\n') {
	//	sendData('\r', stream);
	//}
	Buffer_StoreElement(&Device_to_Host_Buffer, (uint8_t)c);
	return 0;
}

/* Also require a function to receive data from the USB Virtual Serial Port link */
int getData(FILE *__stream) {
	//if (something) return _FDEV_ERR; // cannot implement as GetElement has no error condition
	if (haveData() == 0) {
		return _FDEV_EOF;
	}
	return (int)Buffer_GetElement(&Host_to_Device_Buffer);
}


uint16_t firstNumber = 12345;

/* MainTask is the main application loop */
void MainTask(void)
{
	uint8_t tempChar = 0;	// temporary storage - unsigned 8-bit integer

	if (firstNumber == 12345) {
		lcdClear();
		rprintfChar('#'); // print a single character to the LCD
		// print a value to the LCD in Base-10 using 5 digits.  It is FALSE that the
		// number can be negative (it is positive) and use '0' to pad to 5 digits
		rprintfNum(10, 5, FALSE, '0', firstNumber);
		rprintf(" ABC"); // print some text to the LCD
		lcdGotoXY(0, 1); // go to the first character of the second line (counting starts at 0)
		lcdProgressBar(100, 255, 16); // display a progress bar of 100/255 using 16 characters
		lcdGotoXY(0, 0); // return to start position
		firstNumber = 0; // do not run this if (...) code again
	}

	// while the buffer has elements, write them to the LCD
	while (haveData()) {
		/* Basically each character is written sequentially, first to the top line, then to the next, etc... */
		tempChar = getchar();
		putchar(tempChar); // send char back for echo'ing on terminal
		character_count++;
		rprintfChar(tempChar); // print char to the LCD
		// once we hit the end of a single line, loop to next line
		if (character_count >= LCD_LINE_LENGTH) {
			character_count = 0;
			current_line++;
			// once we hit the end of the last line, loop to the first line
			if (current_line >= LCD_LINES) {
				current_line = 0;
			}
			lcdGotoXY(0, current_line);
		}
	}
}
