The [KiCAD](http://www.lis.inpg.fr/realise_au_lis/kicad/) design files for this board can be found in the latest [Micropendous Distribution](http://code.google.com/p/micropendous/downloads/list) or via [SVN](http://code.google.com/p/micropendous/source/checkout) in the `/Micropendous/Design/Micropendous-DIP` directory.

A PCB of this design can be purchased at cost through [BatchPCB](http://batchpcb.com/index.php/Products/49248).

![http://micropendous.googlecode.com/svn/trunk/Micropendous/Design/Micropendous-DIP/Micropendous-DIP_Top.jpg](http://micropendous.googlecode.com/svn/trunk/Micropendous/Design/Micropendous-DIP/Micropendous-DIP_Top.jpg)

## Pinout ##

![http://micropendous.googlecode.com/svn/trunk/Micropendous/Design/Micropendous-DIP/Micropendous-DIP_Pinout.jpg](http://micropendous.googlecode.com/svn/trunk/Micropendous/Design/Micropendous-DIP/Micropendous-DIP_Pinout.jpg)

## Features ##

  * Based on the [AT90USB1287](http://www.atmel.com/dyn/products/product_card.asp?PN=AT90USB1287)
  * USB Full Speed (12Mbit/s)
  * USB Device/Host/OTG Modes of operation
  * 120kb available FLASH memory for your programs (8kb used by USB bootloader - stock Atmel or [LUFA](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Firmware/BootloaderDFU))
  * 8 kbytes SRAM and 4 kbytes of EEPROM (AT90USB128)
  * 112 kbytes of [external SRAM](http://www.Micropendous.org/SRAM)
  * [microSD connector in SPI Mode](SPIMicroSD.md)
  * USB powered
  * 16MHz crystal
  * 45 available General Purpose IO Pins (25 in Arduino mode when using [LUFAduino](LUFAduino.md))
  * PortB is [bidirectionally voltage translated](http://focus.ti.com/docs/prod/folders/print/txb0108.html)
  * RESET and HWB buttons to enable firmware loading over USB (no external programmer required)
  * HWB can be used as a user button
  * USB-miniB Jack and USB-A Receptacle selectable with a GPIO pin
  * JTAG header
  * Size LxWxH (including headers): 3.1" x 1" x 1" =~ 8cm x 2.54cm x 2.54cm
  * completely OpenHardware Design, schematic/layout/BillOfMaterials/assembly instructions available for Open Source [KiCAD](http://www.lis.inpg.fr/realise_au_lis/kicad/)
  * works with Open Source [LUFA](http://www.fourwalledcubicle.com/LUFA.php) USB firmware library and Open Source [LibUSB](http://libusb.sourceforge.net) software library, or with Open Source [PySerial](http://pyserial.wiki.sourceforge.net/pySerial) library if using USBtoSerial or CDC based firmware.
  * [Linux](http://www.linux.org/) + [AVR-GCC](http://gcc.gnu.org/) + [LUFA](http://www.fourwalledcubicle.com/LUFA.php) + [LUFA Bootloader](http://www.fourwalledcubicle.com/LUFA.php) + [KiCAD](http://kicad.sourceforge.net) + [DFU-Programmer](http://dfu-programmer.sourceforge.net/) allow you to develop Open Source and/or Open Hardware projects around a Micropendous-DIP board without ever touching proprietary software.

## QuickNotes ##
  * **DO NOT** connect **GND** and **VCC**.  This will short your computer's USB power supply and may damage your computer and your Micropendous-DIP
  * to start the bootloader, plug in your board and press both the RESET and HWB buttons, then release RESET, then release HWB
  * Due to the open construction of the board, do not use it on any conductive surface as it may create short circuits
  * PE2 (HWB) is pulled up to Vcc with a 47k resistor to disable the bootloader under normal operation.  This may influence your design decisions and usage of PE2.  Be careful this pin is not Low during RESET/START-UP or the board will enter the bootloader.
  * observe precautions for electrostatic sensitive components while handling the board
  * if your board has a communication error after being plugged in, press RESET
  * important pins: **PE6** controls TXB0108 `OE` pin, **PE4** is SRAM `nCE`, **PE5** is SRAM address bit 17, and **PE7** controls USB switch with USB-B=`1` and USB-A=`0`
  * [MicropendousDefines.h](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/libs/LUFA/LUFA/Common/MicropendousDefines.h) is automatically included during compilation and includes defines for the USB Switch, Voltage Translator, and External SRAM:
```
  #define ENABLE_VOLTAGE_TXRX
  #define DISABLE_VOLTAGE_TXRX

  #define SELECT_USB_A
  #define SELECT_USB_B

  #define PORTE_EXT_SRAM_SETUP
  #define ENABLE_EXT_SRAM
  #define DISABLE_EXT_SRAM
  #define SELECT_EXT_SRAM_BANK0
  #define SELECT_EXT_SRAM_BANK1
  #define CURRENT_SRAM_BANK
```
  * you will need to edit the `makefile` before compiling LUFA or Micropendous firmware demos.  The three key variables you need to be mindful of are:
```
MCU = at90usb1287
F_CPU = 16000000
BOARD = MICROPENDOUS_REV1
```
  * For hosting applications, you will need to edit the `Config/LUFAConfig.h` by uncommenting `NO_AUTO_VBUS_MANAGEMENT` and `INVERTED_VBUS_ENABLE_LINE` to enable manual control of the USB switch:
```
  #define NO_AUTO_VBUS_MANAGEMENT
  #define INVERTED_VBUS_ENABLE_LINE
```

## Removing your Micropendous-DIP from a Breadboard ##

Be careful when removing your Micropendous-DIP from a breadboard.  Use a mini screwdriver to pry one corner, then another, then another, then start from the first, etc...  It should take you about 20 or so careful corner nudges before a Micropendous-DIP will separate from a breadboard.

![http://micropendous.googlecode.com/svn/trunk/Pictures/Remove_From_Breadboard_Micropendous4.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/Remove_From_Breadboard_Micropendous4.jpg)