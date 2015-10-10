The latest version of this firmware is available in the latest [Micropendous Distribution](http://code.google.com/p/micropendous/downloads/list) or via [SVN](http://code.google.com/p/micropendous/source/checkout) in the `/Micropendous/Firmware/BootloaderDFU` directory.  Copied directly from the [LUFA](http://www.fourwalledcubicle.com/LUFA.php) project.

**Purpose:** Allows new firmware to be uploaded to your Micropendous board over its USB connection.  No need for an external programmer.

**Source Code:** [View](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Firmware/BootloaderDFU) or [Download](http://www.Micropendous.org/Distribution)

**Targets:** [Micropendous1](Micropendous1.md), [Micropendous2](Micropendous2.md), [Micropendous3](Micropendous3.md), [Micropendous4](Micropendous4.md) or any USB AVR based hardware

**USB Mode:** Device-mode enumerating as Custom-Class USB device

**Status:** _Working_


## Usage ##

Edit the project's `makefile` for your target USB AVR.  Make sure to alter the `MCU`, `BOOT_START`, `F_CPU`, and `BOARD` settings if necessary.  Then compile the firmware and program it onto your USB AVR by following the instructions in [AVRISP](AVRISP.md).  Or use some other AVR programmer.  All the USB AVRs support ISP programming.