Firmware development for the Micropendous is C/C++ and [makefile](http://en.wikipedia.org/wiki/Make_(software)) based.  This is an industry standard development process and allows you to use your favorite code editor and easily add new libraries.

There are several ways to approach firmware development and which is best depends on your experience and requirements.  Developing projects based on USB Virtual Serial Port firmware is the easiest and most versatile option as you can use traditional Serial Port software and techniques.  Choose one of the following to begin.

# [VirtualSerial\_FreeRTOS](VirtualSerial_FreeRTOS.md) #

Program AVR directly and ignore USB functionality.  Send and receive data over the USB Virtual Serial Port using Standard IO functions.

**Advantages**
  * communicate with the board using standard Serial Port software and techniques
  * multi-tasking: several functions can run "simultaneously"

**Disadvantages**
  * slow
  * Timer1 required by underlying code so take care when using
  * requires 2kbytes+ of SRAM


# [VirtualSerial](VirtualSerial.md) #

Program AVR directly and ignore USB functionality.  Send and receive data over the USB Virtual Serial Port using Standard IO functions.

**Advantages**
  * communicate with the board using standard Serial Port software and techniques
  * direct access to AVR architecture
  * fast - throughput about 16kbytes/s on Windows and 40kbytes/s on Linux when echoing characters

**Disadvantages**
  * must be careful not to interfere with USB functions


# [LoopBack](LoopBack.md) #

Program AVR directly and directly control USB communication on both the host and device.

**Advantages**
  * very fast - throughput can approach 1000kbytes/s when transferring bulk data, see [SpeedTest](SpeedTest.md)

**Disadvantages**
  * requires custom host-side software.
  * [LibUSB Setup for Windows](SetupLibUSBWindows.md) is complicated



## Next ##

Back to [QuickStart](QuickStart.md)