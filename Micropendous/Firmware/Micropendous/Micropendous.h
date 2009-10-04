/*
             LUFA Library
     Copyright (C) Dean Camera, 2009.
              
  dean [at] fourwalledcubicle [dot] com
      www.fourwalledcubicle.com
*/

/*
  Copyright 2009  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Adapted for LUFA + Arduino by Opendous Inc. 2009-10
  For more information visit:  www.Micropendous.org/LUFAduino

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
 *  Header file for LUFA_Arduino.c.
 */

// keep the name _LUFA_ARDUINO_H_ as this is a replacement for that header!
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
		#include <LUFA/Drivers/USB/USB.h>
		#include <LUFA/Drivers/Board/Joystick.h>
		#include <LUFA/Drivers/Board/LEDs.h>
		#include <LUFA/Drivers/Board/Buttons.h>
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

	/* Enums: */
		/** Enum for the possible status codes for passing to the UpdateStatus() function. */
		enum KeyboardMouse_StatusCodes_t
		{
			Status_USBNotReady    = 0, /**< USB is not ready (disconnected from a USB host) */
			Status_USBEnumerating = 1, /**< USB interface is enumerating */
			Status_USBReady       = 2, /**< USB interface is connected and ready */
		};

	/* Macros: */
		/* USB and CDC Tasks must run at same priority so that they do not interrupt each other 
			but must be high priority to make sure of proper USB functionality.
			Main_Task should run whenever USB or CDC task are not
		*/
		#define MAIN_TASK_PRIORITY			( tskIDLE_PRIORITY + 1 )
		#define USB_CDC_TASK_PRIORITY	( tskIDLE_PRIORITY + 5 )

		#define taskDelayPeriod		1

		/** Idle period indicating that reports should be sent only when the inputs have changed */
		#define HID_IDLE_CHANGESONLY   0

		/** HID Class specific request to get the next HID report from the device. */
		#define REQ_GetReport      0x01

		/** HID Class specific request to send the next HID report to the device. */
		#define REQ_SetReport      0x09

		/** HID Class specific request to get the idle timeout period of the device. */
		#define REQ_GetIdle            0x02

		/** HID Class specific request to set the idle timeout period of the device. */
		#define REQ_SetIdle            0x0A

		/** HID Class specific request to get the current HID protocol in use, either report or boot. */
		#define REQ_GetProtocol        0x03

		/** HID Class specific request to set the current HID protocol in use, either report or boot. */
		#define REQ_SetProtocol        0x0B

		/** CDC Class specific request to get the current virtual serial port configuration settings. */
		#define REQ_GetLineEncoding          0x21

		/** CDC Class specific request to set the current virtual serial port configuration settings. */
		#define REQ_SetLineEncoding          0x20

		/** CDC Class specific request to set the current virtual serial port handshake line states. */
		#define REQ_SetControlLineState      0x22

		/** Notification type constant for a change in the virtual serial port handshake line states, for
		 *  use with a USB_Notification_Header_t notification structure when sent to the host via the CDC 
		 *  notification endpoint.
		 */
		#define NOTIF_SerialState            0x20

		/** Mask for the DTR handshake line for use with the REQ_SetControlLineState class specific request
		 *  from the host, to indicate that the DTR line state should be high.
		 */
		#define CONTROL_LINE_OUT_DTR         (1 << 0)

		/** Mask for the RTS handshake line for use with the REQ_SetControlLineState class specific request
		 *  from the host, to indicate that theRTS line state should be high.
		 */
		#define CONTROL_LINE_OUT_RTS         (1 << 1)

		/** Mask for the DCD handshake line for use with the a NOTIF_SerialState class specific notification
		 *  from the device to the host, to indicate that the DCD line state is currently high.
		 */
		#define CONTROL_LINE_IN_DCD          (1 << 0)

		/** Mask for the DSR handshake line for use with the a NOTIF_SerialState class specific notification
		 *  from the device to the host, to indicate that the DSR line state is currently high.
		 */
		#define CONTROL_LINE_IN_DSR          (1 << 1)

		/** Mask for the BREAK handshake line for use with the a NOTIF_SerialState class specific notification
		 *  from the device to the host, to indicate that the BREAK line state is currently high.
		 */
		#define CONTROL_LINE_IN_BREAK        (1 << 2)

		/** Mask for the RING handshake line for use with the a NOTIF_SerialState class specific notification
		 *  from the device to the host, to indicate that the RING line state is currently high.
		 */
		#define CONTROL_LINE_IN_RING         (1 << 3)

		/** Mask for use with the a NOTIF_SerialState class specific notification from the device to the host,
		 *  to indicate that a framing error has occurred on the virtual serial port.
		 */
		#define CONTROL_LINE_IN_FRAMEERROR   (1 << 4)

		/** Mask for use with the a NOTIF_SerialState class specific notification from the device to the host,
		 *  to indicate that a parity error has occurred on the virtual serial port.
		 */
		#define CONTROL_LINE_IN_PARITYERROR  (1 << 5)

		/** Mask for use with the a NOTIF_SerialState class specific notification from the device to the host,
		 *  to indicate that a data overrun error has occurred on the virtual serial port.
		 */
		#define CONTROL_LINE_IN_OVERRUNERROR (1 << 6)

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

	/* Type Defines: */
		/** Type define for the keyboard HID report structure, for creating and sending HID reports to the host PC.
		 *  This mirrors the layout described to the host in the HID report descriptor, in Descriptors.c.
		 */
		typedef struct
		{
			uint8_t ReportID;  /**< Id to identify this as a keyboard report */
			uint8_t Modifier; /**< Modifier mask byte, containing a mask of modifier keys set (such as shift or CTRL) */
			uint8_t Reserved; /**< Reserved, always set as 0x00 */
			uint8_t KeyCode[6]; /**< Array of up to six simultaneous key codes of pressed keys */
		} USB_KeyboardReport_Data_t;

		/** Type define for the mouse HID report structure, for creating and sending HID reports to the host PC.
		 *  This mirrors the layout described to the host in the HID report descriptor, in Descriptors.c.
		 */
		typedef struct
		{
			uint8_t ReportID; /**< Id to identify this as a mouse report */
			uint8_t Button; /**< Bit mask of the currently pressed mouse buttons */
			int8_t  X; /**< Current mouse delta X movement, as a signed 8-bit integer */
			int8_t  Y; /**< Current mouse delta Y movement, as a signed 8-bit integer */
		} USB_MouseReport_Data_t;

		/** Type define for the virtual serial port line encoding settings, for storing the current USART configuration
		 *  as set by the host via a class specific request.
		 */
		typedef struct
		{
			uint32_t BaudRateBPS; /**< Baud rate of the virtual serial port, in bits per second */
			uint8_t  CharFormat; /**< Character format of the virtual serial port, a value from the
			                      *   CDCDevice_CDC_LineCodingFormats_t enum
			                      */
			uint8_t  ParityType; /**< Parity setting of the virtual serial port, a value from the
			                      *   CDCDevice_LineCodingParity_t enum
			                      */
			uint8_t  DataBits; /**< Bits of data per character of the virtual serial port */
		} CDC_Line_Coding_t;
		
		/** Type define for a CDC notification, sent to the host via the CDC notification endpoint to indicate a
		 *  change in the device state asynchronously.
		 */
		typedef struct
		{
			uint8_t  NotificationType; /**< Notification type, a mask of REQDIR_*, REQTYPE_* and REQREC_* constants
			                            *   from the library StdRequestType.h header
			                            */
			uint8_t  Notification; /**< Notification value, a NOTIF_* constant */
			uint16_t wValue; /**< Notification wValue, notification-specific */
			uint16_t wIndex; /**< Notification wIndex, notification-specific */
			uint16_t wLength; /**< Notification wLength, notification-specific */
		} USB_Notification_Header_t;
		
	/* Enums: */
		/** Enum for the possible line encoding formats of a virtual serial port. */
		enum CDCDevice_CDC_LineCodingFormats_t
		{
			OneStopBit          = 0, /**< Each frame contains one stop bit */
			OneAndAHalfStopBits = 1, /**< Each frame contains one and a half stop bits */
			TwoStopBits         = 2, /**< Each frame contains two stop bits */
		};
		
		/** Enum for the possible line encoding parity settings of a virtual serial port. */
		enum CDCDevice_LineCodingParity_t
		{
			Parity_None         = 0, /**< No parity bit mode on each frame */
			Parity_Odd          = 1, /**< Odd parity bit mode on each frame */
			Parity_Even         = 2, /**< Even parity bit mode on each frame */
			Parity_Mark         = 3, /**< Mark parity bit mode on each frame */
			Parity_Space        = 4, /**< Space parity bit mode on each frame */
		};
		
	/* Function Prototypes: */
		void SetupHardware(void);
		void Reconfigure(void);
		void CDC_Task(void);
		void Keyboard_Task(void);
		void Mouse_Task(void);
		void ReceiveNextReport(void);
		void SendNextReport(void);
		void Main_Task(void);
		void Main_Task2(void);

		uint8_t haveData(void);
		int sendData(char c, FILE *stream);
		int getData(FILE *__stream);

		void vApplicationIdleHook(void);
		void USBTask(void *pvParameters);
		void CDCTask(void *pvParameters);
		void HIDTask(void *pvParameters);
		void MainTask(void *pvParameters);
		void MainTask2(void *pvParameters);

		void EVENT_USB_Device_Connect(void);
		void EVENT_USB_Device_Disconnect(void);
		void EVENT_USB_Device_ConfigurationChanged(void);
		void EVENT_USB_Device_UnhandledControlRequest(void);
		
		extern void loop(void);
		extern void loop2(void);

		void CreateKeyboardReport(USB_KeyboardReport_Data_t* ReportData);

#ifdef __cplusplus
}
#endif

#endif // _LUFA_ARDUINO_H_
