/*
             LUFA Library
     Copyright (C) Dean Camera, 2010.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2010  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Modified for maximum STDIO throughput by Opendous Inc. 2011-02-27
  For more information visit: www.Micropendous.org/USBVirtualSerial

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
 *  Main source file for the VirtualSerial demo. This file contains the main tasks of
 *  the demo and is responsible for the initial application hardware configuration.
 */

#include "USBVirtualSerial-ADC.h"

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

/** Standard file stream for the CDC interface when set up, so that the virtual CDC COM port can be
 *  used like any regular character stream in the C APIs
 */
static FILE USBSerialStream;

/* Global buffer for use with STDIO functions */
volatile char buffer[CDC_TXRX_EPSIZE];

/* Global 32-bit integer to store the current value of Timer1, 16-bit to store OverflowCount */
volatile uint32_t Timer1Val = 0;
volatile uint16_t Timer1OverflowCount = 0;

volatile uint8_t waitingForADCResult = 0;

/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */
int main(void)
{
	SetupHardware();

	/* Create a regular character stream for the interface so that it can be used with the stdio.h functions */
	CDC_Device_CreateStream(&VirtualSerial_CDC_Interface, &USBSerialStream);

	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
	sei();

	for (;;)
	{
		MainTask();

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

	// initialize the buffer to be used with STDIO functions
	uint8_t i;
	for (i = 0; i < CDC_TXRX_EPSIZE; i++){
		buffer[i] = 0;
	}

	/* disable JTAG to allow corresponding pins to be used - PF4, PF5, PF6, PF7 */
	/* TODO - remove this if you want to use your JTAG debugger to debug this firmware */
	#if (defined(__AVR_AT90USB1287__) || defined(__AVR_AT90USB647__) ||  \
			defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB646__) ||  \
			defined(__AVR_ATmega16U4__)  || defined(__AVR_ATmega32U4__) ||  \
			defined(__AVR_ATmega32U6__))
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
		PORTD = 0xFF;
		DDRB = 0;
		PORTB = 0xFF;
		DDRC = 0;
		PORTC |= (1 << PC2) | (1 << PC4) | (1 << PC5) | (1 << PC6) | (1 << PC7); //only PC2,4,5,6,7 are pins
		// be careful using PortC as PC0 is used for the Crystal and PC1 is nRESET
	#endif

	#if (defined(__AVR_ATmega16U4__)  || defined(__AVR_ATmega32U4__))
		DDRD = 0;
		PORTD = 0xFF;
		DDRB = 0;
		PORTB = 0xFF;
		DDRC = 0;
		PORTC = (1 << PC6) | (1 << PC7); //only PC6,7 are pins
		DDRE = 0;
		PORTE = (1 << PE2) | (1 << PE6); //only PE2,6 are pins
		DDRF = 0;
		PORTF = (1 << PF0) | (1 << PF1) | (1 << PF4) | (1 << PF5) | (1 << PF6) | (1 << PF7); // only PF0,1,4,5,6,7 are pins
	#endif

	#if (defined(__AVR_AT90USB1287__) || defined(__AVR_AT90USB647__) ||  \
			defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB646__) ||  \
			defined(__AVR_ATmega32U6__))
		DDRA = 0;
		PORTA = 0xFF;
		DDRB = 0;
		PORTB = 0xFF;
		DDRC = 0;
		PORTC = 0xFF;
		DDRD = 0;
		PORTD = 0xFF;
		DDRE = 0;
		PORTE = 0xFF;
		DDRF = 0;
		PORTF = 0xFF;
		#if (BOARD == BOARD_MICROPENDOUS)
		// set PortB pin 1 to an output as it connects to an LED on the Micropendous
		DDRB |= (1 << PB1);
		// Set PE4=1 to disable external SRAM, PE6=0 to disable TXB0108, PE7=1 to select USB-B connector
		DDRE |= ((1 << PE4) | (1 << PE6) | (1 << PE7));
		PORTE |= ((1 << PE4) | (1 << PE7));
		PORTE &= ~(1 << PE6);
		#else // other boards such as the Micropendous3 or Micropendous4
		// Set PE6=1 to disable external SRAM
		DDRE |= (1 << PE6);
		PORTE |= (1 << PE6);
		#endif
	#endif

	// Hardware Initialization

	// 16-bit Timer1 Initialization
	TCCR1A = 0; //start the timer
	TCCR1B = (1 << CS10); // no Timer1 prescaling, use CLK
	//TCCR1B = (1 << CS12); // prescale Timer1 by CLK/256
	// Timer1 is a 32-bit integer and is the concatenation of the overflow count and the current Timer1 value
	Timer1Val = ((uint32_t)Timer1OverflowCount << 16) + (uint32_t)TCNT1; // get current Timer1 value
	TIMSK1 = (1 << TOIE1); // enable Timer1 overflow interrupt

	// ADC Initialization
	// Current device has an ADC, so start the ADC in free-running mode with VCC AREF, default prescale of 128
	// ADCclk should be between 50 and 200kHz, so a prescale of 128 works for both 8MHz and 16MHz boards
	ADC_Init(ADC_SINGLE_CONVERSION | ADC_RIGHT_ADJUSTED | ADC_PRESCALE_128);
	ADC_SetupChannel(0); // set up Channel 0 for ADC
	ADC_SetupChannel(1); // set up Channel 1 for ADC

	LEDs_Init();
	DISABLE_VOLTAGE_TXRX;
	DISABLE_EXT_SRAM;
	SELECT_USB_B;
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


/* Timer1 overflow interrupt */
ISR(TIMER1_OVF_vect) {
	Timer1OverflowCount++;
}


/* MainTask will run continuously in a loop with the USB tasks once initialization is complete */
void MainTask(void)
{
	int count = 0;
	int tempByte = 0;	// temporary storage - integer (16-bit)

	// If the host has sent data then process it, but not if still waiting for a previous ADC result
	if (!waitingForADCResult) {
		if ((count = fread(&buffer, 1, CDC_TXRX_EPSIZE, &USBSerialStream)) > 0) {
			// if host sent a 0, send back value of ADC channel 0, if a 1 then channel 1
			// there is only one ADC with multiplexed inputs so only one channel can be converted at a time
			tempByte = buffer[0];
			if (tempByte == '0') {
				ADC_StartReading(ADC_CHANNEL0 | ADC_REFERENCE_AVCC); // start reading channel 0 in the background
				waitingForADCResult = 1;
			} else if (tempByte == '1') {
				ADC_StartReading(ADC_CHANNEL1 | ADC_REFERENCE_AVCC); // start reading channel 1 in the background
				waitingForADCResult = 1;
			} else {
				// echo back sent data to let host know it has been received
				fwrite(&buffer, 1, count, &USBSerialStream);
			}
		}
	}


	// instead of busy-looping until conversion result exists, allow the result to arrive whenever
	if (ADC_IsReadingComplete() && (waitingForADCResult)) {
		Timer1Val = ((uint32_t)Timer1OverflowCount << 16) + (uint32_t)TCNT1; // update Timer1 value
		// send the ADC and Timer1 data to the host
		fprintf(&USBSerialStream, "%4d,%10ld\r\n", ADC_GetResult(), Timer1Val);
		waitingForADCResult = 0; // just got and sent ADC result
	}

} // MainTask
