The latest version of this firmware is available in the latest [Micropendous Distribution](http://code.google.com/p/micropendous/downloads/list) or via [SVN](http://code.google.com/p/micropendous/source/checkout) in the `/Micropendous/Firmware/USBVirtualSerial-SPI` directory.

**Purpose:** Communicate with electronic devices over the [SPI](http://en.wikipedia.org/wiki/Serial_Peripheral_Interface_Bus) interface.

**Source Code:** [View](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Firmware/USBVirtualSerial-SPI) or [Download](http://www.Micropendous.org/Distribution)

**Targets:** [Micropendous1](Micropendous1.md), [Micropendous2](Micropendous2.md), [Micropendous3](Micropendous3.md), [Micropendous4](Micropendous4.md) or any USB AVR based hardware

**USB Mode:** Device-mode enumerating as USB Virtual Serial Port

**Status:** _Working_

# Usage #

Compile and load the USBVirtualSerial-SPI firmware onto your USB AVR by following the [ProgramAndTest](ProgramAndTest.md) tutorial.

## Pinout ##

| **Micropendous** | **SPI**        |
|:-----------------|:---------------|
| `PB4(^SS)`       | `^CS`          |
| `PB1(SCK)`       | `SCK`          |
| `PB2(MOSI)`      | `MOSI`         |
| `PB3(MISO)`      | `MISO`         |
| `GND`            | `GND`+`AGND`   |
| `Vcc`            | `Vcc`+`AVcc`   |



## Changing Settings ##

USBVirtualSerial-SPI firmware was implemented over standard USBVirtualSerial firmware.  This allows changing [SPI](http://en.wikipedia.org/wiki/Serial_Peripheral_Interface_Bus) parameters by altering the serial port configuration.

Changing the "Line Coding"/"bytesize" will change the SPI mode: `8 bits` = _Mode0_ (Default), `7 bits` = _Mode1_ , `6 bits` = _Mode2_ , `5 bits` = _Mode3_ .  Note that data is still sent in 8-bit bytes over SPI.

Changing "Stop-Bits" determines whether data will be MSB or LSB.  If the default `one stop bit` is selected, data will be transmitted MSB first.  Otherwise LSB.

Changing "Baud-Rate" will change the SPI speed.  Defualt SPI clock speed is 8Mhz / 2 = 4MHz.  8Mhz is the assumed device clock speed.  Serial Port Baud Rate to SPI Clock settings are:
| **Baud Rate** | **SPI Clock**         |
|:--------------|:----------------------|
| 9600          | 125kHz _(Default)_    |
| 14400         | 4MHz                  |
| 19200         | 2MHz                  |
| 38400         | 1MHz                  |
| 57600         | 500kHz                |
| 115200        | 250kHz                |
| All Other     | 62.5kHz               |

Note all these values are doubled if using a 16Mhz clocked USB AVR board.


## Testing ##

### Loopback ###

Testing functionality is very simple.  Connect the `MOSI` (**PB2**) and `MISO` (**PB3**) pins together to form a loopback.  Then plug in your Micropendous, start a serial terminal program such as _gtkterm_ or _HyperTerminal_, and anything you type will echo back.  See [Serial](Serial.md) for instructions on the usage of serial ports.  Or, run the included Python throughput testing script:
```
python  TestThroughputUSBVirtualSerial.py  COM5  64  100
```

![http://micropendous.googlecode.com/svn/trunk/Pictures/SPILoopback_B2B3_Micropendous4.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/SPILoopback_B2B3_Micropendous4.jpg)

### 23K256 ###

![http://micropendous.googlecode.com/svn/trunk/Pictures/USBVirtualSerial-SPI_Picture.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/USBVirtualSerial-SPI_Picture.jpg)

The usage picture above is of a [23K256](http://www.microchip.com/wwwproducts/Devices.aspx?dDocName=en539039) SPI SRAM IC available from [many](http://search.digikey.com/scripts/DkSearch/dksus.dll?Detail&name=23K256-I/P-ND) [suppliers](http://ca.mouser.com/Search/Refine.aspx?Keyword=579-23K256-I/P).  You can test it using the included Python 23K256 testing script (replace COM5 with the port where your USB AVR board [enumerated to](http://www.micropendous.org/Serial)):
```
python  Test_SPI_SRAM_23K256.py  COM5
```

### SPI\_AVRISP\_Test ###

![http://micropendous.googlecode.com/svn/trunk/Pictures/AVRISP_mkII-Usage_Picture.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/AVRISP_mkII-Usage_Picture.jpg)

[SPI\_AVRISP\_Test](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Firmware/SPI_AVRISP_Test) firmware was developed to allow testing [AVRISP](AVRISP.md) and USBVirtualSerial-SPI firmware.  All pictured connections are correct for [AVRISP](AVRISP.md) programming of an [ATtiny261](http://www.atmel.com/dyn/products/product_card_mcu.asp?PN=ATtiny261A) available from [many](http://search.digikey.com/scripts/DkSearch/dksus.dll?Detail&name=ATTINY261-20PU-ND) [suppliers](http://ca.mouser.com/Search/Refine.aspx?Keyword=556-ATTINY261-20PU).

Testing Procedure:
  1. [load](ProgramAndTest.md) your USB AVR based board with [AVRISP](AVRISP.md) firmware
  1. use the resulting USB AVR with AVRISP firmware to [load](AVRISP.md) [SPI\_AVRISP\_Test](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Firmware/SPI_AVRISP_Test) onto an ATtiny261 or similar AVR (edit `makefile` `MCU` target appropriately if using a different AVR)
  1. [load](ProgramAndTest.md) your USB AVR based board with [USBVirtualSerial\_SPI](USBVirtualSerial_SPI.md) firmware
  1. keep all connections as in the picture but remove the ^CS/PB4/RESET wire as SPI\_AVRISP\_Test uses 3-wire SPI
  1. Reset everything and test using the included Python testing script in `/SPI_AVRISP_Test` (replace COM5 with the port where your USB AVR board [enumerated to](http://www.micropendous.org/Serial)):
```
python  Test_SPI_AVRISP_Test_Communication.py  COM5
```
The output should be a couple iterations of the current timer value on the ATtiny261.

## Python Usage ##

Here is a simple example for using USBVirtualSerial-SPI in Python.  The character `a` will be sent over SPI with the `ser.write()` command.  Note any 8-byte value can be sent in any compatible format, like `\af` for hex data.  Note that each call to `ser.write()` will cause the device to _both_ write data over SPI and read data back simultaneously.  Therefore each call to write should be accompanied by a call to read to avoid having buffering errors.  Note that sending two 8-bit bytes of data will appear over SPI as a 16-bit value.  More Python serial usage information is available in [Serial](Serial.md).

```
import serial           # for accessing the serial port on multiple platforms

ser = serial.Serial('COM3')  # open COM3 on Windows
# under Linux, find this value with dmesg look for something like  cdc_acm 2-1:1.0: ttyACM0: USB ACM device
#ser = serial.Serial('/dev/ttyACM0')

print ser               # dump all info regarding serial port being used

ser.write('a')          # write this character to the serial port as an 8-bit value
print ser.read()        # read the above data back, assuming loopback

ser.write('aa')         # write these characters to the serial port as a 16-bit value
print ser.read(2)       # read the above data back, assuming loopback

ser.close()             # release/close the serial port
```