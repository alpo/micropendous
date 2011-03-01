Purpose: Emulate a Serial Port (COM Port) but process data
	 on the AVR.  Use standard serial software to send and
	 receive data from a host over I2C.

Notes:	- SCL is PD0, SDA is PD1
	- Connect 2k to 15k pull-up resistors between SDA and Vcc and
	  SCL and Vcc.  Lower values are better.  4.7k Ohm is standard.

TODO:	- Port to LUFA-101122 which fails to work
	- Implement proper hardware I2C
