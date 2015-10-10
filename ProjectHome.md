The Micropendous boards were Development Boards for Atmel's USB AVR Microcontrollers with a Free and Open Source Software (FOSS) hardware, firmware, and software development platform.  They were designed for USB peripheral and hosting development.

The [Arduino Leonardo](http://arduino.cc/en/Main/arduinoBoardLeonardo) and its clones are a much cheaper and easier to use FOSS platform for USB Device development based on the USB AVRs.  For applications that require USB Hosting, projects like the [BeagleBoneBlack](http://beagleboard.org/products/beaglebone%20black) or [RaspberryPi](http://www.raspberrypi.org/) are more capable and easier to develop with.

[Micropendous AT90USB1287-based](http://www.ebay.com/sch/nooelec/m.html?_nkw=micropendous) and [Micropendous-32U2](http://www.ebay.com/sch/nooelec/m.html?_nkw=atmega32u2+stick) boards might still be available from [NooElec](http://www.ebay.com/sch/nooelec/m.html?_nkw=micropendous).

The [QuickStart](QuickStart.md) guide may be useful if you are developing custom USB AVR firmware.

![http://micropendous.googlecode.com/svn/trunk/Micropendous/Design/Micropendous/Micropendous-Small_Picture.jpg](http://micropendous.googlecode.com/svn/trunk/Micropendous/Design/Micropendous/Micropendous-Small_Picture.jpg)

The above is a [Micropendous](Micropendous.md) board.  There is also a [variety of earlier designs](ClassicMicropendousBoards.md) based on the USB AVRs.  For basic USB connectivity check out the [Micropendous-32U2](http://code.google.com/p/micropendous/wiki/Micropendous_32U2).

## Features ##
  * Based on the [AT90USB1287](http://www.atmel.com/dyn/products/product_card.asp?PN=AT90USB1287)
  * 120kb available FLASH memory for your programs (8kb used by USB bootloader - stock Atmel or [LUFA](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Firmware/BootloaderDFU))
  * 8 kbytes SRAM and 4 kbytes of EEPROM
  * up to 128 kbytes of [external SRAM](http://www.Micropendous.org/SRAM)
  * USB 2.0 Full Speed (12Mbit/s)
  * USB Device/Host/OTG Modes of Operation
  * USB or externally powered
  * USB-B Jack and USB-A Receptacle selectable with a GPIO pin
  * 16MHz crystal
  * microSD connector
  * PortB is bidirectionally voltage translated through a [TXB0108](http://focus.ti.com/docs/prod/folders/print/txb0108.html) IC
  * [Opennect](http://www.Opennect.org) connector
  * HWB button can be used as a user button in your applications
  * JTAG header
  * completely OpenHardware Design, schematic/layout/BillOfMaterials/assembly instructions available for Open Source [KiCAD](http://www.lis.inpg.fr/realise_au_lis/kicad/).  Gerber and other technical files can be [recreated](http://code.google.com/p/opendous/wiki/KiCADTutorialCreatingGerberFiles).
  * works with Open Source [LUFA](http://www.fourwalledcubicle.com/LUFA.php) USB firmware library and Open Source [LibUSB](http://libusb.sourceforge.net) software library, or with Open Source [PySerial](http://pyserial.wiki.sourceforge.net/pySerial) library if using USBVirtualSerial-based firmware.
  * [Linux](http://www.linux.org/) + [AVR-GCC](http://gcc.gnu.org/) + [LUFA](http://www.fourwalledcubicle.com/LUFA.php) + [LUFA Bootloader](http://www.fourwalledcubicle.com/LUFA.php) + [KiCAD](http://kicad.sourceforge.net) + [DFU-Programmer](http://dfu-programmer.sourceforge.net/) allow you to develop Open Source and/or Open Hardware projects around a Micropendous board without ever touching proprietary software.

## Firmware Examples ##

Can be found in the latest [release](http://code.google.com/p/micropendous/downloads/list) or via [SVN](http://code.google.com/p/micropendous/source/checkout).

All demos are based on [LUFA](http://www.fourwalledcubicle.com/LUFA.php) and other LUFA demos can be [modified](ModifyingLUFADemos.md) to work an appropriate Micropendous boards.

  * [LUFAduino](http://www.Micropendous.org/LUFAduino) - Program your Micropendous board using [Arduino](http://www.arduino.cc/)-style code.  Uses the preemptive [FreeRTOS](http://www.freertos.org/) kernel to allow multi-tasking and the use of delays without interfering with USB functions.
  * [USBVirtualSerial-FreeRTOS](http://www.Micropendous.org/FreeRTOS) - USB Virtual Serial communication example with preemptive [FreeRTOS](http://www.freertos.org/) kernel.
  * [USBVirtualSerial](http://www.Micropendous.org/USBVirtualSerial) - USB Virtual Serial communication example
  * [LoopBack](http://www.Micropendous.org/LoopBack) - Custom-Class USB device communication example
  * [AVRISP](AVRISP.md) - LUFA AVR ISP mkII clone AVR programmer
  * [BootloaderDFU](BootloaderDFU.md) - LUFA Bootloader for the USB AVRs
  * [MassStorage\_MMC\_SD\_via\_SPI](MassStorage_MMC_SD_via_SPI.md) - Mass Storage device demo using MMC or SD card connected in SPI mode
  * [MicropendousKeyboardTest](MicropendousKeyboardTest.md) - Keyboard demo to test pin connectivity.  GND'ing pins types their name.
  * [SpeedTest](SpeedTest.md) - firmware and software to test USB throughput
  * [USBVirtualSerial\_ADC](USBVirtualSerial_ADC.md) - Get the current value of ADC pins using serial port software
  * [USBVirtualSerial\_I2C](USBVirtualSerial_I2C.md) - Virtual Serial Port to [I2C](http://en.wikipedia.org/wiki/I2c) interface
  * [USBVirtualSerial\_LCD](USBVirtualSerial_LCD.md) - Virtual Serial Port to [HD44780 Character LCD](http://en.wikipedia.org/wiki/HD44780_Character_LCD) interface
  * [USBVirtualSerial\_SPI](http://www.Micropendous.org/SPI) - Virtual Serial Port to [SPI](http://en.wikipedia.org/wiki/Serial_Peripheral_Interface_Bus) interface
  * [USBtoSerial](USBtoSerial.md) - Virtual Serial Port to [UART](http://en.wikipedia.org/wiki/Universal_asynchronous_receiver/transmitter) interface
  * [SRAM\_Test](SRAM_Test.md) - Test the external SRAM on a board which supports it.  For SRAM Usage information see [SRAM Usage Tutorial](http://www.Micropendous.org/SRAM).


## External Projects ##
  * [opendous-jtag](http://code.google.com/p/opendous-jtag/) - simple USB-JTAG interface
  * [userial](http://www.tty1.net/userial/) - USB to I2C/SPI/GPIO bridge
  * [uhttpd-avr](http://code.google.com/p/uhttpd-avr/): microHTTPD server based on modified uIP-1.0 stack, running on AVR processor and ENC28J60
  * [lwip-avr](http://code.google.com/p/lwip-avr/): Port of lwip (open source TCP/IP stack) to 8bit AVR MCU
  * [BicycleLEDPOV](http://code.google.com/p/bicycleledpov/): Persistence-of-Vision display for a bicycle wheel.
  * [Duce](http://dorkbotpdx.org/wiki/duce): Serial-Over-USB AVR Programmer
  * [Morse Keyboard](http://code.google.com/p/morsekeyboard/): Connect an iambic keyer to type in Morse Code.  This firmware was designed with an earlier version of [LUFA/MyUSB](http://www.fourwalledcubicle.com/LUFA.php).  It works with the [1.5.3 MyUSB](http://www.fourwalledcubicle.com/files/MyUSB/MyUSB%201.5.3.zip) release.