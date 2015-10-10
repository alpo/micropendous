The [KiCAD](http://www.lis.inpg.fr/realise_au_lis/kicad/) design files for this board can be found in the [SVN](http://code.google.com/p/micropendous/source/checkout) repository under the `/Micropendous/Design/Micropendous` [directory](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Design/Micropendous/).

Assembled boards are now [available from NooElec](http://www.ebay.com/sch/nooelec/m.html?_nkw=micropendous)!

A blank PCB of this design can be purchased at cost through [BatchPCB](http://batchpcb.com/index.php/Products/67008).

![http://micropendous.googlecode.com/svn/trunk/Micropendous/Design/Micropendous/Micropendous-Small_Picture.jpg](http://micropendous.googlecode.com/svn/trunk/Micropendous/Design/Micropendous/Micropendous-Small_Picture.jpg)

## Pinout ##

![http://micropendous.googlecode.com/svn/trunk/Micropendous/Design/Micropendous/Micropendous-Pinout.jpg](http://micropendous.googlecode.com/svn/trunk/Micropendous/Design/Micropendous/Micropendous-Pinout.jpg)

## Features ##

  * Based on the [AT90USB1287](http://www.atmel.com/dyn/products/product_card.asp?PN=AT90USB1287)
  * USB Full Speed (12Mbit/s)
  * USB Device and Host Modes of operation
  * 120kb available FLASH memory for your programs (8kb used by USB bootloader - stock Atmel or [LUFA](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Firmware/BootloaderDFU))
  * 8 kbytes internal SRAM and 4 kbytes of internal EEPROM
  * up to 128 kbytes of [external SRAM](http://www.Micropendous.org/SRAM)
  * [microSD connector in SPI Mode](SPIMicroSD.md)
  * USB powered or DC powered
  * DC Power Jack, 2.1mm ID, Center-Positive, 5.5V to 16V Input (6V optimal)
  * 16MHz crystal
  * 25 available General Purpose IO Pins in an [Arduino](http://www.Arduino.cc) configuration
  * [Opennect-MCU](http://www.Opennect.org) connector for small and cheap expansion modules
  * PortB is [bidirectionally voltage translated](http://focus.ti.com/docs/prod/folders/print/txb0108.html) to communicate with microSD or external peripherals
  * RESET and HWB buttons to enable firmware loading over USB (no external programmer required)
  * HWB can be used as a user button
  * USB-B Jack and USB-A Receptacle selectable with a GPIO pin
  * PortB, PortD, and PortB\_3.3V available through [STK500-style](http://www.atmel.com/dyn/products/tools_card.asp?tool_id=2735) 10-pin headers
  * JTAG header
  * Size LxWxH (including headers): 3" x 2.1" x 0.8" =~ 8cm x 5.3cm x 2cm
  * completely OpenHardware Design, schematic/layout/BillOfMaterials/assembly instructions [available](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Design/Micropendous/) for Open Source [KiCAD](http://www.lis.inpg.fr/realise_au_lis/kicad/)
  * works with Open Source [LUFA](http://www.fourwalledcubicle.com/LUFA.php) USB firmware library and Open Source [LibUSB](http://libusb.sourceforge.net) software library, or with Open Source [PySerial](http://pyserial.wiki.sourceforge.net/pySerial) library if using USBtoSerial or CDC based firmware.
  * [Linux](http://www.linux.org/) + [AVR-GCC](http://gcc.gnu.org/) + [LUFA](http://www.fourwalledcubicle.com/LUFA.php) + [LUFA Bootloader](http://www.fourwalledcubicle.com/LUFA.php) + [KiCAD](http://kicad.sourceforge.net) + [DFU-Programmer](http://dfu-programmer.sourceforge.net/) allow you to develop Open Source and/or Open Hardware projects around a Micropendous-DIP board without ever touching proprietary software.

## QuickNotes ##
  * **DO NOT** connect **GND** and **VCC**.  This will short your computer's USB power supply and may damage your computer and your Micropendous board
  * to start the bootloader, plug in your board and press both the RESET and HWB buttons, then release RESET, then release HWB
  * Due to the open construction of the board, do not use it on any conductive surface as it may create short circuits
  * PE2 (HWB) is pulled up to Vcc with a 47k resistor to disable the bootloader under normal operation.  This may influence your design decisions and usage of PE2.  Be careful this pin is not Low during RESET/START-UP or the board will enter the bootloader.
  * observe precautions for electrostatic sensitive components while handling the board
  * if your board has a communication error after being plugged in, press RESET
  * important pins: **PE3** controls TXB0108 `OE` pin, **PE6** is USB-A over-current flag, **PE4** is SRAM `nCE`, **PE5** is SRAM address bit 17, and **PE7** controls USB switch with USB-B=`0` and USB-A=`1`.
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
BOARD = MICROPENDOUS_REV2
```

## Power\_Rails ##

The 5V regulator used is a [MIC39100](http://www.micrel.com/page.do?page=/product-info/products/mic39100.shtml) capable of generating 1A at 5V from a 6V wall-wart such as the [Volgen KTPS24-06035MP](http://search.digikey.com/us/en/products/KTPS24-06035MP/62-1114-ND) or [CUI EMSA060300-P5P-SZ](http://search.digikey.com/us/en/products/EMSA060300-P5P-SZ/T1089-P5P-ND).  The USB-B connector bulk is used for heat dissipation as well as a significantly sized GND fill underneath the IC.  Refer to the [datasheet](http://www.micrel.com/_PDF/mic39100.pdf) to derate output current at higher input voltages or higher operation temperatures.

The 3.3V regulator is a [SOT-89-5 XC6222B331](http://www.torex.co.jp/english/products/detail.php?id=385) and its input power comes from the 5V rail.  Keep this in mind when estimating 5V power consumption.  It can source up to 500mA at 3.3V at 25C.  Refer to Pg.24 of the [XC6222 Datasheet](http://www.torex.co.jp/english/products/voltage_regulators/data/XC6222.pdf) for temperature derating curves.  The regulator is attached to approximately 1000sq.mm of 1oz copper.