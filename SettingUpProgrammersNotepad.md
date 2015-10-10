These instructions will allow you to compile firmware and program Micropendous boards from within [Programmer's Notepad](http://sourceforge.net/projects/pnotepad/).  These instructions assume you have properly set up Windows for firmware development, [WindowsQuickStart](WindowsQuickStart.md), and have installed [Serial Port](SerialPortUsageWindows.md) drivers.

  1. Start [Programmer's Notepad](http://sourceforge.net/projects/pnotepad/) which is installed as part of [WinAVR](http://winavr.sourceforge.net/):
> > ![http://micropendous.googlecode.com/svn/trunk/Pictures/Programmers_Notepad-1.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/Programmers_Notepad-1.jpg)
  1. Open the file(s) you wish to edit, for example [USBVirtualSerial](USBVirtualSerial.md) firmware found in `/Micropendous/Firmware/USBVirtualSerial` in the latest [Micropendous release](http://code.google.com/p/micropendous/downloads/list).
> > ![http://micropendous.googlecode.com/svn/trunk/Pictures/Programmers_Notepad-2.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/Programmers_Notepad-2.jpg)
> > ![http://micropendous.googlecode.com/svn/trunk/Pictures/Programmers_Notepad-3.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/Programmers_Notepad-3.jpg)
  1. Note all the important code for [USBVirtualSerial](USBVirtualSerial.md) firmware is in `Main_Task`:
> > ![http://micropendous.googlecode.com/svn/trunk/Pictures/Programmers_Notepad-4.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/Programmers_Notepad-4.jpg)
  1. Now let us set up firmware compiling and programming.  Select _Tools_, then _Options_.
> > ![http://micropendous.googlecode.com/svn/trunk/Pictures/Programmers_Notepad-5.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/Programmers_Notepad-5.jpg)
  1. Click _Tools_ in the left-hand tree then click the _Add_ button.  You will need to change the settings as pictured:
```
Name: Make FLIP
Command: make.exe
Folder: $(ProjectPath)
Parameters: flip
```
> > ![http://micropendous.googlecode.com/svn/trunk/Pictures/Programmers_Notepad-6.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/Programmers_Notepad-6.jpg)
  1. Click O.K and the new command is now ready.  Click _Tools_ and _Make Flip_ to compile and program the firmware.  More detailed instructions for firmware loading are in [ProgramAndTestWindows](ProgramAndTestWindows.md).
> > ![http://micropendous.googlecode.com/svn/trunk/Pictures/Programmers_Notepad-7.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/Programmers_Notepad-7.jpg)
  1. If successful, the _Output_ window should show a process exit code of `0`.
> > ![http://micropendous.googlecode.com/svn/trunk/Pictures/Programmers_Notepad-8.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/Programmers_Notepad-8.jpg)