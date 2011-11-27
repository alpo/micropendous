/*
             LUFA Library
     Copyright (C) Dean Camera, 2011.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2011  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Updated for compatibility with the Android ADK DemoKit App by Opendous Inc. 2011-11-26
  www.Micropendous.org/ADK
  http://developer.android.com/guide/topics/usb/adk.html#running-demokit

  The firmware is designed to send dummy variables to the DemoKit App.  See AOAHost_Task.

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
 *  Main source file for the AndroidAccessoryHost demo. This file contains the main tasks of
 *  the demo and is responsible for the initial application hardware configuration.
 */

#include "AndroidAccessoryHost.h"

/** LUFA Android Open Accessory Class driver interface configuration and state information. This
 *  structure is passed to all Android Open Accessory Class driver functions, so that multiple
 *  instances of the same class within a device can be differentiated from one another.
 */
USB_ClassInfo_AOA_Host_t AndroidDevice_AOA_Interface =
	{
		.Config =
			{
				.DataINPipeNumber           = 1,
				.DataINPipeDoubleBank       = false,

				.DataOUTPipeNumber          = 2,
				.DataOUTPipeDoubleBank      = false,
				
				.PropertyStrings =
					{
/*						.Manufacturer       = "Dean Camera",
						.Model              = "LUFA Android Demo",
						.Description        = "LUFA Android Demo",
						.Version            = "1.0",
						.URI                = "http://www.lufa-lib.org",
						.Serial             = "0000000012345678",
*/
						.Manufacturer       = "Google, Inc.",
						.Model              = "DemoKit",
						.Description        = "DemoKit Arduino Board",
						.Version            = "1.0",
						.URI                = "http://www.android.com",
						.Serial             = "0000000012345678",
					},
			},
	};


/** Main program entry point. This routine configures the hardware required by the application, then
 *  enters a loop to run the application tasks in sequence.
 */
int main(void)
{
	SetupHardware();
	
	puts_P(PSTR(ESC_FG_CYAN "Android Accessory Host Demo running.\r\n" ESC_FG_WHITE));

	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
	sei();

	for (;;)
	{
		AOAHost_Task();

		AOA_Host_USBTask(&AndroidDevice_AOA_Interface);
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
	Serial_Init(9600, false);
	Board_Init();
	USB_Init();
	SELECT_USB_A;

	/* Create a stdio stream for the serial port for stdin and stdout */
	Serial_CreateStream(NULL);
}



/** Event handler for the USB_DeviceAttached event. This indicates that a device has been attached to the host, and
 *  starts the library USB task to begin the enumeration and USB management process.
 */
void EVENT_USB_Host_DeviceAttached(void)
{
	puts_P(PSTR("Device Attached.\r\n"));
	LEDs_SetAllLEDs(LEDMASK_USB_ENUMERATING);
}

/** Event handler for the USB_DeviceUnattached event. This indicates that a device has been removed from the host, and
 *  stops the library USB task management process.
 */
void EVENT_USB_Host_DeviceUnattached(void)
{
	puts_P(PSTR("\r\nDevice Unattached.\r\n"));
	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
}

/** Event handler for the USB_DeviceEnumerationComplete event. This indicates that a device has been successfully
 *  enumerated by the host and is now ready to be used by the application.
 */
void EVENT_USB_Host_DeviceEnumerationComplete(void)
{
	LEDs_SetAllLEDs(LEDMASK_USB_ENUMERATING);

	USB_Descriptor_Device_t DeviceDescriptor;

	if (USB_Host_GetDeviceDescriptor(&DeviceDescriptor) != HOST_SENDCONTROL_Successful)
	{
		puts_P(PSTR("Error Retrieving Device Descriptor.\r\n"));
		LEDs_SetAllLEDs(LEDMASK_USB_ERROR);
		return;	
	}

	bool NeedModeSwitch;
	if (!(AOA_Host_ValidateAccessoryDevice(&AndroidDevice_AOA_Interface, &DeviceDescriptor, &NeedModeSwitch)))
	{
		puts_P(PSTR("Not an Android device.\r\n"));
		LEDs_SetAllLEDs(LEDMASK_USB_ERROR);
		return;
	}
	
	if (NeedModeSwitch)
	{
		puts_P(PSTR("Not in Accessory mode, switching...\r\n"));
		AOA_Host_StartAccessoryMode(&AndroidDevice_AOA_Interface);
		return;
	}

	uint16_t ConfigDescriptorSize;
	uint8_t  ConfigDescriptorData[512];

	if (USB_Host_GetDeviceConfigDescriptor(1, &ConfigDescriptorSize, ConfigDescriptorData,
	                                       sizeof(ConfigDescriptorData)) != HOST_GETCONFIG_Successful)
	{
		puts_P(PSTR("Error Retrieving Configuration Descriptor.\r\n"));
		LEDs_SetAllLEDs(LEDMASK_USB_ERROR);
		return;
	}

	if (AOA_Host_ConfigurePipes(&AndroidDevice_AOA_Interface,
	                            ConfigDescriptorSize, ConfigDescriptorData) != AOA_ENUMERROR_NoError)
	{
		puts_P(PSTR("Attached Device Not a Valid Android Accessory Class Device.\r\n"));
		LEDs_SetAllLEDs(LEDMASK_USB_ERROR);
		return;
	}

	if (USB_Host_SetDeviceConfiguration(1) != HOST_SENDCONTROL_Successful)
	{
		puts_P(PSTR("Error Setting Device Configuration.\r\n"));
		LEDs_SetAllLEDs(LEDMASK_USB_ERROR);
		return;
	}

	puts_P(PSTR("Android Device Enumerated.\r\n"));
	LEDs_SetAllLEDs(LEDMASK_USB_READY);
}

/** Event handler for the USB_HostError event. This indicates that a hardware error occurred while in host mode. */
void EVENT_USB_Host_HostError(const uint8_t ErrorCode)
{
	USB_Disable();

	printf_P(PSTR(ESC_FG_RED "Host Mode Error\r\n"
	                         " -- Error Code %d\r\n" ESC_FG_WHITE), ErrorCode);

	LEDs_SetAllLEDs(LEDMASK_USB_ERROR);
	for(;;);
}

/** Event handler for the USB_DeviceEnumerationFailed event. This indicates that a problem occurred while
 *  enumerating an attached USB device.
 */
void EVENT_USB_Host_DeviceEnumerationFailed(const uint8_t ErrorCode,
                                            const uint8_t SubErrorCode)
{
	printf_P(PSTR(ESC_FG_RED "Dev Enum Error\r\n"
	                         " -- Error Code %d\r\n"
	                         " -- Sub Error Code %d\r\n"
	                         " -- In State %d\r\n" ESC_FG_WHITE), ErrorCode, SubErrorCode, USB_HostState);

	LEDs_SetAllLEDs(LEDMASK_USB_ERROR);
}



// global variables to store previous hardware states so that only updates are sent
volatile uint16_t prevTempVal = 0;
volatile uint16_t prevlightVal = 0;
volatile uint8_t prevJoystickXVal = 0;
volatile uint8_t prevJoystickYVal = 0;
volatile uint8_t prevCapSenseVal = 0;
volatile uint8_t prevButton1Val = 0;
volatile uint8_t prevButton2Val = 0;
volatile uint8_t prevButton3Val = 0;
volatile uint8_t prevJoystickButtonVal = 0;

/** Task to manage an enumerated USB Android Accessory device once connected
 *  Implements the same communication protocol as the Android DemoKit App
 *  http://developer.android.com/guide/topics/usb/adk.html#running-demokit
 */
void AOAHost_Task(void)
{
	uint16_t RecvCount = 0;
	int16_t ReceivedByte;
	uint16_t tempVal = 0;
	uint16_t lightVal = 0;
	uint8_t joystickXVal = 0;
	uint8_t joystickYVal = 0;
	uint8_t capSenseVal = 0;
	uint8_t buttonVal = 0;
	uint8_t msg[3];
	uint8_t count = 0;
	uint8_t countMOD = 0;
	uint8_t i;


	if (USB_HostState != HOST_STATE_Configured)
	  return;


	if ((RecvCount = AOA_Host_BytesReceived(&AndroidDevice_AOA_Interface)) > 0)
	{
		for (i = 0; i < RecvCount; i++)
		{
			ReceivedByte = AOA_Host_ReceiveByte(&AndroidDevice_AOA_Interface);
			if (!(ReceivedByte < 0))
			{
				msg[i] = (uint8_t)ReceivedByte;
			} else {
				msg[i] = 0;
			}
		}

		// Print the received data over the UART for debugging
		printf_P(PSTR(ESC_FG_YELLOW "Received: msg={0x%02X, 0x%02X, 0x%02X}\r\n" ESC_FG_WHITE), msg[0], msg[1], msg[2]);

		// process msg OUT data packet from DemoKit (OUT is relative to Android Device - these are from an USB IN Endpoint)
		if (msg[0] == 0x02)
		{
			if (msg[1] == 0x00)
			{
				// set LED1 Red color to (255 - msg[2]) value
			} else if (msg[1] == 0x01) {
				// set LED1 Green color to (255 - msg[2]) value
			} else if (msg[1] == 0x02) {
				// set LED1 Blue color to (255 - msg[2]) value
			} else if (msg[1] == 0x03) {
				// set LED2 Red color to (255 - msg[2]) value
			} else if (msg[1] == 0x04) {
				// set LED2 Green color to (255 - msg[2]) value
			} else if (msg[1] == 0x05) {
				// set LED2 Blue color to (255 - msg[2]) value
			} else if (msg[1] == 0x06) {
				// set LED3 Red color to (255 - msg[2]) value
			} else if (msg[1] == 0x07) {
				// set LED3 Green color to (255 - msg[2]) value
			} else if (msg[1] == 0x08) {
				// set LED3 Blue color to (255 - msg[2]) value
			} else if (msg[1] == 0x10) {
				// set first servo to msg[2] value
			} else if (msg[1] == 0x11) {
				// set second servo to msg[2] value
			} else if (msg[1] == 0x12) {
				// set third servo to msg[2] value
			}
		} else if (msg[0] == 0x03)
		{
			if (msg[1] == 0x00)
			{
				// set Relay1 to (msg[2] ? HIGH : LOW) value
			} else if (msg[1] == 0x01) {
				// set Relay2 to (msg[2] ? HIGH : LOW) value
			}
		}
	}

	msg[0] = 0;

	// create msg IN data packet to send to DemoKit (IN is relative to Android Device - these are for an USB OUT Endpoint)

	// if Button1 status has changed then send the keypress value to be displayed in DemoKit
	buttonVal = Buttons_GetStatus();
	if (buttonVal != prevButton1Val)
	{
		msg[0] = 1;
		msg[1] = 0;
		msg[2] = ((buttonVal == BUTTONS_BUTTON1) ? 1 : 0); // this will return the status of the HWB Button
		AOA_Host_SendData(&AndroidDevice_AOA_Interface, msg, 3);
		prevButton1Val = buttonVal;
	}

	// if Button2 status has changed then send the keypress value to be displayed in DemoKit
	buttonVal = 0;
	if (buttonVal != prevButton2Val)
	{
		msg[0] = 1;
		msg[1] = 1;
		msg[2] = (buttonVal ? 0 : 1);
		AOA_Host_SendData(&AndroidDevice_AOA_Interface, msg, 3);
		prevButton2Val = buttonVal;
	}

	// if Button3 status has changed then send the keypress value to be displayed in DemoKit
	buttonVal = 0;
	if (buttonVal != prevButton3Val)
	{
		msg[0] = 1;
		msg[1] = 2;
		msg[2] = (buttonVal ? 0 : 1);
		AOA_Host_SendData(&AndroidDevice_AOA_Interface, msg, 3);
		prevButton3Val = buttonVal;
	}

	// if Joystick Button status has changed then send the keypress value to be displayed in DemoKit
	buttonVal = 0;
	if (buttonVal != prevJoystickButtonVal)
	{
		msg[0] = 1;
		msg[1] = 4;
		msg[2] = (buttonVal ? 0 : 1);
		AOA_Host_SendData(&AndroidDevice_AOA_Interface, msg, 3);
		prevJoystickButtonVal = buttonVal;
	}


	tempVal = 456; // this is the Temperature sensor value to be displayed in DemoKit
	if (tempVal != prevTempVal)
	{
		msg[0] = 0x04;
		msg[1] = tempVal >> 8;
		msg[2] = (tempVal & 0xFF);
		AOA_Host_SendData(&AndroidDevice_AOA_Interface, msg, 3);
		prevTempVal = tempVal;
	}

	lightVal = 1000; // this is the Light sensor value to be displayed in DemoKit
	if (lightVal != prevlightVal)
	{
		msg[0] = 0x05;
		msg[1] = lightVal >> 8;
		msg[2] = (lightVal & 0xFF);
		AOA_Host_SendData(&AndroidDevice_AOA_Interface, msg, 3);
		prevlightVal = lightVal;
	}

	// this is the Joystick position to be displayed in DemoKit
	joystickXVal = 0x33; // X-Axis with physical [-128..127] position mapped onto [0..255]
	joystickYVal = 0x55; // Y-Axis with physical [-128..127] position mapped onto [0..255]
	if ((joystickXVal != prevJoystickXVal) || (joystickYVal != prevJoystickYVal))
	{
		msg[0] = 0x06;
		msg[1] = joystickXVal; 
		msg[2] = joystickYVal; 
		AOA_Host_SendData(&AndroidDevice_AOA_Interface, msg, 3);
		prevJoystickXVal = joystickXVal;
		prevJoystickYVal = joystickYVal;
	}

	capSenseVal = 0x22; // this is the Capacitive sensor value to be displayed in DemoKit
	if (capSenseVal != prevCapSenseVal)
	{
		msg[0] = 0x01;
		msg[1] = 0x03;
		msg[2] = capSenseVal;
		AOA_Host_SendData(&AndroidDevice_AOA_Interface, msg, 3);
		prevCapSenseVal = capSenseVal;
	}


	// make sure data packet is sent but only if it has been updated
	if (msg[0] != 0)
	{
		AOA_Host_Flush(&AndroidDevice_AOA_Interface);
	}

}

