/*
             LUFA Library
     Copyright (C) Dean Camera, 2011.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2011  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Modified for flow control capability by Opendous Inc. 2011-11-14
  www.Micropendous.org/USBtoSerial

  http://www.beyondlogic.org/serial  is a useful introduction to serial/RS-232
  
  Careful consideration was taken to make sure that all signals
  would work with both the Micropendous and Arduino Leonardo boards.

	Micropendous:
	Arduino Pin# - AT90USB1287
	0 - PD2 - RXD - RX : IN : Data Input
	1 - PD3 - TXD - TX : OUT : Data Output
	2 - PD4 - ICP1
	3 - PD5 - 
	4 - PD1 - Int/SDA
	5 - PD0 - Int/SCL
	6 - PD6 - 
	7 - PD7 - 

	Arduino Leonardo:
	Arduino Pin# - ATmega32U4
	0 - PD2 - RXD - RX : IN : Data Input
	1 - PD3 - TXD - TX : OUT : Data Output
	2 - PD1 - Int/SDA
	3 - PD0 - Int/SCL
	4 - PD4 - ICP1
	5 - PC6 - 
	6 - PD7 - 
	7 - PE6 - Int

	Arduino Pin# - v.24 Signal - Micropendous Pin - Leonardo Pin
	0 - RXD (IN)	- PD2		- PD2
	1 - TXD (OUT)	- PD3		- PD3
	2 - nCTS (IN)	- PD4(ICP1)	- PD1
	3 - nDSR (IN)	- PD5		- PD0
	4 - nRTS (OUT)	- PD1		- PD4
	5 - nDTR (OUT)	- PD0		- PC6
	6 - nDCD (IN)	- PD6		- PD7
	7 - nRI (IN)	- PD7		- PE6


  Notes:
	nRTS - OUT - Request to Send
	nDTR - OUT - Data Terminal Ready
	nCTS - IN - Clear To Send Control
	nDSR - IN - Data Set Ready Control
	nDCD - IN - Data Carrier Detect Control
	nRI - IN - Ring Indicator

	For LoopBack or DTE to DTE testing with flow control you will
	need to create a Null Modem cable:
	GND1=GND2
	RXD1=TXD2
	RXD1=TXD2
	RTS1=CTS1 , RTS2=CTS2
	DTR1=DSR1=DCD1, DTR2=DSR2=DCD2


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

// for RS232, a control lone is 'Set' if it is Low='0' and 'Clear' if it is High='1'
#define IS_LINE_SET(PORT_IN, PIN_NUM)		(~PORT_IN & (1 << PIN_NUM))
#define SET_LINE(PORT, PIN_NUM)			PINLOW(PORT, PIN_NUM);
#define CLEAR_LINE(PORT, PIN_NUM)		PINHIGH(PORT, PIN_NUM);


#if (BOARD == BOARD_ARDUINO_LEONARDO)
	#define PIN_CTS		PIND
	#define PORT_CTS	PORTD
	#define DDR_CTS		DDRD
	#define Pn_CTS		PD1

	#define PIN_DSR		PIND
	#define PORT_DSR	PORTD
	#define DDR_DSR		DDRD
	#define Pn_DSR		PD0

	#define PIN_RTS		PIND
	#define PORT_RTS	PORTD
	#define DDR_RTS		DDRD
	#define Pn_RTS		PD4

	#define PIN_DTR		PINC
	#define PORT_DTR	PORTC
	#define DDR_DTR		DDRC
	#define Pn_DTR		PC6

	#define PIN_DCD		PIND
	#define PORT_DCD	PORTD
	#define DDR_DCD		DDRD
	#define Pn_DCD		PD7

	#define PIN_RI		PINE
	#define PORT_RI		PORTE
	#define DDR_RI		DDRE
	#define Pn_RI		PE6
#else
	#define PIN_CTS		PIND
	#define PORT_CTS	PORTD
	#define DDR_CTS		DDRD
	#define Pn_CTS		PD4

	#define PIN_DSR		PIND
	#define PORT_DSR	PORTD
	#define DDR_DSR		DDRD
	#define Pn_DSR		PD5

	#define PIN_RTS		PIND
	#define PORT_RTS	PORTD
	#define DDR_RTS		DDRD
	#define Pn_RTS		PD1

	#define PIN_DTR		PIND
	#define PORT_DTR	PORTD
	#define DDR_DTR		DDRD
	#define Pn_DTR		PD0

	#define PIN_DCD		PIND
	#define PORT_DCD	PORTD
	#define DDR_DCD		DDRD
	#define Pn_DCD		PD6

	#define PIN_RI		PIND
	#define PORT_RI		PORTD
	#define DDR_RI		DDRD
	#define Pn_RI		PD7	
#endif


/** \file
 *
 *  Main source file for the USBtoSerial project. This file contains the main tasks of
 *  the project and is responsible for the initial application hardware configuration.
 */

#include "USBtoSerial.h"

/** Circular buffer to hold data from the host before it is sent to the device via the serial port. */
static RingBuffer_t USBtoUSART_Buffer;

/** Underlying data buffer for \ref USBtoUSART_Buffer, where the stored bytes are located. */
static uint8_t      USBtoUSART_Buffer_Data[128];

/** Circular buffer to hold data from the serial port before it is sent to the host. */
static RingBuffer_t USARTtoUSB_Buffer;

/** Underlying data buffer for \ref USARTtoUSB_Buffer, where the stored bytes are located. */
static uint8_t      USARTtoUSB_Buffer_Data[128];

/** UART error flags */
volatile uint8_t	__UARTLineStatesAndFlags = 0;
volatile uint8_t	__UARTUseRTSCTSFlowControl = 0;

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

	RingBuffer_InitBuffer(&USBtoUSART_Buffer, USBtoUSART_Buffer_Data, sizeof(USBtoUSART_Buffer_Data));
	RingBuffer_InitBuffer(&USARTtoUSB_Buffer, USARTtoUSB_Buffer_Data, sizeof(USARTtoUSB_Buffer_Data));

	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
	sei();

	for (;;)
	{
		// Only try to read in bytes from the CDC interface if the transmit buffer is not full
		if (!(RingBuffer_IsFull(&USBtoUSART_Buffer)))
		{
			int16_t ReceivedByte = CDC_Device_ReceiveByte(&VirtualSerial_CDC_Interface);

			// Read bytes from the USB OUT endpoint into the USART transmit buffer
			if (!(ReceivedByte < 0)) {
				RingBuffer_Insert(&USBtoUSART_Buffer, ReceivedByte);
			}
		}

		// Check if the UART receive buffer flush timer has expired or the buffer is nearly full
		uint16_t BufferCount = RingBuffer_GetCount(&USARTtoUSB_Buffer);
		if ((TIFR0 & (1 << TOV0)) || (BufferCount > ((uint8_t)((sizeof(USARTtoUSB_Buffer_Data)) *.75))))
		{
			// Clear flush timer expiry flag
			TIFR0 |= (1 << TOV0);

			// Read bytes from the USART receive buffer into the USB IN endpoint
			while (BufferCount--)
			{
				// Try to send the next byte of data to the host, abort if there is an error without dequeuing
				if (CDC_Device_SendByte(&VirtualSerial_CDC_Interface,
								RingBuffer_Peek(&USARTtoUSB_Buffer)) != ENDPOINT_READYWAIT_NoError)
				{
					break;
				}

				// Dequeue the already sent byte from the buffer now we have confirmed that no transmission error occurred
				RingBuffer_Remove(&USARTtoUSB_Buffer);
			}
		}

		// Load the next byte from the USART transmit buffer into the USART
		if (!(RingBuffer_IsEmpty(&USBtoUSART_Buffer))) {
			// if RTS/CTS flow control is enabled then only copy to USART if nCTS is 'Set'
			if (__UARTUseRTSCTSFlowControl && IS_LINE_SET(PIN_CTS, Pn_CTS)) {
				Serial_SendByte(RingBuffer_Remove(&USBtoUSART_Buffer));
			} else if (!__UARTUseRTSCTSFlowControl) {
				Serial_SendByte(RingBuffer_Remove(&USBtoUSART_Buffer));
			} else {
				// sending over USART is not valid
			}
		}

		// send any Line state changes or error flags
		LineStates_ErrorFlags();

		CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
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

	/* Hardware Initialization */
	JTAG_DISABLE();

	/* enable Ports based on which IC is being used */
	/* For more information look over the corresponding AVR's datasheet in the
		'I/O Ports' Chapter under subheading 'Ports as General Digital I/O' */
	#if (defined(__AVR_AT90USB162__) || defined(__AVR_AT90USB82__) || \
			defined(__AVR_ATmega16U2__) || defined(__AVR_ATmega32U2__))
		DDRD  = 0;
		PORTD = 0;
		DDRB  = 0;
		PORTB = 0;
		DDRC  |= ((0 << PC2) | (0 << PC4) | (0 << PC5) | (0 << PC6) | (0 << PC7)); //only PC2,4,5,6,7 are pins
		PORTC |= ((0 << PC2) | (0 << PC4) | (0 << PC5) | (0 << PC6) | (0 << PC7)); //only PC2,4,5,6,7 are pins
		// be careful using PortC as PC0 is used for the Crystal and PC1 is nRESET
	#endif

	#if (defined(__AVR_ATmega16U4__)  || defined(__AVR_ATmega32U4__))
		// set up Ports for v.24 UART while maintaining compatibility with Arduino Leonardo pinout
		DDRD  = 0;
		PORTD = 0;
		DDRB  = 0;
		PORTB = 0;
		DDRC  = ((0 << PC6) | (0 << PC7)); //only PC6,7 are pins
		PORTC = ((0 << PC6) | (0 << PC7)); //only PC6,7 are pins
		DDRE  = ((0 << PE2) | (0 << PE6)); //only PE2,6 are pins
		PORTE = ((0 << PE2) | (0 << PE6)); //only PE2,6 are pins
		DDRF  = ((0 << PF0) | (0 << PF1) | (0 << PF4) | (0 << PF5) | (0 << PF6) | (0 << PF7)); // only PF0,1,4,5,6,7 are pins
		PORTF = ((0 << PF0) | (0 << PF1) | (0 << PF4) | (0 << PF5) | (0 << PF6) | (0 << PF7)); // only PF0,1,4,5,6,7 are pins
	#endif

	#if (defined(__AVR_AT90USB1287__) || defined(__AVR_AT90USB647__) ||  \
			defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB646__) ||  \
			defined(__AVR_ATmega32U6__))
		DDRA  = 0;
		PORTA = 0;
		DDRB  = 0;
		PORTB = 0;
		DDRC  = 0;
		PORTC = 0;
		DDRD  = 0;
		PORTD = 0;
		DDRE  = 0;
		PORTE = 0;
		DDRF  = 0;
		PORTF = 0;
	#endif

	// Input pins have DDR=0 and PORT=0 while output pins are DDR=1
	// nCTS, nDSR, nDCD, nRI are inputs so don't need to change anything
	// nRTS, nDTR are outputs with High Logic Levels to start:
	DDR_RTS |= (1 << Pn_RTS); SET_LINE(PORT_RTS, Pn_RTS);
	DDR_DTR |= (1 << Pn_DTR); SET_LINE(PORT_DTR, Pn_DTR);

	Board_Init(); // initialize LEDs and Buttons and any other peripherals
	DISABLE_VOLTAGE_TXRX;
	DISABLE_EXT_SRAM;
	SELECT_USB_B;
	USB_Init();

	/* Start the flush timer so that overflows occur rapidly to push received bytes to the USB interface */
	TCCR0B = (1 << CS02); // prescale core clock by /256 for timer clock
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

/** ISR to manage the reception of data from the serial port, placing received bytes into a circular buffer
 *  for later transmission to the host.
 */
ISR(USART1_RX_vect, ISR_BLOCK)
{
	__UARTLineStatesAndFlags = UCSR1A; // must be read before UDRn
	// for 9-bit bata, the RXB81 bit should be read from UCSR1B BEFORE reading UDR1

	uint8_t ReceivedByte = UDR1;

	if (USB_DeviceState == DEVICE_STATE_Configured)
		RingBuffer_Insert(&USARTtoUSB_Buffer, ReceivedByte);
}

	
/** CDC class driver event for a control line state change on a CDC interface. This event fires each time the host requests a
 *  control line state change (containing the virtual serial control line states, such as DTR). The new control line states
 *  are available in the ControlLineStates.HostToDevice value inside the CDC interface structure passed as a parameter, set as
 *  a mask of CDC_CONTROL_LINE_OUT_* masks.  1 is for 'Set'(Low) and 0 is for 'Clear'(High) as these are active low signals.
 *  \param[in,out] CDCInterfaceInfo  Pointer to a structure containing a CDC Class configuration and state
*/
void EVENT_CDC_Device_ControLineStateChanged(USB_ClassInfo_CDC_Device_t* const CDCInterfaceInfo)
{

	if ((CDCInterfaceInfo->State.ControlLineStates.HostToDevice) & CDC_CONTROL_LINE_OUT_RTS) {
		// Host has set the RTS line
		SET_LINE(PORT_RTS, Pn_RTS);
		__UARTUseRTSCTSFlowControl = 1; // RTS-CTS flow control must be enabled if this flag is set
	} else {
		// Host has cleared the RTS line
		CLEAR_LINE(PORT_RTS, Pn_RTS);
	}

	if ((CDCInterfaceInfo->State.ControlLineStates.HostToDevice) & CDC_CONTROL_LINE_OUT_DTR) {
		// Host has set the DTR line
		SET_LINE(PORT_DTR, Pn_DTR);
		__UARTUseRTSCTSFlowControl = 1; // RTS-CTS flow control must be enabled if this flag is set
	} else {
		CLEAR_LINE(PORT_DTR, Pn_DTR);
	}
}


/** Event handler for the CDC Class driver Line Encoding Changed event.
 *
 *  \param[in] CDCInterfaceInfo  Pointer to the CDC class interface configuration structure being referenced
 */
void EVENT_CDC_Device_LineEncodingChanged(USB_ClassInfo_CDC_Device_t* const CDCInterfaceInfo)
{
	uint8_t ConfigMask = 0;

	// the Line Encoding will change each time a new connection is intitiated with
	// the device so this is a good place to intialize standard settings
	SET_LINE(PORT_RTS, Pn_RTS);
	SET_LINE(PORT_DTR, Pn_DTR);

	switch (CDCInterfaceInfo->State.LineEncoding.ParityType)
	{
		case CDC_PARITY_Odd:
			ConfigMask = ((1 << UPM11) | (1 << UPM10));
			break;
		case CDC_PARITY_Even:
			ConfigMask = (1 << UPM11);
			break;
	}

	if (CDCInterfaceInfo->State.LineEncoding.CharFormat == CDC_LINEENCODING_TwoStopBits) {
		ConfigMask |= (1 << USBS1);
	} // else USBS1=0 and 1 stop-bit is used

	switch (CDCInterfaceInfo->State.LineEncoding.DataBits)
	{
		case 6:
			ConfigMask |= (1 << UCSZ10);
			break;
		case 7:
			ConfigMask |= (1 << UCSZ11);
			break;
		case 8:
			ConfigMask |= ((1 << UCSZ11) | (1 << UCSZ10));
			break;
	}

	/* Must turn off USART before reconfiguring it, otherwise incorrect operation may occur */
	UCSR1B = 0;
	UCSR1A = 0;
	UCSR1C = 0;

	/* Set the new baud rate before configuring the USART */
	UBRR1  = SERIAL_2X_UBBRVAL(CDCInterfaceInfo->State.LineEncoding.BaudRateBPS);

	/* Reconfigure the USART in double speed mode for a wider baud rate range at the expense of accuracy */
	UCSR1C = ConfigMask;
	UCSR1A = (1 << U2X1);
	UCSR1B = ((1 << RXCIE1) | (1 << TXEN1) | (1 << RXEN1));
} // void EVENT_CDC_Device_LineEncodingChanged


/** CDC class driver event for a send break request sent to the device from the host. This is generally used to separate
 *  data or to indicate a special condition to the receiving device.
 *
 *  \param[in,out] CDCInterfaceInfo  Pointer to a structure containing a CDC Class configuration and state.
 *  \param[in]     Duration          Duration of the break that has been sent by the host, in milliseconds.
 */
void EVENT_CDC_Device_BreakSent(USB_ClassInfo_CDC_Device_t* const CDCInterfaceInfo, const uint8_t Duration)
{
	uint8_t PrevUCSR1B = UCSR1B;
	uint8_t PrevUCSR1A = UCSR1A;
	uint8_t PrevUCSR1C = UCSR1C;
	uint8_t PrevUBRR1 = UBRR1;

	// the 'idle' state is Mark=High='1'.  A break is a period of Space=Low='0' and is equivalent
	// to sending 0x00 with no stop bits for the intended period of time. TXD=PD3

	// disable USART to allow manual control of TXD
	cli();
	UCSR1B = 0;
	UCSR1A = 0;
	UCSR1C = 0;
	PINLOW(PORTD, PD3);
	sei();

	_delay_ms(Duration);

	// re-enable the USART as before
	cli();
	PINHIGH(PORTD, PD3);
	UCSR1C = PrevUCSR1C;
	UCSR1A = PrevUCSR1A;
	UCSR1B = PrevUCSR1B;
	UBRR1 = PrevUBRR1;
	sei();
}


void LineStates_ErrorFlags(void)
{

	uint16_t  __PreviousControlLineState = (VirtualSerial_CDC_Interface.State.ControlLineStates.DeviceToHost);

	// The nDCD line state has been set
	if (IS_LINE_SET(PIN_DCD, Pn_DCD)) {
		(VirtualSerial_CDC_Interface.State.ControlLineStates.DeviceToHost) |= CDC_CONTROL_LINE_IN_DCD;
	} else {
		(VirtualSerial_CDC_Interface.State.ControlLineStates.DeviceToHost) &= ~CDC_CONTROL_LINE_IN_DCD;
	}

	// The nDSR line state has been set
	if (IS_LINE_SET(PIN_DSR, Pn_DSR)) {
		(VirtualSerial_CDC_Interface.State.ControlLineStates.DeviceToHost) |= CDC_CONTROL_LINE_IN_DSR;
	} else {
		(VirtualSerial_CDC_Interface.State.ControlLineStates.DeviceToHost) &= ~CDC_CONTROL_LINE_IN_DSR;
	}

	// The nRI line state has been set
	if (IS_LINE_SET(PIN_RI, Pn_RI)) {
		(VirtualSerial_CDC_Interface.State.ControlLineStates.DeviceToHost) |= CDC_CONTROL_LINE_IN_RING;
	} else {
		(VirtualSerial_CDC_Interface.State.ControlLineStates.DeviceToHost) &= ~CDC_CONTROL_LINE_IN_RING;
	}

	// A framing error has occurred which also means that a BREAK has occured
	if (__UARTLineStatesAndFlags & (1 << FE1)) {
		(VirtualSerial_CDC_Interface.State.ControlLineStates.DeviceToHost) |= CDC_CONTROL_LINE_IN_FRAMEERROR;
		(VirtualSerial_CDC_Interface.State.ControlLineStates.DeviceToHost) |= CDC_CONTROL_LINE_IN_BREAK;
	} else {
		(VirtualSerial_CDC_Interface.State.ControlLineStates.DeviceToHost) &= ~CDC_CONTROL_LINE_IN_FRAMEERROR;
		(VirtualSerial_CDC_Interface.State.ControlLineStates.DeviceToHost) &= ~CDC_CONTROL_LINE_IN_BREAK;
	}

	// A parity error has occurred
	if (__UARTLineStatesAndFlags & (1 << UPE1)) {
		(VirtualSerial_CDC_Interface.State.ControlLineStates.DeviceToHost) |= CDC_CONTROL_LINE_IN_PARITYERROR;
	} else {
		(VirtualSerial_CDC_Interface.State.ControlLineStates.DeviceToHost) &= ~CDC_CONTROL_LINE_IN_PARITYERROR;
	}

	// A data overrun error has occurred
	if (__UARTLineStatesAndFlags & (1 << DOR1)) {
		(VirtualSerial_CDC_Interface.State.ControlLineStates.DeviceToHost) |= CDC_CONTROL_LINE_IN_OVERRUNERROR;
	} else {
		(VirtualSerial_CDC_Interface.State.ControlLineStates.DeviceToHost) &= ~CDC_CONTROL_LINE_IN_OVERRUNERROR;
	}

	// If something has changed, tell the host
	if ((VirtualSerial_CDC_Interface.State.ControlLineStates.DeviceToHost) != __PreviousControlLineState) {
		CDC_Device_SendControlLineStateChange(&VirtualSerial_CDC_Interface);
		__UARTLineStatesAndFlags = 0;
	}

} // LineStates_ErrorFlags(void)
