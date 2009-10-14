/*
             LUFA Library
     Copyright (C) Dean Camera, 2009.
              
  dean [at] fourwalledcubicle [dot] com
      www.fourwalledcubicle.com
*/

/*
  Copyright 2009  Dean Camera (dean [at] fourwalledcubicle [dot] com)

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

/*
	Bluetooth Dongle host demo application.
	
	** NOT CURRENTLY FUNCTIONAL - DO NOT USE **
*/

#include "Bluetooth.h"

Bluetooth_Device_t Bluetooth_DeviceConfiguration =
	{
		Class:   (DEVICE_CLASS_SERVICE_CAPTURING | DEVICE_CLASS_MAJOR_COMPUTER | DEVICE_CLASS_MINOR_COMPUTER_PALM),
		PINCode: "0000",
		Name:    "LUFA Bluetooth Dem1"
	};



int main(void)
{
	SetupHardware();
	
	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);

	puts_P(PSTR(ESC_FG_CYAN "Bluetooth Host Demo running.\r\n" ESC_FG_WHITE));
		  

//	xTaskCreate(USBTask, (signed portCHAR *) "usbtask", configMINIMAL_STACK_SIZE, NULL, USB_BT_TASK_PRIORITY, NULL );
//	xTaskCreate(StckTask, (signed portCHAR *) "stcktask", configMINIMAL_STACK_SIZE, NULL, USB_BT_TASK_PRIORITY, NULL );
//	xTaskCreate(MgmtTask, (signed portCHAR *) "mgmttask", configMINIMAL_STACK_SIZE, NULL, USB_BT_TASK_PRIORITY, NULL );
//	xTaskCreate(ScndTask, (signed portCHAR *) "scndtask", configMINIMAL_STACK_SIZE, NULL, USB_BT_TASK_PRIORITY, NULL );
	xTaskCreate(MainTask, (signed portCHAR *) "maintask", configMINIMAL_STACK_SIZE, NULL, MAIN_TASK_PRIORITY, NULL );

	// Start the scheduler
	vTaskStartScheduler();

//for (;;) {
//        Bluetooth_Stack_Task();
//	Bluetooth_Management_Task();
//	USB_USBTask();
//}

	// Should never get here!
	return 0;
}



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
//		MCUCR = (1 << JTD) | (1 << IVCE) | (0 << PUD);
//		MCUCR = (1 << JTD) | (0 << IVSEL) | (0 << IVCE) | (0 << PUD);
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
		DDRD = 0xFF; // output
		PORTD = 0;
		DDRE = (1 << PE6); // set PE6 to HIGH to disable external SRAM, if connected
		PORTE = (1 << PE6);
		DDRF = 0;
		PORTF = 0;
	#endif

	/* Hardware Initialization */
	SerialStream_Init(9600, false);
	LEDs_Init();
	USB_Init();
}



static void USBTask(void *pvParameters)
{
	portTickType xDelayPeriod = taskDelayPeriod;
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

	for(;;) {
	        Bluetooth_Stack_Task();
                Bluetooth_Management_Task();
                USB_USBTask();

		// vTaskDelayUntil is faster and deterministic
		vTaskDelayUntil(&xLastWakeTime, xDelayPeriod);
		// vTaskDelay(xDelayPeriod);
	}
}



static void StckTask(void *pvParameters)
{
	portTickType xDelayPeriod = taskDelayPeriod;
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

	for(;;) {
		Bluetooth_Stack_Task();
		// vTaskDelayUntil is faster and deterministic
		vTaskDelayUntil(&xLastWakeTime, xDelayPeriod);
		//vTaskDelay(xDelayPeriod);
	}
}



static void MgmtTask(void *pvParameters)
{
	portTickType xDelayPeriod = taskDelayPeriod;
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

	for(;;) {
		Bluetooth_Management_Task();
		// vTaskDelayUntil is faster and deterministic
		vTaskDelayUntil(&xLastWakeTime, xDelayPeriod);
		//vTaskDelay(xDelayPeriod);
	}
}



static void ScndTask(void *pvParameters)
{
	portTickType xDelayPeriod = 1000; // once per second
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

	for(;;) {
		Run_Once_Per_Second();
		// vTaskDelayUntil is faster and deterministic
		vTaskDelayUntil(&xLastWakeTime, xDelayPeriod);
		//vTaskDelay(xDelayPeriod);
	}
}



static void MainTask(void *pvParameters)
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




// TODO - CoRoutines are not enabled, but FreeRTOS complains during compile
void vApplicationIdleHook(void)
{
	vCoRoutineSchedule();
}



void EVENT_USB_Host_DeviceAttached(void)
{
	puts_P(PSTR(ESC_FG_GREEN "Device Attached.\r\n" ESC_FG_WHITE));

	LEDs_SetAllLEDs(LEDMASK_USB_ENUMERATING);
}



void EVENT_USB_Host_DeviceUnattached(void)
{
	puts_P(PSTR(ESC_FG_GREEN "\r\nDevice Unattached.\r\n" ESC_FG_WHITE));

	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
}



void EVENT_USB_Host_DeviceEnumerationComplete(void)
{
	LEDs_SetAllLEDs(LEDMASK_USB_READY);
}



void EVENT_USB_Host_HostError(uint8_t ErrorCode)
{
	USB_ShutDown();

	printf_P(PSTR(ESC_FG_RED "Host Mode Error\r\n"
	                         " -- Error Code %d\r\n" ESC_FG_WHITE), ErrorCode);

	LEDs_SetAllLEDs(LEDMASK_USB_ERROR);
	for(;;);
}



void EVENT_USB_Host_DeviceEnumerationFailed(uint8_t ErrorCode, uint8_t SubErrorCode)
{
	printf_P(PSTR(ESC_FG_RED "Dev Enum Error\r\n"
	                         " -- Error Code %d\r\n"
	                         " -- Sub Error Code %d\r\n"
	                         " -- In State %d\r\n" ESC_FG_WHITE), ErrorCode, SubErrorCode, USB_HostState);
	
	LEDs_SetAllLEDs(LEDMASK_USB_ERROR);
}



void Bluetooth_Management_Task(void)
{
	uint8_t ErrorCode;

	switch (USB_HostState)
	{
		case HOST_STATE_Addressed:
			puts_P(PSTR("Getting Device Data.\r\n"));
		
			/* Get and process the configuration descriptor data */
			if ((ErrorCode = ProcessDeviceDescriptor()) != SuccessfulDeviceRead)
			{
				if (ErrorCode == ControlErrorDuringDeviceRead)
				  puts_P(PSTR(ESC_FG_RED "Control Error (Get Device).\r\n"));
				else
				  puts_P(PSTR(ESC_FG_RED "Invalid Device.\r\n"));

				printf_P(PSTR(" -- Error Code: %d\r\n" ESC_FG_WHITE), ErrorCode);
				
				/* Indicate error via status LEDs */
				LEDs_SetAllLEDs(LEDS_LED1);

				/* Wait until USB device disconnected */
				USB_HostState = HOST_STATE_WaitForDeviceRemoval;
				break;
			}

			puts_P(PSTR("Bluetooth Dongle Detected.\r\n"));

			/* Set the device configuration to the first configuration (rarely do devices use multiple configurations) */
			if ((ErrorCode = USB_Host_SetDeviceConfiguration(1)) != HOST_SENDCONTROL_Successful)
			{
				printf_P(PSTR(ESC_FG_RED "Control Error (Set Configuration).\r\n"
				                         " -- Error Code: %d\r\n" ESC_FG_WHITE), ErrorCode);

				/* Indicate error via status LEDs */
				LEDs_SetAllLEDs(LEDS_LED1);

				/* Wait until USB device disconnected */
				USB_HostState = HOST_STATE_WaitForDeviceRemoval;
				break;
			}
				
			puts_P(PSTR("Getting Config Data.\r\n"));
		
			/* Get and process the configuration descriptor data */
			if ((ErrorCode = ProcessConfigurationDescriptor()) != SuccessfulConfigRead)
			{
				if (ErrorCode == ControlErrorDuringConfigRead)
				  puts_P(PSTR(ESC_FG_RED "Control Error (Get Configuration).\r\n"));
				else
				  puts_P(PSTR(ESC_FG_RED "Invalid Device.\r\n"));

				printf_P(PSTR(" -- Error Code: %d\r\n" ESC_FG_WHITE), ErrorCode);
				
				/* Indicate error via status LEDs */
				LEDs_SetAllLEDs(LEDS_LED1);

				/* Wait until USB device disconnected */
				USB_HostState = HOST_STATE_WaitForDeviceRemoval;
				break;
			}

			puts_P(PSTR("Bluetooth Dongle Enumerated.\r\n"));

			USB_HostState = HOST_STATE_Configured;
			break;
	}
}


/* Tasks that need to be run once per second */
void Run_Once_Per_Second(void)
{
	//
}



/* Main_Task will run at lowest priority once initialization is complete */
void Main_Task(void)
{
	Bluetooth_Stack_Task();
	Bluetooth_Management_Task();
	USB_USBTask();
}
