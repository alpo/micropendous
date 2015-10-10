The latest version of this firmware is available in the latest [Micropendous Distribution](http://code.google.com/p/micropendous/downloads/list) or via [SVN](http://code.google.com/p/micropendous/source/checkout) in the `/Micropendous/Firmware/VirtualSerial` directory.

**Purpose:** VirtualSerial demonstrates USB Virtual Serial Port communication.  Data sent to a device loaded with this firmware will be echoed back over a [Serial Terminal](Serial.md)

**Source Code:** [View](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Firmware/VirtualSerial) or [Download](http://www.Micropendous.org/Distribution)

**Targets:** Any USB AVR board.

**USB Mode:** Device-mode enumerating as USB Virtual Serial Port

**Status:** _Working_.  Please post any problems you encounter to the [Micropendous Group](http://groups.google.com/group/Micropendous).

## Usage ##

Assuming you have completed all setup tasks in the [QuickStart](QuickStart.md) tutorial, simply compile and upload the `VirtualSerial` firmware onto your Micropendous board by following the [ProgramAndTest](ProgramAndTest.md) tutorial.

## Firmware Design ##

This firmware is a minor modification to [LUFA](http://www.fourwalledcubicle.com/LUFA.php)'s `VirtualSerial`.  Joystick code is removed and replaced with loopback code with edits to [VirtualSerial.c](http://code.google.com/p/micropendous/source/diff?spec=svn858&r=856&format=side&path=/trunk/Micropendous/Firmware/VirtualSerial/VirtualSerial.c&old_path=/trunk/Micropendous/Firmware/VirtualSerial/VirtualSerial.c&old=855), [VirtualSerial.h](http://code.google.com/p/micropendous/source/diff?spec=svn858&r=856&format=side&path=/trunk/Micropendous/Firmware/VirtualSerial/VirtualSerial.h&old_path=/trunk/Micropendous/Firmware/VirtualSerial/VirtualSerial.h&old=855), and the [makefile](http://code.google.com/p/micropendous/source/diff?spec=svn858&r=856&format=side&path=/trunk/Micropendous/Firmware/VirtualSerial/makefile&old_path=/trunk/Micropendous/Firmware/VirtualSerial/makefile&old=855).

Standard `stdin` and `stdout/stderr` streams are created in [main](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Firmware/VirtualSerial/VirtualSerial.c?spec=svn858&r=856#89) and linked to the USB Virtual Serial Port.  This means you can use standard `fprintf` and `fgetc` functions to read and write data over the USB connection.  Just place your code in `MainTask()` which runs periodically.  See [avr-libc stdio](http://www.nongnu.org/avr-libc/user-manual/group__avr__stdio.html) for detailed instructions on using Standard IO functions.

Note that your code should not busy-loop or block as this will interfere with USB functionality.  To avoid this issue, use [VirtualSerial\_FreeRTOS](VirtualSerial_FreeRTOS.md).

On the host side, `serialpy.py` is an example of serial communication and is explained in [Serial](Serial.md).

The following is the core of this firmware.

```
/* Main_Task will run periodically once initialization is complete */
void MainTask(void)
{
  int count = 0;

  // If the host has sent data then echo it back
  // Throughput is maximized if the full EP buffer is read and sent each time
  // Throughput approaches CDC_TXRX_EPSIZE kbytes/second and
  // depends on transfer size from host
  if ((count = fread(&buffer, 1, CDC_TXRX_EPSIZE, &USBSerialStream)) > 0) {
    fwrite(&buffer, 1, count, &USBSerialStream);
  }

  // If HWB Button is pressed then send formatted strings
  if (Buttons_GetStatus()) {
    // send a constant string stored in FLASH
    fprintf_P(&USBSerialStream, PSTR("\r\nHWB has been pressed!\r\n"));
    // send a string that is dynamic and stored in SRAM
    fprintf(&USBSerialStream, "PORTD = %3x\r\n", PIND);
  }
}
```

The above example is very simple.  `fread()` returns positive if data is available and `fwrite()` sends it back to the Host.  If the HWB button is pressed, dynamically and statically stored text is sent to the Host.

Note that `fprintf()` is stored entirely in SRAM so it is best to use `printf_P(PSTR("...");` to store and print any constant strings.