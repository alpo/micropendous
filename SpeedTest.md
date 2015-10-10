The latest version of this firmware is available in the latest [Micropendous Distribution](http://code.google.com/p/micropendous/downloads/list) or via [SVN](http://code.google.com/p/micropendous/source/checkout) in the `/Micropendous/Firmware/SpeedTest` directory.

**Purpose:** Test various settings to determine the maximum throughput of your USB AVR.  This firmware and the throughput testing software was written by [ilmarin](http://code.google.com/u/@UBVVR1FTBBdBXQN%2F/) [(www)](http://www.ilmarin.info).

**Source Code:** [View](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Firmware/SpeedTest) or [Download](http://www.Micropendous.org/Distribution)

**Targets:** [Micropendous1](Micropendous1.md), [Micropendous2](Micropendous2.md), [Micropendous3](Micropendous3.md), [Micropendous4](Micropendous4.md) or any USB AVR based hardware

**USB Mode:** Device-mode enumerating as a Custom-Class USB device

**Status:** _Working_


## Usage ##

Compile and load the SpeedTest firmware onto your USB AVR by following the [ProgramAndTest](ProgramAndTest.md) tutorial.  Windows drivers for SpeedTest are included in `/Micropendous/Firmware/SpeedTest/WindowsDriver`.

Under Linux you would run the throughput tests using `sudo make linux` after programming your Micropendous board with the SpeedTest firmware.  Under Windows it would be `make win`.  Under Windows you first need to install [MingW](http://www.mingw.org/) and [LibUSB-Win32](http://libusb-win32.sourceforge.net/) according to [SetupLibUSBWindows](SetupLibUSBWindows.md).

### Throughput Testing Results ###

_**Linux i686 - AT90USB1287 with 16Mhz Crystal**_
```
Block_Size,Read_Speed(Kbytes/s),Write_Speed(Kbytes/s) - using transfer_size of 1048576
   16,	    5.33,	    5.33
   32,	   10.67,	   10.67
   64,	   21.33,	   21.33
  128,	   42.67,	   42.67
  256,	   85.35,	   85.35
  512,	  170.72,	  170.72
 1024,	  256.06,	  341.34
 2048,	  409.76,	  512.02
 4096,	  511.00,	  682.70
 8192,	  585.47,	  819.24
16384,	  628.64,	  910.27
32768,	  630.53,	  910.25
```

_**Linux i686 - ATmega32U4 with 8Mhz Crystal**_
```
Block_Size,Read_Speed(Kbytes/s),Write_Speed(Kbytes/s) - using transfer_size of 1048576
   16,	    5.33,	    5.33
   32,	   10.66,	   10.66
   64,	   21.33,	   21.33
  128,	   42.61,	   42.59
  256,	   85.15,	   85.00
  512,	  128.02,	  170.70
 1024,	  204.16,	  256.06
 2048,	  290.46,	  408.49
 4096,	  372.50,	  585.48
 8192,	  431.34,	  683.12
16384,	  468.32,	  743.16
32768,	  468.32,	  745.27
```

_**Windows - AT90USB1287 with 16Mhz Crystal**_
```
Block_Size,Read_Speed(Kbytes/s),Write_Speed(Kbytes/s) - using transfer_size of 1048576
   16,     16.00,          16.00
   32,     31.97,          31.99
   64,     63.97,          64.04
  128,    128.07,         128.07
  256,    255.17,         256.14
  512,    512.28,         512.28
 1024,   1016.80,         512.28
 2048,   1016.80,         684.78
 4096,   1016.80,         818.40
 8192,   1016.80,         906.88
16384,   1016.80,         906.88
32768,   1016.80,         945.20
```

_**Windows - ATmega32U4 with 8Mhz Crystal**_
```
Block_Size,Read_Speed(Kbytes/s),Write_Speed(Kbytes/s) - using transfer_size of 1048576
   16,     16.00,          16.00
   32,     31.99,          32.00
   64,     63.97,          64.04
  128,    128.07,         128.07
  256,    255.17,         256.14
  512,    512.28,         512.28
 1024,    512.28,         512.28
 2048,    684.78,         684.78
 4096,    818.40,         818.40
 8192,    818.40,         818.40
16384,    818.40,         860.37
32768,    838.86,         883.01
```

### Throughput Testing Explanation ###

You may want to first read [BasicUSBDeviceOverview](BasicUSBDeviceOverview.md) for a basic overview of USB.  Or read [USB in a Nutshell](http://www.beyondlogic.org/usbnutshell/) for a more thorough overview of USB which the following is based on.  The following is just a basic overview of the data transfer portion of a successful bulk transfer as in the firmware.  It omits many aspects of the USB protocol.

The legend used in later diagrams:

![http://micropendous.googlecode.com/svn/trunk/Pictures/USB_Transfers_Legend.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/USB_Transfers_Legend.jpg)

USB is a bussed architecture so all devices share the same connection but communicate through different addresses.  The USB Host controls all communication and the Operating System tries to fairly share the bus among all connected devices.  For Full Speed USB (12Mbit/s) each second is partitioned into millisecond (1000ms = 1second) chunks and fairly allocated to connected devices.  Control, Isochronous, and Interrupt transfers take priority while Bulk transfers use up any remaining time.  This is why small transfers top out at their transfer size.  For example, if using 16byte blocks then 1000 times per second 16 bytes will be transferred for a throughput of 16kbytes/s.  Linux uses 3ms chunks for 16/3=5.33kbytes/s.

![http://micropendous.googlecode.com/svn/trunk/Pictures/USB_Transfers_SinglePacket.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/USB_Transfers_SinglePacket.jpg)

Larger block sizes require the connection to remain opened as more data is transferred per transaction.

![http://micropendous.googlecode.com/svn/trunk/Pictures/USB_Transfers_MultiplePackets.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/USB_Transfers_MultiplePackets.jpg)

What does this mean?  To improve throughput simply increase the size of the data to be transferred per transfer in your host-side software.  However, note that your AVR needs to be able to keep up and generate data quickly enough.  Maximum throughput for your particular application can be attained through trial-and-error.

_Note_: The source `.svg` file for the USB transfer notes above is [available](http://code.google.com/p/micropendous/source/browse/trunk/Pictures/USB_Transfers.svg) under the [Creative Commons Attribution License](http://creativecommons.org/licenses/by/3.0/).


## Firmware Design ##

This firmware is based on the [LoopBack](LoopBack.md) firmware which is based on [LUFA](http://www.fourwalledcubicle.com/LUFA.php)'s AudioInput firmware demo.

There are two `// TODO` lines near the bottom of `SpeedTest.c` which just transfer a count value stored in the `temp` variable.  These are the only lines that need to be altered to adapt the firmware to your own project.

## Software Design ##

`DeviceAccessC.c` is the cross-platform [LibUSB](http://libusb.sourceforge.net/)-based software that performs the throughput testing.  The functions `run_write_benchmark` and `run_read_benchmark` loop over the size of the main buffer and send a `block_size` chunk each transfer.  You can adapt these functions to your application by simply filling the `buffer` with your own data.

In `main()` the `for(i=4;i<16;i++)` loop runs the above functions with different block sizes.  You can adapt this to your own application by changing what happens in this loop.  The rest of the code sets up the USB interface.  The `usb_set_debug(255);` line can be commented out if you do not want USB debugging messages.