/*
             LUFA Library
     Copyright (C) Dean Camera, 2009.
              
  dean [at] fourwalledcubicle [dot] com
      www.fourwalledcubicle.com
*/

/*
  Copyright 2009 Denver Gingerich (denver [at] ossguy [dot] com)
    Based on code by Dean Camera (dean [at] fourwalledcubicle [dot] com)
    USBProg OpenOCD porting to LUFA by Opendous Inc. - www.opendous.org
    For more info visit: www.Micropendous.org/USBProg_OpenOCD

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
 *  Header file for USBProg_OpenOCD.c.
 */

#ifndef _USBPROG_OPENOCD_H_
#define _USBPROG_OPENOCD_H_

	/* Includes: */
		#include <avr/io.h>
		#include <avr/wdt.h>
		#include <avr/interrupt.h>
		#include <avr/power.h>
		#include <util/delay.h>
		#include <string.h>
		#include <stdint.h>

		#include "Descriptors.h"

		#include <LUFA/Version.h>
		#include <LUFA/Drivers/USB/USB.h>
		#include <LUFA/Drivers/Board/LEDs.h>

		#include "usbprogjtag.h"

		// USBProg-related defines
		#define UNKOWN_COMMAND	0x00
		#define PORT_DIRECTION	0x01
		#define PORT_SET	0x02
		#define PORT_GET	0x03
		#define PORT_SETBIT	0x04
		#define PORT_GETBIT	0x05
		#define WRITE_TDI	0x06
		#define READ_TDO	0x07
		#define WRITE_AND_READ	0x08
		#define WRITE_TMS	0x09
		#define WRITE_TMS_CHAIN	0x0A



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
		extern volatile char dataReceived[];
		extern volatile char answer[];

		extern volatile uint16_t  sendAnswerLength;
		extern volatile	uint8_t  processCommands;
		extern volatile uint8_t answerNeedsToBeSentFlag;

	/* Function Prototypes: */
		void SetupHardware(void);

		void EVENT_USB_Device_Connect(void);
		void EVENT_USB_Device_Disconnect(void);
		void EVENT_USB_Device_ConfigurationChanged(void);
		void EVENT_USB_Device_UnhandledControlRequest(void);

		void processOUT(void);
		void processIN(void);

#endif	// _USBPROG_OPENOCD_H_
