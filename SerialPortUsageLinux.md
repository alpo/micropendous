There is a similar version of this page for [Windows](SerialPortUsageWindows.md).

# Introduction #

Instructions for setting up and communicating with serial port devices in Linux with emphasis on USB Virtual Serial Ports.  Hardware is assumed to be programmed with one of the [LUFA](http://www.fourwalledcubicle.com/LUFA.php) or [Micropendous](http://code.google.com/p/micropendous/downloads/list) USB Virtual Serial firmware demos: [USBVirtualSerial](USBVirtualSerial.md), [USBVirtualSerial\_FreeRTOS](USBVirtualSerial_FreeRTOS.md), or [LUFAduino](LUFAduino.md).

# Table of Contents #

  * Listing Connected Devices
  * Setting Up Serial Port Security
  * Communicating with Serial Devices using gtkterm
  * Writing Communication Software with Python

# Listing Connected Devices #

After plugging in a USB Virtual Serial device, run `dmesg` in a terminal to find the newly connected device's address:

```
[17653.103108] cdc_acm 1-2:1.0: ttyACM0: USB ACM device
```

Run `lsusb` to list connected USB devices and their Vendor and Product IDs:
```
Bus 001 Device 001: ID 03eb:204b Atmel Corp.
```

![http://micropendous.googlecode.com/svn/trunk/Pictures/SerialTutorial_Linux-01_Find_Enumerated_Serial_Port.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/SerialTutorial_Linux-01_Find_Enumerated_Serial_Port.jpg)

From the above, we know that this particular device enumerated as `/dev/ttyACM0` and has Vendor ID `03eb` and Product ID `204b`.

# Setting Up Serial Port Security #

Linux will automatically start using the appropriate drivers when you connect a Virtual Serial Port device.  However, to use those devices in user-space without needing to be **root** or run commands with `sudo` you must set up Serial Port permissions.  The following adds permissions for two LUFA-based firmware examples.  You can add other device by simply copy-and-pasting the lines and modifying the `idVendor` and `idProduct` variables.

Open a new permission file for editing, `sudo gedit /etc/udev/rules.d/98-lufa-virtualserial.rules` and add:
```
SUBSYSTEM=="usb", ACTION=="add", SYSFS{idVendor}=="03eb", SYSFS{idProduct}=="204b", MODE="660", GROUP="plugdev"
BUS=="usb", ACTION=="add", SYSFS{idVendor}=="03eb", SYSFS{idProduct}=="204b", MODE="660", GROUP="plugdev"

SUBSYSTEM=="usb", ACTION=="add", SYSFS{idVendor}=="03eb", SYSFS{idProduct}=="2044", MODE="660", GROUP="plugdev"
BUS=="usb", ACTION=="add", SYSFS{idVendor}=="03eb", SYSFS{idProduct}=="2044", MODE="660", GROUP="plugdev"

SUBSYSTEM=="usb", ACTION=="add", SYSFS{idVendor}=="03eb", SYSFS{idProduct}=="204f", MODE="660", GROUP="plugdev"
BUS=="usb", ACTION=="add", SYSFS{idVendor}=="03eb", SYSFS{idProduct}=="204f", MODE="660", GROUP="plugdev"

SUBSYSTEM=="usb", ACTION=="add", SYSFS{idVendor}=="03eb", SYSFS{idProduct}=="206f", MODE="660", GROUP="plugdev"
BUS=="usb", ACTION=="add", SYSFS{idVendor}=="03eb", SYSFS{idProduct}=="206f", MODE="660", GROUP="plugdev"

SUBSYSTEM=="usb", ACTION=="add", SYSFS{idVendor}=="03eb", SYSFS{idProduct}=="2104", MODE="660", GROUP="plugdev"
BUS=="usb", ACTION=="add", SYSFS{idVendor}=="03eb", SYSFS{idProduct}=="2104", MODE="660", GROUP="plugdev"
```

Save the file.  Permissions have been set up.  You can also add more permissions for devices with different Vendor and Product IDs.

# Communicating with Serial Devices using gtkterm #

![http://micropendous.googlecode.com/svn/trunk/Pictures/SerialTutorial_Linux-01_Find_Enumerated_Serial_Port.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/SerialTutorial_Linux-01_Find_Enumerated_Serial_Port.jpg)

**gtkterm** is already installed on your system if you have followed the instructions in [LinuxQuickStart](LinuxQuickStart.md).

After plugging in your board, run `gtkterm --port /dev/ttyACM0` in a Terminal.  If you are using a board loaded with [USBVirtualSerial](USBVirtualSerial.md), [USBVirtualSerial\_FreeRTOS](USBVirtualSerial_FreeRTOS.md), or [LUFAduino](LUFAduino.md) firmware then anything you type will echo back.

![http://micropendous.googlecode.com/svn/trunk/Pictures/SerialTutorial_Linux-02_gtkterm.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/SerialTutorial_Linux-02_gtkterm.jpg)

You can also manually change port settings from the _Configuration_ -> _Port_ menu

![http://micropendous.googlecode.com/svn/trunk/Pictures/SerialTutorial_Linux-03_gtkterm_Port_Settings.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/SerialTutorial_Linux-03_gtkterm_Port_Settings.jpg)

# Writing Communication Software with Python #

All necessary software (Python and PySerial) is already installed on your system if you have followed the instructions in [LinuxQuickStart](LinuxQuickStart.md).

The following is Python code for basic Serial Port Communication:
```
import serial

ser = serial.Serial('/dev/ttyACM0')

print ser

ser.write('www.Micropendous.org')
print ser.read(20)

ser.write('www.Micropendous.org\n')
print ser.readline()

ser.close()
```

To run the above Python code, save it into a text file with the file extension`.py`.  For example, `SerialPy.py`.  A slightly more complex version with command-line serial port selection is [available](http://micropendous.googlecode.com/svn/trunk/Micropendous/Firmware/USBVirtualSerial/serialpy.py).  You can run the code from a Terminal with `python serialpy.py /dev/ttyACM0`.  If you have not yet set up USB permissions, try `sudo python serialpy.py /dev/ttyACM0`.


Lets go over what the code does.  Python supports [Object-Oriented](http://en.wikipedia.org/wiki/Object_oriented) programming.

`import serial` adds the PySerial library to your Python code.

`ser = serial.Serial('/dev/ttyACM0')` creates a `Serial` object attached to `/dev/ttyACM0`.  This object is named `ser`.  The code for the serial object is in the `serial` library imported on the previous line, hence the `serial.`.

`print ser` just dumps all information regarding the serial port you have attached the `ser` object to.  It is not necessary.

`ser.write('www.Micropendous.org')` sends the characters `www.Micropendous.org` to the `ser` serial port.

`print ser.read(20)`: `ser.read(20)` reads 20 characters from the `ser` serial port and the data is used by the `print` function to print the data to the screen.  Note that `.read()` will block until it receives 20 characters of data so make sure you get this value correct.

`ser.write('www.Micropendous.org\n')` sends the line `www.Micropendous.org\n` to the serial port.  Note the `\n` newline character.

`print ser.readline()` reads a line from the `ser` serial port and prints the data.  Note that `.readline()` will block until it receives a newline character.

`ser.close()` just closes the serial port so that other programs may access it.

### Some useful Python resources ###

[Python Quick Reference](http://rgruet.free.fr/PQR25/PQR2.5.html) is very handy.

[Python Package Index](http://pypi.python.org/pypi) has Open Source Python libraries for just about any task you could think of.

[PySerial Wiki](http://pyserial.sourceforge.net/) has more info on the PySerial library.


## Next ##

Proceed to [Firmware Development](FirmwareDevelopment.md)

Back to [ProgramAndTestLinux](ProgramAndTestLinux.md) or [QuickStart](QuickStart.md)


## Useful Note ##

To avoid typing `gtkterm --port /dev/ttyACM0` each time you want to start the Terminal for your USB Serial device you can edit your [.bashrc](http://tldp.org/LDP/abs/html/sample-bashrc.html) file and add a permanent alias.  Start a Terminal to edit your `.bashrc`

```
sudo gedit $HOME/.bashrc
```

search for "Aliases" and add your own:

```
alias gtkt0='gtkterm --port /dev/ttyACM0'
```

You can now run `gtkterm --port /dev/ttyACM0` from the Terminal with the `gtkt0` command.