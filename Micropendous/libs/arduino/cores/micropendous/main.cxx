/*
             LUFA Library
     Copyright (C) Dean Camera, 2010.
              
  dean [at] fourwalledcubicle [dot] com
      www.fourwalledcubicle.com
*/

/*
  Copyright 2010  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Altered for USBVirtualSerial_FreeRTOS by Opendous Inc. 2010-02
  For more information visit:  www.Micropendous.org/USBVirtualSerial_FreeRTOS

  Adapted for LUFA + Arduino by Opendous Inc. 2010-03
  For more information visit:  www.Micropendous.org/LUFAduino

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
 *  Main source file for the LUFA Arduino project. This file contains the main tasks of
 *  the application and is responsible for the initial application hardware configuration.
 */


/** Circular buffer to hold data from the host before it is processed by MainTask using standard IO functions */
RingBuff_t Host_to_Device_Buffer;

/** Circular buffer to hold data before it is sent to the host. */
RingBuff_t Device_to_Host_Buffer;


/** LUFA CDC Class driver interface configuration and state information. This structure is
 *  passed to all CDC Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
/*USB_ClassInfo_CDC_Device_t VirtualSerial_CDC_Interface =
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
*/
USB_ClassInfo_CDC_Device_t VirtualSerial_CDC_Interface =
	{
//		.Config = 
			{
				/*ControlInterfaceNumber         =*/ 0,

				/*DataINEndpointNumber           =*/ CDC_TX_EPNUM,
				/*DataINEndpointSize             =*/ CDC_TXRX_EPSIZE,
				/*DataINEndpointDoubleBank       =*/ false,

				/*DataOUTEndpointNumber          =*/ CDC_RX_EPNUM,
				/*DataOUTEndpointSize            =*/ CDC_TXRX_EPSIZE,
				/*DataOUTEndpointDoubleBank      =*/ false,

				/*NotificationEndpointNumber     =*/ CDC_NOTIFICATION_EPNUM,
				/*NotificationEndpointSize       =*/ CDC_NOTIFICATION_EPSIZE,
				/*NotificationEndpointDoubleBank =*/ false,
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

	// Create Tasks for FreeRTOS
	//xTaskCreate(USBTask, (signed portCHAR *) "usbtask", configMINIMAL_STACK_SIZE, NULL, USB_CDC_TASK_PRIORITY, NULL );
	// CDCTask and USBTask run from the same task; CDCTask
	xTaskCreate(CDCTask, (signed portCHAR *) "cdctask", configMINIMAL_STACK_SIZE, NULL, USB_CDC_TASK_PRIORITY, NULL );
	xTaskCreate(MainTask, (signed portCHAR *) "maintask", configMINIMAL_STACK_SIZE, NULL, MAIN_TASK_PRIORITY, NULL );
	xTaskCreate(MainTask2, (signed portCHAR *) "maintsk2", configMINIMAL_STACK_SIZE, NULL, MAIN_TASK_PRIORITY, NULL );

	// Start the scheduler
	vTaskStartScheduler();

	// Should never get here!
	return 0;
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
		#error The AT90USBxx2 AVRs do not have enough SRAM to run FreeRTOS
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

	// Arduino sketch functions:
	init();
	setup();

	/* Hardware Initialization */
	LEDs_Init();
	USB_Init();
}


// FreeRTOS requires a specific type of function
void USBTask(void *pvParameters)
{
	portTickType xDelayPeriod = taskDelayPeriod;
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

	for(;;) {
		USB_USBTask();
		// vTaskDelayUntil is faster and deterministic
		vTaskDelayUntil(&xLastWakeTime, xDelayPeriod);
		// vTaskDelay(xDelayPeriod);
	}
}


void CDCTask(void *pvParameters)
{
	portTickType xDelayPeriod = taskDelayPeriod;
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

	for(;;) {
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
		// CDCTask does all CDC-related functions, including USB communication
		USB_USBTask();
		// vTaskDelayUntil is faster and deterministic
		vTaskDelayUntil(&xLastWakeTime, xDelayPeriod);
		//vTaskDelay(xDelayPeriod);
	}
}


void MainTask(void *pvParameters)
{
	for(;;) {
		loop();
	}
}


void MainTask2(void *pvParameters)
{
	for(;;) {
		loop2();
	}
}


// TODO - CoRoutines are not enabled, but FreeRTOS complains during compile
void vApplicationIdleHook(void)
{
	//vCoRoutineSchedule();
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
int sendData(char c, FILE *stream) {
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
