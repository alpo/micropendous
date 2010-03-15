/*
             LUFA Library
     Copyright (C) Dean Camera, 2010.
              
  dean [at] fourwalledcubicle [dot] com
      www.fourwalledcubicle.com
*/

/*
  Copyright 2010  Denver Gingerich (denver [at] ossguy [dot] com)
      Based on code by Dean Camera (dean [at] fourwalledcubicle [dot] com)

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
 *  Main source file for the Keyboard demo. This file contains the main tasks of
 *  the demo and is responsible for the initial application hardware configuration.
 */

#include "MicropendousKeyboardTest.h"

/** Buffer to hold the previously generated Keyboard HID report, for comparison purposes inside the HID class driver. */
uint8_t PrevKeyboardHIDReportBuffer[sizeof(USB_KeyboardReport_Data_t)];

/** LUFA HID Class driver interface configuration and state information. This structure is
 *  passed to all HID Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_HID_Device_t Keyboard_HID_Interface =
 	{
		.Config =
			{
				.InterfaceNumber              = 0,

				.ReportINEndpointNumber       = KEYBOARD_EPNUM,
				.ReportINEndpointSize         = KEYBOARD_EPSIZE,
				.ReportINEndpointDoubleBank   = false,

				.PrevReportINBuffer           = PrevKeyboardHIDReportBuffer,
				.PrevReportINBufferSize       = sizeof(PrevKeyboardHIDReportBuffer),
			},
    };

/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */
int main(void)
{
	SetupHardware();

	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
	
	for (;;)
	{
		HID_Device_USBTask(&Keyboard_HID_Interface);
		USB_USBTask();
	}
}

/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware()
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
	/* enable Ports for input based on which IC is being used*/
	/* For more information look over the corresponding AVR's datasheet in the
		'I/O Ports' Chapter under subheading 'Ports as General Digital I/O' */
	#if (defined(__AVR_AT90USB162__)  || defined(__AVR_AT90USB82__))
		DDRD = 0;
		PORTD = 0xFF;
		DDRB = 0;
		PORTB = 0xFF;
		DDRC = 0;
		PORTC |= (1 << PC2) | (1 << PC4) | (1 << PC5) | (1 << PC6) | (1 << PC7); //only PC2,4,5,6,7 are pins
		// be careful using PortC as PC0 is used for the Crystal and PC1 is RESET
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
		DDRE = ((1 << PE4) | (1 << PE6));	// set PE4, PE6 to HIGH to disable external SRAM, if connected
		PORTE = 0xFF;	// set PE4, PE6 to HIGH to disable external SRAM, if connected
		DDRF = 0;
		PORTF = 0xFF;
	#endif

	/* Hardware Initialization */
	LEDs_Init();
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
	LEDs_SetAllLEDs(LEDMASK_USB_READY);

	if (!(HID_Device_ConfigureEndpoints(&Keyboard_HID_Interface)))
	  LEDs_SetAllLEDs(LEDMASK_USB_ERROR);

	USB_Device_EnableSOFEvents();
}

/** Event handler for the library USB Unhandled Control Request event. */
void EVENT_USB_Device_UnhandledControlRequest(void)
{
	HID_Device_ProcessControlRequest(&Keyboard_HID_Interface);
}

/** Event handler for the USB device Start Of Frame event. */
void EVENT_USB_Device_StartOfFrame(void)
{
	HID_Device_MillisecondElapsed(&Keyboard_HID_Interface);
}

/** HID class driver callback function for the creation of HID reports to the host.
 *
 *  \param[in] HIDInterfaceInfo  Pointer to the HID class interface configuration structure being referenced
 *  \param[in,out] ReportID  Report ID requested by the host if non-zero, otherwise callback should set to the generated report ID
 *  \param[in] ReportType  Type of the report to create, either REPORT_ITEM_TYPE_In or REPORT_ITEM_TYPE_Feature
 *  \param[out] ReportData  Pointer to a buffer where the created report should be stored
 *  \param[out] ReportSize  Number of bytes written in the report (or zero if no report is to be sent
 *
 *  \return Boolean true to force the sending of the report, false to let the library determine if it needs to be sent
 */
bool CALLBACK_HID_Device_CreateHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo, uint8_t* const ReportID,
                                         const uint8_t ReportType, void* ReportData, uint16_t* ReportSize)
{
	USB_KeyboardReport_Data_t* KeyboardReport = (USB_KeyboardReport_Data_t*)ReportData;

	KeyboardReport->KeyCode[0] = 0x00;

	//KeyboardReport->Modifier = HID_KEYBOARD_MODIFER_LEFTSHIFT;

	// 32-pin USB AVRs
	#if (defined(__AVR_AT90USB162__)  || defined(__AVR_AT90USB82__))
				  if (~PINB & (1 << PINB0)) {
			KeyboardReport->KeyCode[0] = 0x05; //b
			KeyboardReport->KeyCode[1] = 0x27; //0
		} else if (~PINB & (1 << PINB1)) {
			KeyboardReport->KeyCode[0] = 0x05; //b
			KeyboardReport->KeyCode[1] = 0x1E; //1
		} else if (~PINB & (1 << PINB2)) {
			KeyboardReport->KeyCode[0] = 0x05; //b
			KeyboardReport->KeyCode[1] = 0x1F; //2
		} else if (~PINB & (1 << PINB3)) {
			KeyboardReport->KeyCode[0] = 0x05; //b
			KeyboardReport->KeyCode[1] = 0x20; //3
		} else if (~PINB & (1 << PINB4)) {
			KeyboardReport->KeyCode[0] = 0x05; //b
			KeyboardReport->KeyCode[1] = 0x21; //4
		} else if (~PINB & (1 << PINB5)) {
			KeyboardReport->KeyCode[0] = 0x05; //b
			KeyboardReport->KeyCode[1] = 0x22; //5
		} else if (~PINB & (1 << PINB6)) {
			KeyboardReport->KeyCode[0] = 0x05; //b
			KeyboardReport->KeyCode[1] = 0x23; //6
		} else if (~PINB & (1 << PINB7)) {
			KeyboardReport->KeyCode[0] = 0x05; //b
			KeyboardReport->KeyCode[1] = 0x24; //7

		} else if (~PINC & (1 << PINC2)) {
			KeyboardReport->KeyCode[0] = 0x06; //c
			KeyboardReport->KeyCode[1] = 0x1F; //2
		} else if (~PINC & (1 << PINC4)) {
			KeyboardReport->KeyCode[0] = 0x06; //c
			KeyboardReport->KeyCode[1] = 0x21; //4
		} else if (~PINC & (1 << PINC5)) {
			KeyboardReport->KeyCode[0] = 0x06; //c
			KeyboardReport->KeyCode[1] = 0x22; //5
		} else if (~PINC & (1 << PINC6)) {
			KeyboardReport->KeyCode[0] = 0x06; //c
			KeyboardReport->KeyCode[1] = 0x23; //6
		} else if (~PINC & (1 << PINC7)) {
			KeyboardReport->KeyCode[0] = 0x06; //c
			KeyboardReport->KeyCode[1] = 0x24; //7

		} else if (~PIND & (1 << PIND0)) {
			KeyboardReport->KeyCode[0] = 0x07; //d
			KeyboardReport->KeyCode[1] = 0x27; //0
		} else if (~PIND & (1 << PIND1)) {
			KeyboardReport->KeyCode[0] = 0x07; //d
			KeyboardReport->KeyCode[1] = 0x1E; //1
		} else if (~PIND & (1 << PIND2)) {
			KeyboardReport->KeyCode[0] = 0x07; //d
			KeyboardReport->KeyCode[1] = 0x1F; //2
		} else if (~PIND & (1 << PIND3)) {
			KeyboardReport->KeyCode[0] = 0x07; //d
			KeyboardReport->KeyCode[1] = 0x20; //3
		} else if (~PIND & (1 << PIND4)) {
			KeyboardReport->KeyCode[0] = 0x07; //d
			KeyboardReport->KeyCode[1] = 0x21; //4
		} else if (~PIND & (1 << PIND5)) {
			KeyboardReport->KeyCode[0] = 0x07; //d
			KeyboardReport->KeyCode[1] = 0x22; //5
		} else if (~PIND & (1 << PIND6)) {
			KeyboardReport->KeyCode[0] = 0x07; //d
			KeyboardReport->KeyCode[1] = 0x23; //6
		} else if (~PIND & (1 << PIND7)) {
			KeyboardReport->KeyCode[0] = 0x07; //d
			KeyboardReport->KeyCode[1] = 0x24; //7
		}
	#endif


	// 44-pin USB AVRs
	#if (defined(__AVR_ATmega16U4__)  || defined(__AVR_ATmega32U4__))
				  if (~PINB & (1 << PINB0)) {
			KeyboardReport->KeyCode[0] = 0x05; //b
			KeyboardReport->KeyCode[1] = 0x27; //0
		} else if (~PINB & (1 << PINB1)) {
			KeyboardReport->KeyCode[0] = 0x05; //b
			KeyboardReport->KeyCode[1] = 0x1E; //1
		} else if (~PINB & (1 << PINB2)) {
			KeyboardReport->KeyCode[0] = 0x05; //b
			KeyboardReport->KeyCode[1] = 0x1F; //2
		} else if (~PINB & (1 << PINB3)) {
			KeyboardReport->KeyCode[0] = 0x05; //b
			KeyboardReport->KeyCode[1] = 0x20; //3
		} else if (~PINB & (1 << PINB4)) {
			KeyboardReport->KeyCode[0] = 0x05; //b
			KeyboardReport->KeyCode[1] = 0x21; //4
		} else if (~PINB & (1 << PINB5)) {
			KeyboardReport->KeyCode[0] = 0x05; //b
			KeyboardReport->KeyCode[1] = 0x22; //5
		} else if (~PINB & (1 << PINB6)) {
			KeyboardReport->KeyCode[0] = 0x05; //b
			KeyboardReport->KeyCode[1] = 0x23; //6
		} else if (~PINB & (1 << PINB7)) {
			KeyboardReport->KeyCode[0] = 0x05; //b
			KeyboardReport->KeyCode[1] = 0x24; //7

		} else if (~PINC & (1 << PINC6)) {
			KeyboardReport->KeyCode[0] = 0x06; //c
			KeyboardReport->KeyCode[1] = 0x23; //6
		} else if (~PINC & (1 << PINC7)) {
			KeyboardReport->KeyCode[0] = 0x06; //c
			KeyboardReport->KeyCode[1] = 0x24; //7

		} else if (~PIND & (1 << PIND0)) {
			KeyboardReport->KeyCode[0] = 0x07; //d
			KeyboardReport->KeyCode[1] = 0x27; //0
		} else if (~PIND & (1 << PIND1)) {
			KeyboardReport->KeyCode[0] = 0x07; //d
			KeyboardReport->KeyCode[1] = 0x1E; //1
		} else if (~PIND & (1 << PIND2)) {
			KeyboardReport->KeyCode[0] = 0x07; //d
			KeyboardReport->KeyCode[1] = 0x1F; //2
		} else if (~PIND & (1 << PIND3)) {
			KeyboardReport->KeyCode[0] = 0x07; //d
			KeyboardReport->KeyCode[1] = 0x20; //3
		} else if (~PIND & (1 << PIND4)) {
			KeyboardReport->KeyCode[0] = 0x07; //d
			KeyboardReport->KeyCode[1] = 0x21; //4
		} else if (~PIND & (1 << PIND5)) {
			KeyboardReport->KeyCode[0] = 0x07; //d
			KeyboardReport->KeyCode[1] = 0x22; //5
		} else if (~PIND & (1 << PIND6)) {
			KeyboardReport->KeyCode[0] = 0x07; //d
			KeyboardReport->KeyCode[1] = 0x23; //6
		} else if (~PIND & (1 << PIND7)) {
			KeyboardReport->KeyCode[0] = 0x07; //d
			KeyboardReport->KeyCode[1] = 0x24; //7

		} else if (~PINE & (1 << PINE2)) {
			KeyboardReport->KeyCode[0] = 0x08; //e
			KeyboardReport->KeyCode[1] = 0x1F; //2
		} else if (~PINE & (1 << PINE6)) {
			KeyboardReport->KeyCode[0] = 0x08; //e
			KeyboardReport->KeyCode[1] = 0x23; //6

		} else if (~PINF & (1 << PINF0)) {
			KeyboardReport->KeyCode[0] = 0x09; //f
			KeyboardReport->KeyCode[1] = 0x27; //0
		} else if (~PINF & (1 << PINF1)) {
			KeyboardReport->KeyCode[0] = 0x09; //f
			KeyboardReport->KeyCode[1] = 0x1E; //1
		} else if (~PINF & (1 << PINF4)) {
			KeyboardReport->KeyCode[0] = 0x09; //f
			KeyboardReport->KeyCode[1] = 0x21; //4
		} else if (~PINF & (1 << PINF5)) {
			KeyboardReport->KeyCode[0] = 0x09; //f
			KeyboardReport->KeyCode[1] = 0x22; //5
		} else if (~PINF & (1 << PINF6)) {
			KeyboardReport->KeyCode[0] = 0x09; //f
			KeyboardReport->KeyCode[1] = 0x23; //6
		} else if (~PINF & (1 << PINF7)) {
			KeyboardReport->KeyCode[0] = 0x09; //f
			KeyboardReport->KeyCode[1] = 0x24; //7
		}
	#endif


	// 64-pin USB AVRs
	#if (defined(__AVR_AT90USB1287__) || defined(__AVR_AT90USB647__) ||  \
			defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB646__) ||  \
			defined(__AVR_ATmega32U6__))

				  if (~PINA & (1 << PINA0)) {
			KeyboardReport->KeyCode[0] = 0x04; //a
			KeyboardReport->KeyCode[1] = 0x27; //0
		} else if (~PINA & (1 << PINA1)) {
			KeyboardReport->KeyCode[0] = 0x04; //a
			KeyboardReport->KeyCode[1] = 0x1E; //1
		} else if (~PINA & (1 << PINA2)) {
			KeyboardReport->KeyCode[0] = 0x04; //a
			KeyboardReport->KeyCode[1] = 0x1F; //2
		} else if (~PINA & (1 << PINA3)) {
			KeyboardReport->KeyCode[0] = 0x04; //a
			KeyboardReport->KeyCode[1] = 0x20; //3
		} else if (~PINA & (1 << PINA4)) {
			KeyboardReport->KeyCode[0] = 0x04; //a
			KeyboardReport->KeyCode[1] = 0x21; //4
		} else if (~PINA & (1 << PINA5)) {
			KeyboardReport->KeyCode[0] = 0x04; //a
			KeyboardReport->KeyCode[1] = 0x22; //5
		} else if (~PINA & (1 << PINA6)) {
			KeyboardReport->KeyCode[0] = 0x04; //a
			KeyboardReport->KeyCode[1] = 0x23; //6
		} else if (~PINA & (1 << PINA7)) {
			KeyboardReport->KeyCode[0] = 0x04; //a
			KeyboardReport->KeyCode[1] = 0x24; //7

		} else if (~PINB & (1 << PINB0)) {
			KeyboardReport->KeyCode[0] = 0x05; //b
			KeyboardReport->KeyCode[1] = 0x27; //0
		} else if (~PINB & (1 << PINB1)) {
			KeyboardReport->KeyCode[0] = 0x05; //b
			KeyboardReport->KeyCode[1] = 0x1E; //1
		} else if (~PINB & (1 << PINB2)) {
			KeyboardReport->KeyCode[0] = 0x05; //b
			KeyboardReport->KeyCode[1] = 0x1F; //2
		} else if (~PINB & (1 << PINB3)) {
			KeyboardReport->KeyCode[0] = 0x05; //b
			KeyboardReport->KeyCode[1] = 0x20; //3
		} else if (~PINB & (1 << PINB4)) {
			KeyboardReport->KeyCode[0] = 0x05; //b
			KeyboardReport->KeyCode[1] = 0x21; //4
		} else if (~PINB & (1 << PINB5)) {
			KeyboardReport->KeyCode[0] = 0x05; //b
			KeyboardReport->KeyCode[1] = 0x22; //5
		} else if (~PINB & (1 << PINB6)) {
			KeyboardReport->KeyCode[0] = 0x05; //b
			KeyboardReport->KeyCode[1] = 0x23; //6
		} else if (~PINB & (1 << PINB7)) {
			KeyboardReport->KeyCode[0] = 0x05; //b
			KeyboardReport->KeyCode[1] = 0x24; //7

		} else if (~PINC & (1 << PINC0)) {
			KeyboardReport->KeyCode[0] = 0x06; //c
			KeyboardReport->KeyCode[1] = 0x27; //0
		} else if (~PINC & (1 << PINC1)) {
			KeyboardReport->KeyCode[0] = 0x06; //c
			KeyboardReport->KeyCode[1] = 0x1E; //1
		} else if (~PINC & (1 << PINC2)) {
			KeyboardReport->KeyCode[0] = 0x06; //c
			KeyboardReport->KeyCode[1] = 0x1F; //2
		} else if (~PINC & (1 << PINC3)) {
			KeyboardReport->KeyCode[0] = 0x06; //c
			KeyboardReport->KeyCode[1] = 0x20; //3
		} else if (~PINC & (1 << PINC4)) {
			KeyboardReport->KeyCode[0] = 0x06; //c
			KeyboardReport->KeyCode[1] = 0x21; //4
		} else if (~PINC & (1 << PINC5)) {
			KeyboardReport->KeyCode[0] = 0x06; //c
			KeyboardReport->KeyCode[1] = 0x22; //5
		} else if (~PINC & (1 << PINC6)) {
			KeyboardReport->KeyCode[0] = 0x06; //c
			KeyboardReport->KeyCode[1] = 0x23; //6
		} else if (~PINC & (1 << PINC7)) {
			KeyboardReport->KeyCode[0] = 0x06; //c
			KeyboardReport->KeyCode[1] = 0x24; //7

		} else if (~PIND & (1 << PIND0)) {
			KeyboardReport->KeyCode[0] = 0x07; //d
			KeyboardReport->KeyCode[1] = 0x27; //0
		} else if (~PIND & (1 << PIND1)) {
			KeyboardReport->KeyCode[0] = 0x07; //d
			KeyboardReport->KeyCode[1] = 0x1E; //1
		} else if (~PIND & (1 << PIND2)) {
			KeyboardReport->KeyCode[0] = 0x07; //d
			KeyboardReport->KeyCode[1] = 0x1F; //2
		} else if (~PIND & (1 << PIND3)) {
			KeyboardReport->KeyCode[0] = 0x07; //d
			KeyboardReport->KeyCode[1] = 0x20; //3
		} else if (~PIND & (1 << PIND4)) {
			KeyboardReport->KeyCode[0] = 0x07; //d
			KeyboardReport->KeyCode[1] = 0x21; //4
		} else if (~PIND & (1 << PIND5)) {
			KeyboardReport->KeyCode[0] = 0x07; //d
			KeyboardReport->KeyCode[1] = 0x22; //5
		} else if (~PIND & (1 << PIND6)) {
			KeyboardReport->KeyCode[0] = 0x07; //d
			KeyboardReport->KeyCode[1] = 0x23; //6
		} else if (~PIND & (1 << PIND7)) {
			KeyboardReport->KeyCode[0] = 0x07; //d
			KeyboardReport->KeyCode[1] = 0x24; //7

		} else if (~PINE & (1 << PINE0)) {
			KeyboardReport->KeyCode[0] = 0x08; //e
			KeyboardReport->KeyCode[1] = 0x27; //0
		} else if (~PINE & (1 << PINE1)) {
			KeyboardReport->KeyCode[0] = 0x08; //e
			KeyboardReport->KeyCode[1] = 0x1E; //1
		} else if (~PINE & (1 << PINE2)) {
			KeyboardReport->KeyCode[0] = 0x08; //e
			KeyboardReport->KeyCode[1] = 0x1F; //2
		} else if (~PINE & (1 << PINE3)) {
			KeyboardReport->KeyCode[0] = 0x08; //e
			KeyboardReport->KeyCode[1] = 0x20; //3
		} else if (~PINE & (1 << PINE4)) {
			KeyboardReport->KeyCode[0] = 0x08; //e
			KeyboardReport->KeyCode[1] = 0x21; //4
		} else if (~PINE & (1 << PINE5)) {
			KeyboardReport->KeyCode[0] = 0x08; //e
			KeyboardReport->KeyCode[1] = 0x22; //5
		} else if (~PINE & (1 << PINE6)) {
			KeyboardReport->KeyCode[0] = 0x08; //e
			KeyboardReport->KeyCode[1] = 0x23; //6
		} else if (~PINE & (1 << PINE7)) {
			KeyboardReport->KeyCode[0] = 0x08; //e
			KeyboardReport->KeyCode[1] = 0x24; //7

		} else if (~PINF & (1 << PINF0)) {
			KeyboardReport->KeyCode[0] = 0x09; //f
			KeyboardReport->KeyCode[1] = 0x27; //0
		} else if (~PINF & (1 << PINF1)) {
			KeyboardReport->KeyCode[0] = 0x09; //f
			KeyboardReport->KeyCode[1] = 0x1E; //1
		} else if (~PINF & (1 << PINF2)) {
			KeyboardReport->KeyCode[0] = 0x09; //f
			KeyboardReport->KeyCode[1] = 0x1F; //2
		} else if (~PINF & (1 << PINF3)) {
			KeyboardReport->KeyCode[0] = 0x09; //f
			KeyboardReport->KeyCode[1] = 0x20; //3
		} else if (~PINF & (1 << PINF4)) {
			KeyboardReport->KeyCode[0] = 0x09; //f
			KeyboardReport->KeyCode[1] = 0x21; //4
		} else if (~PINF & (1 << PINF5)) {
			KeyboardReport->KeyCode[0] = 0x09; //f
			KeyboardReport->KeyCode[1] = 0x22; //5
		} else if (~PINF & (1 << PINF6)) {
			KeyboardReport->KeyCode[0] = 0x09; //f
			KeyboardReport->KeyCode[1] = 0x23; //6
		} else if (~PINF & (1 << PINF7)) {
			KeyboardReport->KeyCode[0] = 0x09; //f
			KeyboardReport->KeyCode[1] = 0x24; //7
		}
	#endif

	*ReportSize = sizeof(USB_KeyboardReport_Data_t);

	if (KeyboardReport->KeyCode[0] == 0x00) {
		return false;
	} else {
		return true;
	}
	
}

/** HID class driver callback function for the processing of HID reports from the host.
 *
 *  \param[in] HIDInterfaceInfo  Pointer to the HID class interface configuration structure being referenced
 *  \param[in] ReportID  Report ID of the received report from the host
 *  \param[in] ReportData  Pointer to a buffer where the created report has been stored
 *  \param[in] ReportSize  Size in bytes of the received HID report
 */
void CALLBACK_HID_Device_ProcessHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo, const uint8_t ReportID,
                                          const void* ReportData, const uint16_t ReportSize)
{
	uint8_t  LEDMask   = LEDS_NO_LEDS;
	uint8_t* LEDReport = (uint8_t*)ReportData;

	if (*LEDReport & HID_KEYBOARD_LED_NUMLOCK)
	  LEDMask |= LEDS_LED1;
	
	if (*LEDReport & HID_KEYBOARD_LED_CAPSLOCK)
	  LEDMask |= LEDS_LED3;

	if (*LEDReport & HID_KEYBOARD_LED_SCROLLLOCK)
	  LEDMask |= LEDS_LED4;
	  
	LEDs_SetAllLEDs(LEDMask);
}
