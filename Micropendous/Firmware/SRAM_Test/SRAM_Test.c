/*
             LUFA Library
     Copyright (C) Dean Camera, 2009.
              
  dean [at] fourwalledcubicle [dot] com
      www.fourwalledcubicle.com
*/

/*
  Copyright 2009  Dean Camera (dean [at] fourwalledcubicle [dot] com)
  Code for SRAM Testing by Opendous Inc. (www.opendous.org)

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

#include "SRAM_Test.h"


/* Defines related to SRAM */
#if (defined(__AVR_AT90USB1287__) || defined(__AVR_AT90USB1286__))
	#define EXT_SRAM_START		0x2100
#elif (defined(__AVR_AT90USB647__) || defined(__AVR_AT90USB646__) || defined(__AVR_ATmega32U6__))
	#define EXT_SRAM_START		0x1100
#endif
#define EXT_SRAM_END			0xFFFF
#define EXT_SRAM_SIZE			(EXT_SRAM_END - EXT_SRAM_START)
#define SELECT_EXTSRAM_BANK0		PORTE &= ~(1 << PE7);
#define SELECT_EXTSRAM_BANK1		PORTE |= (1 << PE7);


/* Globals: */
/** Contains the current baud rate and other settings of the virtual serial port.
 *
 *  These values are set by the host via a class-specific request, and the physical USART should be reconfigured to match the
 *  new settings each time they are changed by the host.
 */
CDC_Line_Coding_t LineEncoding = { .BaudRateBPS = 0,
                                   .CharFormat  = OneStopBit,
                                   .ParityType  = Parity_None,
                                   .DataBits    = 8            };

/** Ring (circular) buffer to hold the RX data - data from the host to the attached device on the serial port. */
RingBuff_t Rx_Buffer;

/** Ring (circular) buffer to hold the TX data - data from the attached device on the serial port to the host. */
RingBuff_t Tx_Buffer;

/* Global pointer to the start of external memory */
static uint8_t* ExtMemArray = (uint8_t*)EXT_SRAM_START;
static uint16_t extSramStart = (uint16_t)EXT_SRAM_START;
static uint16_t extSramEnd = (uint16_t)EXT_SRAM_END;

volatile uint16_t EMAindex = 0;
volatile uint16_t firstSRAMfailAddress = 0;
volatile uint8_t SRAMTestStatus = 0;
volatile uint8_t FillingRAM = 0;

/** Main program entry point. This routine configures the hardware required by the application, then
 *  starts the scheduler to run the application tasks.
 */
int main(void)
{
	SetupHardware();

	/* Ring buffer Initialization */
	Buffer_Initialize(&Rx_Buffer);
	Buffer_Initialize(&Tx_Buffer);

	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
	
	for (;;)
	{
		CDC_Task();
		USB_USBTask();
		Main_Task();
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

	/* Hardware Initialization */
	DDRA = 0;
	PORTA = 0xFF; //input
	DDRB = 0;
	PORTB = 0xFF; //input
	DDRC = 0;
	PORTC = 0xFF; //input
	DDRD = 0;
	PORTD = 0xFF; //input
	DDRE = 0;
	PORTE = 0xFF; //input
	DDRF = 0;
	PORTF = 0xFF; //input

	/* Enable External SRAM */
	DDRE = 0xFF;
	// PE6 is chip enable (^CS - active low); PE7 is highest address bit (bank); PE0,1,2 are SRAM-related
	PORTE = ((0 << PE6) | (0 << PE7) | (1 << PE0) | (1 << PE1) | (1 << PE2));
	// enable external SRAM with 0 wait states
	XMCRA = ((1 << SRE));
	XMCRB = 0;

	SELECT_EXTSRAM_BANK0; // set bank 0 as current SRAM bank
	SRAMTestStatus = TestEXTSRAM(ExtMemArray, extSramStart, extSramEnd);
	SELECT_EXTSRAM_BANK1;
	SRAMTestStatus = TestEXTSRAM(ExtMemArray, extSramStart, extSramEnd);
	SELECT_EXTSRAM_BANK0;

	LEDs_Init();
	rprintfInit(sendData); // rprintf allows sending formatted data to host
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


/** Use USART configuration settings to control your own code's parameters */
void Reconfigure(void)
{

	/* Determine parity - non odd/even parity mode defaults to no parity */
	if (LineEncoding.ParityType == Parity_Odd) {
		//
	} else if (LineEncoding.ParityType == Parity_Even) {
		//
	} else {
		//
	}


	/* Determine stop bits - 1.5 stop bits is set as 1 stop bit due to hardware limitations */
	if (LineEncoding.CharFormat == TwoStopBits) {
		//
	} else {
		//
	}


	/* Determine data size - 5, 6, 7, or 8 bits are supported */
	if (LineEncoding.DataBits == 6) {
		//
	} else if (LineEncoding.DataBits == 7) {
		//
	} else if (LineEncoding.DataBits == 8) {
		//
	} else { // LineEncoding.DataBits == 5
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


/* In order to use the rprintf function from avrlib, need a
	simplified way to send data
*/
void sendData(uint8_t txData) {
	Buffer_StoreElement(&Tx_Buffer, txData);
}


/* Might as well also have a simplified way to receive data */
uint8_t getData(void) {
	return Buffer_GetElement(&Rx_Buffer);
}



uint16_t TestEXTSRAM(uint8_t* array, uint16_t startAddress, uint16_t endAddress)
{
	uint16_t i = 0;
	uint16_t ArraySize = (uint16_t)(endAddress - startAddress);

	// Test1 - test upward count
	for (i = 0; i < ArraySize; i++) {
		array[i] = (uint8_t)i;
	}
	for (i = 0; i < ArraySize; i++) {
		if (array[i] != (uint8_t)i) {
			firstSRAMfailAddress = (uint16_t)(&array[i]);
			return 1;
		}
	}

	// Test2 - test downward count
	for (i = ArraySize; i > 0; i--) {
		array[i] = (uint8_t)i;
	}
	for (i = ArraySize; i > 0; i--) {
		if (array[i] != (uint8_t)i) {
			firstSRAMfailAddress = (uint16_t)(&array[i]);
			return 2;
		}
	}

	// Test3 - test 0xFF = 0b11111111
	for (i = 0; i < ArraySize; i++) {
		array[i] = 0xFF;
	}
	for (i = 0; i < ArraySize; i++) {
		if (array[i] != 0xFF) {
			firstSRAMfailAddress = (uint16_t)(&array[i]);
			return 3;
		}
	}	

	// Test4 - test 0xFF = 0b00000000
	for (i = 0; i < ArraySize; i++) {
		array[i] = 0x00;
	}
	for (i = 0; i < ArraySize; i++) {
		if (array[i] != 0x00) {
			firstSRAMfailAddress = (uint16_t)(&array[i]);
			return 4;
		}
	}	

	// Test5 - test 0xAA = 0b10101010
	for (i = 0; i < ArraySize; i++) {
		array[i] = 0xAA;
	}
	for (i = 0; i < ArraySize; i++) {
		if (array[i] != 0xAA) {
			firstSRAMfailAddress = (uint16_t)(&array[i]);
			return 5;
		}
	}	

	// Test 6 - test 0x55 = 0b01010101
	for (i = 0; i < ArraySize; i++) {
		array[i] = 0x55;
	}
	for (i = 0; i < ArraySize; i++) {
		if (array[i] != 0x55) {
			firstSRAMfailAddress = (uint16_t)(&array[i]);
			return 6;
		}
	}	

	// Test7 - test upward count again
	for (i = 0; i < ArraySize; i++) {
		array[i] = (uint8_t)i;
	}
	for (i = 0; i < ArraySize; i++) {
		if (array[i] != (uint8_t)i) {
			firstSRAMfailAddress = (uint16_t)(&array[i]);
			return 7;
		}
	}

	return 0;
}



/* Main_Task will only run if USB is connected */
void Main_Task(void)
{
	uint8_t  tempByte1 = 0;
	uint8_t  tempByte2 = 0;
	uint8_t  tempByte3 = 0;
	uint16_t  tempWord = 0;
	uint8_t*  tempBytePtr = 0;

	while (haveData()) {
		tempByte1 = getData();

		if (tempByte1 == 'n') { // get the next byte of data from external SRAM array - use i cmd to get first
			rprintfNum(10, 3, FALSE, '0', ExtMemArray[EMAindex]);
			rprintf("\n");
			EMAindex++;
		} else if ((tempByte1 == 'h') || (tempByte1 == '?')) { // print help
			rprintf("\nAvailable Commnds\n");
			rprintf("o - print info\n");
			rprintf("t - SRAM test status\n");
			rprintf("s - size of SRAM\n");
			rprintf("b - current bank\n");
			rprintf("0 - select bank 0\n");
			rprintf("1 - select bank 1\n");
			rprintf("f??? - fill array at index ?? with data byte ? and return status\n");
            rprintf("g??? - fill array at index ?? with data byte ? but no status msg\n");
			rprintf("a?? - data at address ?? \n");
			rprintf("i?? - data at SRAM array index ?? \n");
			rprintf("n - data at next SRAM array index - set start index with i?? \n");
		} else if (tempByte1 == 'o') { // print info regarding SRAM
			rprintf("SRAM StartAddr = ");
			rprintfNum(10, 5, FALSE, '0', (uint16_t)EXT_SRAM_START);
			rprintf(",");
			rprintf(" EndAddr = ");
			rprintfNum(10, 5, FALSE, '0', (uint16_t)EXT_SRAM_END);
			rprintf(",");
			rprintf(" Size = ");
			rprintfNum(10, 5, FALSE, '0', (uint16_t)EXT_SRAM_SIZE);
			rprintf(" bytes\n");
		} else if (tempByte1 == 't') { // print whether SRAM failed testing
			if (SRAMTestStatus){
				rprintf("SRAM_Fail_Addr = ");
				rprintfNum(10, 5, FALSE, '0', firstSRAMfailAddress);
				rprintf(",");
				rprintf(" Failed_Test ");
				rprintfNum(10, 1, FALSE, '0', SRAMTestStatus);
				rprintf("\n");
			} else {
				rprintf("SRAM_Passed_Tests\n");
			}
		} else if (tempByte1 == 's') { // size of SRAM
			rprintfNum(10, 5, FALSE, '0', (uint16_t)EXT_SRAM_SIZE);
			rprintf("\n");
		} else if (tempByte1 == 'f') { // start filling SRAM, each subsequent byte will fill next index, need to fill all of it
			tempByte2 = getData();
			tempByte3 = getData();
			tempByte1 = getData();
			tempWord = (((uint16_t)tempByte2 << 8) + ((uint16_t)tempByte3));
			ExtMemArray[tempWord] = tempByte1;
			rprintf("Filled EMA[");
			rprintfNum(10, 5, FALSE, '0', tempWord);
			rprintf("] with ");
			rprintfNum(10, 3, FALSE, '0', ExtMemArray[tempWord]);
			rprintf("\n");
        } else if (tempByte1 == 'g') { // start filling SRAM, each subsequent byte will fill next index, need to fill all of it
			tempByte2 = getData();
			tempByte3 = getData();
			tempByte1 = getData();
			tempWord = (((uint16_t)tempByte2 << 8) + ((uint16_t)tempByte3));
			ExtMemArray[tempWord] = tempByte1;
		} else if (tempByte1 == 'b') { // print current SRAM bank
			rprintf("SRAM_Bank = ");
			rprintfNum(10, 1, FALSE, '0', (PINE >> 7));
			rprintf("\n");
		} else if (tempByte1 == '0') { // select SRAM bank 0 for usage
			SELECT_EXTSRAM_BANK0;
		} else if (tempByte1 == '1') { // select SRAM bank 1 for usage
			SELECT_EXTSRAM_BANK1;
		} else if (tempByte1 == 'a') { // get the data byte from memory location defined by subsequent 2 bytes (word)
			tempByte2 = getData();
			tempByte3 = getData();
			tempWord = (((uint16_t)tempByte2 << 8) + ((uint16_t)tempByte3));
			tempBytePtr = (uint8_t*)tempWord;
			rprintf("Addr:");
			rprintfNum(10, 5, FALSE, '0', tempWord);
			rprintf(": = ");
			rprintfNum(10, 3, FALSE, '0', (uint8_t)(*tempBytePtr));
			rprintf("\n");
		} else if (tempByte1 == 'i') { // get the data byte from ExtMemArray at index defined by subsequent 2 bytes (word)
			tempByte2 = getData();
			tempByte3 = getData();
			tempWord = (((uint16_t)tempByte2 << 8) + ((uint16_t)tempByte3));
			EMAindex = tempWord;
			rprintf("EMA[");
			rprintfNum(10, 5, FALSE, '0', tempWord);
			rprintf("] = ");
			rprintf("Addr:");
			rprintfNum(10, 5, FALSE, '0', (uint16_t)&ExtMemArray[EMAindex]);
			rprintf(" = ");
			rprintfNum(10, 3, FALSE, '0', ExtMemArray[EMAindex]);
			rprintf("\n");
		} else {
			rprintfNum(10, 3, FALSE, '0', (uint8_t)'f');
			rprintf("-ERROR\n");
		}
	}

}
