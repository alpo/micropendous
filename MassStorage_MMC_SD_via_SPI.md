The latest version of this firmware is available in the [Micropendous-2011-03-01.zip  Distribution](http://code.google.com/p/micropendous/downloads/list) ~~or via [SVN](http://code.google.com/p/micropendous/source/checkout) in the `/Micropendous/Firmware/MassStorage_MMC_SD_via_SPI` directory~~.

**Purpose:** Use your USB AVR to interface to a MMC or SD card.  More likely this firmware will be used to store data onto a MMC or SD card while the board is disconnected from a host and then use the USB Mass Storage interface to read the data from a PC.  For example, a sensor logger.

**Source Code:** [View](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Firmware/MassStorage_MMC_SD_via_SPI) or [Download](http://www.Micropendous.org/Distribution)

**Targets:** USB AVR with 3.3V IO and 1kb+ SRAM connected to a MMC/SD Card via SPI

**USB Mode:** Device-mode enumerating as Mass Storage device

**Status:** _Working_


## Pinout ##

Make sure your USB AVR is running at 3.3V or that you have 3.3V buffers on all the signal lines to the MMC/SD card (such as [74LVC245](http://search.digikey.com/us/en/products/SN74LVC245AN/296-8503-5-ND/)).

| **Micropendous** | **MMC/SD** |
|:-----------------|:-----------|
| `PB0-CS`         | `1-CS`     |
| `PB1-CLK`        | `5-CLK`    |
| `PB2-MOSI`       | `2-CMD/DI` |
| `PB3-MISO`       | `7-DAT0/DO` |
| `3V3`            | `4-VDD`    |
| `GND`            | `3-VSS1`   |
| `GND`            | `6-VSS2`   |


## Usage ##

Current version of this firmware must be recompiled for each new MMC or SD card you plan to use.  Need to set the number of blocks.  Format your MMC or SD card using a standard Card Reader then note the number of bytes under the device's properties.  Divide the number by 512 to get the number of pages, then open `MassStorage_MMC_SD_SPI_Mode.h` and change `#define DATAFLASH_PAGES  1986976` to the number of pages for your specific card.

Now compile and load the MassStorage\_MMC\_SD\_via\_SPI firmware onto your USB AVR by following the [ProgramAndTest](ProgramAndTest.md) tutorial.


## Firmware Design ##

This firmware is almost an exact copy of [LUFA](http://www.fourwalledcubicle.com/LUFA.php)'s MassStorage demo with the DataFLASH code replaced by MMC and SD SPI mode code.  `DataflashManager.c/.h` is replaced with `MMC_SD_SPI.c/.h`.  MMC and SD SPI Mode functionality is taken from [AVRlib](http://ccrma.stanford.edu/courses/250a/docs/avrlib/html/index.html)'s [mmc.c](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/libs/avrlib/mmc.c).