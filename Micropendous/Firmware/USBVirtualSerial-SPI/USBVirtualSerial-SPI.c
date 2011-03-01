/*
             LUFA Library
     Copyright (C) Dean Camera, 2010.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2010  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Altered for USBVirtualSerial-SPI by Opendous Inc. 2010-03
  For more information visit:  www.Micropendous.org/SPI

	Communications Device Class demonstration application.
	Allows you to use standard serial port software to send and receive
	data over SPI.  This is a USB Serial Port to SPI gateway.

	Serial Port settings are mapped to SPI settings.  For example;
	a default serial port setting of 9600 8-N-1 means SPI Mode-0
	in MSB order at a SPI speed of 125kHz.

	The amount of data per read/write controls the SPI data width.
	If you write two bytes to the SPI gateway the SPI data width
	will be 16-bits.

	Changing the "Line Coding"/"bytesize" will change the SPI mode:
		Mode0 = 8 bits, Mode1 = 7 bits, Mode2 = 6 bits, Mode3 = 5 bits
	Note that data is still sent in 8-bit bytes over SPI, this is just a way to
	overlay SPI functionality over UART functionality.  Default is Mode0.

	Changing "Baud-Rate" will change the SPI speed.  Defualt SPI clock speed
	is 8Mhz / 4 = 2MHz.  8Mhz is the assumed clock speed of the USB AVR.
	The baud-to-SPI-clock translation table is as follows:
		9600 = 125kHz, 14400 = 4MHz, 19200 = 2MHz, 38400 = 1MHz,
		57600 = 500kHz, 115200 = 250kHz, other = 62.5kHz

	Changing "Stop-Bits" determines whether data will be MSB or LSB.  If the
	default one stop bit is selected, data will be transmitted MSB first.  Else LSB.

	Changing "Parity" sets various options:
		Odd:		Leave SCLK high when not in use
		Even:		Leave SCLK low when not in use (default)
		None:	SPI in Master Mode
		Mark:	SPI in Slave Mode
		Space:	Send SPI data registers as two bytes
	To make sure of proper SPI mode, set None or Mark last after all other settings.

	Before running, you will need to install the INF file that
	is located in the USBtoSerial project directory. This will enable
	Windows to use its inbuilt CDC drivers, negating the need
	for special Windows drivers for the device.

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

#include "USBVirtualSerial-SPI.h"

// TODO - define which pin is used for SPI nCS Pins
#define CS_PIN			PB4
#define CS_PORT		PORTB
#define CS_DDR			DDRB

#define CS_HIGH		CS_PORT |= (1 << CS_PIN);
#define CS_LOW		CS_PORT &= ~(1 << CS_PIN);

#define NOP() do { __asm__ __volatile__ ("nop"); } while (0)


// some global variables
volatile uint8_t tempIOreg = 0;
volatile uint8_t firstRun = 1;
volatile uint8_t spiClkHighOff = 0;
volatile uint8_t sendSPIsettingsRegisters = 0;


/** Circular buffer to hold data from the host before it is sent to the device via the serial port. */
RingBuff_t HostToDevice_Buffer;

/** Circular buffer to hold data from the serial port before it is sent to the host. */
RingBuff_t DeviceToHost_Buffer;

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

	RingBuffer_InitBuffer(&HostToDevice_Buffer);
	RingBuffer_InitBuffer(&DeviceToHost_Buffer);

	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
	sei();

	// Alternate for(...) loop retrieved multiple bytes per loop run to comply with SPI protocol
	for (;;)
	{
		// Read bytes from the USB OUT endpoint into the local data buffer
		for (uint8_t DataBytesRem = CDC_Device_BytesReceived(&VirtualSerial_CDC_Interface); DataBytesRem != 0; DataBytesRem--) {
			// Only try to read in bytes from the CDC interface if the transmit buffer is not full
			if (!(RingBuffer_IsFull(&HostToDevice_Buffer)))
			{
				int16_t ReceivedByte = CDC_Device_ReceiveByte(&VirtualSerial_CDC_Interface);

				// Read bytes from the USB OUT endpoint into the local data buffer
				if (!(ReceivedByte < 0))
					RingBuffer_Insert(&HostToDevice_Buffer, ReceivedByte);
			}
		}

		// Read bytes from the local data buffer into the USB IN endpoint
		RingBuff_Count_t BufferCount = RingBuffer_GetCount(&DeviceToHost_Buffer);
		while (BufferCount--)
			CDC_Device_SendByte(&VirtualSerial_CDC_Interface, RingBuffer_Remove(&DeviceToHost_Buffer));

		CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
		USB_USBTask();

		MainTask();
	}


	/* Original for(...) loop with only a single byte retrieved from the OUT buffer per loop
	for (;;)
	{
		// Only try to read in bytes from the CDC interface if the transmit buffer is not full
		if (!(RingBuffer_IsFull(&HostToDevice_Buffer)))
		{
			int16_t ReceivedByte = CDC_Device_ReceiveByte(&VirtualSerial_CDC_Interface);

			// Read bytes from the USB OUT endpoint into the USART transmit buffer
			if (!(ReceivedByte < 0))
				RingBuffer_Insert(&HostToDevice_Buffer, ReceivedByte);
		}
		
		// Check if the UART receive buffer flush timer has expired or the buffer is nearly full
		RingBuff_Count_t BufferCount = RingBuffer_GetCount(&DeviceToHost_Buffer);
		if ((TIFR0 & (1 << TOV0)) || (BufferCount > 200))
		{
			// Clear flush timer expiry flag
			TIFR0 |= (1 << TOV0);

			// Read bytes from the USART receive buffer into the USB IN endpoint
			while (BufferCount--) {
				CDC_Device_SendByte(&VirtualSerial_CDC_Interface, RingBuffer_Remove(&DeviceToHost_Buffer));

				// Try to send the next byte of data to the host, abort if there is an error without dequeuing
				//if (CDC_Device_SendByte(&VirtualSerial_CDC_Interface, RingBuffer_PeekElement(&DeviceToHost_Buffer)) != ENDPOINT_READYWAIT_NoError) {
				//	break;
				//}

				// Dequeue the already sent byte from the buffer now we have confirmed that no transmission error occurred
				//RingBuffer_Remove(&DeviceToHost_Buffer);
			}
		}

		// Load the next byte from the USART transmit buffer into the USART
		//if (!(RingBuffer_IsEmpty(&HostToDevice_Buffer)))
		//	Serial_TxByte(RingBuffer_Remove(&HostToDevice_Buffer));

		CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
		USB_USBTask();

		MainTask();
	}
*/

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
		PORTD = 0;
		DDRB = 0;
		PORTB = 0;
		DDRC = 0;
		PORTC |= (0 << PC2) | (0 << PC4) | (0 << PC5) | (0 << PC6) | (0 << PC7); //only PC2,4,5,6,7 are pins
		// be careful using PortC as PC0 is used for the Crystal and PC1 is nRESET
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
		DDRE = 0;
		PORTE = 0;
		DDRF = 0;
		PORTF = 0;
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

	/* Initialize stdout and stdin for printf and scanf */
	fdevopen(sendData, getData);

	/* TODO - set up ports */
	/* Look over your AVR datasheet's I/O-Ports chapter for more information */
	/* Set up IO Pins for SPI - PB1(SCLK),PB2(MOSI) are output, PB3(MISO) is input*/
	DDRB |= ((1 << PB1) | (1 << PB2) | (0 << PB3));
	PORTB |= ((0 << PB1) | (0 << PB2) | (0 << PB3));
	/* Set ChipSelect pins as output and set to 1/High as they are active-low signals */
	CS_DDR |= (1 << CS_PIN);
	CS_PORT |= (1 << CS_PIN);
	SPI_Init((SPI_MODE_MASTER | SPI_SCK_LEAD_RISING | SPI_SAMPLE_LEADING));

	/* Hardware Initialization */
	LEDs_Init();
	ENABLE_VOLTAGE_TXRX;
	DISABLE_EXT_SRAM;
	SELECT_USB_B;
	USB_Init();

	/* Start the flush timer so that overflows occur rapidly to push received bytes to the USB interface */
	TCCR0B = (1 << CS02);
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


/** Event handler for the CDC Class driver Line Encoding Changed event.
 *
 *  \param[in] CDCInterfaceInfo  Pointer to the CDC class interface configuration structure being referenced
 */
void EVENT_CDC_Device_LineEncodingChanged(USB_ClassInfo_CDC_Device_t* const CDCInterfaceInfo)
{
	/* Use Virtual Serial Port settings to control the application.
		Stop bits control MSB or LSB, baud rate controls SPI clock, databits control
		SPI Mode, and parity controls nothing (for now).
	*/

	uint8_t SPCRmask = (1 << SPE); // SPI Enable
	uint8_t SPSRmask = 0;

	switch (CDCInterfaceInfo->State.LineEncoding.ParityType)
	{
		case CDC_PARITY_Odd:
			spiClkHighOff = 1;
			break;
		case CDC_PARITY_Even:
			spiClkHighOff = 0;
			break;
		case CDC_PARITY_None:
			SPCRmask |= (1 << MSTR);
			break;
		case CDC_PARITY_Mark:
			SPCRmask &= ~(1 << MSTR);
			break;
		case CDC_PARITY_Space:
			sendSPIsettingsRegisters = 1;
			break;
	}


	/* Determine whether data is transmitted LSB (2-stop-bits) or MSB */
	switch (CDCInterfaceInfo->State.LineEncoding.CharFormat)
	{
		case CDC_LINEENCODING_OneStopBit:
			SPCRmask |= (0 << DORD);
			break;
		case CDC_LINEENCODING_OneAndAHalfStopBits:
			SPCRmask |= (0 << DORD);
			break;
		case CDC_LINEENCODING_TwoStopBits:
			SPCRmask |= (1 << DORD);
			break;
	}


	/* Determine data size - 5, 6, 7, or 8 bits are supported */
	/* Changing line coding changes SPI Mode
		CPOL=0, CPHA=0 Sample (Rising) Setup (Falling) SPI-Mode0 == 8 bits line coding
		CPOL=0, CPHA=1 Setup (Rising) Sample (Falling) SPI-Mode1 == 7 bits line coding
		CPOL=1, CPHA=0 Sample (Falling) Setup (Rising) SPI-Mode2 == 6 bits line coding
		CPOL=1, CPHA=1 Setup (Falling) Sample (Rising) SPI-Mode3 == 5 bits line coding
	*/
	switch (CDCInterfaceInfo->State.LineEncoding.DataBits)
	{
		case 8:
			SPCRmask |= ((0 << CPOL) | (0 << CPHA));
			break;
		case 7:
			SPCRmask |= ((0 << CPOL) | (1 << CPHA));
			break;
		case 6:
			SPCRmask |= ((1 << CPOL) | (0 << CPHA));
			break;
		case 5:
			SPCRmask |= ((1 << CPOL) | (1 << CPHA));
			break;
	}


	switch (CDCInterfaceInfo->State.LineEncoding.BaudRateBPS)
	{
		case 9600:
			// 125kHz SPI (Fosc / 64)
			SPCRmask |= ((1 << SPR1) | (0 << SPR0));
			SPSRmask |= (0 << SPI2X);
			break;
		case 14400:
			// 4MHz SPI (Fosc / 2)
			SPCRmask |= ((0 << SPR1) | (0 << SPR0));
			SPSRmask |= (1 << SPI2X);
			break;
		case 19200:
			// 2MHz SPI (Fosc / 4)
			SPCRmask |= ((0 << SPR1) | (0 << SPR0));
			SPSRmask |= (0 << SPI2X);
			break;
		case 38400:
			// 1MHz SPI (Fosc / 8)
			SPCRmask |= ((0 << SPR1) | (1 << SPR0));
			SPSRmask |= (1 << SPI2X);
			break;
		case 57600:
			// 500kHz SPI (Fosc / 16)
			SPCRmask |= ((0 << SPR1) | (1 << SPR0));
			SPSRmask |= (0 << SPI2X);
			break;
		case 115200:
			// 250kHz SPI (Fosc / 32)
			SPCRmask |= ((1 << SPR1) | (0 << SPR0));
			SPSRmask |= (1 << SPI2X);
			break;
		default:
			// 62.5kHz SPI (Fosc / 128)
			SPCRmask |= ((1 << SPR1) | (1 << SPR0));
			SPSRmask |= (0 << SPI2X);
			break;
	}


	if (spiClkHighOff) { PORTB |= (1 << PB1); }

	SPCR = SPCRmask;
	SPSR = SPSRmask;

	// only read if first run
	if (firstRun) {
		tempIOreg = SPSR; //need to read to initiliaze
		tempIOreg = SPDR; //need to read to initiliaze
		firstRun = 0;
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
	return (uint8_t)(!RingBuffer_IsEmpty(&HostToDevice_Buffer));
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
	if (!RingBuffer_IsFull(&DeviceToHost_Buffer)) {
		RingBuffer_Insert(&DeviceToHost_Buffer, (uint8_t)c);
		return 0;
	} else {
		return 1;
	}
}

/* Function to receive data from the USB Virtual Serial Port link */
int getData(FILE *__stream) {
	//if (something) return _FDEV_ERR; // cannot implement as GetElement has no error condition
	if (haveData() == 0) {
		return _FDEV_EOF;
	}
	return (int)RingBuffer_Remove(&HostToDevice_Buffer);
}



/* MainTask will run once initialization is complete */
void MainTask(void)
{
	uint8_t tempByte = 0;

	// If the host has sent data then process it
	while (haveData()) {	// need to check that data exists before processing it
		CS_LOW;		// enable target device by activating its ^CS line

		// send data over SPI (while simultaneously receiving) and send received SPI data over USBVirtualSerial to the host
		tempByte = SPI_TransferByte((uint8_t)getchar());
		putchar((int)tempByte);

	}

	CS_HIGH;

	if (spiClkHighOff) { PORTB |= (1 << PB1); }

	if (sendSPIsettingsRegisters) {
		tempIOreg = SPDR;
		putchar((int)tempIOreg);
		tempIOreg = SPSR;
		putchar((int)tempByte);
		sendSPIsettingsRegisters = 0;
	}

}
