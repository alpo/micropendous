/*
             LUFA Library
     Copyright (C) Dean Camera, 2010.
              
  dean [at] fourwalledcubicle [dot] com
      www.fourwalledcubicle.com
*/

/*
  Copyright 2010  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Altered for USBVirtualSerial-I2C by Opendous Inc. 2011-02
  For more information visit:  www.Micropendous.org/USBVirtualSerial_I2C

  Usage Protocol is:
  <R/^W , DeviceAddress , Sub-Address/Register , DataLength , DataBytesArray>
  Over a serial terminal or using serial software:
  1st byte to send is read/write bit, where 1 is write and 0 is read
  2nd byte is the proper non-RW-adjusted device address from its' datasheet
  3rd byte is the Sub-Address or Register address inside the device
  4th byte is length of data
  5th and later bytes are the data array to send

  Note the firmware is currently set up for a maximum of 32 bytes of data

  Receive(read data from I2C device):
    Note byte array (5th and later bytes) is ignored
    This firmware will return: 1st byte will be length of data, then the data
  Send(write data to I2C device):
    Just takes the array and sends it

  Look for TODO statements in the code for implementation hints

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

#include "USBVirtualSerial-I2C.h"

/* What is the default main clock division value */

#if (F_CLOCK == 16000000)
	#define CLOCK_DIV		clock_div_16
	#define DEFAULT_CLOCK_DIV		clock_div_2
#else
	#define CLOCK_DIV		clock_div_4
	#define DEFAULT_CLOCK_DIV		clock_div_1
#endif

#define PINHIGH(PORT, PIN)		PORT |= (1 << PIN);
#define PINLOW(PORT, PIN)		PORT &= ~(1 << PIN);

/** Circular buffer to hold data from the host before it is processed by MainTask using standard IO functions */
RingBuff_t Host_to_Device_Buffer;

/** Circular buffer to hold data before it is sent to the host. */
RingBuff_t Device_to_Host_Buffer;

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


/** Global 16-bit integer to store the current value of Timer1 */
volatile uint16_t Timer1Val = 0;

/* global variables */
uint8_t i2cData[32]; // to speed things up, inititalize only once


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
	clock_prescale_set(DEFAULT_CLOCK_DIV);

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

	/* Initialize stdout and stdin for printf and scanf */
	fdevopen(sendData, getData);

	/* initialize I2C functionality */
	DDRD = ((1 << PD0) | (1 << PD1));
	PORTD = ((0 << PD0) | (0 << PD1));

	#if (0)
//	#if (defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB646__) || \
//		defined(__AVR_AT90USB1287__) || defined(__AVR_AT90USB647__) || \
//		defined(__AVR_ATmega16U4__)  || defined(__AVR_ATmega32U4__) || \
//		defined(__AVR_ATmega32U6__))
		// use hardware I2C driver
		
	#else	// at90usb162, at90usb82
		// use avrlib software I2C
		i2cInit();
	#endif

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
	/* Use Virtual Serial Port settings to control your application.
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
	/* Use Virtual Serial Port settings to control your application.
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



/* MainTask will run once initialization is complete */
void MainTask(void)
{
	uint8_t  i = 0;
	uint8_t  i2cRW = 0;
	uint8_t  i2cAddress = 0;
	uint8_t  i2cAddressRWAdjusted = 0;
	uint8_t  i2cRegister = 0;
	uint8_t  i2cDataLength = 0;
	uint8_t  i2cExpectedLength = 0;


	#if (0)
//	#if ((defined(__AVR_AT90USB1287__) || defined(__AVR_AT90USB647__) ||  \
//		defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB646__) ||  \
//		defined(__AVR_ATmega16U4__)  || defined(__AVR_ATmega32U4__) ||  \
//		defined(__AVR_ATmega32U6__)))
		// use hardware I2C
		// TODO	- implement hardware I2C, but for now always use software
		//				- for some reason TWINT interrupt flag never gets set

	#else
		// use software I2C for the AT90USBxx2

		// Check if there is enough data to fit the protocol
		// if it contains more than 4 bytes, then we have data to process
		if (haveData() >= 5)
		{
			i2cInit();  // re-initialize the I2C bus and...
			i = 254; while (i > 0) { i--; }  // ... delay a short while to to let it settle
			i2cRW = (uint8_t)getData(stdin);
			i2cAddress = (uint8_t)getData(stdin);
			i2cAddressRWAdjusted = (i2cAddress << 1); // avrlib I2Csw takes care of R/W bit
			i2cDataLength = (uint8_t)getData(stdin);
            i2cExpectedLength = (uint8_t)getData(stdin);
			i2cRegister = (uint8_t)getData(stdin);
			for (i = 0; i < (i2cDataLength - 1); i++) {
				i2cData[i] = (uint8_t)getData(stdin);
			}

			// RW -> 1 for READ, 0 for write
			if (i2cRW == 0) { // want to write data
                sendData((char)0, stdout);
				clock_prescale_set(CLOCK_DIV); // avrlib I2Csw was written for a 2MHz clock
				i2cSend(i2cAddressRWAdjusted, i2cRegister, (i2cDataLength - 1), i2cData);
				clock_prescale_set(DEFAULT_CLOCK_DIV); // return to full clock
			} else { // i2cRW == 1, so we want to read data
				// first byte returned to host is number of subsequent data bytes
                sendData((char)i2cExpectedLength, stdout);

				clock_prescale_set(CLOCK_DIV); // avrlib I2Csw was written for a 2MHz clock
				i2cReceive(i2cAddressRWAdjusted, i2cRegister, i2cExpectedLength, i2cData);
				clock_prescale_set(DEFAULT_CLOCK_DIV); // return to full clock
				// copy the data to the transmit buffer
				for (i = 0; i < i2cExpectedLength; i++) {
                    sendData((char)i2cData[i], stdout);
				}
			}
		}
	#endif

}
