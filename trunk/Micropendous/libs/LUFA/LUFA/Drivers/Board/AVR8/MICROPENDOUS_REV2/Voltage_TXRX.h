/*
  Copyright 2011-11-08 By Opendous Inc.
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
 *  \brief Board specific PORTB voltage translator driver header for the
 *        Micropendous Rev2 board (www.Micropendous.org/Micropendous).
 *  \copydetails Group_Buttons_MICROPENDOUS_REV2
 *
 *  \note This file should not be included directly. It is automatically included by the buttons driver
 *        dispatch header located in LUFA/Drivers/Board/Buttons.h.
 */

/** \ingroup Group_Buttons
 *  \defgroup Group_Buttons_MICROPENDOUS_REV2 MICROPENDOUS_REV2
 *  \brief Board specific PORTB voltage translator driver header for the Micropendous Rev2 board.
 *
 *  Board specific PORTB voltage translator driver header for the
 *        Micropendous Rev2 board (www.Micropendous.org/Micropendous).
 *
 *  @{
 */

#ifndef __VOLTAGE_TXRX_MICROPENDOUS_REV2_H__
#define __VOLTAGE_TXRX_MICROPENDOUS_REV2_H__

	/* Includes: */
	#include "../../../../Common/Common.h"

	/* Enable C linkage for C++ Compilers: */
	#if defined(__cplusplus)
		extern "C" {
	#endif

	/* Public Interface - May be used in end-application: */
	// On Micropendous Rev2 boards the PORTB voltage translator is controlled by PE3
	#define ENABLE_VOLTAGE_TXRX	DDRE |= (1 << PE3); PORTE |= (1 << PE3);
	#define DISABLE_VOLTAGE_TXRX	DDRE |= (1 << PE3); PORTE &= ~(1 << PE3);

	/* Disable C linkage for C++ Compilers: */
	#if defined(__cplusplus)
		}
	#endif

#endif // __VOLTAGE_TXRX_MICROPENDOUS_REV2_H__

/** @} */
