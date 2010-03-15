Purpose: Emulate a Serial Port (COM Port) but process data
	 on the AVR.  Use standard IO calls to send and
	 receive data from a host.

Notes:	- as written it simply loops back any data sent from
	  the host.
	- look for TODO statements in USBVirtualSerial_FreeRTOS.c
	  most of your code should go in Main_Task() and
	  possibly SetupHardware()
