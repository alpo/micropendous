A version of this page for Linux is also [available](ProgramAndTestLinux.md).

**Table of Contents**



The Micropendous [Distribution](http://code.google.com/p/micropendous/downloads/list) is designed for [makefile](http://en.wikipedia.org/wiki/Make_(software))-based development as this is an industry standard development process.  The following tutorial is therefore Command Prompt based.  However, [graphical loading of firmware](LoadingFirmwareWithFLIP.md) is also possible.

  * Plug in your Micropendous board and enter bootloader mode by pressing the RESET and HWB buttons, then release the RESET button, then release the HWB button. HWB stands for Hardware Bootloader and is connected to pin PE2 on Micropendous boards (except on the [Micropendous1](Micropendous1.md) board where it is connected to PD7).

> ![http://micropendous.googlecode.com/svn/trunk/Pictures/Enter_Bootloader.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/Enter_Bootloader.jpg)

# Installing Drivers #

  * Windows should notice your Micropendous board in bootloader mode and ask for Driver installation

> ![http://micropendous.googlecode.com/svn/trunk/Pictures/Windows_FoundNewHardware_StatusBar.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/Windows_FoundNewHardware_StatusBar.jpg)

  * DO NOT let Windows search as it will find nothing.

> ![http://micropendous.googlecode.com/svn/trunk/Pictures/Windows_FoundNewHardwareWizard-1.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/Windows_FoundNewHardwareWizard-1.jpg)

  * You want to install from a custom location.

> ![http://micropendous.googlecode.com/svn/trunk/Pictures/Windows_FoundNewHardwareWizard-2_ATmega32U4.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/Windows_FoundNewHardwareWizard-2_ATmega32U4.jpg)

  * Atmel's [FLIP](http://www.atmel.com/dyn/products/tools_card.asp?tool_id=3886) Driver is in the `\usb` directory wherever you installed FLIP.

> ![http://micropendous.googlecode.com/svn/trunk/Pictures/Windows_FoundNewHardwareWizard-3_AtmelFLIPUSBDriver.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/Windows_FoundNewHardwareWizard-3_AtmelFLIPUSBDriver.jpg)

  * Proceed to install.  If installation asks for `libusb0.dll` or `libusb0.sys`, they are also in the `\usb` directory where you installed FLIP.

> ![http://micropendous.googlecode.com/svn/trunk/Pictures/Windows_FoundNewHardwareWizard-4.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/Windows_FoundNewHardwareWizard-4.jpg)

  * Installation was successful.  Restarting your computer may be required.

> ![http://micropendous.googlecode.com/svn/trunk/Pictures/WindowsDriverInstallTutorial_FoundNewHardware.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/WindowsDriverInstallTutorial_FoundNewHardware.jpg)

# Loading Firmware #

  * Start a Command Prompt.  If you do not have shortcut, run it via _Start_ -> _Run_ -> `cmd`

> ![http://micropendous.googlecode.com/svn/trunk/Pictures/Start_Run_CommandPrompt.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/Start_Run_CommandPrompt.jpg)

  * change into the directory of the firmware you wish to upload.  For testing, [MicropendousKeyboardTest](MicropendousKeyboardTest.md) is preferred.  Run `notepad makefile` to edit the `makefile` for your current Micropendous board.

> ![http://micropendous.googlecode.com/svn/trunk/Pictures/ProgramAndTestTutorial_Windows-01.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/ProgramAndTestTutorial_Windows-01.jpg)

  * There are three settings that need to be set: `MCU`, `F_CPU`, and `BOARD`.  For a [Micropendous4](http://www.Micropendous.org/Micropendous4) board based on the AT90USB1287 with a 16MHz clock, the settings would be:
```
MCU = at90usb1287
F_CPU = 16000000
BOARD = USER
```

> ![http://micropendous.googlecode.com/svn/trunk/Pictures/ProgramAndTestTutorial_Windows-02.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/ProgramAndTestTutorial_Windows-02.jpg)

  * Now run `make flip` to load the firmware.

> ![http://micropendous.googlecode.com/svn/trunk/Pictures/ProgramAndTestTutorial_Windows-03.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/ProgramAndTestTutorial_Windows-03.jpg)

  * If FLIP fails as below, try entering the bootloader again (press RESET and HWB, then release RESET, then release HWB).

> ![http://micropendous.googlecode.com/svn/trunk/Pictures/ProgramAndTestTutorial_Windows-04.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/ProgramAndTestTutorial_Windows-04.jpg)

  * If FLIP succeeds your Micropendous board will be loaded with the firmware and RESET so that your firmware begins to run.

> ![http://micropendous.googlecode.com/svn/trunk/Pictures/ProgramAndTestTutorial_Windows-05.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/ProgramAndTestTutorial_Windows-05.jpg)


# Using a GUI Editor for Development #

If you feel more comfortable in a graphical environment, read [SettingUpProgrammersNotepad](SettingUpProgrammersNotepad.md) for instructions on using [Programmer's Notepad](http://sourceforge.net/projects/pnotepad/) for development.  It comes with [WinAVR](http://winavr.sourceforge.net/) and allows the above firmware uploading to be automated by a single menu item once the above set-up is performed.


# Testing #

Now that your firmware is loaded, connect GPIO pins to GND to type the corresponding pin's name.  For example, GND'ing pin PB2 will type `b2`.  Pressing the HWB button will type `e2` as it is connected to pin PE2.  **DO NOT Connect GND and VCC or VBUS** as this will short your PC's USB power supply and may damage your computer or your Micropendous board.

> ![http://micropendous.googlecode.com/svn/trunk/Pictures/WindowsQuickStart_HWB_button.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/WindowsQuickStart_HWB_button.jpg)

# Communication Testing #

Keyboard firmware may be useful but it is limited to transferring a maximum of 6 characters per keyboard report.  There is also significant overhead in the HID (Human Interface Device) USB class.

More useful are the firmware examples based on USB Virtual Serial which appear as serial ports on your computer and can be communicated with using a serial terminal or standard serial port software.

Try loading your board using the above technique with [VirtualSerial](VirtualSerial.md) or [VirtualSerial\_FreeRTOS](VirtualSerial_FreeRTOS.md) firmware demos.  Then follow the instructions in [SerialPortUsageWindows](SerialPortUsageWindows.md) to test communication (included details on serial driver installation).


## Next ##

[SerialPortUsageWindows](SerialPortUsageWindows.md) to test serial communication.

Proceed to [Firmware Development](FirmwareDevelopment.md) if all necessary software is installed.

Back to [QuickStart](QuickStart.md)