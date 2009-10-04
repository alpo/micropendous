#    Purpose: Test Throughput of LoopBack firmware - www.Micropendous.org/LoopBack
#    Created: 2008-08-15 by Opendous Inc.
#    Last Edit: 2009-10-03 by Opendous Inc.
#    Released Under the MIT License
import usb, time

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

transfersToComplete     = 2048

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
ScriptStartTime = time.time()
while (i < transfersToComplete):
    # read data from the device
    #buffer = handle.interruptRead(in_ep, in_ep_size, timeout)
    buffer = handle.bulkRead(in_ep, in_ep_size, timeout)
    #print buffer
    i = i+1


ScriptEndTime = time.time()
print "\nIt took", (ScriptEndTime - ScriptStartTime), "seconds to read", \
    (transfersToComplete * in_ep_size), "bytes for a throughput of", \
    (((transfersToComplete * in_ep_size) / (ScriptEndTime - ScriptStartTime)) / 1024), "kbytes/second"

# release the device (also called automatically on __del__)
handle.releaseInterface()

