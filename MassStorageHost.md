The latest version of this firmware is available in the latest [Micropendous Distribution](http://code.google.com/p/micropendous/downloads/list) or via [SVN](http://code.google.com/p/micropendous/source/checkout) in the `/Micropendous/Firmware/USBtoSerial` directory.

**Purpose:** Host [USB Flash drives](http://en.wikipedia.org/wiki/USB_flash_drive).  This demo is slightly modified from the LUFA version.  Only minor alterations were needed to [MassStorageHost.c](http://code.google.com/p/micropendous/source/diff?spec=svn758&r=758&format=side&path=/trunk/Micropendous/Firmware/MassStorageHost/MassStorageHost.c), and [MassStorageHost.h](http://code.google.com/p/micropendous/source/diff?spec=svn758&r=758&format=side&path=/trunk/Micropendous/Firmware/MassStorageHost/MassStorageHost.h), and the [makefile](http://code.google.com/p/micropendous/source/diff?spec=svn758&r=758&format=side&path=/trunk/Micropendous/Firmware/MassStorageHost/makefile) for compatibility with Micropendous boards.  In the `makefile` you really only need to change `BOARD = MICROPENDOUS_REV2` and `F_CPU = 16000000`.

**Source Code:** [View](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Firmware/MassStorageHost/) or [Download](http://www.Micropendous.org/Distribution)

**Targets**: AT90USB1287 or AT90USB647 based hardware with hosting capability.

**USB Mode:** Host-mode

**Status**: _Working_

## Usage ##

Compile and load MassStorageHost firmware onto your USB AVR by following the [ProgramAndTest](ProgramAndTest.md) tutorial.

Plug in a 2 gigabyte or smaller USB Flash drive (the demo won't work with larger flash drives).  To view debug messages, connect a USBtoSerial [USBtoSerial](http://code.google.com/p/micropendous/wiki/USBtoSerial) device as follows:

| **Micropendous**     | **UART Terminal** |
|:---------------------|:------------------|
| **0** / PD2 _(RX)_   | TXD (PD3 on [Micropendous-A](http://code.google.com/p/micropendous/wiki/MicropendousA)) |
| **1** / PD3 _(TX)_   | RXD (PD2 on [Micropendous-A](http://code.google.com/p/micropendous/wiki/MicropendousA)) |
| GND                  | GND               |

If using the Micropendous version of [USBtoSerial](USBtoSerial.md) with flow control you will also need to add Null Modem connections (yellow and green wires below): PD1(RTS)=PD4(CTS), and PD0(DTR)=PD5(DSR)=PD6(DCD).

![http://micropendous.googlecode.com/svn/trunk/Micropendous/Firmware/MassStorageHost/MassStorageHost-Micropendous_with_UART_Debug.jpg](http://micropendous.googlecode.com/svn/trunk/Micropendous/Firmware/MassStorageHost/MassStorageHost-Micropendous_with_UART_Debug.jpg)

With a Serial Terminal connected (9600 baud) you will see the following debug messages.  Refer to the [Serial](http://code.google.com/p/micropendous/wiki/Serial) instructions for more information on setting up a Serial Terminal.

![http://micropendous.googlecode.com/svn/trunk/Micropendous/Firmware/MassStorageHost/MassStorageHost-PuTTY_Debug_Session.jpg](http://micropendous.googlecode.com/svn/trunk/Micropendous/Firmware/MassStorageHost/MassStorageHost-PuTTY_Debug_Session.jpg)