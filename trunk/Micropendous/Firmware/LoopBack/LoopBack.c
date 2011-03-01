/*
             LUFA Library
     Copyright (C) Dean Camera, 2010.
              
  dean [at] fourwalledcubicle [dot] com
      www.fourwalledcubicle.com
*/

/*
  Copyright 2009 Dean Camera (dean [at] fourwalledcubicle [dot] com)
    Additional LoopBack demo code by Opendous Inc. 2010-03-03
    For more info visit: www.Micropendous.org/LoopBack

  This firmware enumerates as a vendor-class device meaning
  the developer must decide and code a communication scheme.
  Examples of arbitrary endpoint as well as control endpoint
  communication are given.
  Search for TODO statements for implementation hints.
  Avoid busy loops as the USB task is not preemptive and USB
  has timing contraints.  Use concurrent programming techniques.
  For example, have one function set a flag when an event occurs.
  Have another function do significant processing only if the flag is set.
  Have both functions run from main()'s for(;;) loop, which should be the
  only indefinite loop in your firmware.

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

/** \file
 *
 *  Main source file for the LoopBack demo. This file contains the main tasks of the demo and
 *  is responsible for the initial application hardware configuration.
 */
 
#include "LoopBack.h"

/* Global Variables */
/* EP_SIZE is defined in Descriptors.h and should be
 * less than or equal to 64 bytes in order to work with Control Endpoint data.
 */
volatile uint8_t dataToSend[EP_SIZE];
volatile uint8_t dataReceived[EP_SIZE];



/** Main program entry point. This routine configures the hardware required by the application, then
 *  starts the scheduler to run the USB management task.
 */
int main(void)
{
	SetupHardware();
	
	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
	sei();

	for (;;)
	{
		// TODO - you can add more tasks here but remember that USB
		//		has timing contraints and USBTask should run at least as often as
		//		half your USB software's USB timeout setting, ~ 50ms
		Main_Task();
		USB_USBTask();
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

	// initialize the send and receive buffers
	uint8_t i = 0;
	for (i = 0; i < EP_SIZE; i++) {
		dataToSend[i] = i;
	}

	for (i = 0; i < EP_SIZE; i++) {
		dataReceived[i] = i;
	}

	/* TODO - set up ports */
	/* Look over your AVR datasheet's I/O-Ports chapter for more information */
	/* For now we will set up half of PortD as input and the other half as output  */
	/* PD0,1,2,3 will be input (DDR=0, PORT=0) and PD4,5,6,7 will be output with a 1 start state (DDR=1, PORT=1) */
	DDRD =  (0 << PD0) | (0 << PD1) | (0 << PD2) | (0 << PD3) | (1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7);
	PORTD = (0 << PD0) | (0 << PD1) | (0 << PD2) | (0 << PD3) | (1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7);

	DISABLE_VOLTAGE_TXRX;
	DISABLE_EXT_SRAM;

	LEDs_Init();
	USB_Init();
	SELECT_USB_B;
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
														ENDPOINT_DIR_IN, EP_SIZE,
														ENDPOINT_BANK_SINGLE)))
	{
		LEDs_SetAllLEDs(LEDMASK_USB_ERROR);
	}
	
	/* Setup ReceiveDataFromHost Endpoint */
	if (!(Endpoint_ConfigureEndpoint(OUT_EP, EP_TYPE_BULK,
														ENDPOINT_DIR_OUT, EP_SIZE,
														ENDPOINT_BANK_SINGLE)))
	{
		LEDs_SetAllLEDs(LEDMASK_USB_ERROR);
	}
}

/** Event handler for the USB_UnhandledControlRequest event. This is used to catch standard, class,
 *  and vendor specific control requests that are not handled internally by the USB library so that
 *  they can be handled appropriately for the application.
 */
void EVENT_USB_Device_UnhandledControlRequest(void)
{

	/* TODO - Control Endpoint communication.  Just match up bRequest and bmRequestType values
				in your corresponding USB software to send and/or receive data.
	*/

	switch (USB_ControlRequest.bRequest)
	{
		case 1:
			if (USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_VENDOR | REQTYPE_STANDARD))
			{
				// Send data to the host
				// Acknowledge the SETUP packet, ready for data transfer
				Endpoint_ClearSETUP();

				// Write data to the control endpoint
				Endpoint_Write_Control_Stream_LE(&dataToSend, sizeof(dataToSend));

				// Finalize the stream transfer to send the last packet or clear the host abort
				Endpoint_ClearOUT();
			}

			break;

		case 2:
			if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_VENDOR | REQTYPE_STANDARD))
			{
				// Receive data from the host
				// Acknowledge the SETUP packet, ready for data transfer
				Endpoint_ClearSETUP();

				// Read data in from the host
				Endpoint_Read_Control_Stream_LE(&dataReceived, sizeof(dataToSend));

				// Finalize the stream transfer to clear the last packet from the host
				Endpoint_ClearIN();
			}

			break;

	}
}


/** Sends data to the host via regular endpoints */
void SendDataToHost(void)
{
	/* Select the IN Endpoint */
	Endpoint_SelectEndpoint(IN_EP);

	/* Check if IN Endpoint is ready for Read/Write */
	if (Endpoint_IsReadWriteAllowed())
	{
		/* Write data to the host */
		Endpoint_Write_Stream_LE(&dataToSend, sizeof(dataToSend), NULL);

		/* Finalize the stream transfer to send the last packet */
		Endpoint_ClearIN();
	}
}

/** Reads data from the host via regular endpoints */
void ReceiveDataFromHost(void)
{
	/* Select the OUT Endpoint */
	Endpoint_SelectEndpoint(OUT_EP);

	/* Check if the OUT EP contains a packet */
	if (Endpoint_IsOUTReceived())
	{
		/* Check to see if the packet contains data */
		if (Endpoint_IsReadWriteAllowed())
		{
			/* Read in data from the host */
			Endpoint_Read_Stream_LE(&dataReceived, sizeof(dataReceived), NULL);
		}

		/* Handshake the OUT Endpoint - clear endpoint and get ready for next transfer */
		Endpoint_ClearOUT();
	}
}



/** Main_Task will only run if USB is connected */
void Main_Task(void)
{
	uint8_t i = 0;

	// only run through Main_Task if the device is connected
	if (USB_DeviceState != DEVICE_STATE_Configured) {
		return;
	}

	// TODO - your application code.  For now just loop back data.

	ReceiveDataFromHost();		// get new contents of dataReceived

	// copy contents of dataReceived buffer to dataToSend buffer
	for (i = 0; i < EP_SIZE; i++) {
		dataToSend[i] = (uint8_t)(dataReceived[i]);
	}

	SendDataToHost();				// send contents of dataToSend

}
