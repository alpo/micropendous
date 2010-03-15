/*	Purpose: Interact with USB device firmware using Control Endpoint
	Created: 2008-08-15 by Opendous Inc.
	Last Edit: 2010-03-03 by Opendous Inc.
	Released under the MIT License
*/

#include <stdio.h>
#include <usb.h>

/* all the critical information regarding the device and the interface and endpoints you plan to use */
#define VENDORID		0x03EB
#define PRODUCTID		0x204F
#define CNTRL_BUF_SIZE		64
#define CONFIGNUM		1
#define INTERFACENUM		0
#define TIMEOUT			1500


/* Control Message bmRequestType Masks */
#define REQDIR_HOSTTODEVICE	(0 << 7)
#define REQDIR_DEVICETOHOST	(1 << 7)
#define REQTYPE_STANDARD	(0 << 5)
#define REQTYPE_CLASS		(1 << 5)
#define REQTYPE_VENDOR		(2 << 5)
#define REQREC_DEVICE		(0 << 0)
#define REQREC_INTERFACE	(1 << 0)
#define REQREC_ENDPOINT		(2 << 0)
#define REQREC_OTHER		(3 << 0)

int main(void)
{
	struct usb_bus *bus;
	struct usb_device *dev;
	usb_dev_handle *udev;
	char buffer[CNTRL_BUF_SIZE];
	unsigned char bmRequestType;
	unsigned char bRequest;
	int ret, i, j;

	/* initialize buffer to a sane value */
	for (i = 0; i < CNTRL_BUF_SIZE ; i++) {
		buffer[i] = 0;
	}

	printf("STARTED DeviceAccessC_ControlMSG\n");

	/* let libusb print all debug messages */
	usb_set_debug(255);

	usb_init();
	usb_find_busses();
	usb_find_devices();

	/* loop over all busses and all devices and communicate with devices with the correct vendor and product IDs */
	/* Note that if you have several of the same devices connected, it will communicate with each one in turn */
	for (bus = usb_get_busses(); bus; bus = bus->next) {
		for (dev=bus->devices;dev;dev=dev->next) {

			if ((dev->descriptor.idVendor==VENDORID) && (dev->descriptor.idProduct==PRODUCTID)) {

				udev = usb_open(dev);

				/* tell libusb to use the CONFIGNUM configuration of the device */
				usb_set_configuration(udev, CONFIGNUM);

				/* if your device has an alternate setting for this interface, you can change to it */
				//usb_set_altinterface(udev, INTERFACENUM_OTHER);

				#ifdef LIBUSB_HAS_DETACH_KERNEL_DRIVER_NP
				/* detach the automatically assigned kernel driver from the current interface */
				usb_detach_kernel_driver_np(udev, INTERFACENUM);
				#endif

				/* claim the interface for use by this program */
				usb_claim_interface(udev, INTERFACENUM);


				/* TODO: this for() loop is where you should place your device interaction code */
				for (i = 0; i < 10; i++) {
					// read data from the device and then send it right back, where the first byte should get incremented from 1 to 2
					// reading and writing from/to a device is simply a matter of matching bRequest and bmRequestType values
					bmRequestType = (REQDIR_DEVICETOHOST | REQTYPE_VENDOR | REQTYPE_STANDARD);
					bRequest = 1;	// LoopBack.c sends to data host (this software) with a bRequest of 1
					ret = usb_control_msg(udev, bmRequestType, bRequest, 0, 0, buffer, CNTRL_BUF_SIZE, TIMEOUT);

					// increment all values in data from the host
					for (j = 0; j < CNTRL_BUF_SIZE; j++) {
						buffer[j] += 1;
					}

					// print the first eight values
					printf("[Read] returned: %d bytes, first 8 bytes are: (%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d)\n",
							ret, buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7]);

					bmRequestType = (REQDIR_HOSTTODEVICE | REQTYPE_VENDOR | REQTYPE_STANDARD);
					bRequest = 2;	// LoopBack.c accepts data from host (this software) with a bRequest of 2
					ret = usb_control_msg(udev, bmRequestType, bRequest, 0, 0, buffer, CNTRL_BUF_SIZE, TIMEOUT);
					printf("[Write] returned: %d bytes successfully written\n", ret);
				}


				/* make sure other programs can still access this device */
				/* release the interface and close the device */
				usb_release_interface(udev, INTERFACENUM);
				usb_close(udev);

			}
		}
	}
	return 0;
}
