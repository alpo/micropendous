The latest version of this firmware is available in the latest [Micropendous Distribution](http://code.google.com/p/micropendous/downloads/list) or via [SVN](http://code.google.com/p/micropendous/source/checkout) in the `/Micropendous/Firmware/LUFAduino` directory.

**Purpose:**  [Arduino](http://www.arduino.cc/) is a popular physical computing platform with a [C/C++-based](http://en.wikipedia.org/wiki/Arduino) API that simplifies programming of microcontrollers.  LUFAduino combines the [LUFA](http://www.fourwalledcubicle.com/LUFA.php) USB library, the [Arduino](http://www.arduino.cc/) library, and the preemptive [FreeRTOS](http://www.freertos.org/) kernel to enable programming Micropendous boards in Arduino code.  All USB functionality is hidden and USB Virtual Serial behaves like standard Serial on an Arduino board.  The preemptive FreeRTOS kernel allows the use of delays without interfering with USB functionality.  It also allows for multiple `loop()`s and multitasking.

**Source Code:** [View](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Firmware/LUFAduino) or [Download](http://www.Micropendous.org/Distribution)

**Targets:** [Micropendous2](Micropendous2.md), [Micropendous3](Micropendous3.md), or [Micropendous4](Micropendous4.md), or other USB AVR boards with more than 2kbytes of SRAM.

**USB Mode:** Device-mode enumerating as USB Virtual Serial

**Status:** _Under Development_.  Serial, pin definitions, digitalIn/Out, and analogOut are functional.  Please help out by posting any problems you encounter to the [Micropendous Group](http://groups.google.com/group/Micropendous).

# Usage #

![http://micropendous.googlecode.com/svn/trunk/Pictures/Micropendous_LUFAduino_Testing.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/Micropendous_LUFAduino_Testing.jpg)

There is a short video of LUFAduino in action on [YouTube](http://www.youtube.com/watch?v=qIh320eoV0Q).

Assuming you have completed all setup tasks in the [QuickStart](QuickStart.md) tutorial, simply compile and upload the LUFAduino firmware onto your Micropendous using the process found in  [ProgramAndTest](ProgramAndTest.md).

Note: run **make**, then **make flip** _(Windows)_ or **make dfu** _(Linux)_ to compile and load this firmware onto your Micropendous board.  Need to run **make** twice.
```
make
make flip
```

LUFAduino-programmed devices will enumerate as a Virtual Serial Port and any data sent to the device will be echoed back.  Pin 0 (PD0) will flash a LED connected to GND (remember to use a resistor or your LED may burn out).

# Firmware Design #

Arduino `loop()` and `setup()` functions are in `Sketch.cxx`.  Simply edit this file with your custom code using any code editor.  Under Windows, _Programmers Notepad_ comes with _WinAVR_ and is very useful.  See [SettingUpProgrammersNotepad](SettingUpProgrammersNotepad.md) for setup instructions.

`loop()` - sends any data received over the USB Virtual Serial connection back to the host.


`taskLoop(loop2)` - is a second loop created using [duinOS](http://robotgroup.com.ar/duinos/wiki/index.php?title=Main_Page) syntax which flashes a LED connected to Pin 5 (`PD0`) using `analogOut()`.

Simply edit either or both of the `loop()`s for your project.

For detailed firmware design, see [USBVirtualSerial\_FreeRTOS](USBVirtualSerial_FreeRTOS.md) for details of how FreeRTOS was added and [USBVirtualSerial](USBVirtualSerial.md) for details about LUFA USB.