Micropendous2-JTAG is an Open Hardware Development Board for Atmel's [ATmega32U4](http://www.atmel.com/dyn/products/product_card.asp?PN=ATmega32U4) USB AVR Microcontroller and includes a mini 0.05" JTAG header.

The relevant [KiCAD](http://www.lis.inpg.fr/realise_au_lis/kicad/) design files can be found in the latest [Micropendous Distribution](http://code.google.com/p/micropendous/downloads/list) or via [SVN](http://code.google.com/p/micropendous/source/checkout) in the `/Micropendous/Design/Micropendous2-JTAG` directory.

## Pinout ##

![http://micropendous.googlecode.com/svn/trunk/Pictures/Micropendous2-JTAG_Pinout_Picture.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/Micropendous2-JTAG_Pinout_Picture.jpg)

## Features ##
  * Based on the [Atmel](http://www.atmel.com) [ATmega32U4](http://www.atmel.com/dyn/products/product_card.asp?PN=ATmega32U4) USB AVR Microcontroller
  * USB Full Speed (12Mbit/s)
  * USB Device Mode
  * 28kb of available FLASH memory for your programs (4kb used by USB bootloader - stock Atmel or [LUFA](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Firmware/BootloaderDFU))
  * 2.5 kbytes SRAM, 1 kbytes of EEPROM
  * USB powered
  * 8MHz crystal (16MHz version also possible but must run at 5V)
  * 26 General Purpose IO Pins
  * Vcc\_Sel jumper selects 5V or 3.3V operation and powers external devices through Vcc pins (500mA at 5V, 500mA at 3.3V)
  * On-board 1A 3.3V +/- 5% regulator (limited to 500mA by USB standard and IC requirements)
  * to power board externally, remove Vcc\_Sel jumper and apply a 3.3V-5V regulated power supply to VCC
  * Vcc\_Sel should only be moved when board is not powered
  * RESET and HWB buttons to enable firmware loading over USB (no external programmer required)
  * HWB can be used as a user button
  * standard USB Mini-B connector
  * JTAG header for debugging or bootloader update.  All JTAG pins are also accessible on the outer 0.1" headers
  * Size LxWxH (including headers): 1.55" x 0.8" x 0.6" =~ 4cm x 2cm x 1.5cm
  * completely OpenHardware Design, schematic/layout/BillOfMaterials/assembly instructions available for Open Source [KiCAD](http://www.lis.inpg.fr/realise_au_lis/kicad/)
  * works with Open Source [LUFA](http://www.fourwalledcubicle.com/LUFA.php) USB firmware library and Open Source [LibUSB](http://libusb.sourceforge.net) software library, or with Open Source [PySerial](http://pyserial.wiki.sourceforge.net/pySerial) library if using USBtoSerial or CDC based firmware.
  * [Linux](http://www.linux.org/) + [AVR-GCC](http://gcc.gnu.org/) + [LUFA](http://www.fourwalledcubicle.com/LUFA.php) + [LUFA Bootloader](http://www.fourwalledcubicle.com/LUFA.php) + [KiCAD](http://kicad.sourceforge.net) + [DFU-Programmer](http://dfu-programmer.sourceforge.net/) allow you to develop Open Source and/or Open Hardware projects around a Micropendous without ever touching proprietary software.

## QuickNotes ##
  * the Vcc\_Sel jumper should **ALWAYS** be in place.  Board may be damaged if you remove the jumper while the board is powered.
  * the board's 3.3V regulator supplies 3.3V +/- 5% up to 500mA
  * **DO NOT** connect **GND** and **VCC**.  This will short your computer's USB power supply and may damage your computer and your Micropendous2
  * to start the bootloader, plug in your board and press both the RESET and HWB buttons, then release RESET, then HWB
  * Due to the open construction of the board, do not use it on any conductive surface as it may create short circuits
  * all JTAG pins are accessible on the outer 0.1" headers in addition to the [0.05"x0.05" 5x2](http://search.digikey.com/scripts/DkSearch/dksus.dll?Detail&name=S9009E-05-ND) unpopulated internal header
  * the RESET pin is accessible on the outer header to enable programming of the bootloader with an AVR ISP programmer such as [LUFA](http://www.fourwalledcubicle.com/LUFA.php)'s AVRISP firmware
  * PE2 (HWB) is pulled up to Vcc with a 47k resistor to disable the bootloader under normal operation.  This may influence your design decisions and usage of PE2.  Be careful this pin is not Low during RESET/START-UP or the board will enter the bootloader.
  * observe precautions for electrostatic sensitive components while handling the board
  * if your board has a communication error after being plugged in, press RESET
  * you may need to edit the `makefile` before compiling LUFA or Micropendous firmware demos.  The four key variables you need to be mindful of are:
```
MCU = atmega32u4
BOARD  = USER
F_CPU = 8000000
```
  * you can enable pins PF4,5,6,7 by disabling JTAG via software.  Place the following code near the top of `main()`:
```
    /* disable JTAG to allow corresponding pins to be used */
    #if ((defined(__AVR_AT90USB1287__) || defined(__AVR_AT90USB647__) ||  \
            defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB646__) ||  \
            defined(__AVR_ATmega16U4__)  || defined(__AVR_ATmega32U4__) ||  \
            defined(__AVR_ATmega32U6__)))
        // note the JTD bit must be written twice within 4 clock cycles to disable JTAG
        // you must also set the IVSEL bit at the same time, which requires IVCE to be set first
        // port pull-up resistors are enabled - PUD(Pull Up Disable) = 0
        MCUCR = (1 << JTD) | (1 << IVCE) | (0 << PUD);
        MCUCR = (1 << JTD) | (0 << IVSEL) | (0 << IVCE) | (0 << PUD);
    #endif
```