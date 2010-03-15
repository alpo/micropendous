Purpose: Emulate a Serial Port (COM Port) and send data to
	 an HD44780-based LCD display.  Also, echo back data
	 to the host.

Notes:	- Connect your USB AVR and HD44780 in 4-bit mode using PortD
	- PD0=RS, PD1=RW, PD2=E, PD4=D4, PD5=D5, PD6=D6, PD7=D7, GND=GND
