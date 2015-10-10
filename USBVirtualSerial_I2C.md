The latest version of this firmware is available in the latest [Micropendous Distribution](http://code.google.com/p/micropendous/downloads/list) or via [SVN](http://code.google.com/p/micropendous/source/checkout) in the `/Micropendous/Firmware/USBVirtualSerial-I2C ` directory.

**Purpose:** Allows you to use software that interacts with a serial port to write and read data over [I2C](http://en.wikipedia.org/wiki/I2C).  This firmware acts as an I2C master.  It uses [avrlib's](http://ccrma.stanford.edu/courses/250a/docs/avrlib/html/index.html) [Software I2C](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/libs/avrlib/i2csw.c).

**Source Code:** [View](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Firmware/USBVirtualSerial-I2C) or [Download](http://www.Micropendous.org/Distribution)

**Targets:** [Micropendous1](Micropendous1.md), [Micropendous2](Micropendous2.md), [Micropendous3](Micropendous3.md), [Micropendous4](Micropendous4.md) or any USB AVR based hardware

**USB Mode:** Device-mode enumerating as USB Virtual Serial Port

**Status:** _Working_.  Please post any problems you encounter to the [Micropendous Group](http://groups.google.com/group/Micropendous).


# Usage #

Compile and load the  AVRISP firmware onto your USB AVR by following the [ProgramAndTest](ProgramAndTest.md) tutorial.

## Pinout ##

Connections between an Micropendous board and an I2C device are as follows:

| **Micropendous** | **I2C** |
|:-----------------|:--------|
| `PD0`            | `SCL`   |
| `PD1`            | `SDA`   |
| `VCC`            | `VCC`   |
| `GND`            | `GND`   |

[Pull-up](http://en.wikipedia.org/wiki/Pull-up_resistor) resistors on SCL and SDA are recommended but not required as the AVR's IOs are [push-pull](http://en.wikipedia.org/wiki/Push%E2%80%93pull) as opposed to the [open drain](http://en.wikipedia.org/wiki/Open_drain) configuration of a standard I2C IO pin.  However, this prohibits techniques such as bus arbitration.  Only I2C Master Mode is supported.

## Protocol ##

The firmware enumerates as an `USB Virutal Serial Port`.  It then waits for packets formatted according to:

Packet: <R/^W , DeviceAddress , Sub-Address/Register , DataLength , DataBytesArray>
Where the bytes are as follows:
  1. byte is read/write byte, 1 is read and 0 is write
  1. byte is the proper non-RW-adjusted device address from its datasheet
  1. byte is the length of data being sent, in number of bytes
  1. byte is length of data expected to be received in number of bytes
  1. byte and later bytes are the data array to send

After processing, the firmware returns a byte which denotes the number of bytes still to be received.  For a write, this is usually 0, for a read it is at least 1 byte; the data read from an I2C device.

Make sure to read each returned byte or flush the serial buffer before continuing.

## Setup and Serial Port Usage ##

[Program](ProgramAndTest.md) a Micropendous board with the USBVirutalSerial-I2C firmware.

See the [Serial](Serial.md) instructions for usage of serial ports before continuing.  You will need to be able to determine where the device enumerates to.


## Testing and Communication Examples ##

Under Windows you will need to have [Python 2.5.5](http://python.org/download/releases/2.5.5/) and [Pyserial](http://downloads.sourceforge.net/pyserial/pyserial-2.4.win32.exe) installed.  Under Linux, these are already installed if you followed the [LinuxQuickStart](LinuxQuickStart.md) tutorial.

The following picture shows an I2C EEPROM and temperature sensor connected to a Micropendous board via I2C.

![http://micropendous.googlecode.com/svn/trunk/Pictures/USBVirtualSerial-I2C_Picture.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/USBVirtualSerial-I2C_Picture.jpg)

Included in the `/Micropendous/Firmware/USBVirtualSerial-I2C/` directory of the [Micropendous Distribution](http://code.google.com/p/micropendous/downloads/list) are three [Python](http://www.Python.org) scripts for communicating with a [24AA08](http://search.digikey.com/scripts/DkSearch/dksus.dll?Detail&name=24AA08-I/P-ND) [EEPROM](http://en.wikipedia.org/wiki/EEPROM), a [TC74](http://search.digikey.com/scripts/DkSearch/dksus.dll?Detail&name=TC74A0-3.3VAT-ND) temperature sensor, and also a script to scan all I2C addresses for devices.  More information about using the scripts can be found by running them with no parameters.
```
python  Test_I2C_EEPROM.py
python  Test_I2C_TC74_TempSense.py
python  Test_I2C_ScanAllAdresses.py
```

You can view the Python example scripts via the following links:

[View Test\_I2C\_EEPROM.py](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Firmware/USBVirtualSerial-I2C/Test_I2C_EEPROM.py)

[View Test\_I2C\_TC74\_TempSense.py](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Firmware/USBVirtualSerial-I2C/Test_I2C_TC74_TempSense.py)

[View Test\_I2C\_ScanAllAdresses.py](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Firmware/USBVirtualSerial-I2C/Test_I2C_ScanAllAdresses.py)