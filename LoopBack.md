The latest version of this firmware is available in the latest [Micropendous Distribution](http://code.google.com/p/micropendous/downloads/list) or via [SVN](http://code.google.com/p/micropendous/source/checkout) in the `/Micropendous/Firmware/LoopBack` directory.

**Purpose:** Communicate directly with a USB AVR device

**Source Code:** [View](http://code.google.com/p/micropendous/source/browse/trunk/Micropendous/Firmware/LoopBack) or [Download](http://www.Micropendous.org/Distribution)

**Targets:** [Micropendous1](Micropendous1.md), [Micropendous2](Micropendous2.md), [Micropendous3](Micropendous3.md), [Micropendous4](Micropendous4.md) or any USB AVR based hardware

**USB Mode:** Device-mode enumerating as a Custom-Class USB device

**Status:** _Working_


## Usage ##

Compile and load the LoopBack firmware onto your USB AVR by following the [ProgramAndTest](ProgramAndTest.md) tutorial.  Windows drivers for LoopBack are included in `/Micropendous/Firmware/LoopBack/WindowsDriver`.

Under Linux you would run the software communication example using `sudo make linux` after programming your Micropendous board with the LoopBack firmware.  Under Windows it would be `make win`.  Under Windows you first need to install [MingW](http://www.mingw.org/) and [LibUSB-Win32](http://libusb-win32.sourceforge.net/) according to [SetupLibUSBWindows](SetupLibUSBWindows.md).

The software and firmware demos simply transfer data between the host and device.


## Firmware Design ##

This firmware is based on [LUFA](http://www.fourwalledcubicle.com/LUFA.php)'s AudioInput firmware demo.

The firmware is set up so that any data received from the host is placed into the `dataReceived` buffer and any data to be sent to the host is stored in the `dataToSend` buffer.  This is done using either or both Interrupt and Control Endpoints.

`ReceiveDataFromHost()` and `SendDataToHost()` perform the buffer receiving/sending for the Interrupt Endpoints.  `Main_Task()` runs periodically and in this demo simply transfers data from one buffer to the other, creating a loopback.

`EVENT_USB_Device_UnhandledControlRequest(void)` transfers data to or from the buffers using the Control Endpoint.

### Communication using Interrupt Endpoints ###

`DeviceAccessC.c` is the cross-platform [LibUSB](http://libusb.sourceforge.net/)-based software that communicates with your device over Interrupt Endpoints.  The `for()` loop in `main()` is where all the actual data transfer occurs.  This is what you should alter.  See [libUSB Documentation](http://libusb.sourceforge.net/doc/) for more information.

```
/* TODO: this for() loop is where you should place your device interaction code */
for (i = 0; i < 10 ; i++) {
  // read data from the device...
  ret = usb_interrupt_read(udev, IN_EP, buffer, IN_EP_SIZE, TIMEOUT);

  // increment all values in data from the host
  for (j = 0; j < EP_SIZE; j++) {
    buffer[j] += 1;
  }

  // print the first eight values
  printf("[Read] returned: %d bytes, first 8 bytes are: (%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d)\n",
    ret, buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7]);

  // ...and then send the data back to the device
  ret = usb_interrupt_write(udev, OUT_EP, buffer, OUT_EP_SIZE, TIMEOUT);
  printf("[Write] returned: %d bytes successfully written\n", ret);
}

```

Now compare to the firmware, `LoopBack.c`:

```
/** Main_Task will only run if USB is connected */
void Main_Task(void)
{
  uint8_t i = 0;

  if (USB_DeviceState != DEVICE_STATE_Configured) { return; }

  ReceiveDataFromHost();  

  for (i = 0; i < OUT_EP_SIZE; i++) {
    dataToSend[i] = (uint8_t)(dataReceived[i]);
  }

  SendDataToHost();
}
```

There are also some important `#defines` for critical communication parameters such as device IDs and endpoints.

```
#define VENDORID        0x03eb
#define PRODUCTID       0x204F
#define EP_SIZE         64
#define IN_EP           0x81
#define IN_EP_SIZE      EP_SIZE
#define OUT_EP          0x02
#define OUT_EP_SIZE     EP_SIZE
#define CONFIGNUM       1
#define INTERFACENUM    0
#define TIMEOUT         1500
```

The `usb_set_debug(255);` line can be commented out if you do not want USB debugging messages.  All other code is basic to libUSB functionality and should no be altered.

`DeviceAccessPy.py` is a version of `DeviceAccessC.c` written in Python.  It requires [Python 2.5.5](http://www.python.org/download/releases/2.5.5/) and [PyUSB](http://developer.berlios.de/projects/pyusb/) to be installed.





### Communication using the Control Endpoint ###

`DeviceAccessC_ControlMSG.c` is the cross-platform [LibUSB](http://libusb.sourceforge.net/)-based software that communicates with your device over the Control Endpoint.  The `for()` loop in `main()` is where all the actual data transfer occurs.  This is what you should alter.  See [libUSB Documentation](http://libusb.sourceforge.net/doc/) for more information.

Communicating over the Control Endpoint is very straightforward.  Just match the `bmRequestType` and `bRequest` settings to those in the firmware.

```
/* TODO: this for() loop is where you should place your device interaction code */
for (i = 0; i < 10; i++) {
  // read data from the device and then send it right back, where the first byte should get incremented from 1 to 2
  // reading and writing from/to a device is simply a matter of matching bRequest and bmRequestType values
  bmRequestType = (REQDIR_DEVICETOHOST | REQTYPE_VENDOR | REQTYPE_STANDARD);
  bRequest = 1;   // LoopBack.c sends to data host (this software) with a bRequest of 1
  ret = usb_control_msg(udev, bmRequestType, bRequest, 0, 0, buffer, CNTRL_BUF_SIZE, TIMEOUT);

  // increment all values in data from the host
  for (j = 0; j < CNTRL_BUF_SIZE; j++) {
    buffer[j] += 1;
  }

  // print the first eight values
  printf("[Read] returned: %d bytes, first 8 bytes are: (%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d)\n",
  ret, buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7]);

  bmRequestType = (REQDIR_HOSTTODEVICE | REQTYPE_VENDOR | REQTYPE_STANDARD);
  bRequest = 2;   // LoopBack.c accepts data from host (this software) with a bRequest of 2
  ret = usb_control_msg(udev, bmRequestType, bRequest, 0, 0, buffer, CNTRL_BUF_SIZE, TIMEOUT);
  printf("[Write] returned: %d bytes successfully written\n", ret);
}

```

Now compare to the firmware, `LoopBack.c`:

```
void EVENT_USB_Device_UnhandledControlRequest(void)
{
  switch (USB_ControlRequest.bRequest)
  {
    case 1:
      if (USB_ControlRequest.bmRequestType == (REQDIR_DEVICETOHOST | REQTYPE_VENDOR | REQTYPE_STANDARD))
      {
        // Send data to the host
        // Acknowledge the SETUP packet, ready for data transfer
        Endpoint_ClearSETUP();

        // Write data to the control endpoint
        Endpoint_Write_Control_Stream_LE(&dataToSend, sizeof(dataToSend));

        // Finalize the stream transfer to send the last packet or clear the host abort
        Endpoint_ClearOUT();
      }
      break;

      case 2:
      if (USB_ControlRequest.bmRequestType == (REQDIR_HOSTTODEVICE | REQTYPE_VENDOR | REQTYPE_STANDARD))
      {
        // Receive data from the host
        // Acknowledge the SETUP packet, ready for data transfer
        Endpoint_ClearSETUP();

        // Read data in from the host
        Endpoint_Read_Control_Stream_LE(&dataReceived, sizeof(dataToSend));

        // Finalize the stream transfer to clear the last packet from the host
        Endpoint_ClearIN();
      }
      break;
  }
}

```


There are also some important `#defines` for critical communication parameters such as device IDs.

```
#define VENDORID        0x03EB
#define PRODUCTID       0x204F
#define CNTRL_BUF_SIZE  64
#define CONFIGNUM       1
#define INTERFACENUM    0
#define TIMEOUT         1500
```

The `usb_set_debug(255);` line can be commented out if you do not want USB debugging messages.  All other code is basic to libUSB functionality and should no be altered.

`DeviceAccessPy_ControlMSG.py` is a version of `DeviceAccessC_ControlMSG.c` written in Python.  It requires [Python 2.5.5](http://www.python.org/download/releases/2.5.5/) and [PyUSB](http://developer.berlios.de/projects/pyusb/) to be installed.

## Testing ##

There is a Python script in `/Micropendous/Firmware/LoopBack/` that calculates the throughput of the LoopBack firmware.

```
python TestLoopBackThroughput.py
```