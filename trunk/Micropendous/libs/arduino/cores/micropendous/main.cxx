/*
             LUFA Library
     Copyright (C) Dean Camera, 2009.
              
  dean [at] fourwalledcubicle [dot] com
      www.fourwalledcubicle.com
*/

/*
  Copyright 2009  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Adapted for LUFA + Arduino by Opendous Inc. 2009-09
  For more information visit:  www.Micropendous.org/LUFAduino

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

/* Globals: */
/** Contains the current baud rate and other settings of the virtual serial port.
 *
 *  These values are set by the host via a class-specific request, and the physical USART should be reconfigured to match the
 *  new settings each time they are changed by the host.
 */
CDC_Line_Coding_t LineEncoding = {	BaudRateBPS: 0,
																CharFormat:  OneStopBit,
																ParityType:  Parity_None,
																DataBits: 8	};

/** Ring (circular) buffer to hold the RX data - data from the host to the attached device on the serial port. */
RingBuff_t Rx_Buffer;

/** Ring (circular) buffer to hold the TX data - data from the attached device on the serial port to the host. */
RingBuff_t Tx_Buffer;

/** Flag to indicate if the USART is currently transmitting data from the Rx_Buffer circular buffer. */
volatile bool Transmitting = false;

/** Main program entry point. This routine configures the hardware required by the application, then
 *  starts the scheduler to run the application tasks.
 */
int main(void)
{
	SetupHardware();

	// Ring buffer Initialization
	Buffer_Initialize(&Rx_Buffer);
	Buffer_Initialize(&Tx_Buffer);

	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);

	// Create Tasks for FreeRTOS
	xTaskCreate(USBTask, (signed portCHAR *) "usbtask", configMINIMAL_STACK_SIZE, NULL, USB_CDC_TASK_PRIORITY, NULL );
	// CDCTask was added to USBTask
	//xTaskCreate(CDCTask, (signed portCHAR *) "cdctask", configMINIMAL_STACK_SIZE, NULL, USB_CDC_TASK_PRIORITY, NULL );
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
		CDC_Task(); // Just one Task can handle both CDC and USB functions
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
		CDC_Task();
		// vTaskDelayUntil is faster and deterministic
		vTaskDelayUntil(&xLastWakeTime, xDelayPeriod);
		//vTaskDelay(xDelayPeriod);
	}
}


void MainTask(void *pvParameters)
{
	// MainTask runs at lowest priority so no delays are needed as the
	// FreeRTOS kernel is preemptive
	
	//portTickType xDelayPeriod = taskDelayPeriod;
	//portTickType xLastWakeTime;
	//xLastWakeTime = xTaskGetTickCount();

	for(;;) {
		Main_Task();
		// vTaskDelayUntil is faster and deterministic
		//vTaskDelayUntil(&xLastWakeTime, xDelayPeriod);
		//vTaskDelay(xDelayPeriod);
	}
}

void MainTask2(void *pvParameters)
{
	// MainTask runs at lowest priority so no delays are needed as the
	// FreeRTOS kernel is preemptive
	
	//portTickType xDelayPeriod = taskDelayPeriod;
	//portTickType xLastWakeTime;
	//xLastWakeTime = xTaskGetTickCount();

	for(;;) {
		Main_Task2();
		// vTaskDelayUntil is faster and deterministic
		//vTaskDelayUntil(&xLastWakeTime, xDelayPeriod);
		//vTaskDelay(xDelayPeriod);
	}
}

// TODO - CoRoutines are not enabled, but FreeRTOS complains during compile
//void vApplicationIdleHook(void)
//{
//	vCoRoutineSchedule();
//}


/** Event handler for the USB_Connect event. This indicates that the device is enumerating via the status LEDs and
 *  starts the library USB task to begin the enumeration and USB management process.
 */
void EVENT_USB_Device_Connect(void)
{
	/* Indicate USB enumerating */
	LEDs_SetAllLEDs(LEDMASK_USB_ENUMERATING);
}

/** Event handler for the USB_Disconnect event. This indicates that the device is no longer connected to a host via
 *  the status LEDs and stops the USB management and CDC management tasks.
 */
void EVENT_USB_Device_Disconnect(void)
{	
	/* Reset Tx and Rx buffers, device disconnected */
	Buffer_Initialize(&Rx_Buffer);
	Buffer_Initialize(&Tx_Buffer);

	/* Indicate USB not ready */
	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
}

/** Event handler for the USB_ConfigurationChanged event. This is fired when the host set the current configuration
 *  of the USB device after enumeration - the device endpoints are configured and the CDC management task started.
 */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	/* Indicate USB connected and ready */
	LEDs_SetAllLEDs(LEDMASK_USB_READY);

	/* Setup CDC Notification, Rx and Tx Endpoints */
	if (!(Endpoint_ConfigureEndpoint(CDC_NOTIFICATION_EPNUM, EP_TYPE_INTERRUPT,
		                             ENDPOINT_DIR_IN, CDC_NOTIFICATION_EPSIZE,
	                                 ENDPOINT_BANK_SINGLE)))
	{
		LEDs_SetAllLEDs(LEDMASK_USB_ERROR);
	}
	
	if (!(Endpoint_ConfigureEndpoint(CDC_TX_EPNUM, EP_TYPE_BULK,
		                             ENDPOINT_DIR_IN, CDC_TXRX_EPSIZE,
	                                 ENDPOINT_BANK_SINGLE)))
	{
		LEDs_SetAllLEDs(LEDMASK_USB_ERROR);
	}							   

	if (!(Endpoint_ConfigureEndpoint(CDC_RX_EPNUM, EP_TYPE_BULK,
		                             ENDPOINT_DIR_OUT, CDC_TXRX_EPSIZE,
	                                 ENDPOINT_BANK_SINGLE)))
	{
		LEDs_SetAllLEDs(LEDMASK_USB_ERROR);
	}

	/* Reset line encoding baud rate so that the host knows to send new values */
	LineEncoding.BaudRateBPS = 0;
}

/** Event handler for the USB_UnhandledControlRequest event. This is used to catch standard and class specific
 *  control requests that are not handled internally by the USB library (including the CDC control commands,
 *  which are all issued via the control endpoint), so that they can be handled appropriately for the application.
 */
void EVENT_USB_Device_UnhandledControlRequest(void)
{
	/* Process CDC specific control requests */
	switch (USB_ControlRequest.bRequest)
	{
		case REQ_GetLineEncoding:
			if (USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
			{	
				/* Acknowledge the SETUP packet, ready for data transfer */
				Endpoint_ClearSETUP();

				/* Write the line coding data to the control endpoint */
				Endpoint_Write_Control_Stream_LE(&LineEncoding, sizeof(LineEncoding));
				
				/* Finalize the stream transfer to send the last packet or clear the host abort */
				Endpoint_ClearOUT();
			}
			
			break;
		case REQ_SetLineEncoding:
			if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
			{
				/* Acknowledge the SETUP packet, ready for data transfer */
				Endpoint_ClearSETUP();

				/* Read the line coding data in from the host into the global struct */
				Endpoint_Read_Control_Stream_LE(&LineEncoding, sizeof(LineEncoding));

				/* Finalize the stream transfer to clear the last packet from the host */
				Endpoint_ClearIN();
				
				/* Reconfigure the USART with the new settings */
				Reconfigure();
			}
	
			break;
		case REQ_SetControlLineState:
			if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
			{				
				/* Acknowledge the SETUP packet, ready for data transfer */
				Endpoint_ClearSETUP();
				
				/* NOTE: Here you can read in the line state mask from the host, to get the current state of the output handshake
				         lines. The mask is read in from the wValue parameter in USB_ControlRequest, and can be masked against the
						 CONTROL_LINE_OUT_* masks to determine the RTS and DTR line states using the following code:
				*/

				Endpoint_ClearStatusStage();
			}
	
			break;
	}
}

/** Task to manage CDC data transmission and reception to and from the host, from and to the physical USART. */
void CDC_Task(void)
{
	/* Device must be connected and configured for the task to run */
	if (USB_DeviceState != DEVICE_STATE_Configured)
	  return;
	  
#if 0
	/* NOTE: Here you can use the notification endpoint to send back line state changes to the host, for the special RS-232
			 handshake signal lines (and some error states), via the CONTROL_LINE_IN_* masks and the following code:
	*/

	USB_Notification_Header_t Notification = (USB_Notification_Header_t)
		{
			.NotificationType = (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE),
			.Notification     = NOTIF_SerialState,
			.wValue           = 0,
			.wIndex           = 0,
			.wLength          = sizeof(uint16_t),
		};
		
	uint16_t LineStateMask;
	
	// Set LineStateMask here to a mask of CONTROL_LINE_IN_* masks to set the input handshake line states to send to the host
	
	Endpoint_SelectEndpoint(CDC_NOTIFICATION_EPNUM);
	Endpoint_Write_Stream_LE(&Notification, sizeof(Notification));
	Endpoint_Write_Stream_LE(&LineStateMask, sizeof(LineStateMask));
	Endpoint_ClearIN();
#endif

	/* Select the Serial Rx Endpoint */
	Endpoint_SelectEndpoint(CDC_RX_EPNUM);
	
	/* Check to see if a packet has been received from the host */
	if (Endpoint_IsOUTReceived())
	{
		/* Read the bytes in from the endpoint into the buffer while space is available */
		while (Endpoint_BytesInEndpoint() && (Rx_Buffer.Elements != BUFF_STATICSIZE))
		{
			/* Store each character from the endpoint */
			Buffer_StoreElement(&Rx_Buffer, Endpoint_Read_Byte());
		}
		
		/* Check to see if all bytes in the current packet have been read */
		if (!(Endpoint_BytesInEndpoint()))
		{
			/* Clear the endpoint buffer */
			Endpoint_ClearOUT();
		}
	}

	/* Check if Rx buffer contains data - if so, send it */
	if (Rx_Buffer.Elements) {
		/* Serial_TxByte(Buffer_GetElement(&Rx_Buffer)); */
		/* Do not want to stream data over UART, want to leave it in
			the buffer for Main_Task to process */
		/* Note this is the only alteration to CDC_Task from the original USBtoSerial */
	}

	/* Select the Serial Tx Endpoint */
	Endpoint_SelectEndpoint(CDC_TX_EPNUM);

	/* Check if the Tx buffer contains anything to be sent to the host */
	if ((Tx_Buffer.Elements) && LineEncoding.BaudRateBPS)
	{
		/* Wait until Serial Tx Endpoint Ready for Read/Write */
		Endpoint_WaitUntilReady();
		
		/* Write the bytes from the buffer to the endpoint while space is available */
		while (Tx_Buffer.Elements && Endpoint_IsReadWriteAllowed())
		{
			/* Write each byte retreived from the buffer to the endpoint */
			Endpoint_Write_Byte(Buffer_GetElement(&Tx_Buffer));
		}
		
		/* Remember if the packet to send completely fills the endpoint */
		bool IsFull = (Endpoint_BytesInEndpoint() == CDC_TXRX_EPSIZE);
		
		/* Send the data */
		Endpoint_ClearIN();

		/* If no more data to send and the last packet filled the endpoint, send an empty packet to release
		 * the buffer on the receiver (otherwise all data will be cached until a non-full packet is received) */
		if (IsFull && !(Tx_Buffer.Elements))
		{
			/* Wait until Serial Tx Endpoint Ready for Read/Write */
			Endpoint_WaitUntilReady();
				
			/* Send an empty packet to terminate the transfer */
			Endpoint_ClearIN();
		}
	}
}

/** Reconfigures firmware based on current serial port settings issued by the host. */
void Reconfigure(void)
{
	/* Determine parity - non odd/even parity mode defaults to no parity */
	if (LineEncoding.ParityType == Parity_Odd) {
		//
	} else if (LineEncoding.ParityType == Parity_Even) {
		//
	} else if (LineEncoding.ParityType == Parity_Mark) {
		// 
	} else if (LineEncoding.ParityType == Parity_Space) {
		//
	} else {  //Parity_None
		//
	}

	/* Determine stop bits - 1.5 stop bits is set as 1 stop bit due to hardware limitations */
	if (LineEncoding.CharFormat == TwoStopBits) {
		//
	} else if (LineEncoding.CharFormat == OneAndAHalfStopBits) {
		//
	} else {  // OneStopBit
		// 
	}
	
	/* Determine data size - 5, 6, 7, or 8 bits are supported */
	if (LineEncoding.DataBits == 8) {
		//
	} else if (LineEncoding.DataBits == 7) {
		//
	} else if (LineEncoding.DataBits == 6) {
		//
	} else {  // LineEncoding.DataBits == 5
		//
	}

	/* What BaudRate does the host want to use */
	if        (LineEncoding.BaudRateBPS == 9600) {
		//
	} else if (LineEncoding.BaudRateBPS == 14400) {
		//
	} else if (LineEncoding.BaudRateBPS == 19200) {
		//
	} else if (LineEncoding.BaudRateBPS == 38400) {
		//
	} else if (LineEncoding.BaudRateBPS == 57600) {
		//
	} else if (LineEncoding.BaudRateBPS == 115200) {
		//
	} else {
		//
	}
}

/* Use this function to make sure data exists.
	Need this function as RingBuff will crash if getData is called on
	an empty buffer
*/
uint8_t haveData(void) {
	return (uint8_t)(Rx_Buffer.Elements);
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
	Buffer_StoreElement(&Tx_Buffer, (uint8_t)c);
	return 0;
}

/* Also require a function to receive data from the USB Virtual Serial Port link */
int getData(FILE *__stream) {
	// TODO - implement the following errors - but are they really necessary?
	//if (something) return _FDEV_ERR;
	//if (something) return _FDEV_EOF;
	return (int)Buffer_GetElement(&Rx_Buffer);
}



/* Main_Task will run at lowest priority once initialization is complete */
void Main_Task(void)
{
	loop();
}

/* Main_Task will run at lowest priority once initialization is complete */
void Main_Task2(void)
{
	loop2();
}
