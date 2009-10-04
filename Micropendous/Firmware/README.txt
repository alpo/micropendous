Visit www.Micropendous.org for more information and a QuickStart Guide.

Descriptions of the various firmware examples:

AVRISP
	- LUFA AVR ISP mkII clone AVR programmer
	- allows one Micropendous board to update the bootloader of another

BootloaderDFU
	- LUFA Bootloader for the USB AVRs

LUFAduino
	- enumerates as a Virtual Serial Port
	- allows you to code in the Arduino language
	- Serial functions are connected to Virtual Serial instead of UART

LoopBack
	- enumerates as a custom-class USB device
	- includes host-side communication software example

MassStorage_MMC_SD_SPI_Mode
	- enumerates as a MassStorage device
	- data is read/written to a MMC or SD card connected in SPI mode
	- make sure your Micropendous board's VCC_Sel jumper is set to 3.3V
	- current version must be recompiled for each new memory card used

Micropendous
	- enumerates as a compund USB device - Virtual Serial Port and Keyboard
	- based on LUFAduino and MicropendousKeyboardTest
	- GND'ing a pin will type its name, e.g., GND'ing PB0 will type b0
	- PD0 fades a LED using analogOut PWM control
	- Virtual Serial Port acts as a loopback for throughput testing

MicropendousKeyboardTest
	- enumerates as a Keyboard
	- GND'ing a pin will type its name, e.g., GND'ing PB0 will type b0

SRAM_Test
	- enumerates as a Virtual Serial Port
	- has interface to read/write data to a Micropendous4's external SRAM
	- includes host-side software to randomly test external SRAM

SpeedTest
	- enumerates as a custom-class USB device
	- based on LoopBack demo
	- included host-side software tests throughput based on block size
	- code written by www.ilmarin.info

USBVirtualSerial
	- enumerates as a Virtual Serial Port
	- loopback configuration - data written to the device is sent back
	- designed to allow customization, base your firmware off of this

USBVirtualSerial_ADC
	- enumerates as a Virtual Serial Port
	- send 0 character to get back ADC0 pin value, 1 for ADC1 pin value

USBVirtualSerial_FreeRTOS
	- enumerates as a Virtual Serial Port
	- loopback configuration - data written to the device is sent back
	- USB and user tasks controlled by FreeRTOS to allow multi-tasking
	- designed to allow customization, base your firmware off of this

USBVirtualSerial_I2C
	- enumerates as a Virtual Serial Port
	- data is read/written over I2C interface - PD0=SCL, PD1=SDA
	- on the Micropendous1 (AT90USB162) the I2C interface is software-based

USBVirtualSerial_LCD
	- enumerates as a Virtual Serial Port
	- data is written to a HD44780-based LCD display on PortB in 4bit mode
	- PB0=RS, PB1=RW, PB2=E, PB4=D4, PB5=D5, PB6=D6, PB7=D7

USBVirtualSerial_LCD_2PortVersion
	- enumerates as a Virtual Serial Port
	- data is written to a HD44780-based LCD display in standard(8bit) mode
	- PC7=RS, PC6=RW, PC5=E, PortB is Data -> PB0=D0, PB1=D1, etc...
	- will not work on a Micropendous2 as the ATmega32U4 has no pin PC5

USBVirtualSerial_SPI
	- enumerates as a Virtual Serial Port
	- data is read/written over SPI interface
	- PB0=SS, PB1=SCK, PB2=MOSI, PB3=MISO

USBtoSerial
	- enumerates as a Virtual Serial Port
	- data is read/written over UART interface - PD2=RXD, PD3=TXD


-------------------------------------------------------------------------------
By Opendous Inc.
www.Micropendous.org
October 04, 2009
Copyright Under the Creative Commons Attribution License
http://creativecommons.org/licenses/by/3.0/
