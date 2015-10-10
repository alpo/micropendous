This procedure has been tested on a [Fedora 10 i686 Desktop Edition](http://fedoraproject.org/en/get-fedora) hard drive install.

## Procedure ##

At a terminal:

  1. Enable the [RPM Fusion](http://rpmfusion.org) repository:
> > `su -c 'rpm -Uvh http://download1.rpmfusion.org/free/fedora/rpmfusion-free-release-stable.noarch.rpm http://download1.rpmfusion.org/nonfree/fedora/rpmfusion-nonfree-release-stable.noarch.rpm'`
  1. Install all necessary software:
> > `yum install avr-gcc avr-libc avr-binutils avr-gdb avrdude dfu-programmer gcc binutils autoconf libtool automake texinfo kernel-headers libusb-devel python gtkterm unix2dos subversion subcommander`
  1. Install [dfu-programmer](http://downloads.sourceforge.net/dfu-programmer/dfu-programmer-0.5.2.tar.gz)
    * `tar xvf dfu-programmer-0.5.2.tar.gz`
    * `cd dfu-programmer-0.5.2`
    * `./configure --prefix=/usr ; make ; sudo make install`
    * `sudo chmod 775 /usr/bin/dfu-programmer`
  1. Install [PyUSB](http://developer.berlios.de/project/showfiles.php?group_id=4354&release_id=13488)
    * `tar xvf pyusb-0.4.1.tar.gz`
    * `cd pyusb-0.4.1`
    * `sudo python setup.py install`
  1. Install [PySerial](http://downloads.sourceforge.net/pyserial/pyserial-2.4.tar.gz)
    * `tar xvf pyserial-2.4.tar.gz`
    * `cd pyserial-2.4`
    * `sudo python setup.py install`



## Next ##

Proceed to [Programming and Testing](ProgramAndTestLinux.md) Tutorial after your development system has been set up.

Back to [QuickStart](QuickStart.md)