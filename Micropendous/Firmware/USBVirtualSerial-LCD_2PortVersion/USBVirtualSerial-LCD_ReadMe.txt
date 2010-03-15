Purpose: Emulate a Serial Port (COM Port) and send data to
	 an HD44780-based LCD display.  Also, echo back data
	 to the host.

Notes:	- Connect your USB AVR and HD44780 in 8-bit mode using PortD, and PortC
	- PC7=RS, PC6=RW, PC5=E, GND=GND
	- PD0=D0, PD1=D1, PD2=D2, PD3=D3, PD4=D4, PD5=D5, PD6=D6, PD7=D7
