Micropendous4 is an Open Hardware Development Board for Atmel's [AT90USB1287](http://www.atmel.com/dyn/products/product_card.asp?PN=AT90USB1287) USB AVR Microcontroller.  It has both a USB-miniB connector and a USB-A connector and an on-board USB switch to switch the active connector.  It also includes 128kbytes of external SRAM.

The relevant [KiCAD](http://www.lis.inpg.fr/realise_au_lis/kicad/) design files can be found in the latest [Micropendous Distribution](http://code.google.com/p/micropendous/downloads/list) or via [SVN](http://code.google.com/p/micropendous/source/checkout) in the `/Micropendous/Design/Micropendous4` directory.

## Pinout ##

![http://micropendous.googlecode.com/svn/trunk/Pictures/Micropendous4_Pinout_Picture.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/Micropendous4_Pinout_Picture.jpg)

## Features ##
  * Based on the [Atmel](http://www.atmel.com) [AT90USB1287](http://www.atmel.com/dyn/products/product_card.asp?PN=AT90USB1287) USB AVR Microcontroller
  * USB Full Speed (12Mbit/s)
  * USB Device/Host/OTG Modes are supported
  * 120kb of available FLASH memory for your programs (8kb used by USB bootloader - stock Atmel or [LUFA](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Firmware/BootloaderDFU))
  * 8 kbytes SRAM, 4 kbytes of EEPROM, 128kbytes external SRAM ([112kbytes easily usable](SRAMUsage.md))
  * USB powered
  * 16MHz crystal (8MHz version also possible)
  * 47 General Purpose IO Pins (PE6 must be dedicated to SRAM)
  * RESET and HWB buttons to enable firmware loading over USB (no external programmer required)
  * HWB can be used as a user button for your applications
  * standard USB Mini-B and USB-A connectors
  * active USB connector can be switched
  * JTAG header for debugging or bootloader update and all JTAG pins except RESET are also accessible on the outer 0.1" headers
  * Size LxWxH (including headers): 2.84" x 0.8" x 0.6" =~ 7.2cm x 2cm x 1.5cm
  * completely OpenHardware Design, schematic/layout/BillOfMaterials/assembly instructions available for Open Source [KiCAD](http://www.lis.inpg.fr/realise_au_lis/kicad/)
  * works with Open Source [LUFA](http://www.fourwalledcubicle.com/LUFA.php) USB firmware library and Open Source [LibUSB](http://libusb.sourceforge.net) software library, or with Open Source [PySerial](http://pyserial.wiki.sourceforge.net/pySerial) library if using USBtoSerial or CDC based firmware.
  * [Linux](http://www.linux.org/) + [AVR-GCC](http://gcc.gnu.org/) + [LUFA](http://www.fourwalledcubicle.com/LUFA.php) + [LUFA Bootloader](http://www.fourwalledcubicle.com/LUFA.php) + [KiCAD](http://kicad.sourceforge.net) + [DFU-Programmer](http://dfu-programmer.sourceforge.net/) allow you to develop Open Source and/or Open Hardware projects around a Micropendous4 board without ever touching proprietary software.

## QuickNotes ##
  * **DO NOT** connect **GND** and **VCC**.  This will short your computer's USB power supply and may damage your computer and your Micropendous4
  * to start the bootloader, plug in your board and press both the RESET and HWB buttons, then release RESET, then HWB
  * If you are not using the external SRAM, disable it by setting PE6 high.
  * Leaving the USB\_CONN\_Select pin open or setting it to logic High will enable the USB-miniB connector.  Setting it to logic Low or connecting it to the adjacent GND pin will enable the USB-A connector.  See "Switching Active Connector" notes below.
  * Due to the open construction of the board, do not use it on any conductive surface as it may create short circuits
  * PE2 (HWB) is pulled up to Vcc with a 47k resistor to disable the bootloader under normal operation.  This may influence your design decisions and usage of PE2.  Be careful this pin is not Low during RESET/START-UP or the board will enter the bootloader.
  * observe precautions for electrostatic sensitive components while handling the board
  * if your board has a communication error after being plugged in, press RESET

  * you can enable pins PF4,5,6,7 by disabling JTAG via software. Place `JTAG_DISABLE();` near the top of your `main()` function.
  * important pins: **PE6** is SRAM `nCE`, **PE7** is SRAM address bit 17, and the USB switch has an external control pin with USB-A=`0` and USB-miniB=`1`/unconnected.
  * [MicropendousDefines.h](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/libs/LUFA/LUFA/Common/MicropendousDefines.h) is automatically included during compilation and includes defines for the USB Switch, Voltage Translator, and External SRAM:
```
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
BOARD = MICROPENDOUS_A
```

## Switching Active USB Connector ##

The board has two USB connectors selectable with the **USB-B/^A** header, USB-miniB (device mode) and USB-A (host mode).

The **USB-B/^A** header has two pins, one `GND`, and one connected to the USB switch control.  If the USB switch control is left open or set to logic **HIGH**, the USB-miniB connector is active (left picture below).  If it is `GND`ed or set to logic **LOW**, the USB-A connector is active (right picture below).  Both connectors share GND and VBUS.

![http://micropendous.googlecode.com/svn/trunk/Pictures/Connector_Switch_Micropendous4.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/Connector_Switch_Micropendous4.jpg)

## Enabling OTG ##

On-The-Go (OTG) is a USB mode where connected devices negotiate which will be the host and which will be the device.  For example, a photo printer might host a camera but will become a device when connected to a PC.  To enable OTG mode the ENG resistor must be mounted.  This resistor connects the miniB USB connector's _ID_ pin to pin PE3 of the USB AVR.  Note that mounting this resistor will mean you cannot use pin PE3 in your application.  The ENH resistor is a standard 0805 resistor and can be any value between 0 and 100 Ohm.  You can also just short the two pads with some wire or solder.  Be careful not to solder to the USB-A connector's shield.

![http://micropendous.googlecode.com/svn/trunk/Pictures/Enable_OTG_Micropendous4.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/Enable_OTG_Micropendous4.jpg)

## Removing your Micropendous4 from a Breadboard ##

Be careful when removing your Micropendous4 from a Breadboard.  Be careful not to damage the SRAM IC.  Use a mini screwdriver to pry one corner, then another, then another, then start from the first, etc...  It should take you about 20 or so careful corner nudges before the Micropendous4 will separate from a breadboard.

![http://micropendous.googlecode.com/svn/trunk/Pictures/Remove_From_Breadboard_Micropendous4.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/Remove_From_Breadboard_Micropendous4.jpg)