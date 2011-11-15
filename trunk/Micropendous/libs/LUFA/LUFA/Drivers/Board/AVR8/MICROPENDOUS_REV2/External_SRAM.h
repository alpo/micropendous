/*
  Copyright 2011-11-11 By Opendous Inc.
  For use with the Micropendous Rev2 board (www.Micropendous.org/Micropendous)

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
 *  \brief Board specific external SRAM driver header for the Micropendous Rev2 board (www.Micropendous.org/Micropendous).
 *  \copydetails Group_EXTERNALSRAM_MICROPENDOUS_REV2
 *
 *  \note This file should not be included directly. It is automatically included by the buttons driver
 *        dispatch header located in LUFA/Drivers/Board/Buttons.h.
 */

/** \ingroup Group_Buttons
 *  \defgroup Group_EXTERNALSRAM_MICROPENDOUS_REV2 MICROPENDOUS_REV2
 *  \brief Board specific external SRAM driver header for the Micropendous Rev2 board.
 *
 *  Board specific external SRAM driver header for the Micropendous Rev2 board (www.Micropendous.org/Micropendous).
 *
 *  @{
 */

#ifndef __EXTERNAL_SRAM_MICROPENDOUS_REV2_H__
#define __EXTERNAL_SRAM_MICROPENDOUS_REV2_H__

	/* Includes: */
	#include "../../../../Common/Common.h"

	/* Enable C linkage for C++ Compilers: */
	#if defined(__cplusplus)
		extern "C" {
	#endif

	/* Public Interface - May be used in end-application: */
	// On Micropendous Rev2 boards nCE is PE4 and Address Bit 17 is PE5.  PE0,1,2 are also control signals.
	#define PORTE_EXT_SRAM_SETUP	DDRE = ((1 << PE0) | (1 << PE1) | (1 << PE2) | (1 << PE4) | (1 << PE5)); PORTE = ((1 << PE0) | (1 << PE1) | (1 << PE2) | (1 << PE4));
	#define ENABLE_EXT_SRAM		DDRE |= (1 << PE4); PORTE &= ~(1 << PE4);
	#define DISABLE_EXT_SRAM	DDRE |= (1 << PE4); PORTE |= (1 << PE4);
	#define SELECT_EXT_SRAM_BANK0	DDRE |= (1 << PE5); PORTE &= ~(1 << PE5);
	#define SELECT_EXT_SRAM_BANK1	DDRE |= (1 << PE5); PORTE |= (1 << PE5);
	#define CURRENT_SRAM_BANK	((PINE >> 5) & (0x01))

	/* Disable C linkage for C++ Compilers: */
	#if defined(__cplusplus)
		}
	#endif

#endif // __EXTERNAL_SRAM_MICROPENDOUS_REV2_H__

/** @} */
