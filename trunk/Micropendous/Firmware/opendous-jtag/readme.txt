Read README.txt in ./openocd/ for further usage instructions.

Note SRST = System ReSeT = RESET line of target

This is firmware for AT90USB162 MCU for making simple USB-JTAG interface
primary goal is to replace parallel port based adapters (Wiggler and alike).

This firmware and associated openocd and urjtag patches were developed
by ilmarin (www.ilmarin.info) and are hosted at:
http://code.google.com/p/opendous-jtag/

Patches for openocd and urjtag are included in patches directory. 

To compile make sure that you have AVR gnu toolchain installed and LUFA 090605
library ( http://www.fourwalledcubicle.com/LUFA.php ).

To connect to the actual hardware one must connect pins of PORTB as following:

TDI  0
TMS  1
TCK  4
TDO  5

EMU  6 (optional)
TRST 2 (optinal)
SRST 3 (optinal) = RESET

Maximum achievable speed is about 60KHz . Speed control is implemented 
in firmware, but not in the patches for UrJtag and openocd yet. So, 
the interface will always run at maximum speed
