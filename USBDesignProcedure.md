## Zen of USB Design ##

The goal of this tutorial is to give people who have experience programming microcontrollers a sense of how to easily extend their designs with native USB communication, without the use of intermediary devices.  This tutorial assumes you have read or are familiar with the material in [BasicUSBDeviceOverview](BasicUSBDeviceOverview.md).

A few quick definitions.  Firmware is software that resides on the peripheral device, such as a Micropendous board.  Software resides on the host/computer.  This page assumes [LUFA](http://www.fourwalledcubicle.com/LUFA.php) is the library used for firmware development and [LibUSB](http://libusb.sourceforge.net/) is used for software development.


## How much data do you need to move ##

The MCU in an Micropendous can have several endpoints, each either 8, 16, 32, or 64 bytes in size.  These are the only endpoint sizes available across all USB AVRs.

Consider `Descriptors.h` of the LoopBack demo (available in the latest [Distribution](http://code.google.com/p/micropendous/downloads/list)):
```
   #define IN_EP                       1
   #define OUT_EP                      2
   #define IN_EP_SIZE                  8
   #define OUT_EP_SIZE                 8
```

An IN and an OUT endpoint are defined and each stores 8 bytes.  Note that `IN_EP` will have address `0x81` on the host and `OUT_EP` will have address `0x02` on the host.  This is just how it is.  `IN` endpoints have 0x80 added to their address.

Remember that the larger the endpoint size, the more data has to be moved each transfer.  Since USB is a bussed architecture, this will reduce available capacity for other devices.  Use as little as you need.


## Interrupt or Bulk Transfers ##

LibUSB only supports interrupt and bulk transfers.  Bulk transfers use spare capacity but the host will flood a USB peripheral device with "Have any data yet?" requests.  For interrupt transfers, the host will ask the device if it has any data it wants to send or receive every few milliseconds as defined by `PollingIntervalMS` in `USB_Descriptor_Configuration_t` in `Descriptors.c`:
```
DataINEndpoint:
{
    Header: {Size: sizeof(USB_Descriptor_Endpoint_t), Type: DTYPE_Endpoint},
    EndpointAddress:    (ENDPOINT_DESCRIPTOR_DIR_IN | IN_EP),
    Attributes:         EP_TYPE_INTERRUPT,
    EndpointSize:       IN_EP_SIZE,
    PollingIntervalMS:  0x02
```

Interrupt transfers are the easiest and safest to use as you know the time between transfers and can code your program accordingly to maximize throughput.


## How is the above 'programmed' ##

`USB_Descriptor_Configuration_t  USB_Descriptor_Configuration_t` in `Descriptors.c` just formats information about the device's capability for transferring to the host.  It is best to alter existing code than to write the configuration descriptor code from scratch.  Note `DeviceDescriptor` contains details such as the Vendor and Product IDs.  You can either use Atmel's Vendor ID and `0x0000` for Product ID (general purpose testing), or use LUFA's Product IDs if your device has a similar function.  Last resort is to buy IDs from the [USB-IF](http://www.usb.org).


## Software ##

The above happens on the device.  On the PC side of things, both `DeviceAccessC.c` and `DeviceAccessPy.py` in the LoopBack demo follow the same procedure.

  1. define important device specific details like IDs and endpoints
  1. search through the USB bus for a device that matches the Vendor and Product IDs
  1. initialize communication with the device by opening it, setting which configuration to use, and then claiming the interface
  1. start a communication loop with the device.  This is where the action is and where your code should focus on.
    * `usb_interrupt_read(udev, IN_EP, buffer, IN_EP_SIZE, TIMEOUT)` in C or `handle.interruptRead(in_ep, in_ep_size, timeout)` in Python
    * `usb_interrupt_write(udev, OUT_EP, buffer, OUT_EP_SIZE, TIMEOUT)` in C or `handle.interruptWrite(out_ep, buffer, timeout)` in Python
    * the above functions either read or write a local buffer (`char` array in C, `tuple` in Python) to the device
    * `ISR(ENDPOINT_PIPE_vect)` in `LoopBack.c` firmware then tests for either `if (Endpoint_HasEndpointInterrupted(IN_EP))` or `if (Endpoint_HasEndpointInterrupted(OUT_EP))` and does whatever you tell it to do with data communicated between the device and the host
  1. release and close the device