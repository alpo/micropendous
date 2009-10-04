/*
  Copyright 2009 Dean Camera (dean [at] fourwalledcubicle [dot] com)
  Modified for MMC SD SPI by Opendous Inc. (C) 2009 www.Micropendous.org

  This is LUFA's DataflashManager.c rewritten for MMC and SD
  card SPI mode using avrlib's MMC functions.

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

/*
	TODO - a couple of extra mmcResets are used to fix a fault condition where
				a MMC or SD card will not function on initial power up and require a
				manual RESET of the board.  mmcResets before each read and write
				fix this fault.  Would it not be better to just figure out where to place
				one good reset or startup delay?
*/

/** \file
 *
 *  Functions to manage the physical dataflash media, including reading and writing of
 *  blocks of data. These functions are called by the SCSI layer when data must be stored
 *  or retrieved to/from the physical storage media. If a different media is used (such
 *  as a SD card or EEPROM), functions similar to these will need to be generated.
 */

#define  INCLUDE_FROM_MMC_SD_SPI_C
#include "MMC_SD_SPI.h"

uint8_t buffer[512];	// block buffer


/** Writes blocks (OS blocks, not Dataflash pages) to the storage medium, the board dataflash IC(s), from
 *  the pre-selected data OUT endpoint. This routine reads in OS sized blocks from the endpoint and writes
 *  them to the dataflash in Dataflash page sized blocks.
 *
 *  \param[in] BlockAddress  Data block starting address for the write sequence
 *  \param[in] TotalBlocks   Number of blocks of data to write
 */
void MMC_SD_SPI_WriteBlocks(uint32_t BlockAddress, uint16_t TotalBlocks)
{
//	uint16_t CurrDFPage          = ((BlockAddress * VIRTUAL_MEMORY_BLOCK_SIZE) / 512);
	uint16_t CurrDFPageByte      = ((BlockAddress * VIRTUAL_MEMORY_BLOCK_SIZE) % 512);
	uint8_t  CurrDFPageByteDiv16 = (CurrDFPageByte >> 4);
//	bool     UsingSecondBuffer   = false;
	uint8_t BytesInBlockDiv16 = 0;
	uint16_t  currBlockStartAddr = 0;
	uint16_t i = 0;

	mmcReset();	// TODO - there needs to be an extra reset somewhere

	if (Endpoint_WaitUntilReady()) return; // Wait until endpoint is ready


	for (i = 0; i < TotalBlocks; i++)
	{
		BytesInBlockDiv16 = 0;

		// Write an endpoint packet sized data block to the dataflash
		while (BytesInBlockDiv16 < (VIRTUAL_MEMORY_BLOCK_SIZE >> 4))
		{
			// Check if the endpoint is currently empty
			if (!(Endpoint_IsReadWriteAllowed()))
			{
				// Clear the current endpoint bank
				Endpoint_ClearOUT();

				if (Endpoint_WaitUntilReady()) return;// Wait until endpoint is ready
			}

			currBlockStartAddr = (BytesInBlockDiv16 * 16);

			// Write one 16-byte chunk of data to the dataflash
			buffer[(currBlockStartAddr + 0)] = Endpoint_Read_Byte();
			buffer[(currBlockStartAddr + 1)] = Endpoint_Read_Byte();
			buffer[(currBlockStartAddr + 2)] = Endpoint_Read_Byte();
			buffer[(currBlockStartAddr + 3)] = Endpoint_Read_Byte();
			buffer[(currBlockStartAddr + 4)] = Endpoint_Read_Byte();
			buffer[(currBlockStartAddr + 5)] = Endpoint_Read_Byte();
			buffer[(currBlockStartAddr + 6)] = Endpoint_Read_Byte();
			buffer[(currBlockStartAddr + 7)] = Endpoint_Read_Byte();
			buffer[(currBlockStartAddr + 8)] = Endpoint_Read_Byte();
			buffer[(currBlockStartAddr + 9)] = Endpoint_Read_Byte();
			buffer[(currBlockStartAddr + 10)] = Endpoint_Read_Byte();
			buffer[(currBlockStartAddr + 11)] = Endpoint_Read_Byte();
			buffer[(currBlockStartAddr + 12)] = Endpoint_Read_Byte();
			buffer[(currBlockStartAddr + 13)] = Endpoint_Read_Byte();
			buffer[(currBlockStartAddr + 14)] = Endpoint_Read_Byte();
			buffer[(currBlockStartAddr + 15)] = Endpoint_Read_Byte();

			// Increment the dataflash page 16 byte block counter
			CurrDFPageByteDiv16++;

			// Increment the block 16 byte block counter
			BytesInBlockDiv16++;

			if (IsMassStoreReset) return; // is host aborting current command?
		}

		// write the new buffer to the MMC or SD card in SPI mode
		mmcWrite(BlockAddress, buffer);
		BlockAddress++;
	}

	if (!(Endpoint_IsReadWriteAllowed())) Endpoint_ClearOUT(); // endpoint full, send to host
}

/** Reads blocks (OS blocks, not Dataflash pages) from the storage medium, the board dataflash IC(s), into
 *  the pre-selected data IN endpoint. This routine reads in Dataflash page sized blocks from the Dataflash
 *  and writes them in OS sized blocks to the endpoint.
 *
 *  \param[in] BlockAddress  Data block starting address for the read sequence
 *  \param[in] TotalBlocks   Number of blocks of data to read
 */
void MMC_SD_SPI_ReadBlocks(uint32_t BlockAddress, uint16_t TotalBlocks)
{
//	uint16_t CurrDFPage          = ((BlockAddress * VIRTUAL_MEMORY_BLOCK_SIZE) / 512);
	uint16_t CurrDFPageByte      = ((BlockAddress * VIRTUAL_MEMORY_BLOCK_SIZE) % 512);
	uint8_t  CurrDFPageByteDiv16 = (CurrDFPageByte >> 4);
	uint8_t BytesInBlockDiv16 = 0;
	uint16_t  currBlockStartAddr = 0;
	uint16_t  i = 0;

	mmcReset();	// TODO - there needs to be an extra reset somewhere

	if (Endpoint_WaitUntilReady()) return; // Wait until endpoint is ready


	for (i = 0; i < TotalBlocks; i++)
	{
		mmcRead(BlockAddress, buffer);
		BlockAddress++;
		BytesInBlockDiv16 = 0;

		// Write an endpoint packet sized data block to the dataflash
		while (BytesInBlockDiv16 < (VIRTUAL_MEMORY_BLOCK_SIZE >> 4))
		{
			// Check if the endpoint is currently full
			if (!(Endpoint_IsReadWriteAllowed()))
			{
				// Clear the endpoint bank to send its contents to the host
				Endpoint_ClearIN();

				if (Endpoint_WaitUntilReady()) return; // Wait until endpoint is ready
			}

			currBlockStartAddr = (BytesInBlockDiv16 * 16);

			// Read one 16-byte chunk of data from the dataflash
			Endpoint_Write_Byte(buffer[(currBlockStartAddr + 0)]);
			Endpoint_Write_Byte(buffer[(currBlockStartAddr + 1)]);
			Endpoint_Write_Byte(buffer[(currBlockStartAddr + 2)]);
			Endpoint_Write_Byte(buffer[(currBlockStartAddr + 3)]);
			Endpoint_Write_Byte(buffer[(currBlockStartAddr + 4)]);
			Endpoint_Write_Byte(buffer[(currBlockStartAddr + 5)]);
			Endpoint_Write_Byte(buffer[(currBlockStartAddr + 6)]);
			Endpoint_Write_Byte(buffer[(currBlockStartAddr + 7)]);
			Endpoint_Write_Byte(buffer[(currBlockStartAddr + 8)]);
			Endpoint_Write_Byte(buffer[(currBlockStartAddr + 9)]);
			Endpoint_Write_Byte(buffer[(currBlockStartAddr + 10)]);
			Endpoint_Write_Byte(buffer[(currBlockStartAddr + 11)]);
			Endpoint_Write_Byte(buffer[(currBlockStartAddr + 12)]);
			Endpoint_Write_Byte(buffer[(currBlockStartAddr + 13)]);
			Endpoint_Write_Byte(buffer[(currBlockStartAddr + 14)]);
			Endpoint_Write_Byte(buffer[(currBlockStartAddr + 15)]);

			// Increment the dataflash page 16 byte block counter
			CurrDFPageByteDiv16++;

			// Increment the block 16 byte block counter
			BytesInBlockDiv16++;

			if (IsMassStoreReset) return; // is host aborting current command?
		}
	}

	if (!(Endpoint_IsReadWriteAllowed())) Endpoint_ClearIN(); // endpoint full, send to host
}


void MMC_SD_SPI_ResetDataflashProtections(void)
{
	mmcReset();
}
