/*
             LUFA Library
     Copyright (C) Dean Camera, 2009.
              
  dean [at] fourwalledcubicle [dot] com
      www.fourwalledcubicle.com
*/

/*
  Copyright 2009  Denver Gingerich (denver [at] ossguy [dot] com)
      Based on code by Dean Camera (dean [at] fourwalledcubicle [dot] com)

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
 *  Main source file for the Keyboard demo. This file contains the main tasks of the demo and
 *  is responsible for the initial application hardware configuration.
 */
 
#include "MicropendousKeyboardTest.h"

/* Global Variables */
/** Indicates what report mode the host has requested, true for normal HID reporting mode, false for special boot
 *  protocol reporting mode.
 */
bool UsingReportProtocol = true;

/** Current Idle period. This is set by the host via a Set Idle HID class request to silence the device's reports
 *  for either the entire idle duration, or until the report status changes (e.g. the user presses a key).
 */
uint16_t IdleCount = 500;

/** Current Idle period remaining. When the IdleCount value is set, this tracks the remaining number of idle
 *  milliseconds. This is separate to the IdleCount timer and is incremented and compared as the host may request 
 *  the current idle period via a Get Idle HID class request, thus its value must be preserved.
 */
uint16_t IdleMSRemaining = 0;


/** Main program entry point. This routine configures the hardware required by the application, then
 *  starts the scheduler to run the USB management task.
 */
int main(void)
{
	SetupHardware();
	
	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);

	for (;;)
	{
		HID_Task();
		USB_USBTask();
	}
}

/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware(void)
{
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

    // disable JTAG to allow corresponding pins to be used
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

	/* Disable clock division */
	clock_prescale_set(clock_div_1);

	/* Hardware Initialization */
	Joystick_Init();
	LEDs_Init();

	// enable Ports based on which IC is being used
	#if (defined(__AVR_AT90USB162__)  || defined(__AVR_AT90USB82__))
		DDRD = 0;
		PORTD = 0xFF; //input
		DDRB = 0;
		PORTB = 0xFF; //input
		DDRC = 0;
		PORTC |= (1 << PC2) | (1 << PC4) | (1 << PC5) | (1 << PC6) | (1 << PC7); //input only on PC2,4,5,6,7
		// be careful using PortC as PC0 is used for the Crystal
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

	USB_Init();
}

/** Event handler for the USB_Connect event. This indicates that the device is enumerating via the status LEDs and
 *  starts the library USB task to begin the enumeration and USB management process.
 */
void EVENT_USB_Device_Connect(void)
{
	/* Indicate USB enumerating */
	LEDs_SetAllLEDs(LEDMASK_USB_ENUMERATING);

	/* Default to report protocol on connect */
	UsingReportProtocol = true;
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
 *  of the USB device after enumeration, and configures the keyboard device endpoints.
 */
void EVENT_USB_Device_ConfigurationChanged(void)
{	
	/* Indicate USB connected and ready */
	LEDs_SetAllLEDs(LEDMASK_USB_READY);

	/* Setup Keyboard Keycode Report Endpoint */
	if (!(Endpoint_ConfigureEndpoint(KEYBOARD_EPNUM, EP_TYPE_INTERRUPT,
		                             ENDPOINT_DIR_IN, KEYBOARD_EPSIZE,
	                                 ENDPOINT_BANK_SINGLE)))
	{
		LEDs_SetAllLEDs(LEDMASK_USB_ERROR);
	}
	
	/* Setup Keyboard LED Report Endpoint */
	if (!(Endpoint_ConfigureEndpoint(KEYBOARD_LEDS_EPNUM, EP_TYPE_INTERRUPT,
		                             ENDPOINT_DIR_OUT, KEYBOARD_EPSIZE,
	                                 ENDPOINT_BANK_SINGLE)))
	{
		LEDs_SetAllLEDs(LEDMASK_USB_ERROR);
	}
	
	USB_Device_EnableSOFEvents();
}

/** Event handler for the USB_UnhandledControlRequest event. This is used to catch standard and class specific
 *  control requests that are not handled internally by the USB library (including the HID commands, which are
 *  all issued via the control endpoint), so that they can be handled appropriately for the application.
 */
void EVENT_USB_Device_UnhandledControlRequest(void)
{
	/* Handle HID Class specific requests */
	switch (USB_ControlRequest.bRequest)
	{
		case REQ_GetReport:
			if (USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
			{
				USB_KeyboardReport_Data_t KeyboardReportData;

				Endpoint_ClearSETUP();
	
				/* Create the next keyboard report for transmission to the host */
				CreateKeyboardReport(&KeyboardReportData);

				/* Write the report data to the control endpoint */
				Endpoint_Write_Control_Stream_LE(&KeyboardReportData, sizeof(KeyboardReportData));
				
				/* Finalize the stream transfer to send the last packet or clear the host abort */
				Endpoint_ClearOUT();
			}
		
			break;
		case REQ_SetReport:
			if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
			{
				Endpoint_ClearSETUP();
				
				/* Wait until the LED report has been sent by the host */
				while (!(Endpoint_IsOUTReceived()))
				{
					if (USB_DeviceState == DEVICE_STATE_Unattached)
					  return;
				}

				/* Read in the LED report from the host */
				uint8_t LEDStatus = Endpoint_Read_Byte();

				/* Process the incoming LED report */
				ProcessLEDReport(LEDStatus);
			
				/* Clear the endpoint data */
				Endpoint_ClearOUT();

				Endpoint_ClearStatusStage();
			}
			
			break;
		case REQ_GetProtocol:
			if (USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
			{
				Endpoint_ClearSETUP();
				
				/* Write the current protocol flag to the host */
				Endpoint_Write_Byte(UsingReportProtocol);
				
				/* Send the flag to the host */
				Endpoint_ClearIN();

				Endpoint_ClearStatusStage();
			}
			
			break;
		case REQ_SetProtocol:
			if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
			{
				Endpoint_ClearSETUP();

				/* Set or clear the flag depending on what the host indicates that the current Protocol should be */
				UsingReportProtocol = (USB_ControlRequest.wValue != 0);

				Endpoint_ClearStatusStage();
			}
			
			break;
		case REQ_SetIdle:
			if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE))
			{
				Endpoint_ClearSETUP();
				
				/* Get idle period in MSB, IdleCount must be multiplied by 4 to get number of milliseconds */
				IdleCount = ((USB_ControlRequest.wValue & 0xFF00) >> 6);
				
				Endpoint_ClearStatusStage();
			}
			
			break;
		case REQ_GetIdle:
			if (USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE))
			{		
				Endpoint_ClearSETUP();
				
				/* Write the current idle duration to the host, must be divided by 4 before sent to host */
				Endpoint_Write_Byte(IdleCount >> 2);
				
				/* Send the flag to the host */
				Endpoint_ClearIN();

				Endpoint_ClearStatusStage();
			}

			break;
	}
}

/** Event handler for the USB device Start Of Frame event. */
void EVENT_USB_Device_StartOfFrame(void)
{
	/* One millisecond has elapsed, decrement the idle time remaining counter if it has not already elapsed */
	if (IdleMSRemaining)
	  IdleMSRemaining--;
}

/** Fills the given HID report data structure with the next HID report to send to the host.
 *
 *  \param[out] ReportData  Pointer to a HID report data structure to be filled
 */
void CreateKeyboardReport(USB_KeyboardReport_Data_t* ReportData)
{
	uint8_t JoyStatus_LCL = Joystick_GetStatus();

	// Clear the report contents
	memset(ReportData, 0, sizeof(USB_KeyboardReport_Data_t));

	ReportData->KeyCode[0] = 0x00;

	// 32-pin USB AVRs
	#if (defined(__AVR_AT90USB162__)  || defined(__AVR_AT90USB82__))
				  if (~PINB & (1 << PINB0)) {
			ReportData->KeyCode[0] = 0x05; //b
			ReportData->KeyCode[1] = 0x27; //0
		} else if (~PINB & (1 << PINB1)) {
			ReportData->KeyCode[0] = 0x05; //b
			ReportData->KeyCode[1] = 0x1E; //1
		} else if (~PINB & (1 << PINB2)) {
			ReportData->KeyCode[0] = 0x05; //b
			ReportData->KeyCode[1] = 0x1F; //2
		} else if (~PINB & (1 << PINB3)) {
			ReportData->KeyCode[0] = 0x05; //b
			ReportData->KeyCode[1] = 0x20; //3
		} else if (~PINB & (1 << PINB4)) {
			ReportData->KeyCode[0] = 0x05; //b
			ReportData->KeyCode[1] = 0x21; //4
		} else if (~PINB & (1 << PINB5)) {
			ReportData->KeyCode[0] = 0x05; //b
			ReportData->KeyCode[1] = 0x22; //5
		} else if (~PINB & (1 << PINB6)) {
			ReportData->KeyCode[0] = 0x05; //b
			ReportData->KeyCode[1] = 0x23; //6
		} else if (~PINB & (1 << PINB7)) {
			ReportData->KeyCode[0] = 0x05; //b
			ReportData->KeyCode[1] = 0x24; //7

		} else if (~PINC & (1 << PINC2)) {
			ReportData->KeyCode[0] = 0x06; //c
			ReportData->KeyCode[1] = 0x1F; //2
		} else if (~PINC & (1 << PINC4)) {
			ReportData->KeyCode[0] = 0x06; //c
			ReportData->KeyCode[1] = 0x21; //4
		} else if (~PINC & (1 << PINC5)) {
			ReportData->KeyCode[0] = 0x06; //c
			ReportData->KeyCode[1] = 0x22; //5
		} else if (~PINC & (1 << PINC6)) {
			ReportData->KeyCode[0] = 0x06; //c
			ReportData->KeyCode[1] = 0x23; //6
		} else if (~PINC & (1 << PINC7)) {
			ReportData->KeyCode[0] = 0x06; //c
			ReportData->KeyCode[1] = 0x24; //7

		} else if (~PIND & (1 << PIND0)) {
			ReportData->KeyCode[0] = 0x07; //d
			ReportData->KeyCode[1] = 0x27; //0
		} else if (~PIND & (1 << PIND1)) {
			ReportData->KeyCode[0] = 0x07; //d
			ReportData->KeyCode[1] = 0x1E; //1
		} else if (~PIND & (1 << PIND2)) {
			ReportData->KeyCode[0] = 0x07; //d
			ReportData->KeyCode[1] = 0x1F; //2
		} else if (~PIND & (1 << PIND3)) {
			ReportData->KeyCode[0] = 0x07; //d
			ReportData->KeyCode[1] = 0x20; //3
		} else if (~PIND & (1 << PIND4)) {
			ReportData->KeyCode[0] = 0x07; //d
			ReportData->KeyCode[1] = 0x21; //4
		} else if (~PIND & (1 << PIND5)) {
			ReportData->KeyCode[0] = 0x07; //d
			ReportData->KeyCode[1] = 0x22; //5
		} else if (~PIND & (1 << PIND6)) {
			ReportData->KeyCode[0] = 0x07; //d
			ReportData->KeyCode[1] = 0x23; //6
		} else if (~PIND & (1 << PIND7)) {
			ReportData->KeyCode[0] = 0x07; //d
			ReportData->KeyCode[1] = 0x24; //7
		}
	#endif


	// 44-pin USB AVRs
	#if (defined(__AVR_ATmega16U4__)  || defined(__AVR_ATmega32U4__))
				  if (~PINB & (1 << PINB0)) {
			ReportData->KeyCode[0] = 0x05; //b
			ReportData->KeyCode[1] = 0x27; //0
		} else if (~PINB & (1 << PINB1)) {
			ReportData->KeyCode[0] = 0x05; //b
			ReportData->KeyCode[1] = 0x1E; //1
		} else if (~PINB & (1 << PINB2)) {
			ReportData->KeyCode[0] = 0x05; //b
			ReportData->KeyCode[1] = 0x1F; //2
		} else if (~PINB & (1 << PINB3)) {
			ReportData->KeyCode[0] = 0x05; //b
			ReportData->KeyCode[1] = 0x20; //3
		} else if (~PINB & (1 << PINB4)) {
			ReportData->KeyCode[0] = 0x05; //b
			ReportData->KeyCode[1] = 0x21; //4
		} else if (~PINB & (1 << PINB5)) {
			ReportData->KeyCode[0] = 0x05; //b
			ReportData->KeyCode[1] = 0x22; //5
		} else if (~PINB & (1 << PINB6)) {
			ReportData->KeyCode[0] = 0x05; //b
			ReportData->KeyCode[1] = 0x23; //6
		} else if (~PINB & (1 << PINB7)) {
			ReportData->KeyCode[0] = 0x05; //b
			ReportData->KeyCode[1] = 0x24; //7

		} else if (~PINC & (1 << PINC6)) {
			ReportData->KeyCode[0] = 0x06; //c
			ReportData->KeyCode[1] = 0x23; //6
		} else if (~PINC & (1 << PINC7)) {
			ReportData->KeyCode[0] = 0x06; //c
			ReportData->KeyCode[1] = 0x24; //7

		} else if (~PIND & (1 << PIND0)) {
			ReportData->KeyCode[0] = 0x07; //d
			ReportData->KeyCode[1] = 0x27; //0
		} else if (~PIND & (1 << PIND1)) {
			ReportData->KeyCode[0] = 0x07; //d
			ReportData->KeyCode[1] = 0x1E; //1
		} else if (~PIND & (1 << PIND2)) {
			ReportData->KeyCode[0] = 0x07; //d
			ReportData->KeyCode[1] = 0x1F; //2
		} else if (~PIND & (1 << PIND3)) {
			ReportData->KeyCode[0] = 0x07; //d
			ReportData->KeyCode[1] = 0x20; //3
		} else if (~PIND & (1 << PIND4)) {
			ReportData->KeyCode[0] = 0x07; //d
			ReportData->KeyCode[1] = 0x21; //4
		} else if (~PIND & (1 << PIND5)) {
			ReportData->KeyCode[0] = 0x07; //d
			ReportData->KeyCode[1] = 0x22; //5
		} else if (~PIND & (1 << PIND6)) {
			ReportData->KeyCode[0] = 0x07; //d
			ReportData->KeyCode[1] = 0x23; //6
		} else if (~PIND & (1 << PIND7)) {
			ReportData->KeyCode[0] = 0x07; //d
			ReportData->KeyCode[1] = 0x24; //7

		} else if (~PINE & (1 << PINE2)) {
			ReportData->KeyCode[0] = 0x08; //e
			ReportData->KeyCode[1] = 0x1F; //2
		} else if (~PINE & (1 << PINE6)) {
			ReportData->KeyCode[0] = 0x08; //e
			ReportData->KeyCode[1] = 0x23; //6

		} else if (~PINF & (1 << PINF0)) {
			ReportData->KeyCode[0] = 0x09; //f
			ReportData->KeyCode[1] = 0x27; //0
		} else if (~PINF & (1 << PINF1)) {
			ReportData->KeyCode[0] = 0x09; //f
			ReportData->KeyCode[1] = 0x1E; //1
		} else if (~PINF & (1 << PINF4)) {
			ReportData->KeyCode[0] = 0x09; //f
			ReportData->KeyCode[1] = 0x21; //4
		} else if (~PINF & (1 << PINF5)) {
			ReportData->KeyCode[0] = 0x09; //f
			ReportData->KeyCode[1] = 0x22; //5
		} else if (~PINF & (1 << PINF6)) {
			ReportData->KeyCode[0] = 0x09; //f
			ReportData->KeyCode[1] = 0x23; //6
		} else if (~PINF & (1 << PINF7)) {
			ReportData->KeyCode[0] = 0x09; //f
			ReportData->KeyCode[1] = 0x24; //7
		}
	#endif


	// 64-pin USB AVRs
	#if (defined(__AVR_AT90USB1287__) || defined(__AVR_AT90USB647__) ||  \
			defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB646__) ||  \
			defined(__AVR_ATmega32U6__))

				  if (~PINA & (1 << PINA0)) {
			ReportData->KeyCode[0] = 0x04; //a
			ReportData->KeyCode[1] = 0x27; //0
		} else if (~PINA & (1 << PINA1)) {
			ReportData->KeyCode[0] = 0x04; //a
			ReportData->KeyCode[1] = 0x1E; //1
		} else if (~PINA & (1 << PINA2)) {
			ReportData->KeyCode[0] = 0x04; //a
			ReportData->KeyCode[1] = 0x1F; //2
		} else if (~PINA & (1 << PINA3)) {
			ReportData->KeyCode[0] = 0x04; //a
			ReportData->KeyCode[1] = 0x20; //3
		} else if (~PINA & (1 << PINA4)) {
			ReportData->KeyCode[0] = 0x04; //a
			ReportData->KeyCode[1] = 0x21; //4
		} else if (~PINA & (1 << PINA5)) {
			ReportData->KeyCode[0] = 0x04; //a
			ReportData->KeyCode[1] = 0x22; //5
		} else if (~PINA & (1 << PINA6)) {
			ReportData->KeyCode[0] = 0x04; //a
			ReportData->KeyCode[1] = 0x23; //6
		} else if (~PINA & (1 << PINA7)) {
			ReportData->KeyCode[0] = 0x04; //a
			ReportData->KeyCode[1] = 0x24; //7

		} else if (~PINB & (1 << PINB0)) {
			ReportData->KeyCode[0] = 0x05; //b
			ReportData->KeyCode[1] = 0x27; //0
		} else if (~PINB & (1 << PINB1)) {
			ReportData->KeyCode[0] = 0x05; //b
			ReportData->KeyCode[1] = 0x1E; //1
		} else if (~PINB & (1 << PINB2)) {
			ReportData->KeyCode[0] = 0x05; //b
			ReportData->KeyCode[1] = 0x1F; //2
		} else if (~PINB & (1 << PINB3)) {
			ReportData->KeyCode[0] = 0x05; //b
			ReportData->KeyCode[1] = 0x20; //3
		} else if (~PINB & (1 << PINB4)) {
			ReportData->KeyCode[0] = 0x05; //b
			ReportData->KeyCode[1] = 0x21; //4
		} else if (~PINB & (1 << PINB5)) {
			ReportData->KeyCode[0] = 0x05; //b
			ReportData->KeyCode[1] = 0x22; //5
		} else if (~PINB & (1 << PINB6)) {
			ReportData->KeyCode[0] = 0x05; //b
			ReportData->KeyCode[1] = 0x23; //6
		} else if (~PINB & (1 << PINB7)) {
			ReportData->KeyCode[0] = 0x05; //b
			ReportData->KeyCode[1] = 0x24; //7

		} else if (~PINC & (1 << PINC0)) {
			ReportData->KeyCode[0] = 0x06; //c
			ReportData->KeyCode[1] = 0x27; //0
		} else if (~PINC & (1 << PINC1)) {
			ReportData->KeyCode[0] = 0x06; //c
			ReportData->KeyCode[1] = 0x1E; //1
		} else if (~PINC & (1 << PINC2)) {
			ReportData->KeyCode[0] = 0x06; //c
			ReportData->KeyCode[1] = 0x1F; //2
		} else if (~PINC & (1 << PINC3)) {
			ReportData->KeyCode[0] = 0x06; //c
			ReportData->KeyCode[1] = 0x20; //3
		} else if (~PINC & (1 << PINC4)) {
			ReportData->KeyCode[0] = 0x06; //c
			ReportData->KeyCode[1] = 0x21; //4
		} else if (~PINC & (1 << PINC5)) {
			ReportData->KeyCode[0] = 0x06; //c
			ReportData->KeyCode[1] = 0x22; //5
		} else if (~PINC & (1 << PINC6)) {
			ReportData->KeyCode[0] = 0x06; //c
			ReportData->KeyCode[1] = 0x23; //6
		} else if (~PINC & (1 << PINC7)) {
			ReportData->KeyCode[0] = 0x06; //c
			ReportData->KeyCode[1] = 0x24; //7

		} else if (~PIND & (1 << PIND0)) {
			ReportData->KeyCode[0] = 0x07; //d
			ReportData->KeyCode[1] = 0x27; //0
		} else if (~PIND & (1 << PIND1)) {
			ReportData->KeyCode[0] = 0x07; //d
			ReportData->KeyCode[1] = 0x1E; //1
		} else if (~PIND & (1 << PIND2)) {
			ReportData->KeyCode[0] = 0x07; //d
			ReportData->KeyCode[1] = 0x1F; //2
		} else if (~PIND & (1 << PIND3)) {
			ReportData->KeyCode[0] = 0x07; //d
			ReportData->KeyCode[1] = 0x20; //3
		} else if (~PIND & (1 << PIND4)) {
			ReportData->KeyCode[0] = 0x07; //d
			ReportData->KeyCode[1] = 0x21; //4
		} else if (~PIND & (1 << PIND5)) {
			ReportData->KeyCode[0] = 0x07; //d
			ReportData->KeyCode[1] = 0x22; //5
		} else if (~PIND & (1 << PIND6)) {
			ReportData->KeyCode[0] = 0x07; //d
			ReportData->KeyCode[1] = 0x23; //6
		} else if (~PIND & (1 << PIND7)) {
			ReportData->KeyCode[0] = 0x07; //d
			ReportData->KeyCode[1] = 0x24; //7

		} else if (~PINE & (1 << PINE0)) {
			ReportData->KeyCode[0] = 0x08; //e
			ReportData->KeyCode[1] = 0x27; //0
		} else if (~PINE & (1 << PINE1)) {
			ReportData->KeyCode[0] = 0x08; //e
			ReportData->KeyCode[1] = 0x1E; //1
		} else if (~PINE & (1 << PINE2)) {
			ReportData->KeyCode[0] = 0x08; //e
			ReportData->KeyCode[1] = 0x1F; //2
		} else if (~PINE & (1 << PINE3)) {
			ReportData->KeyCode[0] = 0x08; //e
			ReportData->KeyCode[1] = 0x20; //3
		} else if (~PINE & (1 << PINE4)) {
			ReportData->KeyCode[0] = 0x08; //e
			ReportData->KeyCode[1] = 0x21; //4
		} else if (~PINE & (1 << PINE5)) {
			ReportData->KeyCode[0] = 0x08; //e
			ReportData->KeyCode[1] = 0x22; //5
		} else if (~PINE & (1 << PINE6)) {
			ReportData->KeyCode[0] = 0x08; //e
			ReportData->KeyCode[1] = 0x23; //6
		} else if (~PINE & (1 << PINE7)) {
			ReportData->KeyCode[0] = 0x08; //e
			ReportData->KeyCode[1] = 0x24; //7

		} else if (~PINF & (1 << PINF0)) {
			ReportData->KeyCode[0] = 0x09; //f
			ReportData->KeyCode[1] = 0x27; //0
		} else if (~PINF & (1 << PINF1)) {
			ReportData->KeyCode[0] = 0x09; //f
			ReportData->KeyCode[1] = 0x1E; //1
		} else if (~PINF & (1 << PINF2)) {
			ReportData->KeyCode[0] = 0x09; //f
			ReportData->KeyCode[1] = 0x1F; //2
		} else if (~PINF & (1 << PINF3)) {
			ReportData->KeyCode[0] = 0x09; //f
			ReportData->KeyCode[1] = 0x20; //3
		} else if (~PINF & (1 << PINF4)) {
			ReportData->KeyCode[0] = 0x09; //f
			ReportData->KeyCode[1] = 0x21; //4
		} else if (~PINF & (1 << PINF5)) {
			ReportData->KeyCode[0] = 0x09; //f
			ReportData->KeyCode[1] = 0x22; //5
		} else if (~PINF & (1 << PINF6)) {
			ReportData->KeyCode[0] = 0x09; //f
			ReportData->KeyCode[1] = 0x23; //6
		} else if (~PINF & (1 << PINF7)) {
			ReportData->KeyCode[0] = 0x09; //f
			ReportData->KeyCode[1] = 0x24; //7
		}
	#endif
}

/** Processes a received LED report, and updates the board LEDs states to match.
 *
 *  \param[in] LEDReport  LED status report from the host
 */
void ProcessLEDReport(uint8_t LEDReport)
{
	uint8_t LEDMask = LEDS_LED2;
	
	if (LEDReport & 0x01) // NUM Lock
	  LEDMask |= LEDS_LED1;
	
	if (LEDReport & 0x02) // CAPS Lock
	  LEDMask |= LEDS_LED3;

	if (LEDReport & 0x04) // SCROLL Lock
	  LEDMask |= LEDS_LED4;

	/* Set the status LEDs to the current Keyboard LED status */
	LEDs_SetAllLEDs(LEDMask);
}

/** Sends the next HID report to the host, via the keyboard data endpoint. */
void SendNextReport(void)
{
	static USB_KeyboardReport_Data_t PrevKeyboardReportData;
	USB_KeyboardReport_Data_t        KeyboardReportData;
	bool                             SendReport = true;
	
	/* Create the next keyboard report for transmission to the host */
	CreateKeyboardReport(&KeyboardReportData);
	
	/* Check to see if the report data has changed - if so a report MUST be sent */
	SendReport = (memcmp(&PrevKeyboardReportData, &KeyboardReportData, sizeof(USB_KeyboardReport_Data_t)) != 0);
	
	/* Save the current report data for later comparison to check for changes */
	PrevKeyboardReportData = KeyboardReportData;
	
	/* Check if the idle period is set and has elapsed */
	if ((IdleCount != HID_IDLE_CHANGESONLY) && (!(IdleMSRemaining)))
	{
		/* Reset the idle time remaining counter */
		IdleMSRemaining = IdleCount;
		
		/* Idle period is set and has elapsed, must send a report to the host */
		SendReport = true;
	}
	
	/* Select the Keyboard Report Endpoint */
	Endpoint_SelectEndpoint(KEYBOARD_EPNUM);

	/* Check if Keyboard Endpoint Ready for Read/Write and if we should send a new report */
	if (Endpoint_IsReadWriteAllowed() && SendReport)
	{
		/* Write Keyboard Report Data */
		Endpoint_Write_Stream_LE(&KeyboardReportData, sizeof(KeyboardReportData));
		
		/* Finalize the stream transfer to send the last packet */
		Endpoint_ClearIN();
	}
}

/** Reads the next LED status report from the host from the LED data endpoint, if one has been sent. */
void ReceiveNextReport(void)
{
	/* Select the Keyboard LED Report Endpoint */
	Endpoint_SelectEndpoint(KEYBOARD_LEDS_EPNUM);

	/* Check if Keyboard LED Endpoint contains a packet */
	if (Endpoint_IsOUTReceived())
	{
		/* Check to see if the packet contains data */
		if (Endpoint_IsReadWriteAllowed())
		{
			/* Read in the LED report from the host */
			uint8_t LEDReport = Endpoint_Read_Byte();

			/* Process the read LED report from the host */
			ProcessLEDReport(LEDReport);
		}

		/* Handshake the OUT Endpoint - clear endpoint and ready for next report */
		Endpoint_ClearOUT();
	}
}

/** Function to manage HID report generation and transmission to the host, when in report mode. */
void HID_Task(void)
{
	/* Device must be connected and configured for the task to run */
	if (USB_DeviceState != DEVICE_STATE_Configured)
	  return;
	  
	/* Send the next keypress report to the host */
	SendNextReport();
		
	/* Process the LED report sent from the host */
	ReceiveNextReport();
}
