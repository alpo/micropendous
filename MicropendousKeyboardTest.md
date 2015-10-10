The latest version of this firmware is available in the latest [Micropendous Distribution](http://code.google.com/p/micropendous/downloads/list) or via [SVN](http://code.google.com/p/micropendous/source/checkout) in the `/Micropendous/Firmware/ MicropendousKeyboardTest` directory.

**Purpose:** Test pin connectivity or use as a keyboard that sends custom text depending on which pin is GND'ed.

**Source Code:** [View](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Firmware/MicropendousKeyboardTest) or [Download](http://www.Micropendous.org/Distribution)

**Targets:** [Micropendous1](Micropendous1.md), [Micropendous2](Micropendous2.md), [Micropendous3](Micropendous3.md), [Micropendous4](Micropendous4.md) or any USB AVR based hardware

**USB Mode:** Device-mode enumerating as HID (Human Interface Device) Keyboard

**Status:** _Working_


## Usage ##

Compile and load the MicropendousKeyboardTest firmware onto your USB AVR by following the [ProgramAndTest](ProgramAndTest.md) tutorial.

Plug your board into a computer and it should enumerate as a keyboard.  Connecting the GND pin to any other pin will type that pin's name.  For example, pressing the HWB button will type `e2` as HWB is connected to pin PE2.

## Firmware Design ##

Copied from the [LUFA](http://www.fourwalledcubicle.com/LUFA.php) project's Keyboard demo with code to test which pin is currently being GND'ed and send its name.

Some setup code in `main()` to make all relevant pins input pins:
```
  DDRD = 0;
  PORTD = 0xFF; //input
  ... and etc...
```

Then a mass of code in `CreateKeyboardReport()` to test which pin is GND'ed:
```
} else if (~PINB & (1 << PINB1)) {
  ReportData->KeyCode[0] = 0x05; //b
  ReportData->KeyCode[1] = 0x1E; //1
  ... and etc...
}
```

The `ReportData->KeyCode[1]` array can store up to 6 characters.  However, most Operating Systems ignore rapidly typed characters so each adjacent character in the array must be different.