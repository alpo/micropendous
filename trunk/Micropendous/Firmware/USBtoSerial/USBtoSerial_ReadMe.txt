Purpose: Emulate a Serial Port (COM Port).  The communications
	 protocol is emulated on the USB AVR's UART.

Pins:	RX - PD2
	TX - PD3
	DTR - PD7
	RTS - PD5

Notes:	- DTR and RTS are active low, meaning that if you
	  "set" them the pins will go to 0/Low and if you
	  "clear" them they will go to 1/High.
