/*	Purpose: Test the throughput of a USB AVR device loaded with SpeedTest firmware
	Created: 2008-08-15 by Opendous Inc. - basic Custom-Class USB communication example
	Edit: 2009-08-28 by www.ilmarin.info - all SpeedTest-related code
	Last Edit: 2009-10-04 by Opendous Inc. - minor alterations
	Released under the MIT License

	Expected Output:
	Block_Size,Read_Speed(Kbytes/s),Write_Speed(Kbytes/s) - using transfer_size of 1048576
	16,     16.00,          16.00
	32,     32.00,          32.00
	64,     64.04,          63.97
	128,    127.83,         128.07
	256,    256.14,         256.14
	512,    512.28,         512.28
	1024,   1001.62,        512.28
	2048,   1016.80,        684.78
	4096,   1016.80,        818.40
	8192,   1016.80,        919.30
	16384,  1016.80,        906.88
	32768,  1016.80,        945.20
*/

#include <stdio.h>
#include <usb.h>
#include <time.h>
#include <sys/time.h>

/* all the critical information regarding the device and the interface and endpoints you plan to use */
#define VENDORID		0x03eb
#define PRODUCTID		0x204F
#define IN_EP			    0x81
#define IN_EP_SIZE		 64
#define OUT_EP			  0x02
#define OUT_EP_SIZE		 64
#define CONFIGNUM		     1
#define INTERFACENUM		 0
#define TIMEOUT			   2000


#define BUFFER_SIZE 102400

double run_write_benchmark(usb_dev_handle *udev,int block_size,int total_size)
{
	unsigned char buffer[BUFFER_SIZE];
	int bytes_written=0;
	struct timeval start,finish;
	int j, ret;

	gettimeofday(&start,0);

	if(block_size>BUFFER_SIZE) block_size=BUFFER_SIZE;

	for(j=0;j<block_size;j++)
		buffer[j]=j;

	/* TODO: this for() loop is where you should place your device interaction code */
	for(bytes_written = 0; bytes_written < total_size ;) {
		// write data to the device
		ret = usb_bulk_write(udev, OUT_EP, buffer, block_size, TIMEOUT);
		if(ret<0 || ret!=block_size)
		{
			printf("[Write] returned: %d , failed to write\n", ret);
			continue;
		}
		bytes_written+=ret;
		//fprintf(stderr,"W%d\t",ret);
		fflush(stderr);
	}
	gettimeofday(&finish,0);
	return (double)bytes_written/(finish.tv_sec-start.tv_sec+(finish.tv_usec-start.tv_usec)/1000000.0);
}

float run_read_benchmark(usb_dev_handle *udev,int block_size,int total_size)
{
	unsigned char buffer[BUFFER_SIZE];
	int bytes_read;
	int ret;
	struct timeval start,finish;

	gettimeofday(&start,0);

	if(block_size>BUFFER_SIZE) block_size=BUFFER_SIZE;


	/* TODO: this for() loop is where you should place your device interaction code */
	for (bytes_read = 0; bytes_read < total_size ;) {
		// read data from the device
		ret = usb_bulk_read(udev, IN_EP, buffer, block_size, TIMEOUT);
		if(ret<0)
		{
			printf("[Read] returned: %d , failed to read\n", ret);
			continue;
		}
		bytes_read+=ret;
		//fprintf(stderr,"R%d\t",ret);
		fflush(stderr);
	}
	gettimeofday(&finish,0);
	return (double)bytes_read/(finish.tv_sec-start.tv_sec+(finish.tv_usec-start.tv_usec)/1000000.0);
}


int main(void)
{
	struct usb_bus *bus;
	struct usb_device *dev;
	usb_dev_handle *udev;
	int transfer_size=1<<20; //1048576
	//int transfer_size=1<<18; //262144
	//int transfer_size=1<<16; //65536
	//int transfer_size=32;
	int i;

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

				printf("Please be patient, this will take a while...\n");
				printf("Block_Size,Read_Speed(Kbytes/s),Write_Speed(Kbytes/s) - using transfer_size of %d\n", transfer_size);
				for(i=4;i<16;i++)
				{
					int bs=1<<i;
					printf("%5d,\t%8.2f,\t%8.2f\n",bs,run_read_benchmark(udev,bs,transfer_size)/1000.0,run_write_benchmark(udev,bs,transfer_size)/1000.0);
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
