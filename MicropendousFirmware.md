This firmware is no longer being actively developed.  You can find it in the [Micropendous-2009-10-11 Distribution](http://micropendous.googlecode.com/files/Micropendous-2009-10-11.zip) or via [SVN](http://code.google.com/p/micropendous/source/checkout) in the `/Micropendous/Firmware/Micropendous` directory.

**Purpose:** Combine [LUFAduino](LUFAduino.md) with [MicropendousKeyboardTest](MicropendousKeyboardTest.md) to allow both Serial communication and Keyboard testing of a Micropendous board.

**Source Code:** [View](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Firmware/Micropendous) or [Download](http://www.Micropendous.org/Distribution)

**Targets:** [Micropendous2](Micropendous2.md), [Micropendous3](Micropendous3.md), [Micropendous4](Micropendous4.md) or any USB AVR based hardware with 2kb+ of SRAM

**USB Mode:** Device-mode enumerating as Compound USB Device -  USB Virtual Serial Port and HID Keyboard

**Status:** _Discontinued_.  Works under Windows, fails under Linux.


## Usage ##

Compile and load the Micropendous firmware onto your USB AVR by following the [ProgramAndTest](ProgramAndTest.md) tutorial.

Just as with [MicropendousKeyboardTest](MicropendousKeyboardTest.md), GND'ing pins will type their name.  Also, you can use the techniques in [Serial](Serial.md) to communicate with the board, which is programmed to loop back any data.

## Firmware Design ##

[LUFAduino](LUFAduino.md) and [MicropendousKeyboardTest](MicropendousKeyboardTest.md) mashed together.