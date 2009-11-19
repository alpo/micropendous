Taken from http://code.google.com/p/opendous-jtag/



*** NOTE for openocd-0.3.1 ***
Fully patched source is available as openocd-0.3.1_patched.tar.bz2 and compiled is openocd-0.3.1_patched_compiled_Ubuntu904_x86.tar.bz2

Compile and install under Ubuntu with:
  sudo apt-get -y install libusb-dev automake autoconf libtool
  ./configure --enable-opendous    --enable-verbose-jtag-io --enable-verbose-usb-io --enable-verbose-usb-comms
  make
  sudo make install

* Note you do not need the 'verbose' commands unless you need to debug the interface
* Note you need to enable any other interfaces specifically
* Check correct functionality and included interfaces with
  openocd -c interface_list

*** END NOTE for openocd-0.3.1 ***




Under Ubuntu, make sure to install:

sudo apt-get -y install git autoconf automake doxygen texinfo


A simple USB-JTAG interface implemented using LUFA library running AT90USB162 based on Open Hardware board from opendous.
Primary purpose is to create a cheap replacement for the parallel port JTAG adapter "Wiggler" and alike.
Connecting pins to JTAG interface

PORTB is used for JTAG communications, with following pins configuration:

Pin	Function
0	TDI
1	TMS
2	TRST
3	SRST
4	TCK
5	TDO

Various JTAG cable pin outs are available here: http://www.jtagtest.com/pinouts/
Configuring openocd

Install:

  Can ignore first two steps and just use openocd-svn-r1454-patched.tar.gz

  get the rev 1454 of openocd:

    svn export -r 1454 http://svn.berlios.de/svnroot/repos/openocd/trunk openocd

  apply the patch from patches subdirectory:

    cd openocd
    patch -p0 < ../opendous-jtag/patches/openocd-r1454.patch

  Run the ./bootstrap script to refresh autoconf/automake rules, and then run ./configure

    ./bootstrap
    ./configure --prefix=/opt/arm --enable-opendous
    make 
    sudo make install

  make sure that PATH includes /opt/arm/bin directory, edit /etc/bash.bashrc:

     sudo gedit /etc/bash.bashrc

  add the following line at the end:

    export PATH=$PATH:/opt/arm/bin

  re-initialize bash with the new settings (at a Terminal prompt, it is a . then a space, then the file name):

    . /etc/bash.bashrc

  run openocd in a terminal to start the server:

    openocd -f interface/opendous -f target/<your cpu target>

  run gdb in another terminal and connect to the 'remote' debugging session:

    gdb
    (gdb) target remote localhost:3333


Testing speed

At the moment it is quite slow, following is a test of uploading binary on a blackfin cpu using gdbproxy:

(gdb) load u-boot
Loading section .text, size 0x15d30 lma 0x3fc0000
Loading section .rodata, size 0x5ba4 lma 0x3fd5d30
Loading section .data, size 0x4100 lma 0x3fdb8d4
Loading section .u_boot_cmd, size 0x658 lma 0x3fdf9d4
Loading section .text_l1, size 0x2c lma 0x3fe002c
Start address 0x3fc0000, load size 131160
Transfer rate: 2892 bits/sec, 10930 bytes/write. 

Test of loading a FreeRTOS demo project on a Stellaris LM3S2965 demo kit

> openocd -f opendous.cfg -f target/lm3s6965.cfg

Info : OPENDOUS JTAG Interface ready
Info : JTAG tap: lm3s6965.cpu tap/device found: 0x3ba00477 (Manufacturer: 0x23b, Part: 0xba00, Version: 0x3)
Info : JTAG Tap/device matched
Warn : no telnet port specified, using default port 4444
Warn : no gdb port specified, using default port 3333
Warn : no tcl port specified, using default port 6666
Info : accepting 'telnet' connection from 0
target state: halted
target halted due to undefined, current mode: Thread 
xPSR: 0x00000000 pc: 0x00000000
Info : JTAG tap: lm3s6965.cpu tap/device found: 0x3ba00477 (Manufacturer: 0x23b, Part: 0xba00, Version: 0x3)
Info : JTAG Tap/device matched

> halt
target was in unknown state when halt was requested

> flash probe 0
flash 'stellaris' found at 0x00000000

> flash erase_sector 0 0 255
erased sectors 0 through 255 on flash bank 0 in 0.811001s

> flash write_image RTOSDemo.bin 0 bin  
Algorithm flash write 2048 words to 0x0, 18758 remaining
Algorithm flash write 2048 words to 0x2000, 16710 remaining
Algorithm flash write 2048 words to 0x4000, 14662 remaining
Algorithm flash write 2048 words to 0x6000, 12614 remaining
Algorithm flash write 2048 words to 0x8000, 10566 remaining
Algorithm flash write 2048 words to 0xa000, 8518 remaining
Algorithm flash write 2048 words to 0xc000, 6470 remaining
Algorithm flash write 2048 words to 0xe000, 4422 remaining
Algorithm flash write 2048 words to 0x10000, 2374 remaining
Algorithm flash write 326 words to 0x12000, 326 remaining
wrote 75032 byte from file RTOSDemo.bin in 25.443260s (2.879876 kb/s)





LPC1758 Example Debugging

Note: the currently used version of openocd does not support LPC17xx devices.  The following
      is meant purely as a connectivity test.

1) compile and load opendous-jtag onto your USB AVR based board
2) connect your LPC board to your opendous-jtag device and connect according to above
3) compile the patched openocd and start the server
     openocd -f interface/opendous.cfg -f ./lpctest.cfg
   Note you need the direct path to the lpctest.cfg configuration file
4) start gdb and connect to the 'remote session' --> target remote localhost:3333

