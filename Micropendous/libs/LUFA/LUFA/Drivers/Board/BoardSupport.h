/*
             LUFA Library
     Copyright (C) Dean Camera, 2011.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2011  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  This is Buttons.h modified for general "Board Support" by Opendous Inc. 2011-11-11

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
 *  \brief Board Support - drivers for all relevant board hardware.
 *
 *  This file is the master dispatch header file for the board-specific drivers.
 *
 *  User code should include this file, which will in turn include all the correct driver header
 *  files for the currently selected board.
 *
 *  If the \c BOARD value is set to \c BOARD_USER, this will include the \c /Board/BoardSupport.h
 *  file in the user project directory.
 *
 *  For possible \c BOARD makefile values, see \ref Group_BoardTypes.
 */

/** \ingroup Group_BoardDrivers
 *  \defgroup Group_BoardSupport Board Support Drivers - LUFA/Drivers/Board/BoardSupport.h
 *  \brief Drivers for all relevant board hardware.
 *
 *  \section Sec_Dependencies Module Source Dependencies
 *  The following files must be built with any user project that uses this module:
 *    - None
 *
 *  \section Sec_ModDescription Module Description
 *  Hardware buttons driver. This provides an easy to use driver for the hardware buttons present on many boards.
 *  It provides a way to easily configure and check the status of all the buttons on the board so that appropriate
 *  actions can be taken.
 *
 *  If the \c BOARD value is set to \c BOARD_USER, this will include the \c /Board/BoardSupport.h
 *  file in the user project directory. Otherwise, it will include the appropriate built in board driver header file.
 *
 *  For possible \c BOARD makefile values, see \ref Group_BoardTypes.
 *
 *  \section Sec_ExampleUsage Example Usage
 *  The following snippet is an example of how this module may be used within a typical
 *  application.
 *
 *  \code
 *      // Initialize the button driver before first use
 *      Board_Init();
 *
 *      // Turn on each of the four LEDs in turn
 *      LEDs_SetAllLEDs(LEDS_LED1); 
 *      Delay_MS(500);
 *      LEDs_SetAllLEDs(LEDS_LED2); 
 *      Delay_MS(500);
 *
 *      printf("Waiting for button press...\r\n");
 *
 *      // Loop until a board button has been pressed
 *      uint8_t ButtonPress;
 *      while (!(ButtonPress = Buttons_GetStatus())) {};
 *
 *      // Display which button was pressed (assuming two board buttons)
 *      printf("Button pressed: %s\r\n", (ButtonPress == BUTTONS_BUTTON1) ? "Button 1" : "Button 2");
 *  \endcode
 * 
 *  @{
 */

#ifndef __BOARDSUPPORT_H__
#define __BOARDSUPPORT_H__

	/* Macros: */
		#define __INCLUDE_FROM_BUTTONS_H

	/* Includes: */
		#include "../../Common/Common.h"

		// Button and LED drivers have a do-nothing/empty option so that the following will always successfully include
		#include "Buttons.h"
		#include "LEDs.h"

		// The Micropendous-A board has external SRAM but otherwise should grab the defines frm the following else clause
		#if (BOARD == BOARD_MICROPENDOUS_A)
			#include "AVR8/MICROPENDOUS_A/External_SRAM.h"
		#endif

		#if (BOARD == BOARD_MICROPENDOUS_REV1)
			#include "AVR8/MICROPENDOUS_REV1/External_SRAM.h"
			#include "AVR8/MICROPENDOUS_REV1/USB_Switch.h"
			#include "AVR8/MICROPENDOUS_REV1/Voltage_TXRX.h"
		#elif (BOARD == BOARD_MICROPENDOUS_REV2)
			#include "AVR8/MICROPENDOUS_REV2/External_SRAM.h"
			#include "AVR8/MICROPENDOUS_REV2/USB_Switch.h"
			#include "AVR8/MICROPENDOUS_REV2/Voltage_TXRX.h"
		#else
			// the following are board defines that can still exist in functional programs that do
			// not use External SRAM, a Voltage Translator, or the USB Signal+Power switches
			#define DISABLE_VOLTAGE_TXRX	__asm__ volatile ("NOP" ::)
			#define ENABLE_VOLTAGE_TXRX	__asm__ volatile ("NOP" ::)
			#if !defined(DISABLE_EXT_SRAM)
				#define DISABLE_EXT_SRAM	__asm__ volatile ("NOP" ::)
			#endif
			#define SELECT_USB_A		__asm__ volatile ("NOP" ::)
			#define SELECT_USB_B		__asm__ volatile ("NOP" ::)
			#define OVERCURRENT_FLAG_ENABLE		__asm__ volatile ("NOP" ::)
			#define OVERCURRENT_FLAG_STATUS		0
		#endif

		#define Board_Init()			Buttons_Init(); LEDs_Init();

	/* Pseudo-Functions for Doxygen: */
	#if defined(__DOXYGEN__)
		/** Initializes the BOARDSUPPORT driver, so that the current button position can be read. This sets the appropriate
		 *  I/O pins to an inputs with pull-ups enabled.
		 *
		 *  \return None
		 *
		 *  This must be called before any driver functions are used.
		 */
		static inline void Board_Init(void);
	#endif

#endif // __BOARDSUPPORT_H__

/** @} */
