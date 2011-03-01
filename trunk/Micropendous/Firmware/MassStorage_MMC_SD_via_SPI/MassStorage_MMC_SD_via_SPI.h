/*
             LUFA Library
     Copyright (C) Dean Camera, 2010.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2010  Dean Camera (dean [at] fourwalledcubicle [dot] com)

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
 *  Header file for MassStorage.c.
 */

#ifndef _MASS_STORAGE_MMC_SD_VIA_SPI_H_
#define _MASS_STORAGE_MMC_SD_VIA_SPI_H_

	/* Includes: */
		#include <avr/io.h>
		#include <avr/wdt.h>
		#include <avr/power.h>
		#include <avr/interrupt.h>
		#include <string.h>

		#include "Descriptors.h"

		#include "Lib/SCSI.h"
		#include "Lib/MMC_SD_SPI.h"

		#include <LUFA/Version.h>
		#include <LUFA/Drivers/Board/LEDs.h>
		#include <LUFA/Drivers/USB/USB.h>

	/* Macros: */
		#define DATAFLASH_PAGE_SIZE								512
		#define DATAFLASH_TOTALCHIPS								1
		// 2GB drive (1969483776 bytes - FAT32 formatted):					3846648
		//#define DATAFLASH_PAGES									3846648	// 2GB card
		// for 1GB drive (998424576 bytes - FAT formatted):
		//#define DATAFLASH_PAGES								1950048	// 1GB card
		// for 1GB drive (996732928 bytes - FAT formatted):
		//#define DATAFLASH_PAGES								1946744	// 1GB card
		// for 2GB drive (996732928 bytes - FAT formatted):
		//#define DATAFLASH_PAGES								3853632	// 2GB card
		// for 1GB drive (1017331712 - FAT formatted):
		#define DATAFLASH_PAGES								1986976	// 1GB card
		// TODO - change the above defines for your MMC/SD card

		// the following quite the preprocessor as there is Dataflash code around
		#define DF_CMD_BUFF1WRITE			0
		#define DF_CMD_BUFF2TOMAINMEMWITHERASE		0
		#define DF_CMD_BUFF1TOMAINMEMWITHERASE		0
		#define DF_CMD_MAINMEMPAGEREAD			0
		#define DATAFLASH_CHIP1				0
		#define DF_CMD_GETSTATUS			0
		#define DF_STATUS_SECTORPROTECTION_ON		0




		/** LED mask for the library LED driver, to indicate that the USB interface is not ready. */
		#define LEDMASK_USB_NOTREADY      LEDS_LED1

		/** LED mask for the library LED driver, to indicate that the USB interface is enumerating. */
		#define LEDMASK_USB_ENUMERATING  (LEDS_LED2 | LEDS_LED3)

		/** LED mask for the library LED driver, to indicate that the USB interface is ready. */
		#define LEDMASK_USB_READY        (LEDS_LED2 | LEDS_LED4)

		/** LED mask for the library LED driver, to indicate that an error has occurred in the USB interface. */
		#define LEDMASK_USB_ERROR        (LEDS_LED1 | LEDS_LED3)

		/** LED mask for the library LED driver, to indicate that the USB interface is busy. */
		#define LEDMASK_USB_BUSY          LEDS_LED2

		/** Total number of logical drives within the device - must be non-zero. */
		#define TOTAL_LUNS                1

		/** Blocks in each LUN, calculated from the total capacity divided by the total number of Logical Units in the device. */
		#define LUN_MEDIA_BLOCKS         (VIRTUAL_MEMORY_BLOCKS / TOTAL_LUNS)

	/* Function Prototypes: */
		void SetupHardware(void);

		void EVENT_USB_Device_Connect(void);
		void EVENT_USB_Device_Disconnect(void);
		void EVENT_USB_Device_ConfigurationChanged(void);
		void EVENT_USB_Device_ControlRequest(void);

		bool CALLBACK_MS_Device_SCSICommandReceived(USB_ClassInfo_MS_Device_t* const MSInterfaceInfo);

#endif // end _MASS_STORAGE_MMC_SD_VIA_SPI_H_
