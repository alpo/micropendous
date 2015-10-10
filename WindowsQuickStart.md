A version of this page for Linux is also [available](LinuxQuickStart.md).

In general, **Administrator** privileges are required on a Windows machine during development.  The easiest thing to do is to log in as Administrator during development.  This creates security issues so it is best disconnect from the Internet when not using it.  Also, having a separate computer for development is preferable in case you incorrectly connect something and fry your system.

  1. Download and Install [Atmel AVR 8-bit Toolchain 3.4.1](http://www.atmel.com/tools/atmelavrtoolchainforwindows.aspx) _(Required for firmware development)_
  1. Download and Install [Atmel's FLIP](http://www.atmel.com/tools/flip.aspx) _(Needed for programming firmware onto USB AVRs with default DFU Bootloader)_
  1. Download and Extract [AVRdude](http://download.savannah.gnu.org/releases/avrdude/avrdude-5.11-Patch7610-win32.zip) into a directory in your `PATH` such as `C:\WINDOWS` _(Needed for programming firmware onto USB AVRs with CDC Bootloader)_
  1. Download and Install [Python 2.7](http://www.python.org/download/releases/2.7.3/) _(Optional - Needed for developing software using Python)_
  1. Download and Install [PyUSB](http://sourceforge.net/projects/pyusb/files/PyUSB%201.0/1.0.0-alpha-3/pyusb-1.0.0a3.zip/download) _(Optional - Needed for developing USB software using Python)_
  1. Download and Install [PySerial](https://pypi.python.org/pypi/pyserial) _(Optional - Needed for developing Serial Port software using Python)_

### Did FLIP Install Correctly? ###

  * Start a Command Prompt.  If you do not have shortcut, run it via _Start_ -> _Run_ -> `cmd`

> ![http://micropendous.googlecode.com/svn/trunk/Pictures/Start_Run_CommandPrompt.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/Start_Run_CommandPrompt.jpg)

  * check to make sure FLIP was correctly installed by trying to run the `batchisp.exe` command which is FLIP's command-line interface.

> ![http://micropendous.googlecode.com/svn/trunk/Pictures/BatchISP_Success.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/BatchISP_Success.jpg)

  * the above means everything is O.K.  If you get other output, either add FLIP to your `PATH` [permanently](AddingFLIPtoPATH.md) or temporarily each time you start a Command Prompt with the following command:
> > ![http://micropendous.googlecode.com/svn/trunk/Pictures/BatchISP_Fail.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/BatchISP_Fail.jpg)

> Replace `C:\Program Files\Atmel\Flip 3.3.2\bin` above with whatever directory you installed FLIP into.


## Next ##

Proceed to [Programming and Testing](ProgramAndTestWindows.md) Tutorial after your development system has been set up.

Back to [QuickStart](QuickStart.md)