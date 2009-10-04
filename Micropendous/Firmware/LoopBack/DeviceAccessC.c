/*	Purpose: Interact with USB device firmware - www.Micropendous.org/LoopBack
	Created: 2008-08-15 by Opendous Inc.
	Last Edit: 2009-10-03 by Opendous Inc.
	Released under the MIT License
*/

#include <stdio.h>
#include <usb.h>

/* all the critical information regarding the device and the interface and endpoints you plan to use */
#define VENDORID		0x03eb
#define PRODUCTID		0x204F
#define IN_EP			0x81
#define IN_EP_SIZE		64
#define OUT_EP			0x02
#define OUT_EP_SIZE		64
#define CONFIGNUM		1
#define INTERFACENUM		0
#define TIMEOUT			1500


int main(void)
{
	struct usb_bus *bus;
	struct usb_device *dev;
	usb_dev_handle *udev;
	char buffer[IN_EP_SIZE];
	int ret, i;

	/* initialize buffer to a sane value */
	for (i = 0; i < IN_EP_SIZE ; i++) {
		buffer[i] = 'A';
	}

	printf("STARTED DeviceAccessC\n");

	/* let libusb print all debug messages */
	usb_set_debug(255);

	usb_init();
	usb_find_busses();
	usb_find_devices();

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
				for (i = 0; i < 10 ; i++) {
					// read data from the device...
					ret = usb_interrupt_read(udev, IN_EP, buffer, IN_EP_SIZE, TIMEOUT);
					printf("[Read] returned: %d bytes, first 8 bytes are: (%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d)\n",
							ret, buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7]);

					// ...and then send the data back to the device
					ret = usb_interrupt_write(udev, OUT_EP, buffer, OUT_EP_SIZE, TIMEOUT);
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
