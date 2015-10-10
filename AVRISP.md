The latest version of this firmware is available in the latest [Micropendous Distribution](http://code.google.com/p/micropendous/downloads/list) or via [SVN](http://code.google.com/p/micropendous/source/checkout) in the `libs/LUFA/Projects/AVRISP-MKII` directory.

**Purpose:** Emulates [Atmel](http://www.atmel.com)'s [AVRISP mkII In-System Programmer](http://www.atmel.com/tools/avrispmkii.aspx).  Allows in-system programming of "In-System Programmable via SPI Port" AVRs, including another Micropendous board's [DFU](BootloaderDFU.md), [CDC](BootloaderCDC.md), or [MassStorage](BootloaderMassStorage.md) Bootloader.

**Source Code:** [View](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/libs/LUFA/Projects/AVRISP-MKII) or [Download](http://www.Micropendous.org/Distribution).

**Targets**: Any USB AVR based board with accessible SPI pins

**USB Mode:** Device-mode enumerating as Custom-Class USB Device

**Status:** _Working_


## Usage ##

Compile and load the  `AVRISP-mkII` firmware onto your USB AVR by following the [ProgramAndTest](ProgramAndTest.md) tutorial.

Connect your Micropendous board programmed with `AVRISP-mkII` firmware to another AVR using SPI:

| **Micropendous**    | **Target AVR** |
|:--------------------|:---------------|
| `PB4 (10) (nSS)`    | `RESET`        |
| `PB2 (11) (MOSI)`   | `MOSI/PDI`     |
| `PB3 (12) (MISO)`   | `MISO/PDO`     |
| `PB1 (13) (SCK)`    | `SCK`          |
| `GND`               | `GND`+`AGND`   |
| `Vcc`               | `Vcc`+`AVcc`   |

**NOTE** Vcc does not need to be connected if the target is self-powered.

![http://micropendous.googlecode.com/svn/trunk/Pictures/AVRISP_mkII-Usage_Picture.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/AVRISP_mkII-Usage_Picture.jpg)

You can program your target AVR using [AVRdude](http://savannah.nongnu.org/projects/avrdude) or [Atmel Studio](http://www.atmel.com/tools/atmelstudio.aspx).

### Using AVRdude ###

Start a **Command Prompt** under _Windows_ or a **Terminal** under _Linux_.  Change to the directory with the firmware you will upload to the target AVR.  You may need to run the following with `sudo` under _Linux_.  Run the following command:

```
avrdude -vv -P usb -c avrispmkII -p m32
```

which will check the connection to an ATmega32 IC.  To program a file called "FIRMWARE.hex", run the following command:

```
avrdude -vv -P usb -c avrispmkII -p m32 -U flash:w:FIRMWARE.hex
```

Remember to change `m32` to your target AVR's name.  You can list all supported AVRs with the following command:

```
avrdude -c avrispmkII -p a
```

### Using AVRdude to Program the Bootloader of another Micropendous Board ###

![http://micropendous.googlecode.com/svn/trunk/Pictures/AVRISP_mkII-Micropendous_32U2_Programming_Micropendous.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/AVRISP_mkII-Micropendous_32U2_Programming_Micropendous.jpg)

Connect your Micropendous board loaded with `AVRISP-mkII` firmware to your target board.  Change to the `/libs/LUFA/?` bootloader directory.  Edit the `makefile` to create a bootloader for your target AVR by changing `MCU`, `BOARD`, `F_CPU`, `FLASH_SIZE_KB`, and `BOOT_SECTION_SIZE_KB` values as appropriate.

Pictured above is a [Micropendous\_32U2](Micropendous_32U2.md) programming a [Micropendous](Micropendous.md) board.  Note `Vcc` from the `AVRISP-mkII` programmer is not connected to the target since it is self-powered.  The bootloader `makefile` changes are:

```
MCU                   = at90usb1287
BOARD                 = MICROPENDOUS_REV2
F_CPU                 = 16000000
...
FLASH_SIZE_KB         = 128
BOOT_SECTION_SIZE_KB  = 8
```

Run `make` to compile and `avrdude` to program.

```
make
avrdude -vv -P usb -c avrispmkII -p usb1287 -U flash:w:BootloaderNAME.hex
```

Edit `usb1287` above to the name of your USB AVR (`usb82`, `usb162`, `m16u2`, `m32u2`, `m32u4`, `usb646`, `usb647`, `usb1286`, `usb1287`).