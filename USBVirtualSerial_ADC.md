The latest version of this firmware is available in the latest [Micropendous Distribution](http://code.google.com/p/micropendous/downloads/list) or via [SVN](http://code.google.com/p/micropendous/source/checkout) in the `/Micropendous/Firmware/USBVirtualSerial-ADC` directory.

**Purpose:** USBVirtualSerial-ADC uses the [LUFA](http://www.fourwalledcubicle.com/LUFA.php) USB library to send [ADC](http://en.wikipedia.org/wiki/Analog-to-digital_converter) data to a host system for further processing.

**Source Code:** [View](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Firmware/USBVirtualSerial-ADC) or [Download](http://www.Micropendous.org/Distribution)

**Targets:** [Micropendous2](Micropendous2.md), [Micropendous3](Micropendous3.md), or [Micropendous4](Micropendous4.md), or other USB AVR boards with ADC.

**USB Mode:** Device-class enumerating as USB Virtual Serial Port

**Status:** _Working_.  Please post any problems you encounter to the [Micropendous Group](http://groups.google.com/group/Micropendous).


# QuickStart #

Plug in your Micropendous board [loaded](http://code.google.com/p/micropendous/wiki/ProgramAndTestWindows) with USBVirtualSerial-ADC firmware into a PC.  Connect to it with a serial port terminal.  [Serial instructions](Serial.md) are available.  Then type 0 or 1 to get back the value of ADC channel 0 or 1, respectively.  The first value is the ADC value and the second is the 32-bit value of Timer1.

![http://micropendous.googlecode.com/svn/trunk/Pictures/USBVirtualSerial-ADC_PuTTY-2.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/USBVirtualSerial-ADC_PuTTY-2.jpg)



# Graphical Display of ADC Data #

## Overview ##

First an explanation of the ADC's capabilities.  The ADC clock in USBVirtualSerial-ADC firmware is set up to be the core clock divided by 128.  This ensures compatibility across various boards.  At 8MHz this means the ADC clock runs at 62.5kHz.  The [sampling](http://en.wikipedia.org/wiki/Sample_and_hold) time of each ADC sample is 1.5 ADC clock ticks.  The [analog to digital conversion](http://en.wikipedia.org/wiki/Analog-to-digital_converter) then takes up to 30 ADC clock ticks.  Therefore each sample takes ((30+1.5) `*` 128))/8M = 0.000504 seconds = 504 microseconds for a maximum sampling rate/frequency of 8M/((30+1.5) `*` 128) ~= 2kHz and a [Nyquist frequency](http://en.wikipedia.org/wiki/Nyquist_frequency) of (8M/((30+1.5) `*` 128))/2 ~= 1kHz.  To prevent [aliasing](http://en.wikipedia.org/wiki/Aliasing) you should [low-pass filter](http://en.wikipedia.org/wiki/Low-pass_filter) ADC inputs.  A 0.1uF capacitor will be an adequate filter for most applications.

The not-relative-to-scale graphic below attempts to illustrate how the ADC's capabilities are used in this firmware and software example.  Each sample captures a portion of the signal over 24 microseconds and holds it, then the signal takes 480 microseconds to be digitized before the next sample is gathered.  The software is very basic and reads samples one at a time.  The software then groups samples into frames which are then displayed on a voltage vs. time plot while the next samples are being captured.  All of this limits the effective sampling rate to about 200SPS (samples-per-second) for a 100Hz bandwidth and any signals viewed should be filtered accordingly.  This firmware and software example is not designed to get the most out of the ADC.

![http://micropendous.googlecode.com/svn/trunk/Pictures/USBVirtualSerial-ADC_SPS_FPS_Note.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/USBVirtualSerial-ADC_SPS_FPS_Note.jpg)

The output [impedance](http://en.wikipedia.org/wiki/Electrical_impedance) of the signal source (per [AT90USB1287 datasheet](http://www.atmel.com/dyn/Products/product_card.asp?part_id=3875)) should be 10k Ohm or less.

The vertical scale of the signal is GND to VCC.  You can adjust this in `USBVirtualSerial-ADC.c` by changing the ADC reference from `ADC_REFERENCE_AVCC` which causes VCC to be used as a reference to `ADC_REFERENCE_AREF` which uses the AREF pin or `ADC_REFERENCE_INT2560MV` which uses an internal 2.56V reference.

![http://micropendous.googlecode.com/svn/trunk/Pictures/USBVirtualSerial-ADC_Bandwidth_Scale_Note.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/USBVirtualSerial-ADC_Bandwidth_Scale_Note.jpg)



## Installation ##

To run under Linux install:
```
sudo apt-get install python-matplotlib python-numpy python-wxversion 
```

To run under Windows install:
  * [Python 2.5.5](http://python.org/download/releases/2.5.5/) - _may already be installed_
  * [Pyserial](http://downloads.sourceforge.net/pyserial/pyserial-2.4.win32.exe) - _may already be installed_
  * [wxPython](http://downloads.sourceforge.net/wxpython/wxPython2.8-win32-unicode-2.8.10.1-py25.exe)
  * [matplotlib](http://downloads.sourceforge.net/matplotlib/matplotlib-0.99.1.win32-py2.5.exe)
  * [numpy](http://downloads.sourceforge.net/numpy/numpy-1.3.0-win32-superpack-python2.5.exe)

## Usage ##

In a **Command Prompt** _(Windows)_ or **Terminal** _(Linux)_ change to the `/Micropendous/Firmware/USBVirtualSerial-ADC` directory and run the software with:
```
python PlotADCData.py COM5  128
```

`COM5` is the Serial Port where the device enumerated to.  Under Linux it would be something like `/dev/ttyACM0`.  See **Listing Connected Devices** in [SerialPortUsageWindows](SerialPortUsageWindows.md) or [SerialPortUsageLinux](SerialPortUsageLinux.md) for instructions on determining serial port.

`128` is the number of data points per frame.

To test the ADC a simple voltage divider circuit with a 50k Ohm potentiometer is used.  To avoid shorting VCC and GND, 5.1k Ohm resistors pull the "signal" up to VCC and down to GND.  Turning the potentiometer control quickly produces the following data plot:

![http://micropendous.googlecode.com/svn/trunk/Pictures/USBVirtualSerial-ADC_PotentiometerTest.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/USBVirtualSerial-ADC_PotentiometerTest.jpg)


_Note_: The source `.svg` file for the ADC notes above is [available](http://micropendous.googlecode.com/svn/trunk/Pictures/USBVirtualSerial-ADC_Note.svg) under the [Creative Commons Attribution License](http://creativecommons.org/licenses/by/3.0/)