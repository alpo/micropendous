**Current Status**: _2011-11-01_ : The Arduino1 Diskloader Bootloader is being  [ported](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Firmware/Arduino1_Bootloader/) to the AT90USB1287.

## Programming the Arduino1 Bootloader into a Micropendous Board ##

You will need some sort of AVR Programmer.  If you have another USB AVR board such as the [Micropendous\_32U2 ] you can load it with [AVRISP](AVRISP.md) firmware and use it to program your [Micropendous](Micropendous.md) board.  If you already have a programmer then the `avrdude` command should look something like the following.  You may need to change the `-P` _port_ option and the `-c` _programmer_ option.  Note there is a copy of `avrdude` in the `/arduino-1.0-rc2/hardware/tools/avr/bin/` directory.

```
avrdude -vvv -P usb -c avrispmkII -p at90usb1287 -U flash:w:DiskLoader_Micropendous_Rev2_Working.hex
```

To use another USB AVR board for programming, download the latest version of the [LUFA](http://www.lufa-lib.org/) library and follow the [ProgramAndTest](ProgramAndTest.md) instructions to load AVRISP-MKII firmware found in `LUFA/Projects/AVRISP-MKII` onto your board.

Connect your USB AVR board to your Micropendous board according to the [AVRISP](AVRISP.md) instructions.

![http://micropendous.googlecode.com/svn/trunk/Micropendous/Design/Micropendous/Micropendous_Bootloader_Programming.jpg](http://micropendous.googlecode.com/svn/trunk/Micropendous/Design/Micropendous/Micropendous_Bootloader_Programming.jpg)

The following assumes you have prepared your computer for compiling USB AVR code according to the [QuickStart](QuickStart.md) instructions.

Now download the [Arduino1 AT90USB1287 Diskloader](http://code.google.com/p/micropendous/downloads) firmware and program your Micropendous board.

Under Linux you may need to run the `make program` command as root.

![http://micropendous.googlecode.com/svn/trunk/Micropendous/Design/Micropendous/Micropendous_Bootloader_Programming_Linux.jpg](http://micropendous.googlecode.com/svn/trunk/Micropendous/Design/Micropendous/Micropendous_Bootloader_Programming_Linux.jpg)

Under Windows you will need to start a _Command Prompt_ and run `make` then `make program`:

![http://micropendous.googlecode.com/svn/trunk/Micropendous/Design/Micropendous/Micropendous_Bootloader_Programming_Windows.jpg](http://micropendous.googlecode.com/svn/trunk/Micropendous/Design/Micropendous/Micropendous_Bootloader_Programming_Windows.jpg)

## Setting up Arduino1 for use with a Micropendous Board ##

_Coming Soon!_