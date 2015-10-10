The latest version of this firmware is available in the latest [Micropendous Distribution](http://code.google.com/p/micropendous/downloads/list) or via [SVN](http://code.google.com/p/micropendous/source/checkout) in the `/Micropendous/Firmware/SRAM_Test` directory.

Instructions for using the external SRAM are [available](http://www.micropendous.org/SRAM).

**Purpose:** SRAM\_Test uses the [LUFA](http://www.fourwalledcubicle.com/LUFA.php) USB library to send random bytes of data to a [Micropendous4](http://www.micropendous.org/Micropendous4) board's SRAM then checks to make sure it is read back correctly.  The firmware also runs internal SRAM tests.

**Source Code:** [View](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Firmware/SRAM_Test) or [Download](http://www.Micropendous.org/Distribution)

**Targets:** [Micropendous](Micropendous.md), [Micropendous-DIP](MicropendousDIP.md), [Micropendous3](Micropendous3.md), or [Micropendous4](Micropendous4.md) with 128kbytes of external SRAM

**USB Mode:** Device-mode enumerating as USB Virtual Serial Port

**Status:** _Working_.  Please post any problems you encounter to the [Micropendous Group](http://groups.google.com/group/Micropendous).

## Usage ##

Compile and load the SRAM\_Test firmware onto your USB AVR by following the [ProgramAndTest](ProgramAndTest.md) tutorial.

Reset your board and find out which Serial Port it enumerated to using the instructions in [Serial](Serial.md).

Run the Python test program with `python TestSRAM.py COM5 0` where `COM5` is the serial port name and `0` is the maximum number of bytes to test (`0` means test them all).  For Linux usage refer to [SerialPortUsageLinux](SerialPortUsageLinux.md).


## Firmware Design ##

This is just [USBVirtualSerial](USBVirtualSerial.md) with a command interface for reading and writing to the external SRAM.  More info on how the SRAM is accessed is [available](http://www.micropendous.org/SRAM).  SRAM is accessed via the _"Slightly Complex"_ array method.

The `TestEXTSRAM()` function also performs pattern tests on the external SRAM.