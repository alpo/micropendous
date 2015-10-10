The latest version of this firmware is available in the latest [Micropendous Distribution](http://code.google.com/p/micropendous/downloads/list) or via [SVN](http://code.google.com/p/micropendous/source/checkout) in the `/Micropendous/Firmware/GeneralMCUDev` directory.

**Purpose:** Use a USB AVR like a non-USB microcontroller.  It is easy to program using the USB bootloader which simplifies development.  Each USB AVR has functionality similar to a non-USB AVR.

**Source Code:** [View](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Firmware/GeneralMCUDev) or [Download](http://www.Micropendous.org/Distribution)

**Targets:** [Micropendous1](Micropendous1.md), [Micropendous2](Micropendous2.md), [Micropendous3](Micropendous3.md), [Micropendous4](Micropendous4.md) or any USB AVR based hardware

**USB Mode:** No USB functionality.

**Status:** _DRAFT_

# Usage #

Compile and load the GeneralMCUDev firmware onto your USB AVR by following the [ProgramAndTest](ProgramAndTest.md) tutorial.


# Code #

The code is very basic.  Simply disable the USB interface.

```
    /* disable USB */       
    USBCON = ((0 << USBE) | (1 << FRZCLK));
    PRR1 |= (PRUSB);
```