Purpose: Simple AVR firmware for testing SPI interface.

Usage:	- How to test AVRISP-mkII and USBVirtualSerial-SPI firmware
	  1) Load AVRISP-mkII firmware onto your USB AVR board
	  2) Connect your USB AVR board to an ATtiny261 for ISP programming
	  3) Compile and program SPI_AVRISP_Test firmware onto your
	     ATtiny261 with   make program
	  4) Disconnect just the CS line (PB4 by default) from the AVR
	  5) Load USBVirtualSerial-SPI firmware onto your USB AVR board
	  6) Run  python Test_SPI_AVRISP_Test_Communication.py COM5
	     to test SPI communication.


Notes:	- This firmware was prepared for and tested using an ATtiny261
	- Edit makefile to port this code to a different AVR
	- SPI_AVRISP_Test firmware is compatible with any
	  AVR that has a USI Peripheral and is In-System Programmable via SPI Port



Programming Output:

C:\Micropendous\Firmware\SPI_AVRISP_Test>make program
avrdude -c avrispmkII -P usb -p attiny261 -U flash:w:SPI_AVRISP_Test.hex
avrdude: stk500v2_recv_mk2: error in USB receive

avrdude: stk500v2_recv_mk2: error in USB receive
avrdude: AVR device initialized and ready to accept instructions

Reading | ################################################## | 100% 0.00s

avrdude: Device signature = 0x1e910c
avrdude: NOTE: FLASH memory has been specified, an erase cycle will be performed
         To disable this feature, specify the -D option.
avrdude: erasing chip
avrdude: reading input file "SPI_AVRISP_Test.hex"
avrdude: input file SPI_AVRISP_Test.hex auto detected as Intel Hex
avrdude: writing flash (274 bytes):

Writing | ################################################## | 100% 0.14s

avrdude: 274 bytes of flash written
avrdude: verifying flash memory against SPI_AVRISP_Test.hex:
avrdude: load data flash data from input file SPI_AVRISP_Test.hex:
avrdude: input file SPI_AVRISP_Test.hex auto detected as Intel Hex
avrdude: input file SPI_AVRISP_Test.hex contains 274 bytes
avrdude: reading on-chip flash data:

Reading | ################################################## | 100% 0.08s

avrdude: verifying ...
avrdude: 274 bytes of flash verified

avrdude: safemode: Fuses OK

avrdude done.  Thank you.




SPI Communication Testing Output:

C:\Micropendous\Firmware\SPI_AVRISP_Test>python Test_SPI_AVRISP_Test_Communication.py COM5
Serial<id=0xbab410, open=True>(port='COM5', baudrate=9600, bytesize=8, parity='N', stopbits=1, timeout=2000, xonxoff=0, rtscts=0, dsrdtr=0)

get first byte - should be 170
Data Sent (85,)  Data Returned: [170]

get upper byte
76543210 - Hex  - Int - Data Sent (85,)  Data Returned: [5]
00000101 - 0x5 - 005

get lower byte
76543210 - Hex  - Int - Data Sent (85,)  Data Returned: [1]
00000001 - 0x1 - 001

Received Timer0 Value =  1281


get first byte - should be 170
Data Sent (85,)  Data Returned: [170]

get upper byte
76543210 - Hex  - Int - Data Sent (85,)  Data Returned: [18]
00010010 - 0x12 - 018

get lower byte
76543210 - Hex  - Int - Data Sent (85,)  Data Returned: [0]
00000000 - 0x0 - 000

Received Timer0 Value =  4608
