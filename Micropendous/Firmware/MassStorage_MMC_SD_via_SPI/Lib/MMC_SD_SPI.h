/*
             LUFA Library
     Copyright (C) Dean Camera, 2009.
              
  dean [at] fourwalledcubicle [dot] com
      www.fourwalledcubicle.com
*/

/*
  Copyright 2009  Dean Camera (dean [at] fourwalledcubicle [dot] com)

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
 *  Header file for DataflashManager.c.
 */
 
#ifndef _MMC_SD_SPI_H
#define _MMC_SD_SPI_H

	/* Includes: */
		#include <avr/io.h>

		#include "MassStorage_MMC_SD_via_SPI.h"
		#include "Descriptors.h"

		#include "global.h"
		#include "mmcconf.h"
		#include <avrlib/mmc.h>
		#include <avrlib/spi.h>

		#include <LUFA/Common/Common.h>              // Function Attribute, Atomic, Debug and ISR Macros
		#include <LUFA/Drivers/USB/USB.h>            // USB Functionality

	/* Defines: */
		/** Total number of bytes of the storage medium, comprised of one or more dataflash ICs. */
		#define VIRTUAL_MEMORY_BYTES                ((uint32_t)DATAFLASH_PAGES * 512 * DATAFLASH_TOTALCHIPS)

		/** Block size of the device. This is kept at 512 to remain compatible with the OS despite the underlying
		 *  storage media (Dataflash) using a different native block size.
		 */
		#define VIRTUAL_MEMORY_BLOCK_SIZE           512
		
		/** Total number of blocks of the virtual memory for reporting to the host as the device's total capacity. */
		#define VIRTUAL_MEMORY_BLOCKS               (VIRTUAL_MEMORY_BYTES / VIRTUAL_MEMORY_BLOCK_SIZE)
		
	/* Function Prototypes: */
		void MMC_SD_SPI_ReadBlocks(USB_ClassInfo_MS_Device_t* MSInterfaceInfo, const uint32_t BlockAddress, uint16_t TotalBlocks);
		void MMC_SD_SPI_WriteBlocks(USB_ClassInfo_MS_Device_t* MSInterfaceInfo, const uint32_t BlockAddress, uint16_t TotalBlocks);
		uint8_t MMC_SD_SPI_ResetProtections(void);
		bool MMC_SD_SPI_CheckOperation(void);

#endif //_MMC_SD_SPI_H
