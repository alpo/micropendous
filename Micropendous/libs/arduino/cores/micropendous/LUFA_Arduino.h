/*
             LUFA Library
     Copyright (C) Dean Camera, 2010.
              
  dean [at] fourwalledcubicle [dot] com
      www.fourwalledcubicle.com
*/

/*
  Copyright 2010  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Adapted for LUFA + Arduino by Opendous Inc. 2010-03
  For more information visit:  www.Micropendous.org/LUFAduino

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
 *  Header file for LUFAduino
 */

#ifndef _LUFA_ARDUINO_H_
#define _LUFA_ARDUINO_H_

	/* Includes: */
		#include <avr/io.h>
		#include <avr/wdt.h>
		#include <avr/interrupt.h>
		#include <avr/power.h>
		#include <avr/eeprom.h> 

		#include "Descriptors.h"

		#include "Lib/RingBuff.h"

		#include <LUFA/Version.h>
		#include <LUFA/Drivers/Board/LEDs.h>
		#include <LUFA/Drivers/USB/USB.h>
		#include <LUFA/Drivers/USB/Class/CDC.h>
		#include <LUFA/Drivers/Peripheral/SPI.h>
		#include <LUFA/Drivers/Peripheral/ADC.h>

		/* Scheduler include files. */
		#include "FreeRTOS.h"
		#include "task.h"
		#include "croutine.h"
		#include "FreeRTOSConfig.h"


#ifdef __cplusplus
extern "C" {
#endif
	#include "USBVirtualSerial.h"


	/* Macros: */
		/* USB and CDC Tasks must run at same priority so that they do not interrupt each other 
			but must be high priority to make sure of proper USB functionality.
			Main_Task should run whenever USB or CDC task are not
		*/
		#define MAIN_TASK_PRIORITY			( tskIDLE_PRIORITY + 1 )
		#define USB_CDC_TASK_PRIORITY	( tskIDLE_PRIORITY + 5 )

		#define taskDelayPeriod		1


		/** LED mask for the library LED driver, to indicate that the USB interface is not ready. */
		#define LEDMASK_USB_NOTREADY      LEDS_LED1

		/** LED mask for the library LED driver, to indicate that the USB interface is enumerating. */
		#define LEDMASK_USB_ENUMERATING  (LEDS_LED2 | LEDS_LED3)

		/** LED mask for the library LED driver, to indicate that the USB interface is ready. */
		#define LEDMASK_USB_READY        (LEDS_LED2 | LEDS_LED4)

		/** LED mask for the library LED driver, to indicate that an error has occurred in the USB interface. */
		#define LEDMASK_USB_ERROR        (LEDS_LED1 | LEDS_LED3)
		
	/* Function Prototypes: */
		void SetupHardware(void);
		void Main_Task(void);
		uint8_t haveData(void);
		int sendData(char c, FILE *stream);
		int getData(FILE *__stream);

		void vApplicationIdleHook(void);
		void USBTask(void *pvParameters);
		void CDCTask(void *pvParameters);
		void MainTask(void *pvParameters);
		void MainTask2(void *pvParameters);

		void EVENT_USB_Device_Connect(void);
		void EVENT_USB_Device_Disconnect(void);
		void EVENT_USB_Device_ConfigurationChanged(void);
		void EVENT_USB_Device_UnhandledControlRequest(void);

		void EVENT_CDC_Device_LineEncodingChanged(USB_ClassInfo_CDC_Device_t* const CDCInterfaceInfo);

#ifdef __cplusplus
}
#endif

#endif	// _LUFA_ARDUINO_H_
