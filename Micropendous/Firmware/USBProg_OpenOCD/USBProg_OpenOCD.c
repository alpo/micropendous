/*
             LUFA Library
     Copyright (C) Dean Camera, 2009.
              
  dean [at] fourwalledcubicle [dot] com
      www.fourwalledcubicle.com
*/

/*
    Based on code by Dean Camera (dean [at] fourwalledcubicle [dot] com)
    Based on code by Benedikt Sauter for USBProg - www.embedded-projects.net
    USBProg OpenOCD porting to LUFA by Opendous Inc. - www.opendous.org
    For more info visit: www.Micropendous.org/USBProg_OpenOCD

  Permission to use, copy, modify, and distribute this software
  and its documentation for any purpose and without fee is hereby
  granted, provided that the above copyright notice appear in all
  copies and that both that the copyright notice and this
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

/*
	TODO - current error when running python DeviceAccessPy.py:
		final bulk_read fails due to timeout - processIN is not running?

	TODO - current error when running OpenOCD 0.3.1:
		Info : JTAG tap: lpc1768.cpu tap/device found: 0x000000ff (mfg: 0x07f, part: 0x0000, ver: 0x0)
		Warn : JTAG tap: lpc1768.cpu       UNEXPECTED: 0x000000ff (mfg: 0x07f, part: 0x0000, ver: 0x0)
		Error: JTAG tap: lpc1768.cpu  expected 1 of 1: 0x4ba00477 (mfg: 0x23b, part: 0xba00, ver: 0x4)
		Warn : Unexpected idcode after end of chain: 480 0xa00477ff
		Warn : Unexpected idcode after end of chain: 512 0x0000004b
		Error: double-check your JTAG setup (interface, speed, missing TAPs, ...)

	USBProg Pinout (defined in usbprogjtag.h)
	BIT0	PB6	// TDO
	BIT1	PB0	// TMS
	BIT2	PB7	// TCK
	BIT3	PB5	// TDI
	BIT4	PB3	// TRST = JTAG RESET
	BIT5	PB4	// SRST = RESET
	BIT6	PD4

*/

/** \file
 *
 *  Main source file for the LoopBack demo. This file contains the main tasks of the demo and
 *  is responsible for the initial application hardware configuration.
 */
 
#include "USBProg_OpenOCD.h"

/* Global Variables */
volatile char dataReceived[OUT_EP_SIZE];
volatile char answer[64];
volatile uint16_t jtag_SPEED = 255;
const uint8_t  firmwareVersion[4] = {0x11,0x22,0x33,0x44};

volatile uint8_t answerNeedsToBeSentFlag = 0;
volatile uint16_t  sendAnswerLength = 0;
volatile uint8_t  processCommands = 0;


/** Main program entry point. This routine configures the hardware required by the application, then
 *  starts the scheduler to run the USB management task.
 */
int main(void)
{
	SetupHardware();

	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);

	for (;;)
	{
		// an IN packet always follows an OUT or another IN
		// a bulk_read always follows a bulk_write but some bulk_writes do not have reads after them
		// note that OpenOCD 'while' loops with bulk_read until a packet with > 0 bytes arrives
		// TODO - figure out how to comply with all OpenOCD transfer possibilities for USBProg
		// TODO - better yet, figure out the complete protocol

		USB_USBTask();
		processOUT();
		if (processCommands == 1) {
			Commands((char *)dataReceived);
			processCommands = 0;
		}
		USB_USBTask();
		if (answerNeedsToBeSentFlag == 1) {
			processIN();
		}
	}

}

/** Configures the board hardware and chip peripherals for this firmware's functionality. */
void SetupHardware(void)
{
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Disable clock division */
	clock_prescale_set(clock_div_1);

	/* disable JTAG to allow corresponding pins to be used - PF4, PF5, PF6, PF7 */
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
//		DDRB = 0;
//		PORTB = 0;
		DDRC = 0;
		PORTC |= ((0 << PC2) | (0 << PC4) | (0 << PC5) | (0 << PC6) | (0 << PC7)); //only pins PC2,4,5,6,7 exist
		// be careful using PortC as PC0 is used for the Crystal and PC1 is RESET
	#endif

	#if (defined(__AVR_ATmega16U4__)  || defined(__AVR_ATmega32U4__))
		DDRD = 0;
		PORTD = 0;
//		DDRB = 0;
//		PORTB = 0;
		DDRC = 0;
		PORTC = ((0 << PC6) | (0 << PC7)); //only pins PC6,7 exist
		DDRE = 0;
		PORTE = ((0 << PE2) | (0 << PE6)); //only pins PE2,6 exist
		DDRF = 0;
		PORTF = ((0 << PF0) | (0 << PF1) | (0 << PF4) | (0 << PF5) | (0 << PF6) | (0 << PF7)); // only pins PF0,1,4,5,6,7 exist
	#endif

	#if (defined(__AVR_AT90USB1287__) || defined(__AVR_AT90USB647__) ||  \
			defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB646__) ||  \
			defined(__AVR_ATmega32U6__))
		DDRA = 0;
		PORTA = 0;
//		DDRB = 0;
//		PORTB = 0;
		DDRC = 0;
		PORTC = 0;
		DDRD = 0;
		PORTD = 0;
		DDRE = (1 << PE6); // set PE6 to HIGH to disable external SRAM, if connected
		PORTE = (1 << PE6); // set PE6 to HIGH to disable external SRAM, if connected
		DDRF = 0;
		PORTF = 0;
	#endif

	// for JTAG pins
	DDRB = (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3) | (1 << PB4) | (1 << PB5) | (0 << PB6) | (1 << PB7);
	PORTB = (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3) | (1 << PB4) | (1 << PB5) | (1 << PB6) | (0 << PB7);
	DDRD = (1 << PD0) | (1 << PD1) | (1 << PD4);
	PORTD = (1 << PD0) | (1 << PD1) | (1 << PD4);


	// initialize the send and receive buffers
	for (uint8_t i = 0; i < OUT_EP_SIZE; i++) {
		dataReceived[i] = 0;
	}

	for (uint8_t i = 0; i < 64; i++) {
		answer[i] = 0;
	}

	LEDs_Init();
	USB_Init();
}

/** Event handler for the USB_Connect event. This indicates that the device is enumerating via the status LEDs and
 *  starts the library USB task to begin the enumeration and USB management process.
 */
void EVENT_USB_Device_Connect(void)
{
	/* Indicate USB enumerating */
	LEDs_SetAllLEDs(LEDMASK_USB_ENUMERATING);
}

/** Event handler for the USB_Disconnect event. This indicates that the device is no longer connected to a host via
 *  the status LEDs.
 */
void EVENT_USB_Device_Disconnect(void)
{
	/* Indicate USB not ready */
	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
}

/** Event handler for the USB_ConfigurationChanged event. This is fired when the host sets the current configuration
 *  of the USB device after enumeration, and configures the device endpoints.
 */
void EVENT_USB_Device_ConfigurationChanged(void)
{	
	/* Indicate USB connected and ready */
	LEDs_SetAllLEDs(LEDMASK_USB_READY);

	/* Setup SendDataToHost Endpoint */
	if (!(Endpoint_ConfigureEndpoint(IN_EP, EP_TYPE_BULK,
		                             ENDPOINT_DIR_IN, IN_EP_SIZE,
//	                                 ENDPOINT_BANK_DOUBLE)))
	                                 ENDPOINT_BANK_SINGLE)))
	{
		LEDs_SetAllLEDs(LEDMASK_USB_ERROR);
	}
	
	/* Setup ReceiveDataFromHost Endpoint */
	if (!(Endpoint_ConfigureEndpoint(OUT_EP, EP_TYPE_BULK,
		                             ENDPOINT_DIR_OUT, OUT_EP_SIZE,
//	                                 ENDPOINT_BANK_DOUBLE)))
	                                 ENDPOINT_BANK_SINGLE)))
	{
		LEDs_SetAllLEDs(LEDMASK_USB_ERROR);
	}
}

/** Event handler for the USB_UnhandledControlRequest event. This is used to catch standard and class specific
 *  control requests that are not handled internally by the USB library (including the HID commands, which are
 *  all issued via the control endpoint), so that they can be handled appropriately for the application.
 */
void EVENT_USB_Device_UnhandledControlRequest(void)
{
	uint8_t zeroByte = 0;

	// Handle Vendor Requests
	switch (USB_ControlRequest.bRequest)
	{
		// 0x01 - FIRMWARE UPDATE which is unimplemented
		case 0x01:
			Endpoint_ClearSETUP();
			// Write data to the control endpoint
			Endpoint_Write_Control_Stream_LE(&zeroByte, sizeof(zeroByte));
			// Finalize the stream transfer to send the last packet or clear the host abort
			Endpoint_ClearOUT();

			break;


		// 0x02 - set SPEED
		case 0x02:
			Endpoint_ClearSETUP();
			// set JTAG Speed
			jtag_SPEED = (uint16_t)(USB_ControlRequest.wIndex);
			Endpoint_Write_Control_Stream_LE(&zeroByte, sizeof(zeroByte));
			// Finalize the stream transfer by sending the last packet or clear the host abort
			Endpoint_ClearOUT();

			break;


		// 0x03 - get SPEED
		case 0x03:
			Endpoint_ClearSETUP();
			// Write JTAG Speed data to the control endpoint
			Endpoint_Write_Control_Stream_LE((uint8_t*)&jtag_SPEED, sizeof(jtag_SPEED));
			// Finalize the stream transfer by sending the last packet or clear the host abort
			Endpoint_ClearIN();

			break;


		// 0x04 - USER
		case 0x04:
			Endpoint_ClearSETUP();
			if (USB_ControlRequest.wIndex == 0x01) {  // set LED
				// TODO: set the LED
				Endpoint_Write_Control_Stream_LE(&zeroByte, sizeof(zeroByte));
				Endpoint_ClearOUT();
			} else if (USB_ControlRequest.wIndex == 0x02) {  // clear LED
				// TODO: clear the LED
				Endpoint_Write_Control_Stream_LE(&zeroByte, sizeof(zeroByte));
				Endpoint_ClearOUT();
			} else if (USB_ControlRequest.wIndex == 0x03){  // get jumper position
				// TODO: determine jumper position then return 0 for open and 1 for closed
				Endpoint_Write_Control_Stream_LE(&zeroByte, sizeof(zeroByte));
				Endpoint_ClearIN();
			} else {  // ??? can this happen
				Endpoint_Write_Control_Stream_LE(&zeroByte, sizeof(zeroByte));
				Endpoint_ClearOUT();
			}

			break;


		// 0x05 - FIRMWARE VERSION
		case 0x05:
			Endpoint_ClearSETUP();
			// Write 4-byte Firmware Version data to the control endpoint
			Endpoint_Write_Control_Stream_LE(&firmwareVersion, sizeof(firmwareVersion));
			// Finalize the stream transfer by sending the last packet or clear the host abort
			Endpoint_ClearIN();

			break;
	}
}



void processOUT(void) {
	// Select the OUT endpoint
	Endpoint_SelectEndpoint(OUT_EP);

	// Check if the OUT EP contains a packet
	if (Endpoint_IsOUTReceived())
	{
		// Read in data from the host
		Endpoint_Read_Stream_LE(&dataReceived, sizeof(dataReceived));
		if (dataReceived[0] != 0x00) {
			processCommands = 1;
			Endpoint_ClearOUT();
		}

		// Check to see if the bank is now empty
		if (!(Endpoint_IsReadWriteAllowed()))
		{
			// Acknowledge the packet, clear the bank ready for the next packet
			Endpoint_ClearOUT();
		}
	}
}


void processIN(void) {
	// Select the IN endpoint
	Endpoint_SelectEndpoint(IN_EP);

	// Check if IN Endpoint is ready Write
	if (Endpoint_IsINReady())
	{
		// if data to be sent exists, send it
		if (answerNeedsToBeSentFlag == 1) {
			Endpoint_Write_Stream_LE(&answer, sendAnswerLength);
			answerNeedsToBeSentFlag = 0;
			Endpoint_ClearIN();
		} else {
			// TODO - this is wrong but it prevents timeouts
			Endpoint_ClearIN();
		}
	}
}
