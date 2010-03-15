Purpose: Emulate an AVRISP-mkII USB Programmer

Notes:	- PB1 = SCK, PB2 = PDI(MOSI), PB3 = PDO(MISO), PB4 = nRESET
	- PF0(ADC0) = VTARGET Voltage Detection (Vcc Pin of Target)
	- the above can be set in the makefile under "AVRISP-MKII Configuration"
	- you also need to connect GND of programmer and target
	- AT90USB162/82 do not support target voltage detection
