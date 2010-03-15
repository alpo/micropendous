/*
             LUFA Library
     Copyright (C) Dean Camera, 2009.
              
  dean [at] fourwalledcubicle [dot] com
      www.fourwalledcubicle.com
*/

/*
  Copyright 2009  Dean Camera (dean [at] fourwalledcubicle [dot] com)
  SpeedTest Throughput Test Code by www.ilmarin.info - 2009-08

  Load this firmware onto an USB AVR and run the accompanying
  DeviceAccessC.c throughput test code to determine highest
  possible throughput based on transfer block size.

  Consider the TODO comments to customize this firmware.

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

#include "SpeedTest.h"


/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */
int main(void)
{
	SetupHardware();

	for (;;)
	{
		USB_SpeedTest_Task();
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

	/* TODO - disable JTAG to allow corresponding pins to be used - PF4, PF5, PF6, PF7 */
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
	/* TODO - enable Ports based on which IC is being used */
	/* For more information look over the corresponding AVR's datasheet in the
		'I/O Ports' Chapter under subheading 'Ports as General Digital I/O' */
	#if (defined(__AVR_AT90USB162__)  || defined(__AVR_AT90USB82__))
		DDRD = 0;
		PORTD = 0;
		DDRB = 0;
		PORTB = 0;
		DDRC = 0;
		PORTC |= (0 << PC2) | (0 << PC4) | (0 << PC5) | (0 << PC6) | (0 << PC7); //only PC2,4,5,6,7 are pins
		// be careful using PortC as PC0 is used for the Crystal
	#endif

	#if (defined(__AVR_ATmega16U4__)  || defined(__AVR_ATmega32U4__))
		DDRD = 0;
		PORTD = 0;
		DDRB = 0;
		PORTB = 0;
		DDRC = 0;
		PORTC |= (0 << PC6) | (0 << PC7); //only PC6,7 are pins
		DDRE = 0;
		PORTE |= (0 << PE2) | (0 << PE6); //only PE2,6 are pins
		DDRF = 0;
		PORTF |= (0 << PF0) | (0 << PF1) | (0 << PF4) | (0 << PF5) | (0 << PF6) | (0 << PF7); // only PF0,1,4,5,6,7 are pins
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
		DDRE = ((1 << PE4) | (1 << PE6));	// set PE4,PE6 to HIGH to disable external SRAM, if connected
		PORTE = ((1 << PE4) | (1 << PE6));	// set PE4,PE6 to HIGH to disable external SRAM, if connected
		DDRF = 0;
		PORTF = 0;
	#endif

	USB_Init();
}

/** Event handler for the USB_Connect event.
 */
void EVENT_USB_Device_Connect(void)
{
//TODO - handle device connect
}

/** Event handler for the USB_Disconnect event.
 */
void EVENT_USB_Device_Disconnect(void)
{
//TODO - handle device disconnect
}

/** Event handler for the USB_ConfigurationChanged event. This is fired when the host set the current configuration
 *  of the USB device after enumeration - the device endpoints are configured.
 */
void EVENT_USB_Device_ConfigurationChanged(void)
{

	if (!(Endpoint_ConfigureEndpoint(IN_EP, EP_TYPE_BULK,
		                             ENDPOINT_DIR_IN, IN_EP_SIZE,
                                 ENDPOINT_BANK_DOUBLE)))//ENDPOINT_BANK_SINGLE
	{
		//ERROR
	}

	if (!(Endpoint_ConfigureEndpoint(OUT_EP, EP_TYPE_BULK,
		                             ENDPOINT_DIR_OUT, OUT_EP_SIZE,
                                 ENDPOINT_BANK_DOUBLE))) //ENDPOINT_BANK_SINGLE
	{
		//ERROR
	}
}

/** Event handler for the USB_UnhandledControlRequest event. This is used to catch standard and class specific
 *  control requests that are not handled internally by the USB library
 */
void EVENT_USB_Device_UnhandledControlRequest(void)
{
	/* Process specific control requests */
	switch (USB_ControlRequest.bRequest)
	{
		default: break;
	}
}

/** Task to manage the custom communication interface - read and write data from the host */
void USB_SpeedTest_Task(void)
{
	static uint8_t temp=0;

	/* Device must be connected and configured for the task to run */
	if (USB_DeviceState != DEVICE_STATE_Configured)
		return;

	/* Select the IN stream endpoint */
	Endpoint_SelectEndpoint(IN_EP);
	
	/* Check if the current endpoint can be written to and that the next sample should be stored */
	if (Endpoint_IsINReady())
	{
		/* Write the sample to the buffer */
		while(Endpoint_IsReadWriteAllowed())
		{
			// TODO - instead of writing a count value, write data from your data structure
			Endpoint_Write_Byte(temp);
			temp++;
		}
		Endpoint_ClearIN();
	}

	/* Select the OUT stream endpoint */
	Endpoint_SelectEndpoint(OUT_EP);

	/* Check if the current endpoint can be read */
	if (Endpoint_IsOUTReceived())
	{
		
		/* Write the sample to the buffer */
		while(Endpoint_IsReadWriteAllowed())
		{
			// TODO - instead of a temporary byte, read data into your data structure
			temp=Endpoint_Read_Byte();
		}
		Endpoint_ClearOUT();
	}

}
