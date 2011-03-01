Micropendous USB AVR Development Boards by Opendous Inc.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Table of Contents:

  (1)  Introduction
  (2)  How do I get one?
  (3)  Archive Contents
  (4)  Initial Testing
  (5)  Support
  (6)  License


(1) Introduction
----------------

  Description:
    Micropendous (pronounced micro--pen-dus) boards are a set of Open Hardware
    development boards for Atmel's USB AVR microcontrollers.  They are general
    purpose development boards designed for USB interfacing but can also be
    used for general microcontroller development like other AVR-based boards.
    A USB bootloader allows Micropendous boards to be programmed without the
    need for an external programmer.  Micropendous boards work with the
    Open Source LUFA (http://www.LUFA-lib.org) AVR USB firmware library.
    Tutorials, examples, and firmware can be found online: www.Micropendous.org

    The Micropendous Firmware/Software/Design Distribution is available at
    http://www.Micropendous.org/Distribution


  Common Features:
    -USB powered
    -all I/O pins are accessible
    -RESET and HWB buttons to enable firmware loading over USB
    -HWB button can be used by user applications
    -Open Hardware - all design files available for Open Source KiCAD

  General Advice:
    -Read "USB in a Nutshell" to get a sense of writing USB software and
     firmware: (http://www.beyondlogic.org/usbnutshell/usb-in-a-nutshell.pdf)
    -http://www.LUFA-lib.org for info on the firmware library
    -http://libusb.sourceforge.net/doc for info on the libusb software library



(2) How do I get one?
---------------------

  Visit www.Micropendous.com to purchase a fully built Micropendous board.

  If you need larger quantities you may contact Opendous Inc. for quantity
  discounts.  Or, you can manufacture your own as all Micropendous boards are
  Open Hardware designs with all design files available for Open Source KiCAD
  (schematic/layout/gerbers/BillOfMaterials/assembly instructions).



(3) Initial Testing
-------------------

  A fully built Micropendous comes pre-installed with MicropendousKeyboardTest
  firmware.  This firmware enumerates as a keyboard.  Connecting any IO pins
  to GND will cause the pin's name to be typed.  For example, GND'ing B0 will
  type 'b0'.

  After plugging in a MicropendousKeyboardTest programmed board, if Windows
  complains about drivers simply follow the automated prompts to do a
  non Internet-based search for drivers and install Window's own drivers.

  *** Be careful NOT to connect GND and VCC/VBUS as this will short your board
  and may damage it or your computer! ***

  Under Windows, the first time you plug in a board loaded with Micropendous
  firmware Windows will ask for a driver installation file.  This file is
  available in /Micropendous/Firmware/Micropendous of the Micropendous
  Distribution archive.  It is based on LUFA's USBSerial driver install file
  and simply tells Windows to install its own Virtual Serial Port driver.

  If you get a USB device error, make sure that the Vcc_Sel jumper is in place.
  Also, pressing RESET will usually fix any such errors.



(4) Archive Contents
--------------------

  This archive is organized as follows:
  \Design      - KiCAD design files for the various Micropendous boards
  \Firmware    - firmware examples, for details visit Micropendous.org
  \libs        - copies of avrlib, LUFA, arduino, and other libraries

  To keep the distribution size small, the more obscure designs and firmware
  examples are kept in the Micropendous Subversion repository:
  http://code.google.com/p/micropendous/source/checkout

  Designs no longer supported are kept in Opendous Inc.'s SVN repository
  along with other abandoned hardware designs:
  http://code.google.com/p/opendous/source/browse/trunk/Abandoned_Designs



(5) Support
-----------

  Visit www.Micropendous.org for the latest Tutorials, Guides, example firmware
  and more.  There are guides for setting up Windows and Linux systems for
  Micropendous firmware development in C and software development in C and
  Python.  USBVirtualSerial firmware demos also allow using serial port
  software and techniques to communicate with the boards.  LUFAduino-based
  firmware allows the use of Arduino code.



(6) License
-----------

  The license for all Micropendous design files is the Creative Commons
  Attribution License.  All related software/firmware is licensed under
  the same license as the LUFA firmware library, which is the MIT license.
  Other libraries are licensed under their respective Open Source licenses.
  The actual hardware design comes with the following added disclaimer:
    The author(s) disclaim all warranties with regard to this
    design, including all implied warranties of merchantability
    and fitness for any purpose.  In no event shall the author
    be liable for any special, indirect or consequential damages
    or any damages whatsoever resulting from loss of use, data or
    profits, whether in an action of contract, negligence or other
    tortious action, arising out of or in connection with the use or
    performance of this hardware design and any associated software.



-------------------------------------------------------------------------------
By Opendous Inc.
www.Micropendous.org
www.opendous.org
February 12, 2011
Copyright Under the Creative Commons Attribution License
http://creativecommons.org/licenses/by/3.0/
