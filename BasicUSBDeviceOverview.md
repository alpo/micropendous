# USB #

This is meant to be a quick outline of USB related to software and firmware development.  For a proper overview, see [USB in a Nutshell](http://www.beyondlogic.org/usbnutshell/), [Wikipedia](http://en.wikipedia.org/wiki/USB), or read the actual [USB 2.0 specification](http://www.usb.org/developers/docs/).

## Basic Definitions ##

[USB](http://en.wikipedia.org/wiki/USB) is a communication interface that uses serial communication and has the ability to power connected devices.

USB is host-centric.  Everything happens relative to the host.  The host controls communication and is the thing that you attach peripheral devices to.  The host is usually a personal computer, but can also be a something like a picture printer that connects with cameras.  Micropendous boards are based on Atmel's USB AVR series of chips, all of which support Device-mode operation.

Data is transmitted in packets which are bundles of data and control information.

Enumeration is when a device starts communication with the host and establishes communication parameters like timing, packet size, and device and endpoint addresses.

Endpoints are "addresses" on a device that accept packets from the host or store packets before transmission to the host.

As USB is host-centric, everything is named relative to the host.  So "addresses" on a device that store packets before transmission are **IN** endpoints as the data will be sent to the host.  "Addresses" on a device that receive data are **OUT** endpoints as the data will be coming out from the host.

## Timeline ##

A rough timeline of how various levels of the USB system interact with each other.  Firmware refers to code you can change that resides on a device.  Device is software and hardware specific functions that are part of a firmware library like [LUFA](http://www.fourwalledcubicle.com/LUFA.php).  Host refers to drivers and the hardware functions the host performs.  Software refers to software on the host you can alter to communicate with firmware on a device.  [LibUSB](http://libusb.sourceforge.net/) will be the presumed software library.

| _**Firmware**_ | **Device** | **Host** | _**Software**_ |
|:---------------|:-----------|:---------|:---------------|
|                |            | several times a second sends control packets to Address 0 asking if there are any new devices on the USB bus  |                |
|                | being just plugged in, a device will respond to such a request if it has not yet done so  |          |                |
|                | MyUSB firmware takes care of sending all communication parameters  |          |                |
| ...            | ...        | ...      | ...            |
|                |            | host doles out communication time to all devices as per their communication parameters, trying to be fair  |                |
| ...            | ...        | ...      | ...            |
|                |            | host will poll the device as often as the device requested in its communication parameters asking if device has data to send or receive  |                |
| ...            | ...        | ...      | ...            |
|                |            |          | your software calls LibUSB's `usb_interrupt_read` |
|                |            | host waits for next polling interval and sends the request to the device  |                |
|                | if device is not busy, it acknowledges the request and calls an interrupt to send data  |          |                |
| firmware either already has data available and loads it into the send buffer and tells device to send, or it performs some computations before doing so, like reading IO ports|            |          |                |
|                | if firmware set the right flags and filled the transmit buffer, device will send data to host  |          |                |
|                |            | host will accept the data and pass it on to software as a return value  |                |
|                |            |          | software does something useful with the data  |

Writes are similar.


## What next? ##

Read [USBDesignProcedure](USBDesignProcedure.md) for an overview of how to design USB firmware.