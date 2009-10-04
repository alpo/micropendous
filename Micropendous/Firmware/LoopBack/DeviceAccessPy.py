#    Purpose: Interact with USB device firmware - www.Micropendous.org/LoopBack
#    Created: 2008-08-15 by Opendous Inc.
#    Last Edit: 2009-10-03 by Opendous Inc.
#    Released Under the MIT License
import usb

# all the critical information regarding the device and the interface and endpoints you plan to use.
vendorid    = 0x03EB
productid   = 0x204F
in_ep       = 0x81
in_ep_size  = 64
out_ep      = 0x02
out_ep_size = 64
confignum   = 1
interfacenum= 0
timeout     = 1500


# loop over all busses and all devices and find the one with the correct vendor and product IDs
print "STARTED DeviceAccessPy"
busses = usb.busses()
for bus in busses:
    devices = bus.devices
    for dev in devices:
        if (dev.idVendor == vendorid) & (dev.idProduct == productid):
            founddev = dev
            foundbus = bus

# the device has been found, otherwise crash
bus = foundbus
dev = founddev

# open the device for communication
handle = dev.open()

# choose which of the device's configurations to use
handle.setConfiguration(confignum)

# choose which interface to interact with
handle.claimInterface(interfacenum)


#TODO: this while() loop is where you should place your device interaction code
buffer = () # initialize the read/write buffer
i = 0
while (i < 10):
    # read data from the device
    buffer = handle.interruptRead(in_ep, in_ep_size, timeout)
    # write data to the device
    handle.interruptWrite(out_ep, buffer, timeout)
    print buffer
    i = i+1


# release the device (also called automatically on __del__)
handle.releaseInterface()

