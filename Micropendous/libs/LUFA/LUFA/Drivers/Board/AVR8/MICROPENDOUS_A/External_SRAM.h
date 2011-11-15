/*
  Copyright 2011-11-11 By Opendous Inc.
  For use with Micropendous-A, Micropendous3, or Micropendous4 boards where
  PE6 is the external SRAM Chip Enable Pin and PE7 is Address Bit 17/Bank Selector Pin

  www.Micropendous.org/Micropendous-A
  www.Micropendous.org/Micropendous3
  www.Micropendous.org/Micropendous4

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
 *  \brief Board specific external SRAM driver header for the older style Micropendous-A/3/4 boards (www.Micropendous.org/Micropendous-A).
 *        where PE6 is the external SRAM Chip Enable Pin and PE7 is Address Bit 17/Bank Selector Pin
 *  \copydetails Group_Buttons_MICROPENDOUS_A
 *
 *  \note This file should not be included directly. It is automatically included by the buttons driver
 *        dispatch header located in LUFA/Drivers/Board/Buttons.h.
 */

/** \ingroup Group_Buttons
 *  \defgroup Group_Buttons_MICROPENDOUS_A MICROPENDOUS_A
 *  \brief Board specific external SRAM driver header for the older style Micropendous-A/3/4 board
 *        where PE6 is the external SRAM Chip Enable Pin and PE7 is Address Bit 17/Bank Selector Pin
 *
 *  Board specific external SRAM driver header for the Micropendous board (www.Micropendous.org/Micropendous-A).
 *
 *  @{
 */

#ifndef __EXTERNAL_SRAM_MICROPENDOUS_A_H__
#define __EXTERNAL_SRAM_MICROPENDOUS_A_H__

	/* Includes: */
	#include "../../../../Common/Common.h"

	/* Enable C linkage for C++ Compilers: */
	#if defined(__cplusplus)
		extern "C" {
	#endif

	/* Public Interface - May be used in end-application: */
	// On older Micropendous3/4 boards, nCE is PE6 and Address Bit 17 is PE7.  PE0,1,2 are also control signals.
	#define PORTE_EXT_SRAM_SETUP	DDRE = ((1 << PE0) | (1 << PE1) | (1 << PE2) | (1 << PE6) | (1 << PE7)); PORTE = ((1 << PE0) | (1 << PE1) | (1 << PE6) | (1 << PE7));
	#define ENABLE_EXT_SRAM		DDRE |= (1 << PE6); PORTE &= ~(1 << PE6);
	#define DISABLE_EXT_SRAM	DDRE |= (1 << PE6); PORTE |= (1 << PE6);
	#define SELECT_EXT_SRAM_BANK0	DDRE |= (1 << PE7); PORTE &= ~(1 << PE7);
	#define SELECT_EXT_SRAM_BANK1	DDRE |= (1 << PE7); PORTE |= (1 << PE7);
	#define CURRENT_SRAM_BANK	((PINE >> 7) & (0x01))

	#if (!defined(NO_AUTO_VBUS_MANAGEMENT))
		#error Micropendous-A boards require the NO_AUTO_VBUS_MANAGEMENT compile-time LUFA option.
	#endif

	/* Disable C linkage for C++ Compilers: */
	#if defined(__cplusplus)
		}
	#endif

#endif // __EXTERNAL_SRAM_MICROPENDOUS_A_H__

/** @} */
