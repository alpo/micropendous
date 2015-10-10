**WILL SOON BE UPDATED**

[LibUSB-Win32](http://libusb-win32.sourceforge.net/) allows you to access USB devices from user space without needing to develop custom drivers.  The API is compatible with [LibUSB](http://libusb.sourceforge.net/) for Linux and other Operating Systems.

In general, **Administrator** privileges are required on a Windows machine during development.  The easiest thing to do is to log in as Administrator during development.  This creates security issues so it is best disconnect from the Internet when not using it.  Having a separate computer for development is preferable in case you incorrectly connect something and fry your system.

  1. Install [MingW](http://www.mingw.org)
    * Install [7-Zip](http://www.7-zip.org/) if you do not have archive extracting software.  Note that you have to open each of the following archives with the 7-Zip File Manager, double-click the `tar` file, then select all the displayed files and directories and click the **Extract** button to open the _extract to..._ window, where you must enter the extraction directory: `C:\MingW`.
    * Download and Extract [gcc-core-3.4.5-20060117-3.tar.gz](http://downloads.sourceforge.net/mingw/gcc-core-3.4.5-20060117-3.tar.gz) to `C:\MingW`
    * Download and Extract [gcc-g++-3.4.5-20060117-3.tar.gz](http://downloads.sourceforge.net/mingw/gcc-g%2B%2B-3.4.5-20060117-3.tar.gz) to `C:\MingW`
    * Download and Extract [binutils-2.19.1-mingw32-bin.tar.gz](http://downloads.sourceforge.net/mingw/binutils-2.19.1-mingw32-bin.tar.gz) to `C:\MingW`
    * Download and Extract [mingwrt-3.15.2-mingw32-dev.tar.gz](http://downloads.sourceforge.net/mingw/mingwrt-3.15.2-mingw32-dev.tar.gz) to `C:\MingW`
    * Download and Extract [gdb-6.8-mingw-3.tar.bz2](http://downloads.sourceforge.net/mingw/gdb-6.8-mingw-3.tar.bz2) to `C:\MingW`
    * Download and Extract [mingwrt-3.15.1-mingw32-dev.tar.gz](http://downloads.sourceforge.net/mingw/mingwrt-3.15.1-mingw32-dev.tar.gz) to `C:\MingW`
    * Download and Extract [w32api-3.13-mingw32-dev.tar.gz](http://downloads.sourceforge.net/mingw/w32api-3.13-mingw32-dev.tar.gz) to `C:\MingW`
    * Download and Extract [mingw-utils-0.3.tar.gz](http://downloads.sourceforge.net/mingw/mingw-utils-0.3.tar.gz) to `C:\MingW`
    * Do not install `GNU make` or it will conflict with WinAVR
  1. Edit Windows `PATH` to include MingW
    * Right-Click **My Computer**, then click **Properties**, then **Advanced** tab, then **Environment Variables**, then double-click _Path_ and add `;C:\MingW\bin` to the end, or whatever your install directory for MingW is.
> > ![http://micropendous.googlecode.com/svn/trunk/Pictures/Windows_Editing_PATH.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/Windows_Editing_PATH.jpg)
  1. Install [LibUSB-Win32](http://libusb-win32.sourceforge.net/)
    * Note that in Windows Vista, LibUSB-Win32 must be run in XP SP2 Compatibility Mode and with Administrator privileges.
    * Download and Install [libusb-win32-filter-bin-0.1.12.1.exe](http://downloads.sourceforge.net/libusb-win32/libusb-win32-filter-bin-0.1.12.1.exe) to `C:\Program Files\LibUSB-Win32` (_Default_)
    * Download and Extract [libusb-win32-device-bin-0.1.12.1.tar.gz](http://downloads.sourceforge.net/libusb-win32/libusb-win32-device-bin-0.1.12.1.tar.gz) to `C:\Program Files\LibUSB-Win32`
    * Download and Extract [libusb-win32-src-0.1.12.1.tar.gz](http://downloads.sourceforge.net/libusb-win32/libusb-win32-src-0.1.12.1.tar.gz) to `C:\Program Files\LibUSB-Win32`
  1. Compile LibUSB-Win32
    * Open a Command Prompt (if you do not have a Command Prompt shortcut, you can click **Start**, then **Run** and type `cmd`):
> > ![http://micropendous.googlecode.com/svn/trunk/Pictures/Windows_Run_CMD.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/Windows_Run_CMD.jpg)
    * then change directory to `C:\Program Files\LibUSB-Win32\src` and run `make`.
```
cd "Program Files"
cd "LibUSB-Win32"
cd src
make
```
> > > ![http://micropendous.googlecode.com/svn/trunk/Pictures/LibUSB-Win32_make.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/LibUSB-Win32_make.jpg)
    * the above should run without errors and will compile LibUSB-Win32
  1. Copy files compiled in previous step to their appropriate locations
    * Copy `C:\Program Files\LibUSB-Win32\src\src\usb.h` to `C:\MingW\include\`
    * Copy `C:\Program Files\LibUSB-Win32\src\libusb.a` to `C:\MingW\lib\`
    * Copy `C:\Program Files\LibUSB-Win32\src\libusb0.dll` to `C:\MingW\bin\`
  1. Test if everything works by running the [LoopBack](LoopBack.md) demo.


## Next ##

Proceed to [Firmware Development](FirmwareDevelopment.md) if all necessary software is installed.

Back to [QuickStart](QuickStart.md)