Purpose: Emulate a Serial Port (COM Port) and send data to
	 an HD44780-based LCD display.  Also, echo back data
	 to the host.

	 If no HD44780 LCD is properly connected the firmware will crash.

Notes:	- To connect your USB AVR and HD44780 in 4-bit mode using PortD:
	  - In the makefile add USE_4BIT_LCD_INTERFACE to LUFA_OPTS
	  - PD0=RS, PD1=RW, PD2=E, PD4=D4, PD5=D5, PD6=D6, PD7=D7, GND=GND

	- To connect your USB AVR and HD44780 in 8-bit mode using PortB, and PortC
	  - In the makefile comment out USE_4BIT_LCD_INTERFACE addition to LUFA_OPTS
	  - PC7=RS, PC6=RW, PC5=E, GND=GND
	  - PB0=D0, PB1=D1, PB2=D2, PB3=D3, PB4=D4, PB5=D5, PB6=D6, PB7=D7

	- Port usage definitions are in lcdconf.h