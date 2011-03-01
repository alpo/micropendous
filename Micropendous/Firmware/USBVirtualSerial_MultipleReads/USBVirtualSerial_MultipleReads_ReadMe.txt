Purpose: Emulate a Serial Port (COM Port) but process data
	 on the AVR.  Use standard IO calls to send and
	 receive data from a host.

	 The main difference between the standard USBVirtualSerial
	 firmware and this _MultipleReads version is that  the
	 Timer is not used and throughput is lower but all
	 available bytes are accessed from the OUT endpoint.

	 This is behaviour required by USBVirtualSerial-SPI and
	 SRAM_Test firmware examples.

Notes:	- as written it simply loops back any data sent from
	  the host.
	- look for TODO statements in USBVirtualSerial.c
	- most of your code should go in Main_Task() and
	  possibly SetupHardware()
