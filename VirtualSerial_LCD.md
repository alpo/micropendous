The latest version of this firmware is available in the latest [Micropendous Distribution](http://code.google.com/p/micropendous/downloads/list) or via [SVN](http://code.google.com/p/micropendous/source/checkout) in the `/Micropendous/Firmware/VirtualSerial_LCD ` directory.

**Purpose:** Display text on a [HD44780](http://en.wikipedia.org/wiki/HD44780_Character_LCD) character LCD display.  This page provides details for incorporating [avrlib](http://www.procyonengineering.com/embedded/avr/avrlib/) functions into [LUFA](http://www.fourwalledcubicle.com/LUFA.php)-based projects.  Note that the LUFA project has a [SerialToLCD](http://code.google.com/p/lufa-lib/source/browse/trunk/Projects/SerialToLCD) demo.

**Source Code:** [View](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Firmware/VirtualSerial_LCD) or [Download](http://www.Micropendous.org/Distribution)

**Targets:** [Micropendous1](Micropendous1.md), [Micropendous2](Micropendous2.md), [Micropendous3](Micropendous3.md), [Micropendous4](Micropendous4.md) or any USB AVR based hardware

**USB Mode:** Device-mode enumerating as USB Virtual Serial Port

**Status:** _Working_

# Usage #

### [Micropendous](Micropendous.md) ###

![http://micropendous.googlecode.com/svn/trunk/Pictures/VirtualSerial_LCD-Picture.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/VirtualSerial_LCD-Picture.jpg)

### [Micropendous2](Micropendous2.md) ###

![http://micropendous.googlecode.com/svn/trunk/Pictures/USBVirtualSerial-LCD_Picture2.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/USBVirtualSerial-LCD_Picture2.jpg)

Compile and load the `VirtualSerial_LCD` firmware onto your USB AVR by following the [ProgramAndTest](ProgramAndTest.md) tutorial.

The HD44780 LCD pictured is a [Lumex LCM-S01602DTR](http://www.lumex.com/specs/LCM-S01602DTR%20M.pdf).  It can be purchased from [Digi-Key](http://www.digikey.com/product-detail/en/LCM-S01602DTR%2FM/67-1781-ND) along with a breakable [right-angle](http://www.digikey.com/product-detail/en/PREC040SBAN-M71RC/S1112EC-40-ND) or [straight-through](http://www.digikey.com/product-detail/en/PREC040SABN-RC/S1022EC-40-ND) header which needs to be soldered to the LCD.  It can be connected to any USB AVR board (5V or 3.3V IO) but must be powered at 5V.

## Connecting a Micropendous Board to a HD44780 LCD ##

To save pins the 4-bit HD44780 communication protocol is used.  You can modify the port being used and associated pins in `/Micropendous/Firmware/VirtualSerial_LCD/lcdconf.h`.  This is explained later.

The connections are as follows for the standard firmware:

| **Micropendous**  | **LCD Name**    | **LCD Pin** |
|:------------------|:----------------|:------------|
| GND               | GND             | 1           |
| 5V                | VDD             | 2           |
| GND               | Vo (Contrast)   | 3           |
| PD0 (5)           | RS              | 4           |
| PD1 (4)           | RW              | 5           |
| PD2 (0)           | E               | 6           |
| PD4 (2)           | Data4           | 11          |
| PD5 (3)           | Data5           | 12          |
| PD6 (6)           | Data6           | 13          |
| PD7 (7)           | Data7           | 14          |

## Testing ##

Connect your Micropendous board to your PC.  Connect to the board with a serial port terminal following the [Serial](Serial.md) instructions.  Anything you type will echo back to the terminal and will also be displayed on the LCD.

# Firmware Design #

The goal is to use [AVRlib's](http://ccrma.stanford.edu/courses/250a/docs/avrlib/html/index.html) character LCD functions with data sent to the _Virtual Serial Port_ made up of the Micropendous running modified [VirtualSerial](VirtualSerial.md) firmware.  The following steps are how the [VirtualSerial\_LCD](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Firmware/VirtualSerial_LCD) firmware was created.

The first step is to copy [VirtualSerial](VirtualSerial.md) firmware to a new `VirtualSerial_LCD` directory and rename `VirtualSerial.c` and `VirtualSerial.h`.  The `#include "VirtualSerial.h"` line near the top of our new `VirtualSerial_LCD.c` must be edited for the renamed files.

Examine AVRlib's example code for [LCD functionality](http://code.google.com/p/micropendous/source/browse/#svn/trunk/Micropendous/libs/avrlib/examples/lcd) to determine what files are needed to use the LCD code.

Files `global.h` and `lcdconf.h` must be copied to the `VirtualSerial_LCD` directory from `/libs/avrlib/examples/lcd`.

AVRlib's `rprintf.c, lcd.c, and timer.c` will need to be included for proper functionality of the LCD code.  Open the `makefile` for editing in an editor that does not add formatting (such as [Programmer's Notepad](SettingUpProgrammersNotepad.md)).  Set `TARGET` to use the renamed code base, `SRC` to include AVRlib's files during compilation, and `CC_FLAGS` to include the AVRlib path when searching for include files.

```
TARGET = VirtualSerial_LCD
```

```
AVRLIB_PATH  = ../../libs/avrlib
SRC = $(TARGET).c Descriptors.c $(LUFA_SRC_USB) $(LUFA_SRC_USBCLASS)   \
      $(AVRLIB_PATH)/rprintf.c $(AVRLIB_PATH)/lcd.c $(AVRLIB_PATH)/timer.c
...
CC_FLAGS = -DUSE_LUFA_CONFIG_HEADER -IConfig/   -I$(AVRLIB_PATH)
...
```

AVRlib also requires some [header files](http://en.wikipedia.org/wiki/Header_file).  Add the following `#includes` to `VirtualSerial_LCD.h`:

```
  #include "global.h"
  #include "lcdconf.h"
  #include <lcd.h>
  #include <timer.h>
  #include <rprintf.h>
```

For the AVRlib LCD code to work with the USB AVRs and your particular HD44780-based LCD display `lcdconf.h` must be edited.  The chosen display has 2 lines of 16 characters.  Data and control signals are sent over `PortD` using a 4bit interface.  This configuration was chosen as `PortD` exists on all USB AVRs and will work with the [AT90USBKey](http://www.atmel.com/tools/at90usbkey.aspx).

```
#define USE_4BIT_LCD_INTERFACE
#define LCD_PORT_INTERFACE
...
#ifdef LCD_PORT_INTERFACE
    #define LCD_CTRL_PORT   PORTD
    #define LCD_CTRL_DDR    DDRD
    #define LCD_CTRL_RS     0
    #define LCD_CTRL_RW     1
    #define LCD_CTRL_E      2
    // port you will use for data lines
    #define LCD_DATA_POUT   PORTD
    #define LCD_DATA_PIN    PIND
    #define LCD_DATA_DDR    DDRD
    // access mode you will use (default is 8bit unless 4bit is selected)
    // for 4bit mode, upper nibble (pins 4,5,6,7) of above port is used
    #define LCD_DATA_4BIT
    ...

#define LCD_LINES           2
#define LCD_LINE_LENGTH     16

```

`global.h` is edited to remove `F_CPU` defines, as the `makefile` defines this variable.
```
#include <avrlibdefs.h>
#include <avrlibtypes.h>

#define CYCLES_PER_US ((F_CPU+500000)/1000000)  // cpu cycles per microsecond
```

In `SetupHardware()` which runs at start of `main()` in `VirtualSerial_LCD.c`, the LCD functions must be initialized:

**NOTE** the firmware will not function unless an LCD display is connected as `lcdInit()` blocks.

```
  lcdInit();
  rprintfInit(lcdDataWrite);  // direct rprintf to LCD
  lcdClear();  // clear the LCD
  rprintf("GO AVRlib!");
```

Finally, while data from the host is in the input buffer, send it to the LCD and back to the host.  Make sure characters past the end of the second line get sent back to the start.

```
volatile uint8_t lcd_character_count = 0;
volatile uint8_t lcd_current_line = 0;
...
void Main_Task(void)
{
  int     tempInt  = 0;
  uint8_t tempChar = 0;

  // while there are characters in the USB stream, write them to LCD
  // Each character is written sequentially,
  // first to the top line, then to the next, etc...
  while ((tempInt = fgetc(&USBSerialStream)) > 0)
  {
    tempChar = (uint8_t)tempInt;
    // convert to char
    // send char back for echo'ing on terminal
    fputc(tempChar, &USBSerialStream);
    lcd_character_count++; // wrote another character
    rprintfChar(tempChar); // print char to the LCD
    // once we hit the end of a single line, loop to next line
    if (lcd_character_count >= LCD_LINE_LENGTH)
    {
      lcd_character_count = 0;
      lcd_current_line++;
      // once we hit the end of the last line, loop to the first line
      if (lcd_current_line >= LCD_LINES)
      {
        lcd_current_line = 0;
      }
      lcdGotoXY(0, lcd_current_line);
    }
  }
}

```

### Using AVRlib's LCD Functions ###
```
/* Print some text to the LCD */
rprintf("Hello there")


/* Print value of myValue in base 10 using 5 digits, it is
 * FALSE that this value can be negative (it is positive),
 * and pad the value to 5 digits using 0s if necessary
 */
rprintfNum(10, 5, FALSE, '0', myValue);


/* Move to a particular cursor location */
/* lcdGotoXY(cursorPositionInLine, whichLine); */
/* In this case the 3rd position of line 2 (counting starts at 0) */
lcdGotoXY(2, 1);


/* Clear the LCD */
lcdClear();


/* Text and Numbers, the following would print "Value = 12345 of something" */
uint16_t myValue = 12345;
rprintf("Value = ")
rprintfNum(10, 5, FALSE, '0', myValue);
rprintf(" of something")

```

Do not use `\n` or `\r\n` newline characters as they are not properly handled.  Use `lcdGotoXY()` to position your text.

# Connecting an [AT90USBKey](http://www.atmel.com/tools/at90usbkey.aspx) to a HD44780 LCD #

![http://micropendous.googlecode.com/svn/trunk/Pictures/USBVirtualSerial_LCD-AT90USBKey.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/USBVirtualSerial_LCD-AT90USBKey.jpg)

Pictured above is an [AT90USBKey](http://www.digikey.com/product-detail/en/AT90USBKey/AT90USBKEY2-ND) with [LPPB052CFFN-RC](http://www.digikey.com/product-detail/en/LPPB052CFFN-RC/S9009E-05-ND) headers soldered but [87024-605LF](http://www.digikey.com/product-detail/en/87024-605LF/609-3157-ND) 10-pin 0.05"x0.05" SMT headers are easier to work with once their mounting posts are broken off.  It is connected to a [LCM-S01602DTR](http://www.digikey.com/product-detail/en/LCM-S01602DTR%2FM/67-1781-ND) 16x2 character display with [right-angle](http://www.digikey.com/product-detail/en/PREC040SBAN-M71RC/S1112EC-40-ND) headers soldered ([straight-through](http://www.digikey.com/product-detail/en/PREC040SABN-RC/S1022EC-40-ND) headers would allow flat mounting) stuck into a [solderless breadboard](http://www.digikey.com/product-detail/en/TW-E40-1020/438-1045-ND).  The 0.05"x0.05" headers work with [24 AWG](http://www.digikey.com/product-detail/en/C2003A.12.01/C2003B-100-ND) wire whereas 22 AWG is standard for [breadboard kits](http://www.digikey.com/product-detail/en/A000032/1050-1012-ND).

The `VirtualSerial_LCD` firmware will work with an AT90USBKey.  However, most HD44780 LCDs run at 5V while the AT90USBKey runs at 3.3V.  You must source 5V and its' corresponding GND from somewhere else (such as a [USB connector](http://en.wikipedia.org/wiki/Universal_Serial_Bus#Connector_types)) and connect them to your LCD's VDD and GND pins.  All non-VDD connections will work at 3.3V.