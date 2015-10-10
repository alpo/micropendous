There is a similar version of this page for [Linux](SerialPortUsageLinux.md).

# Introduction #

Instructions for setting up and communicating with serial port devices in Windows with emphasis on USB Virtual Serial Ports.  Hardware is assumed to be programmed with [LUFA](http://www.fourwalledcubicle.com/LUFA.php) or [Micropendous](http://code.google.com/p/micropendous/downloads/list) USB Virtual Serial firmware.  Firmware upload is explained in detail in [ProgramAndTestWindows](ProgramAndTestWindows.md).

One useful link is [UninstallingDriversInWindows](http://code.google.com/p/micropendous/wiki/UninstallingDriversInWindows) if you ever need to un-install drivers in Windows.

# Table of Contents #

  * Installing Drivers
  * Listing Connected Devices
  * Communicating with Serial Devices using PuTTY
  * Communicating with Serial Devices using HyperTerminal
  * Writing Serial Port Communication Software with Python

# Installing Drivers #

You may need to be logged in as Administrator to perform the following.

Plug in your Micropendous board [loaded](ProgramAndTestWindows.md) with [USBVirtualSerial](USBVirtualSerial.md)-based firmware.  Windows' _New Hardware Wizard_ should appear.  You should proceed by telling Windows to install its own Virtual Serial port drivers.  This is done using a specially crafted `.inf` file which simply tells Windows which USB device should use which Windows drivers.  An `.inf` file specific to the device firmware you are using is in the associated directory.  For example, the `.inf` file for [USBVirtualSerial](USBVirtualSerial.md) firmware is in the `/Micropendous/Firmware/USBVirtualSerial` directory of the latest Micropendous [release](http://code.google.com/p/micropendous/downloads/list).

  1. Once your device is plugged in Windows should notice your device and ask for drivers.
> > ![http://micropendous.googlecode.com/svn/trunk/Pictures/SerialInstallTutorial_Windows-01.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/SerialInstallTutorial_Windows-01.jpg)
  1. DO NOT let Windows search as it will not find anything.
> > ![http://micropendous.googlecode.com/svn/trunk/Pictures/SerialInstallTutorial_Windows-02.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/SerialInstallTutorial_Windows-02.jpg)
  1. Select _Install from a specific location_
> > ![http://micropendous.googlecode.com/svn/trunk/Pictures/SerialInstallTutorial_Windows-03.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/SerialInstallTutorial_Windows-03.jpg)
  1. Find the `.inf` file for your device.  As mentioned, for [USBVirtualSerial](USBVirtualSerial.md) it would be in the `/Micropendous/Firmware/USBVirtualSerial` directory.
> > ![http://micropendous.googlecode.com/svn/trunk/Pictures/SerialInstallTutorial_Windows-04.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/SerialInstallTutorial_Windows-04.jpg)
  1. Windows should proceed to install drivers.  Note that the `.inf` file does not contain any code.  You are installing Windows' own drivers.  You are forcing Windows to communicate with [USBVirtualSerial](USBVirtualSerial.md)-loaded Micropendous boards using standard USB CDC class communication.
> > ![http://micropendous.googlecode.com/svn/trunk/Pictures/SerialInstallTutorial_Windows-05.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/SerialInstallTutorial_Windows-05.jpg)
  1. If everything is successful unplug and then plug in your device again (or RESET the device) and proceed to find out what Windows has decided to call it.
> > ![http://micropendous.googlecode.com/svn/trunk/Pictures/SerialInstallTutorial_Windows-06.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/SerialInstallTutorial_Windows-06.jpg)
  1. Restart Windows.
> > ![http://micropendous.googlecode.com/svn/trunk/Pictures/WindowsDriverInstallTutorial_FoundNewHardware.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/WindowsDriverInstallTutorial_FoundNewHardware.jpg)


# Listing Connected Devices #

After enumeration (USB term meaning _"connect and establish communication with"_) Windows will assign to your USB Virtual Serial device a serial port - **COM?**.


Right-Click on **My Computer**, then click **Properties**, then the **Hardware** tab, then **Device Manager**, then find **USB Virtual Serial Port** under "Ports (COM & LPT)".  Note that on this system it has enumerated as `COM7`.

![http://micropendous.googlecode.com/svn/trunk/Pictures/USBtoSerial_Windows_DeviceManager.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/USBtoSerial_Windows_DeviceManager.jpg)

# Communicating with Serial Devices using PuTTY #

Communication with Serial Port devices is done either using a Serial Terminal or device specific software.  For Serial Terminal communication [PuTTY](http://www.chiark.greenend.org.uk/~sgtatham/putty/) is recommended as it is a single executable, full-featured, open source, and easy to use.  Start PuTTY and click the **Serial** button, then enter `COM7` or whatever you noted above in the _"Serial Line"_ field.  That's it.  Press **Open**.  Note that the _Speed_ setting is irrelevant as communication with a USB Virtual Serial device is done at USB speed (12Mbit/s for Full Speed devices such as Micropendous boards).

![http://micropendous.googlecode.com/svn/trunk/Pictures/PuTTY_USBtoSerial_Setup.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/PuTTY_USBtoSerial_Setup.jpg)

You should now see a blank screen.

![http://micropendous.googlecode.com/svn/trunk/Pictures/PuTTY_BlankScreen.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/PuTTY_BlankScreen.jpg)

If you have connected a device running [USBVirtualSerial](USBVirtualSerial.md) firmware then anything you type will be echoed back to you.

![http://micropendous.googlecode.com/svn/trunk/Pictures/PuTTY_USBtoSerial_Test.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/PuTTY_USBtoSerial_Test.jpg)


# Communicating with Serial Devices using HyperTerminal #

If you do not want to install any new software you can use Windows' built-in HyperTerminal.

  1. Start HyperTerminal.  It should be under _Programs_ -> _Accessories_ -> _Communications_ -> HyperTerminal:
> > ![http://micropendous.googlecode.com/svn/trunk/Pictures/USBVirtualSerial-HyperTerminal-1.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/USBVirtualSerial-HyperTerminal-1.jpg)
  1. Give the connection you are about to establish a name and picture:
> > ![http://micropendous.googlecode.com/svn/trunk/Pictures/USBVirtualSerial-HyperTerminal-2.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/USBVirtualSerial-HyperTerminal-2.jpg)
  1. Select the appropriate **COM** port which you noted earlier in the _Listing Connected Devices_ section:
> > ![http://micropendous.googlecode.com/svn/trunk/Pictures/USBVirtualSerial-HyperTerminal-3.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/USBVirtualSerial-HyperTerminal-3.jpg)
  1. For a connected device running [USBVirtualSerial](USBVirtualSerial.md) firmware these settings are irrelevant.  Just click OK.
> > ![http://micropendous.googlecode.com/svn/trunk/Pictures/USBVirtualSerial-HyperTerminal-4.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/USBVirtualSerial-HyperTerminal-4.jpg)
  1. [USBVirtualSerial](USBVirtualSerial.md) firmware will echo back any characters you type.  Some versions may also print the current Timer1 value.
> > ![http://micropendous.googlecode.com/svn/trunk/Pictures/USBVirtualSerial-HyperTerminal-6.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/USBVirtualSerial-HyperTerminal-6.jpg)


# Writing Communication Software with Python #

Simply install [Python 2.5.5](http://www.python.org/download/releases/2.5.5/) and [PySerial 2.5](http://sourceforge.net/projects/pyserial/files/).

The following is Python code for basic Serial Port Communication:
```
import serial

ser = serial.Serial('COM7')

print ser

ser.write('www.Micropendous.org')
print ser.read(20)

ser.write('www.Micropendous.org\n')
print ser.readline()

ser.close()
```

To run the above Python code, save it into a text file with the file extension`.py`.  For example `SerialPy.py`.  A slightly more complex version with command-line COM port selection is [available](http://micropendous.googlecode.com/svn/trunk/Micropendous/Firmware/USBVirtualSerial/serialpy.py).  You can run the code from a _Command Prompt_ with `python serialpy.py  COM7`.

Lets go over what the code does.  Python supports [Object-Oriented](http://en.wikipedia.org/wiki/Object_oriented) programming.

`import serial` adds the PySerial library to your Python code.

`ser = serial.Serial('COM7')` creates a `Serial` object attached to the COM7 port.  This object is named `ser`.  The code for the serial object is in the `serial` library imported on the previous line, hence the `serial.`.

`print ser` just dumps all information regarding the serial port you have attached the `ser` object to.  It is not necessary but useful for debugging.

`ser.write('www.Micropendous.org')` sends the characters `www.Micropendous.org` to the `ser` serial port.

`print ser.read(20)`: `ser.read(20)` reads 20 characters from the `ser` serial port and the data is used by the `print` function to print the data to the screen.  Note that `.read()` will block until is receives 20 characters of data so make sure you get this value correct.

`ser.write('www.Micropendous.org\n')` sends the line `www.Micropendous.org\n` to the serial port.  Note the `\n` newline character.

`print ser.readline()` reads a line from the `ser` serial port and prints the data.  Note that `.readline()` will block until it receives a newline character.

`ser.close()` closes the serial port so that other programs may access it.


### Some useful Python resources ###

[Python Quick Reference](http://rgruet.free.fr/PQR25/PQR2.5.html) is very handy.

[Python Package Index](http://pypi.python.org/pypi) has Open Source Python libraries for just about any task you could think of.

[PySerial Wiki](http://pyserial.sourceforge.net/) has more info on the PySerial library.


## Next ##

Proceed to [Firmware Development](FirmwareDevelopment.md)

Back to [ProgramAndTestWindows](ProgramAndTestWindows.md) or [QuickStart](QuickStart.md)