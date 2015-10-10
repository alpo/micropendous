Note that not all LUFA demos can be run on Micropendous boards with compatible features.  LUFA demos target the AT90USB1287-based USBKey which has several additional peripherals.

## Required Software ##

To compile [LUFA](http://www.fourwalledcubicle.com/LUFA.php) demos you will require [Atmel's FLIP](http://www.atmel.com/dyn/products/tools_card.asp?tool_id=3886) and [WinAVR](http://winavr.sourceforge.net/) under Windows, or `dfu-programmer` and `avr-gcc` under Linux.  See the [QuickStart Guide](http://www.micropendous.org/QuickStart) for more information.

## Preparing Files ##

  1. Extract the latest [Micropendous Distribution](http://code.google.com/p/micropendous/downloads/list) to a working directory.
  1. Extract the latest [LUFA](http://www.fourwalledcubicle.com/LUFA.php) release to a different working directory.
  1. copy over the `Board` directory from `Micropendous/libs/LUFA/LUFA/Drivers/Board/` to `LUFA/LUFA/Drivers/Board/`.  This allows you to compile any of the Micropendous-compatible LUFA demos without changing any code, just `makefile` defines.

## Modifying `Makefile` ##

Open up the `makefile` for editing in a non-formatting text editor, such as Window's Notepad, [NotePadd++](http://notepad-plus.sourceforge.net/), [Eclipse](http://www.eclipse.org), or your favorite programmer's editor.

  * Change `MCU` from `at90usb1287` to the microcontroller on your board.  For example:
```
MCU = at90usb162
```
  * Change `BOARD` from `USBKEY` to `USER`
```
BOARD  = USER
```
  * If necessary, change `F_CPU` and `F_CLOCK` if you have a 16MHz version.
```
F_CPU = 16000000
F_CLOCK = 16000000
```
  * If you get a massive number of errors when you try to compile, you may need to change `LUFA_PATH`:
```
LUFA_PATH = ../../libs/LUFA
```

You are now ready to compile a LUFA demo for the Micropendous and load the resulting firmware.  See [ProgramAndTest](ProgramAndTest.md) for more information.