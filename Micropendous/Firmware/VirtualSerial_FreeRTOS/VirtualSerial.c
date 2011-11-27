/*
             LUFA Library
     Copyright (C) Dean Camera, 2011.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2011  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Updated for FreeRTOS by Opendous Inc. 2011-11-25
  www.Micropendous.org/VirtualSerial_FreeRTOS

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

#include "VirtualSerial.h"

// Global buffer for use with STDIO functions
volatile char buffer[CDC_TXRX_EPSIZE];

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


/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */
int main(void)
{
	SetupHardware();

	/* Create a regular character stream for the interface so that it can be used with the stdio.h functions */
	CDC_Device_CreateStream(&VirtualSerial_CDC_Interface, &USBSerialStream);

	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);

	// Create Tasks for FreeRTOS
	// The VirtualSerial/USB-CDC task is highest priority to ensure USB functions run in time
	// TODO/NOTE: AVRlibc stdio functions are not thread-safe
	xTaskCreate(MainTask, (signed portCHAR *) "MainTask", configMINIMAL_STACK_SIZE, NULL, MAIN_TASK_PRIORITY, NULL );
	xTaskCreate(VirtualSerialTask, (signed portCHAR *) "ViSeTask", configMINIMAL_STACK_SIZE, NULL, (ViSe_TASK_PRIORITY | portPRIVILEGE_BIT), NULL );

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

	// TODO: disabling JTAG allows PF4, PF5, PF6, and PF7 to be used as GPIO pins
	JTAG_DISABLE();

	/* Disable clock division */
	clock_prescale_set(clock_div_1);

	/* Hardware Initialization */
	Board_Init();
	DISABLE_VOLTAGE_TXRX;
	DISABLE_EXT_SRAM;
	SELECT_USB_B;
	USB_Init();
}


// CoRoutines are not enabled, but FreeRTOS complains during compile
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
	bool ConfigSuccess = true;

	ConfigSuccess &= CDC_Device_ConfigureEndpoints(&VirtualSerial_CDC_Interface);

	LEDs_SetAllLEDs(ConfigSuccess ? LEDMASK_USB_READY : LEDMASK_USB_ERROR);
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
	CDC_Device_ProcessControlRequest(&VirtualSerial_CDC_Interface);
}


static void VirtualSerialTask(void *pvParameters)
{

	for (;;)
	{
		// Must throw away unused bytes from the host, or it will lock up while waiting for the device
		// TODO: this causes loopback to fail
		//CDC_Device_ReceiveByte(&VirtualSerial_CDC_Interface);

		// want CDC and USB functions to run without interruption but
		// with interrupts enabled so ENTER/EXIT_CRITICAL won't work
		vTaskSuspendAll();

			CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
			USB_USBTask();

		xTaskResumeAll();

		vTaskDelay((portTickType) taskDelayPeriod );
	}

}



static void MainTask(void *pvParameters)
{
	for(;;) {
		MainTaskLoop();
		portYIELD();
	}
}



void MainTaskLoop(void)
{
	int count = 0;

	// If the host has sent data then echo it back
	// Throughput is maximized if the full EP buffer is read and sent each time
	// Throughput approaches CDC_TXRX_EPSIZE kbytes/second and depends on transfer size from host 
	// NOTE: AVRlibc stdio functions are not thread-safe and must therefore be in a Suspend-Resume section
	vTaskSuspendAll();
		count = fread(&buffer, 1, CDC_TXRX_EPSIZE, &USBSerialStream);
	xTaskResumeAll();

	//TODO: you can process the received buffer data here

	vTaskSuspendAll();
		if (count > 0) {
			fwrite(&buffer, 1, count, &USBSerialStream);
		}
	xTaskResumeAll();


	// If HWB Button is pressed then send formatted strings
	if (Buttons_GetStatus()) {
		// NOTE: AVRlibc stdio functions are not thread-safe and must therefore be in a Suspend-Resume section
		vTaskSuspendAll();
			fprintf_P(&USBSerialStream, PSTR("\r\nHWB has been pressed!\r\n")); // send a constant string stored in FLASH
			fprintf(&USBSerialStream, "PORTD = %3x\r\n", PIND); // send a string that is dynamic and stored in SRAM
		xTaskResumeAll();
	}

}
