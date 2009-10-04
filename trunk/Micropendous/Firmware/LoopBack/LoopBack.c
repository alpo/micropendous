/*
             LUFA Library
     Copyright (C) Dean Camera, 2009.
              
  dean [at] fourwalledcubicle [dot] com
      www.fourwalledcubicle.com
*/

/*
  Copyright 2009  Denver Gingerich (denver [at] ossguy [dot] com)
    Based on code by Dean Camera (dean [at] fourwalledcubicle [dot] com)
    Additional LoopBack demo code by Opendous Inc.
    For more info visit: www.Micropendous.org/LoopBack

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
volatile uint8_t dataToSend[IN_EP_SIZE];
volatile uint8_t dataReceived[OUT_EP_SIZE];



/** Main program entry point. This routine configures the hardware required by the application, then
 *  starts the scheduler to run the USB management task.
 */
int main(void)
{
	SetupHardware();
	
	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);

	for (;;)
	{
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
		#error The AT90USBxx2 AVRs do not have enough SRAM to run FreeRTOS
	#endif

	#if (defined(__AVR_ATmega16U4__)  || defined(__AVR_ATmega32U4__))
		DDRD = 0;
		PORTD = 0xFF; //input
		DDRB = 0;
		PORTB = 0xFF; //input
		DDRC = 0;
		PORTC |= (1 << PC6) | (1 << PC7); //input only on PC6,7
		DDRE = 0;
		PORTE |= (1 << PE2) | (1 << PE6); //input only on PE2,6
		DDRF = 0;
		PORTF |= (1 << PF0) | (1 << PF1) | (1 << PF4) | (1 << PF5) | (1 << PF6) | (1 << PF7); //input only on PF0,1,4,5,6,7
	#endif

	#if (defined(__AVR_AT90USB1287__) || defined(__AVR_AT90USB647__) ||  \
			defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB646__) ||  \
			defined(__AVR_ATmega32U6__))
		DDRA = 0;
		PORTA = 0xFF; //input
		DDRB = 0;
		PORTB = 0xFF; //input
		DDRC = 0;
		PORTC = 0xFF; //input
		DDRD = 0;
		PORTD = 0xFF; //input
		DDRE = (1 << PE6);	// set PE6 to HIGH to disable external SRAM, if connected
		PORTE = 0xFF; //input
		DDRF = 0;
		PORTF = 0xFF; //input
	#endif

	// initialize the send and receive buffers
	uint8_t i = 0;
	for (i = 0; i < IN_EP_SIZE; i++) {
		dataToSend[i] = i;
	}

	for (i = 0; i < OUT_EP_SIZE; i++) {
		dataReceived[i] = i;
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
	//NOTE - this is here as a template only, LoopBack does not make use of it

	/* Handle HID Class specific requests */
	switch (USB_ControlRequest.bRequest)
	{
		case 0x01:
			if (USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
			{
				Endpoint_ClearSETUP();

				/* Write data to the control endpoint */
				Endpoint_Write_Control_Stream_LE(&dataToSend, sizeof(dataToSend));

				/* Finalize the stream transfer to send the last packet or clear the host abort */
				Endpoint_ClearOUT();
			}
		
			break;
	}
}


/** Sends data to the host */
void SendDataToHost(void)
{
	/* Select the IN Endpoint */
	Endpoint_SelectEndpoint(IN_EP);

	/* Check if IN Endpoint is ready for Read/Write */
	if (Endpoint_IsReadWriteAllowed())
	{
		/* Write data to the host */
		Endpoint_Write_Stream_LE(&dataToSend, sizeof(dataToSend));

		/* Finalize the stream transfer to send the last packet */
		Endpoint_ClearIN();
	}
}

/** Reads data from the host */
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
			Endpoint_Read_Stream_LE(&dataReceived, sizeof(dataReceived));
		}

		/* Handshake the OUT Endpoint - clear endpoint and get ready for next transfer */
		Endpoint_ClearOUT();
	}
}



/** Main_Task will only run if USB is connected */
void Main_Task(void)
{
	uint8_t i = 0;

	if (USB_DeviceState != DEVICE_STATE_Configured) {
		return;
	}

	ReceiveDataFromHost();	

	for (i = 0; i < OUT_EP_SIZE; i++) {
		dataToSend[i] = (uint8_t)(dataReceived[i]);
	}

	SendDataToHost();

}
