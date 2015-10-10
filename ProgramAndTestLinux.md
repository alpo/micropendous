A version of this page for Windows is also [available](ProgramAndTestWindows.md).

**Table of Contents**



## Loading Firmware ##

  * This tutorial assumes you have already completed [LinuxQuickStart](LinuxQuickStart.md) installation steps.

  * Download and extract the latest [Micropendous Distribution](http://code.google.com/p/micropendous/downloads/list)

  * Plug in your Micropendous board and enter bootloader mode by pressing the RESET and HWB buttons, then release the RESET button, then release the HWB button.  HWB stands for _Hardware Bootloader_ and is connected to pin PE2 on Micropendous boards (except on the [Micropendous1](Micropendous1.md) board where it is connected to PD7).

> ![http://micropendous.googlecode.com/svn/trunk/Pictures/Enter_Bootloader.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/Enter_Bootloader.jpg)

  * open a terminal and run `lsusb` to show connected devices
> ![http://micropendous.googlecode.com/svn/trunk/Pictures/LinuxQuickStart_lsusb.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/LinuxQuickStart_lsusb.jpg)
```
Bus 002 Device 039: ID 03eb:2ff4 Atmel Corp.
```
> The above is the code for the [Atmega32U4](http://www.atmel.com/dyn/products/product_card.asp?PN=ATmega32U4) on the [Micropendous2](Micropendous2.md).  If something similar does not show up, try entering the bootloader again or using a different USB cable or USB port.

  * change to the directory of the firmware you wish to upload.  In the case of testing, this would be MicropendousKeyboardTest in the `/Micropendous/Firmware/MicropendousKeyboardTest`

  * now edit the `makefile` for your current USB AVR.  **gedit** is a simple text editor with code editing capability included with Ubuntu Linux.

> ![http://micropendous.googlecode.com/svn/trunk/Pictures/LinuxQuickStart_change_directory.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/LinuxQuickStart_change_directory.jpg)

  * edit the key settings based on which USB AVR you are using: `MCU`, `F_CPU`, and `BOARD`.  For example, a [Micropendous2](Micropendous2.md) board is based on the [Atmega32U4](http://www.atmel.com/dyn/products/product_card.asp?PN=ATmega32U4) and has an 8MHz clock.  `BOARD=USER` simply includes general purpose drivers that don't do anything in order to allow compilation and loading of various [LUFA-Lib](http://www.LUFA-lib.org) and Micropendous demos.
> ![http://micropendous.googlecode.com/svn/trunk/Pictures/LinuxQuickStart_gedit.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/LinuxQuickStart_gedit.jpg)
```
MCU = atmega32u4
F_CPU = 8000000
BOARD  = USER
```

  * now compile and load the firmware.  If you get errors, try running `make clean` before `make dfu`.  If you get a "Device not present" error, try running `sudo make dfu`.

> ![http://micropendous.googlecode.com/svn/trunk/Pictures/LinuxQuickStart_compile_load.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/LinuxQuickStart_compile_load.jpg)

> The above is a successful firmware upload.  Depending on your version of _dfu-programmer_ you may need to manually RESET the board.

## Testing ##

Now that your firmware is loaded, connect GPIO pins to GND to type the corresponding pin's name.  For example, GND'ing pin PB2 will type `b2`.  Pressing the HWB button will type `e2` as it is connected to pin PE2.  **DO NOT Connect GND and VCC or VBUS** as this will short your PC's USB power supply and may damage your computer or your Micropendous board.

> ![http://micropendous.googlecode.com/svn/trunk/Pictures/LinuxQuickStart_HWB_button.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/LinuxQuickStart_HWB_button.jpg)

## Communication Testing ##

Keyboard firmware may be useful but it is limited to transferring a maximum of 6 characters per keyboard report.  There is also significant overhead in the HID (Human Interface Device) USB class.

More useful are the firmware examples based on USB Virtual Serial which appear as serial ports on your computer and can be communicated with using a serial terminal or standard serial port software.

Try loading your board using the above technique with one of [USBVirtualSerial](USBVirtualSerial.md), [USBVirtualSerial\_FreeRTOS](USBVirtualSerial_FreeRTOS.md), or [LUFAduino](LUFAduino.md), firmware demos.  Then follow the instructions in [SerialPortUsageLinux](SerialPortUsageLinux.md) to test communication.

## Next ##

[SerialPortUsageLinux](SerialPortUsageLinux.md) to test serial communication.

Now that all necessary software is installed and your Micropendous board has been tested, proceed to [Firmware Development](FirmwareDevelopment.md) tutorials.

Back to [QuickStart](QuickStart.md)