The latest version of this firmware is available in the latest [Micropendous Distribution](http://code.google.com/p/micropendous/downloads/list) or via [SVN](http://code.google.com/p/micropendous/source/checkout) in the `/Micropendous/Firmware/USBtoSerial` directory.

**Purpose:** Communicate with other electronic devices over the [UART](http://en.wikipedia.org/wiki/Universal_asynchronous_receiver/transmitter) interface.  The LUFA version of [USBtoSerial](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/libs/LUFA/Projects/USBtoSerial/) uses 3-pin UART (TXD, RXD, GND) while the [Micropendous version](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Firmware/USBtoSerial/) implements full flow control.  [Compare the differences.](http://code.google.com/p/micropendous/source/diff?spec=svn759&r=759&format=side&path=/trunk/Micropendous/Firmware/USBtoSerial/USBtoSerial.c)

**Source Code:** [View](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Firmware/USBtoSerial) or [Download](http://www.Micropendous.org/Distribution)

**Targets**: USB AVR based hardware

**USB Mode:** Device-mode enumerating as USB Virtual Serial Port

**Status**: _Working_

## Usage ##

Compile and load USBtoSerial firmware onto your USB AVR by following the [ProgramAndTest](ProgramAndTest.md) tutorial.

Connections to other devices are as follows:

| **Micropendous**      | **UART** |
|:----------------------|:---------|
| **0** - PD2 _(RXD1)_  | RXD      |
| **1** - PD3 _(TXD1)_  | TXD      |
| **2** - PD4 _(CTS)_   | CTS      |
| **4** - PD1 _(RTS)_   | RTS      |
| **3** - PD5 _(DSR)_   | DSR      |
| **5** - PD0 _(DTR)_   | DTR      |
| **6** - PD6 _(DCD)_   | DCD      |
| GND                   | GND      |

## Testing ##

Create a [Null Modem](http://en.wikipedia.org/wiki/Null_modem) loopback by connecting **0**/PD2_(RXD1)_=**1**/PD3_(TXD1)_, **2**/PD4_(CTS)_=**4**/PD1_(RTS)_, and **3**/PD5_(DSR)_=**5**/PD0_(DTR)_=**6**/PD6_(DCD)_.  For the [LUFA](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/libs/LUFA/Projects/USBtoSerial/) 3-pin UART you only need to connect **0**/PD2_(RXD1)_=**1**/PD3 _(TXD1)_.

![http://micropendous.googlecode.com/svn-history/r800/trunk/Micropendous/Firmware/USBtoSerial/USBtoSerial_NullModem_with_Loopback.jpg](http://micropendous.googlecode.com/svn-history/r800/trunk/Micropendous/Firmware/USBtoSerial/USBtoSerial_NullModem_with_Loopback.jpg)

Follow the instructions in [Serial](Serial.md) to connect to the device using a serial terminal and any characters you type will be looped back but only while the loopback wire is in place.