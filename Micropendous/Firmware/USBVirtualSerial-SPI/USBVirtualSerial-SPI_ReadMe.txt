Purpose: Emulate a Serial Port (COM Port) but communicate
	 over SPI.  Serial Port settings are mapped to
	 SPI settings.

Notes:	- PB1 = SCLK, PB2 = MOSI, PB3 = MISO, PB4 = nCS

	- default serial port setting of 9600 8-N-1 means
	  SPI Mode-0 in MSB order at a SPI speed of 125kHz.

	- The amount of data per read/write controls the SPI data width.
	  If you write two bytes to the SPI gateway the SPI data width
	  will be 16-bits.

	- Changing the "Line Coding"/"bytesize" will change the SPI mode:
		Mode0 = 8 bits, Mode1 = 7 bits, Mode2 = 6 bits, Mode3 = 5 bits
	  Note that data is still sent in 8-bit bytes over SPI, this is just a way to
	  overlay SPI functionality over UART functionality.  Default is Mode0.

	- Changing "Baud-Rate" will change the SPI speed.  Defualt SPI clock speed
	  is 8Mhz / 4 = 2MHz.  8Mhz is the assumed clock speed of the USB AVR.
	  The baud-to-SPI-clock translation table is as follows:
		9600 = 125kHz, 14400 = 4MHz, 19200 = 2MHz, 38400 = 1MHz,
		57600 = 500kHz, 115200 = 250kHz, other = 62.5kHz

	- Changing "Stop-Bits" determines whether data will be MSB or LSB.  If the
	  default one stop bit is selected, data will be transmitted MSB first.  Else LSB.

	- Changing "Parity" sets various options:
		Odd:		Leave SCLK high when not in use
		Even:		Leave SCLK low when not in use (default)
		None:	SPI in Master Mode
		Mark:	SPI in Slave Mode
		Space:	Send SPI data registers as two bytes
	  To make sure of proper SPI mode, set None or Mark last after all other settings.
