Purpose: Emulate a Serial Port (COM Port) and communicate
	 ADC readings.

Notes:	- sending a 0 or 1 (using a serial terminal) to a
	  USB AVR programmed with this firmware will send
	  back the current ADC reading of channel 0 or 1
	- requires a USB AVR with an ADC
	- For nicer output run  python PlotDataADC.py  COM5 128
	  where COM5 is the virtual serial port the device
	  enumerated to.  Under Linux this would be something
	  like /dev/ttyACM0
