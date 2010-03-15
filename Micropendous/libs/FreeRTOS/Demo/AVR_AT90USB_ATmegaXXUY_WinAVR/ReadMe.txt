Demo Name: AVR_AT90USB_ATmegaXXUY_WinAVR

Targets: USB AVR's with 2kbytes+ SRAM such as the ATmega32U4,
	 ATmega32U6, AT90USB646, AT90USB647, AT90USB1286, and AT90USB1287

Purpose: Emulate a Serial Port (COM Port) using USB CDC Class.
	 Also run standard FreeRTOS test tasks.  LEDs are on PortB.

Notes:	- Target name and variables are set near the top of the makefile
	- MAKE SURE to properly set MCU and F_CPU variables in the makefile
	  before compiling

TODO:	- UART test tasks cause the firmware to fail and are disabled in main.c
