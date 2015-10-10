The latest version of this firmware is available in the latest [Micropendous Distribution](http://code.google.com/p/micropendous/downloads/list) or via [SVN](http://code.google.com/p/micropendous/source/checkout) in the `/Micropendous/Firmware/AndroidAccessoryHost` directory.

**Purpose:** Communicate with [Android 2.3.4+](http://en.wikipedia.org/wiki/Android_%28operating_system%29) devices over USB using the [ADK Protocol](http://developer.android.com/tools/adk/adk.html).

**Source Code:** [View](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Firmware/AndroidAccessoryHost) or [Download](http://www.Micropendous.org/Distribution)

**Targets:** [Micropendous](Micropendous.md), [MicropendousREV1](MicropendousREV1.md), [MicropendousDIP](MicropendousDIP.md) or any [AT90USB1287](http://www.atmel.com/dyn/products/product_card.asp?PN=AT90USB1287)/[AT90USB647](http://www.atmel.com/dyn/products/product_card.asp?PN=AT90USB647) USB AVR based hardware with hosting support.  Android devices with heavily discharged batteries [require a high power DC power supply](http://code.google.com/p/micropendous/wiki/ADK?ts=1342938900&updated=ADK#Example) for charging.

**USB Mode:** Host-mode enumerating as [Android Accessory Host](http://developer.android.com/tools/adk/adk.html#firmware)

**Status:** _Working_

# Usage #

Compile and load the [AndroidAccessoryHost](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Firmware/AndroidAccessoryHost/) firmware onto your USB AVR by following the [ProgramAndTest](ProgramAndTest.md) tutorial.  Install the [DemoKit App](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Firmware/AndroidAccessoryHost/AndroidDeviceSoftware/) onto your Android device by pointing its browser to the following link, or [using ADB](http://developer.android.com/tools/help/adb.html#move), or [via the Market](https://play.google.com/store/apps/details?id=com.diyphonegadgets.DemoKit).
```
http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Firmware/AndroidAccessoryHost/AndroidDeviceSoftware/DemoKIT.apk
```

HWB button presses are shown under the IN heading as B1.  Other buttons are [ready to be implemented](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Firmware/AndroidAccessoryHost/AndroidAccessoryHost.c#324):

Data sent from the Android device to the Micropendous under the OUT heading can be viewed over UART or [customized behavior can be implemented](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Firmware/AndroidAccessoryHost/AndroidAccessoryHost.c#280).

## Example ##

The following shows a [Micropendous](Micropendous.md) programmed with [LUFA](http://www.LUFA-lib.org)'s [AndroidAccessoryHost](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Firmware/AndroidAccessoryHost/) firmware communicating with a [Nexus-S](http://www.google.com/phone/detail/nexus-s) phone running [DemoKit](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Firmware/AndroidAccessoryHost/AndroidDeviceSoftware) software while debug messages are sent to a PC through a [MicropendousA](MicropendousA.md) running [USBtoSerial](USBtoSerial.md) firmware.  Note the [Micropendous](Micropendous.md) board needs to be powered with a high power (6V 1A+) wall adapter to meet the charging requirements of the connected Android device.  The pictured adapter is a [Volgen KTPS24-06035MP](http://search.digikey.com/us/en/products/KTPS24-06035MP/62-1114-ND) but other 6V 1A+ 2.1mm center-positive DC power supplies such as the [CUI EMSA060300-P5P-SZ](http://search.digikey.com/us/en/products/EMSA060300-P5P-SZ/T1089-P5P-ND) should work as well.  Although the [Micropendous](Micropendous.md) can accept DC power from 5.5V to 12V, voltages above 7V will cause overheating due to the Nexus-S' high current consumption while charging.

![http://micropendous.googlecode.com/svn/trunk/Micropendous/Design/Micropendous/Micropendous_AndroidAccessoryHost.jpg](http://micropendous.googlecode.com/svn/trunk/Micropendous/Design/Micropendous/Micropendous_AndroidAccessoryHost.jpg)

The UART debug log for the above communication:

![http://micropendous.googlecode.com/svn/trunk/Micropendous/Design/Micropendous/Micropendous_AndroidAccessoryHost_UART_Debugging.jpg](http://micropendous.googlecode.com/svn/trunk/Micropendous/Design/Micropendous/Micropendous_AndroidAccessoryHost_UART_Debugging.jpg)