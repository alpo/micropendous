The first step is to choose which Operating System you will be using for development:

[Linux](LinuxQuickStart.md) - All development can be done using a [LiveCD](http://help.ubuntu.com/community/LiveCD) session

[Windows](WindowsQuickStart.md)



---


## QuickStart Hierarchy Map ##

  * Get Started (Setup and Testing)
    * on [Linux](LinuxQuickStart.md)
      * [Program and/or test a Micropendous board](ProgramAndTestLinux.md)
        * [Serial Port Usage](SerialPortUsageLinux.md)
    * on [Windows](WindowsQuickStart.md)
      * [Program and/or test a Micropendous board](ProgramAndTestWindows.md)
        * [Serial Port Usage](SerialPortUsageWindows.md)

  * Develop Firmware:
    * [Choose Development Complexity](FirmwareDevelopment.md)
      * [LUFAduino](LUFAduino.md) - Use [Arduino](http://www.arduino.cc) code and style
      * [USBVirtualSerial\_FreeRTOS](USBVirtualSerial_FreeRTOS.md) - Program AVR directly but ignore USB
      * [USBVirtualSerial](USBVirtualSerial.md) - Program AVR directly and customize USB handling
      * [LoopBack](LoopBack.md) - Program AVR directly and use custom USB class for maximum throughput
        * [SetupLibUSBWindows](SetupLibUSBWindows.md) Setting up for [LibUSB-Win32](http://libusb-win32.sourceforge.net/) development under Windows

  * More Firmware Examples Included in the latest [Distribution](http://code.google.com/p/micropendous/downloads/list):
    * [AVRISP](AVRISP.md) - LUFA AVR ISP mkII clone AVR programmer
    * [BootloaderDFU](BootloaderDFU.md) - LUFA Bootloader for the USB AVRs
    * [MassStorage\_MMC\_SD\_SPI\_Mode](MassStorage_MMC_SD_SPI_Mode.md) - Mass Storage device demo using MMC or SD card connected in SPI mode
    * [Micropendous](Micropendous.md) - Combined [LUFAduino](LUFAduino.md) and [MicropendousKeyboardTest](MicropendousKeyboardTest.md).  GND'ing pins types their name.
    * [MicropendousKeyboardTest](MicropendousKeyboardTest.md) - Keyboard demo to test pin connectivity.  GND'ing pins types their name.
    * [SRAM\_Test](SRAM_Test.md) - firmware and software to test a [Micropendous4](http://www.Micropendous.org/Micropendous4)'s external [SRAM](http://www.Micropendous.org/SRAM)
    * [SpeedTest](SpeedTest.md) - firmware and software to test USB throughput
    * [USBVirtualSerial\_ADC](USBVirtualSerial_ADC.md) - Get the current value of ADC pins using serial port software
    * [USBVirtualSerial\_I2C](USBVirtualSerial_I2C.md) - Virtual Serial Port to [I2C](http://en.wikipedia.org/wiki/I2c) interface
    * [USBVirtualSerial\_LCD](USBVirtualSerial_LCD.md) - Virtual Serial Port to [HD44780 Character LCD](http://en.wikipedia.org/wiki/HD44780_Character_LCD) interface
    * [USBVirtualSerial\_SPI](USBVirtualSerial_SPI.md) - Virtual Serial Port to [SPI](http://en.wikipedia.org/wiki/Serial_Peripheral_Interface_Bus) interface
    * [USBtoSerial](USBtoSerial.md) - Virtual Serial Port to [UART](http://en.wikipedia.org/wiki/Universal_asynchronous_receiver/transmitter) interface