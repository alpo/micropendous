For more complete instructions please see the [QuickStart](http://www.Micropendous.org/QuickStart) guide or the ProgramAndTest wiki pages.


Atmel has a similar [FLIP Usage Tutorial](http://atmel.com/dyn/resources/prod_documents/doc7769.pdf) for firmware uploading.



# Loading Firmware #

This Tutorial assumes you have already compiled your firmware by following the instructions in [ProgramAndTestWindows](ProgramAndTestWindows.md).

  1. Plug in your Micropendous board and enter the bootloader.  Press the RESET and HWB buttons, then release the RESET button, then release the HWB button.  If Windows' Driver Installation begins, refer to [ProgramAndTestWindows](ProgramAndTestWindows.md).
> > ![http://micropendous.googlecode.com/svn/trunk/Pictures/Enter_Bootloader.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/Enter_Bootloader.jpg)
  1. Start Atmel FLIP
> > ![http://micropendous.googlecode.com/svn/trunk/Pictures/FLIP_Tutorial-01.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/FLIP_Tutorial-01.jpg)
  1. Under the _Device_ menu, select _Select_.
> > ![http://micropendous.googlecode.com/svn/trunk/Pictures/FLIP_Tutorial-02.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/FLIP_Tutorial-02.jpg)
  1. Choose your Micropendous board's corresponding MCU.  The picture below assumes a [Micropendous](Micropendous.md) with an [AT90USB1287](http://www.atmel.com/dyn/products/product_card.asp?PN=AT90USB1287) MCU will be programmed.
> > ![http://micropendous.googlecode.com/svn/trunk/Pictures/FLIP_Tutorial-03.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/FLIP_Tutorial-03.jpg)
  1. Under the _Communication_ menu, select _USB_.
> > ![http://micropendous.googlecode.com/svn/trunk/Pictures/FLIP_Tutorial-04.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/FLIP_Tutorial-04.jpg)
  1. Click _Open_ to establish communication with your board.
> > ![http://micropendous.googlecode.com/svn/trunk/Pictures/FLIP_Tutorial-05.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/FLIP_Tutorial-05.jpg)
  1. From the _File_ menu select _Load HEX File..._
> > ![http://micropendous.googlecode.com/svn/trunk/Pictures/FLIP_Tutorial-06.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/FLIP_Tutorial-06.jpg)
  1. Find and Open the firmware file you want to upload
> > ![http://micropendous.googlecode.com/svn/trunk/Pictures/FLIP_Tutorial-07.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/FLIP_Tutorial-07.jpg)
  1. Select the _Erase_, _Program_, and _Verify_ options and click Run
> > ![http://micropendous.googlecode.com/svn/trunk/Pictures/FLIP_Tutorial-08.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/FLIP_Tutorial-08.jpg)
  1. If successful all the tasks should have a green dot next to them.  You can now click _Start Application_ to start running the new firmware on your board
> > ![http://micropendous.googlecode.com/svn/trunk/Pictures/FLIP_Tutorial-09.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/FLIP_Tutorial-09.jpg)


# Loading EEPROM #

The EEPROM of a Micropendous board can similarly be programmed.  The following assumes you have already entered the bootloader, selected your target device, and opened USB communication as explained above.

  1. Click the _Select EEPROM_ button to switch to EEPROM mode
> > ![http://micropendous.googlecode.com/svn/trunk/Pictures/Atmel_FLIP_Programming_EEPROM-1.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/Atmel_FLIP_Programming_EEPROM-1.jpg)
  1. Choose the _Program_ and _Verify_ options
> > ![http://micropendous.googlecode.com/svn/trunk/Pictures/Atmel_FLIP_Programming_EEPROM-2.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/Atmel_FLIP_Programming_EEPROM-2.jpg)
  1. Find and load your EEPROM `.hex` file
> > ![http://micropendous.googlecode.com/svn/trunk/Pictures/Atmel_FLIP_Programming_EEPROM-3.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/Atmel_FLIP_Programming_EEPROM-3.jpg)
  1. Click _Run_ to upload to EEPROM
> > ![http://micropendous.googlecode.com/svn/trunk/Pictures/Atmel_FLIP_Programming_EEPROM-4.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/Atmel_FLIP_Programming_EEPROM-4.jpg)
  1. If successful all the tasks should have a green dot next to them.  You can now click _Start Application_ to start running the new firmware on your board
> > ![http://micropendous.googlecode.com/svn/trunk/Pictures/Atmel_FLIP_Programming_EEPROM-5.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/Atmel_FLIP_Programming_EEPROM-5.jpg)