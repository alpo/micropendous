Micropendous1 is an Open Hardware Development Board for Atmel's [AT90USB162](http://www.atmel.com/dyn/products/product_card.asp?PN=AT90USB162) USB AVR Microcontroller.

The relevant [KiCAD](http://www.lis.inpg.fr/realise_au_lis/kicad/) design files can be found in the latest [Micropendous Distribution](http://code.google.com/p/micropendous/downloads/list) or via [SVN](http://code.google.com/p/micropendous/source/checkout) in the `/Micropendous/Design/Micropendous1` directory.  PCBs of this design are available from [BatchPCB](http://batchpcb.com/index.php/Products/44151).

## Pinout ##

![http://micropendous.googlecode.com/svn/trunk/Pictures/Micropendous1_Pinout_Picture.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/Micropendous1_Pinout_Picture.jpg)
![http://micropendous.googlecode.com/svn/trunk/Micropendous/Design/Micropendous1/Micropendous1_Pinout.jpg](http://micropendous.googlecode.com/svn/trunk/Micropendous/Design/Micropendous1/Micropendous1_Pinout.jpg)

## Features ##
  * Based on the [Atmel](http://www.atmel.com) [AT90USB162](http://www.atmel.com/dyn/products/product_card.asp?PN=AT90USB162) USB AVR Microcontroller
  * USB Full Speed (12Mbit/s)
  * USB Device Mode
  * 12kb of available FLASH memory for your programs (4kb used by USB bootloader - stock Atmel or [LUFA](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Firmware/BootloaderDFU))
  * 512 bytes SRAM, 512 bytes of EEPROM
  * USB powered
  * 8MHz crystal (16MHz version also possible but must run at 5V)
  * 21 General Purpose IO Pins
  * Vcc\_Sel jumper selects 5V or 3.3V operation and powers external devices through Vcc pins (500mA at 5V, 50mA at 3.3V)
  * RESET and HWB buttons to enable firmware loading over USB (no external programmer required)
  * HWB can be used as a user button
  * standard USB Mini-B connector
  * Size LxWxH (including headers): 1.25" x 0.8" x 0.6" =~ 3.2cm x 2cm x 1.5cm
  * completely OpenHardware Design, schematic/layout/BillOfMaterials/assembly instructions available for Open Source [KiCAD](http://www.lis.inpg.fr/realise_au_lis/kicad/)
  * works with Open Source [LUFA](http://www.fourwalledcubicle.com/LUFA.php) USB firmware library and Open Source [LibUSB](http://libusb.sourceforge.net) software library, or with Open Source [PySerial](http://pyserial.wiki.sourceforge.net/pySerial) library if using USBtoSerial or CDC based firmware.
  * [Linux](http://www.linux.org/) + [AVR-GCC](http://gcc.gnu.org/) + [LUFA](http://www.fourwalledcubicle.com/LUFA.php) + [LUFA Bootloader](http://www.fourwalledcubicle.com/LUFA.php) + [KiCAD](http://kicad.sourceforge.net) + [DFU-Programmer](http://dfu-programmer.sourceforge.net/) allow you to develop Open Source and/or Open Hardware projects around Micropendous boards without ever touching proprietary software.

## QuickNotes ##
  * the Vcc\_Sel jumper should **ALWAYS** be in place.  Board may be damaged if you remove the jumper while the board is powered.
  * at 3.3V the AT90USB162's internal regulator is used which can supply at most 50mA to external devices
  * **DO NOT** connect **GND** and **VCC**.  This will short your computer's USB power supply and may damage your computer and your Micropendous1
  * to start the bootloader, plug in your board and press both the RESET and HWB buttons, then release RESET, then HWB
  * Due to the open construction of the board, do not use it on any conductive surface as it may create short circuits
  * the RESET pin is accessible on the outer header to enable programming of the bootloader with an AVR ISP programmer such as [LUFA](http://www.fourwalledcubicle.com/LUFA.php)'s AVRISP firmware
  * PD7 (HWB) is pulled up to Vcc with a 47k resistor to disable the bootloader under normal operation.  This may influence your design decisions and usage of PD7.  Be careful this pin is not Low during RESET/START-UP or the board will enter the bootloader.
  * observe precautions for electrostatic sensitive components while handling the board
  * if your board has a communication error after being plugged in, press RESET
  * you may need to edit the `makefile` before compiling LUFA or Micropendous firmware demos.  The three key variables you need to be mindful of are:
```
MCU = at90usb162
BOARD  = USER
F_CPU = 8000000
```