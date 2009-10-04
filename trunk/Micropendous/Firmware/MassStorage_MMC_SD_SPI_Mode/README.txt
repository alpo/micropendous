- works with any MMC or SD card which has SPI mode (all but a handful of microSD cards support SPI)

- requires one manual setting before compilation and programming
    - format your memory card using a dedicated memory card reader
    - figure out the number of available bytes - in Windows, right-click on the drive and click Properties
    - note the memory card's Capacity (total available bytes)
        - for example, my FAT32 formatted 1GB microSD card has 996732928 bytes available
    - divide by 512 to get the number of pages: (996732928 / 512) = 1946744
    - open MassStorage_MMC_SD_SPI_Mode.h and change #define DATAFLASH_PAGES to the above number of pages

- that is it, your Micropendous programmed with MassStorage_MMC_SD_SPI_Mode firmware will now behave like a
  slow USB storage device.  Allow a minute or so for your system to recognize the card.