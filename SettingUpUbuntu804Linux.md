## Procedure ##

This procedure has been tested on an [Ubuntu 8.04 LTS Desktop Edition](http://www.ubuntu.com/getubuntu/download) live session.

  1. enable the universe repository, press close, then allow the reloading of data
> > ![http://micropendous.googlecode.com/svn/trunk/Pictures/Ubuntu804_Enable_Repositories.jpg](http://micropendous.googlecode.com/svn/trunk/Pictures/Ubuntu804_Enable_Repositories.jpg)
    * You can also do the above manually with:
      * `sudo gedit /etc/apt/sources.list`
      * then uncomment the `universe` line: `deb http://archive.ubuntu.com/ubuntu intrepid universe`
      * then run `sudo apt-get -y update`
  1. Install software:
> > `sudo apt-get -y install build-essential texinfo flex byacc bison automake gcc libc6-dev  binutils gcc-avr avr-libc gdb-avr python python-dev libusb-dev autoconf avrdude libtool rapidsvn tofrodos gtkterm rapidsvn subversion`
  1. Install [dfu-programmer](http://downloads.sourceforge.net/dfu-programmer/dfu-programmer-0.5.2.tar.gz)
    * `tar xvf dfu-programmer-0.5.2.tar.gz`
    * `cd dfu-programmer-0.5.2`
    * `./configure --prefix=/usr ; make ; sudo make install`
    * `sudo chmod 775 /usr/bin/dfu-programmer`
  1. Install latest [avr-libc](http://download.savannah.gnu.org/releases/avr-libc/avr-libc-1.6.2.tar.bz2) (contains necessary `util/atomic.h`)
    * `tar xvf avr-libc-1.6.2.tar.bz2`
    * `cd avr-libc-1.6.2/`
    * `./bootstrap ; ./configure --prefix=/usr --build=````./config.guess````` --host=avr ; make ; sudo make install``
  1. Install [PyUSB](http://developer.berlios.de/project/showfiles.php?group_id=4354&release_id=13488)
    * `tar xvf pyusb-0.4.1.tar.gz`
    * `cd pyusb-0.4.1`
    * `sudo python setup.py install`
  1. Install [PySerial](http://downloads.sourceforge.net/pyserial/pyserial-2.4.tar.gz)
    * `tar xvf pyserial-2.4.tar.gz`
    * `cd pyserial-2.4`
    * `sudo python setup.py install`
  1. To enable flashing the Micropendous without being root, you must edit USB permissions.  The following has been adapted from [AVRfreaks.net](http://www.avrfreaks.net/wiki/index.php/Documentation:Tutorials_AT90UsbKey_under_Linux).
    * open for editing, `sudo gedit /etc/udev/rules.d/99-dfu-programmer.rules` and add:
```
SUBSYSTEM=="usb", ACTION=="add", SYSFS{idVendor}=="03eb", SYSFS{idProduct}=="2ffa", MODE="660", GROUP="plugdev", SYMLINK+="at90usb-%k"
BUS=="usb", ACTION=="add", SYSFS{idVendor}=="03eb", SYSFS{idProduct}=="2ffa", MODE="660", GROUP="plugdev"

SUBSYSTEM=="usb", ACTION=="add", SYSFS{idVendor}=="03eb", SYSFS{idProduct}=="2ffb", MODE="660", GROUP="plugdev", SYMLINK+="at90usb-%k"
BUS=="usb", ACTION=="add", SYSFS{idVendor}=="03eb", SYSFS{idProduct}=="2ffb", MODE="660", GROUP="plugdev"

SUBSYSTEM=="usb", ACTION=="add", SYSFS{idVendor}=="03eb", SYSFS{idProduct}=="2ff9", MODE="660", GROUP="plugdev", SYMLINK+="at90usb-%k"
BUS=="usb", ACTION=="add", SYSFS{idVendor}=="03eb", SYSFS{idProduct}=="2ff9", MODE="660", GROUP="plugdev"

SUBSYSTEM=="usb", ACTION=="add", SYSFS{idVendor}=="03eb", SYSFS{idProduct}=="2ff7", MODE="660", GROUP="plugdev", SYMLINK+="at90usb-%k"
BUS=="usb", ACTION=="add", SYSFS{idVendor}=="03eb", SYSFS{idProduct}=="2ff7", MODE="660", GROUP="plugdev"

SUBSYSTEM=="usb", ACTION=="add", SYSFS{idVendor}=="03eb", SYSFS{idProduct}=="2ff4", MODE="660", GROUP="plugdev", SYMLINK+="at90usb-%k"
BUS=="usb", ACTION=="add", SYSFS{idVendor}=="03eb", SYSFS{idProduct}=="2ff4", MODE="660", GROUP="plugdev"

SUBSYSTEM=="usb", ACTION=="add", SYSFS{idVendor}=="03eb", SYSFS{idProduct}=="2ff3", MODE="660", GROUP="plugdev", SYMLINK+="at90usb-%k"
BUS=="usb", ACTION=="add", SYSFS{idVendor}=="03eb", SYSFS{idProduct}=="2ff3", MODE="660", GROUP="plugdev"
```
    * Note the above is for the USB AVR DFU Bootloaders.  Similar `rules` can be created for your own devices by adding lines with your own `idVendor` and `idProduct` values.


## Next ##


Proceed to [Programming and Testing](ProgramAndTestLinux.md) Tutorial after your development system has been set up.

Back to [QuickStart](QuickStart.md)