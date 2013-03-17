/*
  Written 2012-07-21 By Opendous Inc.
  For use with Micropendous USB AVR boards http://www.Micropendous.org

  This source file is hereby deeded by Opendous Inc. under the
  Creative Commons CC0 Dedication into the Public Domain.

  http://creativecommons.org/publicdomain/zero/1.0/

  You can copy, modify, distribute and utilize the work, even for commercial
  purposes, all without asking permission, persuent to the following:
  - In no way are the patent or trademark rights of any person affected by
    CC0, nor are the rights that other persons may have in the work or in how
    the work is used, such as publicity or privacy rights.
  - Unless expressly stated otherwise, the person who associated a work with
    this deed makes no warranties about the work, and disclaims liability for
    all uses of the work, to the fullest extent permitted by applicable law.
  - When using or citing the work, you should not imply endorsement by the
    author or the affirmer.
*/


#ifndef __MICROPENDOUS_DEFINES_H__
#define __MICROPENDOUS_DEFINES_H__

	/* Includes: */

	/* Enable C linkage for C++ Compilers: */
	#if defined(__cplusplus)
		extern "C" {
	#endif



	// Defines related to the TXB0108 8-bit Bidirectional Voltage Translator connected to PortB
	#if ((BOARD == BOARD_MICROPENDOUS_REV2))

		// On Micropendous Rev2 boards, the translator is on PE3 and is Active-Low enabled
		#define DISABLE_VOLTAGE_TXRX()	PORTE &= ~(1 << PE3); DDRE |= (1 << PE3);
		#define ENABLE_VOLTAGE_TXRX()	PORTE |= (1 << PE3); DDRE |= (1 << PE3);

	#elif ((BOARD == BOARD_MICROPENDOUS_REV1) || (BOARD == BOARD_MICROPENDOUS_DIP))

		// On Micropendous Rev1 boards, the translator is on PE6 and is Active-Low enabled
		#define DISABLE_VOLTAGE_TXRX()	PORTE &= ~(1 << PE6); DDRE |= (1 << PE6);
		#define ENABLE_VOLTAGE_TXRX()	PORTE |= (1 << PE6); DDRE |= (1 << PE6);

	#else 
		// other Micropendous boards do not have a voltage translator but NOPs maintain code operation count
		#define ENABLE_VOLTAGE_TXRX()	__asm__ volatile ("NOP" ::)
		#define DISABLE_VOLTAGE_TXRX()	__asm__ volatile ("NOP" ::)

	#endif



	// Defines related to the USB Selector
	#if ((BOARD == BOARD_MICROPENDOUS_REV2))

		// On Micropendous Rev2 boards, USB-A port selection is Active-High with overcurrent flag on PE6
		#define SELECT_USB_A()			PORTE |= (1 << PE7); DDRE |= (1 << PE7);
		#define SELECT_USB_B()			PORTE &= ~(1 << PE7); DDRE |= (1 << PE7);
		#define USB_A_OVERCURRENT_FLAG_ENABLE()	PORTE &= ~(1 << PE6); DDRE &= ~(1 << PE6);
		#define USB_A_OVERCURRENT_FLAG()	((PINE >> 6) & (0x01))

	#elif ((BOARD == BOARD_MICROPENDOUS_REV1) || (BOARD == BOARD_MICROPENDOUS_DIP))

		// On Micropendous Rev1 and DIP boards, USB-A port selection is Active-Low with overcurrent flag on a test-point
		#define SELECT_USB_A()			PORTE &= ~(1 << PE7); DDRE |= (1 << PE7);
		#define SELECT_USB_B()			PORTE |= (1 << PE7); DDRE |= (1 << PE7);
		#define USB_A_OVERCURRENT_FLAG_ENABLE()	__asm__ volatile ("NOP" ::)
		#define USB_A_OVERCURRENT_FLAG()	__asm__ volatile ("NOP" ::)
		#warning Remember to uncomment NO_AUTO_VBUS_MANAGEMENT and INVERTED_VBUS_ENABLE_LINE in Config/LUFAConfig.h

	#else

		// other Micropendous boards do not have an USB selector but NOPs maintain code operation count
		#define SELECT_USB_A()			__asm__ volatile ("NOP" ::)
		#define SELECT_USB_B()			__asm__ volatile ("NOP" ::)
		#define USB_A_OVERCURRENT_FLAG_ENABLE()	__asm__ volatile ("NOP" ::)
		#define USB_A_OVERCURRENT_FLAG()	__asm__ volatile ("NOP" ::)

	#endif



	// Defines related to external SRAM
	#if (defined(__AVR_AT90USB1287__) || defined(__AVR_AT90USB1286__))
		#define EXT_SRAM_START		0x2100
	#elif (defined(__AVR_AT90USB647__) || defined(__AVR_AT90USB646__))
		#define EXT_SRAM_START		0x1100
	#endif

	#ifndef EXT_SRAM_END
		#define EXT_SRAM_END			0xFFFF
	#endif
	#ifndef EXT_SRAM_SIZE
		#define EXT_SRAM_SIZE			(EXT_SRAM_END - EXT_SRAM_START)
	#endif

	#if ((BOARD == BOARD_MICROPENDOUS_REV2) || (BOARD == BOARD_MICROPENDOUS_REV1))

		// On Micropendous boards nCE is PE4 and Address Bit 17 is PE5.  PE0,1,2 are also control signals.
		#define PORTE_EXT_SRAM_SETUP()	DDRE = ((1 << PE0) | (1 << PE1) | (1 << PE2) | (1 << PE4) | (1 << PE5)); PORTE = ((1 << PE0) | (1 << PE1) | (1 << PE2) | (1 << PE4));
		#define ENABLE_EXT_SRAM()	DDRE |= (1 << PE4); PORTE &= ~(1 << PE4);
		#define DISABLE_EXT_SRAM()	DDRE |= (1 << PE4); PORTE |= (1 << PE4);
		#define SELECT_EXT_SRAM_BANK0()	DDRE |= (1 << PE5); PORTE &= ~(1 << PE5);
		#define SELECT_EXT_SRAM_BANK1()	DDRE |= (1 << PE5); PORTE |= (1 << PE5);
		#define CURRENT_SRAM_BANK()	((PINE >> 5) & (0x01))

	#elif ((BOARD == BOARD_MICROPENDOUS_A) || (BOARD == BOARD_MICROPENDOUS_3) || (BOARD == BOARD_MICROPENDOUS_4))

		// On Micropendous3/4/A boards nCE is PE6 and Address Bit 17 is PE7.  PE0,1,2 are also control signals.
		#define PORTE_EXT_SRAM_SETUP()	DDRE = ((1 << PE0) | (1 << PE1) | (1 << PE2) | (1 << PE6) | (1 << PE7)); PORTE = ((1 << PE0) | (1 << PE1) | (1 << PE2) | (1 << PE6));
		#define ENABLE_EXT_SRAM()	DDRE |= (1 << PE6); PORTE &= ~(1 << PE6);
		#define DISABLE_EXT_SRAM()	DDRE |= (1 << PE6); PORTE |= (1 << PE6);
		#define SELECT_EXT_SRAM_BANK0()	DDRE |= (1 << PE7); PORTE &= ~(1 << PE7);
		#define SELECT_EXT_SRAM_BANK1()	DDRE |= (1 << PE7); PORTE |= (1 << PE7);
		#define CURRENT_SRAM_BANK()	((PINE >> 7) & (0x01))

	#else
		// other Micropendous boards do not have external SRAM but NOPs maintain code operation count
		#define PORTE_EXT_SRAM_SETUP()	__asm__ volatile ("NOP" ::)
		#define ENABLE_EXT_SRAM()	__asm__ volatile ("NOP" ::)
		#define DISABLE_EXT_SRAM()	__asm__ volatile ("NOP" ::)
		#define SELECT_EXT_SRAM_BANK0()	__asm__ volatile ("NOP" ::)
		#define SELECT_EXT_SRAM_BANK1()	__asm__ volatile ("NOP" ::)
		#define CURRENT_SRAM_BANK()	__asm__ volatile ("NOP" ::)

	#endif



	/* Disable C linkage for C++ Compilers: */
	#if defined(__cplusplus)
		}
	#endif

#endif // __MICROPENDOUS_DEFINES_H__

