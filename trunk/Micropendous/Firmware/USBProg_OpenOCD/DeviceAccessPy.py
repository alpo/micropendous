#    Purpose: Interact with USB device firmware
#    Created: 2008-08-15 by Opendous Inc.
#    Last Edit: 2009-11-18 by Opendous Inc.
#    Released Under the MIT License
import usb

REQDIR_HOSTTODEVICE = (0 << 7)
REQDIR_DEVICETOHOST = (1 << 7)
REQTYPE_STANDARD    = (0 << 5)
REQTYPE_CLASS       = (1 << 5)
REQTYPE_VENDOR      = (2 << 5)
REQREC_DEVICE       = (0 << 0)
REQREC_INTERFACE    = (1 << 0)
REQREC_ENDPOINT     = (2 << 0)
REQREC_OTHER        = (3 << 0)

UNKOWN_COMMAND	= 0x00
PORT_DIRECTION	= 0x01
PORT_SET	= 0x02
PORT_GET	= 0x03
PORT_SETBIT	= 0x04
PORT_GETBIT	= 0x05
WRITE_TDI	= 0x06
READ_TDO	= 0x07
WRITE_AND_READ = 0x08
WRITE_TMS	= 0x09
WRITE_TMS_CHAIN	= 0x0A

# all the critical information regarding the device and the interface and endpoints you plan to use.
vendorid    = 0x1781
productid   = 0x0C63
in_ep       = 0x82
in_ep_size  = 64
out_ep      = 0x03
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


# get port
buffer = (PORT_GETBIT, 0) # initialize the read/write buffer
i = 0
# write data to the device
handle.bulkWrite(out_ep, buffer, timeout)
# read data from the device
# buffer = handle.interruptRead(in_ep, in_ep_size, timeout)
buffer = handle.bulkRead(in_ep, 2, timeout)
print buffer, "- Port Get Bit - 5,1"


# set JTAG speed
buffer = () # initialize the read/write buffer
maxlen = 8 # size of buffer
bmRTmask = (REQDIR_DEVICETOHOST | REQTYPE_VENDOR | REQTYPE_STANDARD)
buffer = handle.controlMsg(requestType = bmRTmask, request = 2, value = 0, index = 123, buffer = maxlen, timeout = timeout)

# get JTAG speed
buffer = () # initialize the read/write buffer
maxlen = 8 # size of buffer
bmRTmask = (REQDIR_DEVICETOHOST | REQTYPE_VENDOR | REQTYPE_STANDARD)
buffer = handle.controlMsg(requestType = bmRTmask, request = 3, value = 0, index = 0, buffer = maxlen, timeout = timeout)
print buffer, "- Speed Set then Get - 123,0"

buffer = (PORT_GET, 1)
handle.bulkWrite(out_ep, buffer, timeout)
buffer = handle.bulkRead(in_ep, 2, timeout)
print buffer, "- Port Get - 3, ?"

buffer = (READ_TDO, 32, 0)
handle.bulkWrite(out_ep, buffer, timeout)
buffer = handle.bulkRead(in_ep, 2, timeout)
print buffer, "- READ_TDO - 7, ?"


buffer = (PORT_GETBIT, 0)
handle.bulkWrite(out_ep, buffer, timeout)
buffer = handle.bulkRead(in_ep, 2, timeout)
print buffer, "- Port Get Bit - 5, 1"




buffer = (PORT_SETBIT, 0, 1)
handle.bulkWrite(out_ep, buffer, timeout)
print "Just sent ", buffer, " - Set Bit 0 to 1"

buffer = (PORT_GETBIT, 0)
handle.bulkWrite(out_ep, buffer, timeout)
buffer = handle.bulkRead(in_ep, 2, timeout)
print buffer, "- Port Get Bit - 5, 0"


# release the device (also called automatically on __del__)
handle.releaseInterface()

