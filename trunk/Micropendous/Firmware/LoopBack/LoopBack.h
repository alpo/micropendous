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
 *  Header file for Keyboard.c.
 */

#ifndef _LOOPBACK_H_
#define _LOOPBACK_H_

	/* Includes: */
		#include <avr/io.h>
		#include <avr/wdt.h>
		#include <avr/interrupt.h>
		#include <avr/power.h>
		//#include <stdbool.h>
		#include <string.h>

		#include "Descriptors.h"

		#include <LUFA/Version.h>
		#include <LUFA/Drivers/USB/USB.h>
		#include <LUFA/Drivers/Board/LEDs.h>

	/* Macros: */
		/** LED mask for the library LED driver, to indicate that the USB interface is not ready. */
		#define LEDMASK_USB_NOTREADY      LEDS_LED1

		/** LED mask for the library LED driver, to indicate that the USB interface is enumerating. */
		#define LEDMASK_USB_ENUMERATING  (LEDS_LED2 | LEDS_LED3)

		/** LED mask for the library LED driver, to indicate that the USB interface is ready. */
		#define LEDMASK_USB_READY        (LEDS_LED2 | LEDS_LED4)

		/** LED mask for the library LED driver, to indicate that an error has occurred in the USB interface. */
		#define LEDMASK_USB_ERROR        (LEDS_LED1 | LEDS_LED3)


	/* Missing WinAVR include defines */
	/* WinAVR does not define these for the ATmega??u4*/
	#if (defined(__AVR_ATmega16U4__)  || defined(__AVR_ATmega32U4__))
		#ifndef PB7
			#define PB7		7
		#endif
		#ifndef PB6
			#define PB6		6
		#endif
		#ifndef PB5
			#define PB5		5
		#endif
		#ifndef PB4
			#define PB4		4
		#endif
		#ifndef PB3
			#define PB3		3
		#endif
		#ifndef PB2
			#define PB2		2
		#endif
		#ifndef PB1
			#define PB1		1
		#endif
		#ifndef PB0
			#define PB0		0
		#endif
		#ifndef PC7
			#define PC7		7
		#endif
		#ifndef PC6
			#define PC6		6
		#endif
		#ifndef PD7
			#define PD7		7
		#endif
		#ifndef PD6
			#define PD6		6
		#endif
		#ifndef PD5
			#define PD5		5
		#endif
		#ifndef PD4
			#define PD4		4
		#endif
		#ifndef PD3
			#define PD3		3
		#endif
		#ifndef PD2
			#define PD2		2
		#endif
		#ifndef PD1
			#define PD1		1
		#endif
		#ifndef PD0
			#define PD0		0
		#endif
		#ifndef PE2
			#define PE2		2
		#endif
		#ifndef PE6
			#define PE6		6
		#endif
		#ifndef PF7
			#define PF7		7
		#endif
		#ifndef PF6
			#define PF6		6
		#endif
		#ifndef PF5
			#define PF5		5
		#endif
		#ifndef PF4
			#define PF4		4
		#endif
		#ifndef PF1
			#define PF1		1
		#endif
		#ifndef PF0
			#define PF0		0
		#endif
	#endif


	/* Global Variables: */
		extern volatile uint8_t dataToSend[];
		extern volatile uint8_t dataReceived[];

	/* Function Prototypes: */
		void SetupHardware(void);
		void Main_Task(void);
	
		void EVENT_USB_Device_Connect(void);
		void EVENT_USB_Device_Disconnect(void);
		void EVENT_USB_Device_ConfigurationChanged(void);
		void EVENT_USB_Device_UnhandledControlRequest(void);

		void ReceiveDataFromHost(void);
		void SendDataToHost(void);

#endif
