Micropendous-A is an Open Hardware Development Board for Atmel's 64-pin [AT90USB1287](http://www.atmel.com/dyn/products/product_card.asp?PN=AT90USB1287) USB AVR Microcontroller.

The Micropendous-A is pin and function-compatible with the [AT90USB1287 Micropendous3](http://code.google.com/p/micropendous/wiki/Micropendous3).

The Micropendous-A is unfortunately too expensive to produce at a competitive price.  If you are interested in batches of these boards (40+) you may contact me through the [Micropendous Group](http://groups.google.com/group/Micropendous) for information on an assembler with experience producing these boards.

The relevant [KiCAD](http://www.lis.inpg.fr/realise_au_lis/kicad/) design files can be found in [SVN](http://code.google.com/p/micropendous/source/checkout) in the `/Micropendous/Design/Micropendous-A` [directory](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Design/Micropendous-A/).

![http://micropendous.googlecode.com/svn/trunk/Micropendous/Design/Micropendous-A/Micropendous-A_USB_Plug_Connector_Picture-Top-Small.jpg](http://micropendous.googlecode.com/svn/trunk/Micropendous/Design/Micropendous-A/Micropendous-A_USB_Plug_Connector_Picture-Top-Small.jpg)

## Pinout ##

![http://micropendous.googlecode.com/svn/trunk/Micropendous/Design/Micropendous-A/Micropendous-A_Pinout.jpg](http://micropendous.googlecode.com/svn/trunk/Micropendous/Design/Micropendous-A/Micropendous-A_Pinout.jpg)


## Features ##

  * Based on the [AT90USB1287](http://www.atmel.com/dyn/products/product_card.asp?PN=AT90USB1287)
  * USB Full Speed (12Mbit/s)
  * USB Device and/or Host Modes supported
  * 120kb of available FLASH memory for your programs (8kb used by USB bootloader - stock Atmel or [LUFA](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Firmware/BootloaderDFU))
  * On-chip 8 kbytes SRAM and 4 kbytes of EEPROM
  * optional [128kbytes External SRAM](http://www.Micropendous.org/SRAM)
  * USB or externally powered and can be [modified for 3.3V IO](http://code.google.com/p/micropendous/wiki/MicropendousA#Modifying_the_board_for_3.3V_IO)
  * 16MHz crystal
  * 48 General Purpose IO Pins operating at 5V (up to 47 with [External SRAM](http://www.Micropendous.org/SRAM))
  * RESET and HWB buttons to enable firmware loading over USB (no external programmer required)
  * HWB can be used as a user button
  * USB-A Plug or Receptacle
  * JTAG ([Header](http://search.digikey.com/scripts/DkSearch/dksus.dll?Detail&name=609-3243-ND) DIL 0.1" 5x2) for debugging or bootloader update
  * Size LxWxH (including headers): 3.2" x 0.8" x 0.6" =~ 8cm x 2cm x 1.5cm
  * completely OpenHardware Design, schematic/layout/BillOfMaterials/assembly instructions [available](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Design/Micropendous-A/) for Open Source [KiCAD](http://www.lis.inpg.fr/realise_au_lis/kicad/)
  * works with Open Source [LUFA](http://www.fourwalledcubicle.com/LUFA.php) USB firmware library and Open Source [LibUSB](http://libusb.sourceforge.net) software library, or with Open Source [PySerial](http://pyserial.wiki.sourceforge.net/pySerial) library if using USBtoSerial or CDC based firmware.
  * [Linux](http://www.linux.org/) + [AVR-GCC](http://gcc.gnu.org/) + [LUFA](http://www.fourwalledcubicle.com/LUFA.php) + [LUFA Bootloader](http://www.fourwalledcubicle.com/LUFA.php) + [KiCAD](http://kicad.sourceforge.net) + [DFU-Programmer](http://dfu-programmer.sourceforge.net/) allow you to develop Open Source and/or Open Hardware projects around a Micropendous-A without ever touching proprietary software.

## QuickNotes ##
  * **DO NOT** connect **GND** and **VCC**.  This will short your computer's USB power supply and may damage your computer and your Micropendous-A
  * to start the bootloader, plug in your board and press both the RESET and HWB buttons, then release RESET, then HWB
  * Due to the open construction of the board, do not use it on any conductive surface as it may create short circuits
  * PE2 (HWB) is pulled up to Vcc with a 43k resistor to disable the bootloader under normal operation.  This may influence your design decisions and usage of PE2.  Be careful this pin is not Low during RESET/START-UP or the board will enter the bootloader.
  * to power the board externally when USB Hosting, simply provide regulated 4.5V-5.5V power to the board through the VBUS or VCC pins (VBUS=VCC)
  * to power the board externally at other IO voltages, simply cut the VBUS=VCC trace jumper on the underside of the board and provide regulated 3V-5.5V power to the board through the VCC pin and regulated 4.5V-5.5V power to the board through the VBUS pin.  The VBUS pin is always connected to the USB connector power rail so do not provide VBUS if the board is connected to a USB Host.  Only connect power to the VBUS pin when the board is the USB Host.  Note that operation at 16MHz and/or use of the external SRAM requires 4.5V-5.5V VCC.
  * observe precautions for electrostatic sensitive components while handling the board
  * if your board has a communication error after being plugged in, press RESET
  * you can enable pins PF4,5,6,7 by disabling JTAG via software. Place `JTAG_DISABLE();` near the top of your `main()` function.
  * you will need to edit the `makefile` before compiling LUFA or Micropendous firmware demos.  The three key variables you need to be mindful of are:
```
MCU = at90usb1287
F_CPU = 16000000
BOARD = MICROPENDOUS_A
```

## Power Supply Design ##

The power supply follows _Figure 21-3_ of the [Datasheet](http://www.atmel.com/dyn/resources/prod_documents/doc7593.pdf) by default and _Figure 21-4_ with the `VBUS=VCC` `JP1` trace cut and the board powered externally through the `VCC` pin.  When USB Hosting 5V must also be supplied to the VBUS pin.

![http://micropendous.googlecode.com/svn/trunk/Micropendous/Design/Micropendous-A/Micropendous-A_Power_Schematic.jpg](http://micropendous.googlecode.com/svn/trunk/Micropendous/Design/Micropendous-A/Micropendous-A_Power_Schematic.jpg)

## Powering the board I/O externally ##

A Micropendous-A board can have its I/O externally powered through its VCC pin (regardless whether it is connected to a USB Host) after removing the VCC=VBUS trace jumper on the underside of the board.  This disconnects VBUS from VCC to allow powering the board with an external regulated 3.1V to 5V power supply through the VCC pin.

To allow USB communication 5V power must still be available on the VBUS pin.  Either by connecting the board to a host or connecting a regulated 5V power supply to the VBUS pin.

![http://micropendous.googlecode.com/svn/trunk/Micropendous/Design/Micropendous3/Micropendous3_ExternalPower.jpg](http://micropendous.googlecode.com/svn/trunk/Micropendous/Design/Micropendous3/Micropendous3_ExternalPower.jpg)

## Modifying the board for 3.3V IO ##

Use a [rotary tool](http://en.wikipedia.org/wiki/Rotary_tool) with an [engraving cutter](http://www.dremel.com/en-us/Accessories/Pages/ProductDetail.aspx?pid=107) to remove the PCB trace jumper (**JP1**) linking VBUS to VCC on the underside of the board.  Then solder a [XC6222 SOT-235](http://search.digikey.com/us/en/products/XC6222B331MR-G/893-1143-1-ND) or similar 3.3V voltage regulator into the unpopulated footprint.  For a list of compatible regulators refer to the [Bill of Materials](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Design/Micropendous-A/Micropendous-A_BillOfMaterials.txt).

![http://micropendous.googlecode.com/svn/trunk/Micropendous/Design/Micropendous-A/Micropendous-A_3.3V_Conversion.jpg](http://micropendous.googlecode.com/svn/trunk/Micropendous/Design/Micropendous-A/Micropendous-A_3.3V_Conversion.jpg)

The board can now be powered over USB or the VBUS pin (5V) and the I/O voltage will be 3.3V.  Up to about 150mA is available at 3.3V from the VCC pin.  Refer to Pg.23 of the [XC6222 Datasheet](http://www.torex.co.jp/english/products/pro02/pdf021/61-XC6222.pdf) for temperature derating curves.  The regulator is attached to approximately 800sq.mm of 1oz copper.

If you decide to go back to 5V operation you will need to [desolder](http://search.digikey.com/us/en/products/SMD1NL/SMD1NL-ND) the 3.3V regulator and install a [2-pin 2mm header](http://search.digikey.com/us/en/products/951102-8622-AR/3M9321-ND) and [2mm jumper](http://search.digikey.com/us/en/products/SPN02SYBN-RC/S3404-ND).

## Hosting USB Devices ##

The board needs to be powered externally with a regulated 5V power supply through the VBUS pin to allow USB Hosting:

![http://micropendous.googlecode.com/svn/trunk/Micropendous/Design/Micropendous3/Micropendous3_BluetoothHosting.jpg](http://micropendous.googlecode.com/svn/trunk/Micropendous/Design/Micropendous3/Micropendous3_BluetoothHosting.jpg)

An USB A-A Adapter [such as this](http://search.digikey.com/us/en/products/A-USB-4-R/AE1474-ND) is needed if your Micropendous-A has a USB-A Plug connector.

## Removing your Micropendous-A from a Breadboard ##

Be careful when removing your Micropendous-A from a Breadboard.  Use a mini screwdriver to pry one corner, then another, then another, then start from the first, etc...  It should take you about 20 or so careful corner nudges before the Micropendous-A will separate from a breadboard.  Be careful not to damage the SRAM IC.

![http://micropendous.googlecode.com/svn/trunk/Pictures/Remove_From_Breadboard_Micropendous4.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/Remove_From_Breadboard_Micropendous4.jpg)