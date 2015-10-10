Micropendous-32U2 is an Open Hardware Development Board for Atmel's [ATmega32U2](http://www.atmel.com/dyn/products/product_card.asp?PN=ATmega32U2) USB AVR Microcontroller.

The relevant [KiCAD](http://www.lis.inpg.fr/realise_au_lis/kicad/) design files can be found in the latest [Micropendous Distribution](http://code.google.com/p/micropendous/downloads/list) or via [SVN](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Design/Micropendous-32U2/).  PCBs of this design are available at cost from [BatchPCB](http://batchpcb.com/index.php/Products/55435).  This board is [available for sale](http://stores.ebay.com/NooElec/Atmel-USB-Devices-/_i.html?_fsub=1881869012) from [NooElec](http://stores.ebay.com/NooElec).


![http://micropendous.googlecode.com/svn/trunk/Micropendous/Design/Micropendous-32U2/Micropendous-32U2_Picture.jpg](http://micropendous.googlecode.com/svn/trunk/Micropendous/Design/Micropendous-32U2/Micropendous-32U2_Picture.jpg)

## Pinout ##

![http://micropendous.googlecode.com/svn/trunk/Micropendous/Design/Micropendous-32U2/Micropendous-32U2_Pinout.jpg](http://micropendous.googlecode.com/svn/trunk/Micropendous/Design/Micropendous-32U2/Micropendous-32U2_Pinout.jpg)

## Features ##
  * Based on the [Atmel](http://www.atmel.com) [ATmega32U2](http://www.atmel.com/dyn/products/product_card.asp?PN=ATmega32U2) USB AVR Microcontroller
  * USB Full Speed (12Mbit/s)
  * USB Device Mode
  * 28kb of available FLASH memory for your programs (4kb used by USB bootloader - stock Atmel or [LUFA](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Firmware/BootloaderDFU))
  * 1024 bytes SRAM, 1024 bytes of EEPROM
  * USB powered
  * 16MHz crystal
  * 21 General Purpose IO Pins
  * Vcc\_Sel footprint jumper selects 5V or 3.3V operation and powers external devices through Vcc pins (500mA at 5V, 40mA at 3.3V)
  * RESET and HWB buttons to enable firmware loading over USB (no external programmer required)
  * HWB can be used as a user button
  * Two LEDs: POWER and user-controllable STATUS
  * standard USB Mini-B connector
  * Size LxWxH (including headers): 1.5" x 1" x 0.6" =~ 3.8cm x 2.5cm x 1.5cm
  * completely OpenHardware Design, schematic/layout/BillOfMaterials/assembly instructions available for Open Source [KiCAD](http://www.lis.inpg.fr/realise_au_lis/kicad/)
  * works with Open Source [LUFA](http://www.fourwalledcubicle.com/LUFA.php) USB firmware library and Open Source [LibUSB](http://libusb.sourceforge.net) software library, or with Open Source [PySerial](http://pyserial.wiki.sourceforge.net/pySerial) library if using USBtoSerial or CDC based firmware.
  * [Linux](http://www.linux.org/) + [AVR-GCC](http://gcc.gnu.org/) + [LUFA](http://www.fourwalledcubicle.com/LUFA.php) + [LUFA Bootloader](http://www.fourwalledcubicle.com/LUFA.php) + [KiCAD](http://kicad.sourceforge.net) + [DFU-Programmer](http://dfu-programmer.sourceforge.net/) allow you to develop Open Source and/or Open Hardware projects around Micropendous boards without ever touching proprietary software.

## QuickNotes ##
  * the Vcc\_Sel jumper (a soldered down ferrite bead) should only connect VCC to one of 5V or 3.3V.
  * to power the board externally (regardless whether the board is connected to a USB host through the miniB connector) remove the Vcc\_Sel jumper and provide regulated 4.5V to 5V power to the Vcc pin.
  * at 3.3V the ATmega32U2's internal regulator is used which can supply at most 40mA to external devices
  * **DO NOT** connect **GND** and **VCC**.  This will short your computer's USB power supply and may damage your computer and your Micropendous1
  * to start the bootloader, plug in your board and press both the RESET and HWB buttons, then release RESET, then HWB
  * Due to the open construction of the board, do not use it on any conductive surface as it may create short circuits
  * the RESET pin is accessible on the outer header to enable programming of the bootloader with an AVR ISP programmer such as [LUFA](http://www.fourwalledcubicle.com/LUFA.php)'s [AVRISP](http://code.google.com/p/micropendous/wiki/AVRISP) firmware
  * PD7 (HWB) is pulled up to Vcc with a 47k resistor to disable the bootloader under normal operation.  This may influence your design decisions and usage of PD7.  Be careful this pin is not Low during RESET/START-UP or the board will enter the bootloader.
  * there is one LED connected to PD6 and another to Vcc to show the board's power status.  You will need to remove the power LED for compliance with the USB Specification for standby current consumption.
  * observe precautions for electrostatic sensitive components while handling the board
  * if your board has a communication error after being plugged in, press RESET
  * Due to the use of a 16MHz crystal, at 3.3V you must enable the clock divider  `clock_prescale_set(clock_div_2);`   Note that the USB AVRs are not fully specified for 3.3V operation with a 16MHz crystal so reliability may be reduced.
  * you may need to edit the `makefile` before compiling LUFA or Micropendous firmware demos.  The three key variables you need to be mindful of are:
```
MCU = atmega32u2
BOARD  = USER
F_CPU = 16000000
```