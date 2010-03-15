#    Purpose: I2C helper functions for use with Micropendous USBVirtualSerial-I2C-based devices
#                   Currently only supports devices with 8-bit registers
#    Created: 2009-04-15 by Opendous Inc.
#    Last Edit: 2010-03-07 by Opendous Inc.
#    Released under the MIT License
import binascii       # for converting received ASCII data to a workable form
import baseconvert    # for printing in binary format


# i2c_read: read data from an I2C device
#    device      - an object supporting read() and write() operations which sends bytes to a physical device over I2C
#    address     - proper, non-RW-adjusted, non-shifted, 7-bit I2C device address from its' datasheet
#    expectedlength        - length of data to read, i.e., number of bytes to read
#    data           - raw data tuple to send over I2C - should include any sub-addresses
#    @return    - return a list of the returned data bytes as 8-bit integers (0...255)
# Note that a 1-element tuple is created with a comma:  (0x15,)
def i2c_read(device, address, expectedRECVlength, data):
    # Com. format is : Read/^Write - Device Address - Expected # of Bytes to Receive - Data
    # Device Address is proper address from device datasheet, not R/W adjusted address
    # for reads, data byte(s) is irrelevent
    length = len(data)
    s = chr(1) + chr(address) + chr(length) + chr(expectedRECVlength)
    for i in data:
        s = s + chr(i)
    device.write(s)

    # first returned value is the number of data bytes that should be received
    index = 0
    count = int((binascii.hexlify((device.read(1)).strip("\ x"))), 16)
    returnedData = []
    # copy all bytes into a list
    while (index < count):
        returnedData.append(int((binascii.hexlify((device.read(1)).strip("\ x"))), 16))
        index = index + 1

    return returnedData



# i2c_write: write data to an I2C device
#    device      - an object supporting read() and write() operations which sends bytes to a physical device over I2C
#    address     - proper, non-RW-adjusted, non-shifted, 7-bit I2C device address from its' datasheet
#    writelength        - length of data to write, i.e., number of bytes to read
#    data           - raw data tuple to send over I2C - should include any sub-addresses
#    @return    - return a list of the data that was written as 8-bit integers (0...255)
# Note that a 1-element tuple is created with a comma:  (0x15,)
def i2c_write(device, address, writelength, data):
    # Com. format is : Read/^Write - Device Address - Sub-Address/Register - Length - Data
    # Device Address is proper address from device datasheet, not R/W adjusted address
    length = len(data)
    s = chr(0) + chr(address) + chr(length) + chr(writelength)
    writtenData = []
    for i in data:
        s = s + chr(i)
        writtenData.append(int(i))
    device.write(s)

    # first returned value is the number of data bytes that should be received
    # already know only one byte, count, is returned, but need to clear it from buffer
    count = int((binascii.hexlify((device.read(1)).strip("\ x"))), 16)

    return writtenData



# i2c_readprint: read data from an I2C device and print returned data nicely formatted
#    device      - an object supporting read() and write() operations which sends bytes to a physical device over I2C
#    address     - proper, non-RW-adjusted, non-shifted, 7-bit I2C device address from its' datasheet
#    expectedRECVlength        - length of data to read, i.e., number of bytes to read
#    data           - raw data tuple to send over I2C - should include any sub-addresses
# Note that a 1-element tuple is created with a comma:  (0x15,)
def i2c_readprint(device, address, expectedRECVlength, data):
    returnedData = i2c_read(device, address, expectedRECVlength, data)

    print "\n76543210 - Hex  - Int - Read from DeviceAddress:", hex(address), "SubAddress:", hex(data[0]), \
        "Length:", "%02d Data:" % int(expectedRECVlength), (returnedData)
    for i in range(len(returnedData)):
        print baseconvert.show_base(returnedData[i], 2, 8), \
            "-", hex(returnedData[i]), "-", \
            '%03d' % returnedData[i]



# i2c_writeprint: write data to an I2C device and print what data is being sent
#    device      - an object supporting read() and write() operations which sends bytes to a physical device over I2C
#    address     - proper, non-RW-adjusted, non-shifted, 7-bit I2C device address from its' datasheet
#    writelength        - length of data to write, i.e., number of bytes to read
#    data           - raw data tuple to send over I2C - should include any sub-addresses
# Note that a 1-element tuple is created with a comma:  (0x15,)
def i2c_writeprint(device, address, writelength, data):
    writtenData = i2c_write(device, address, writelength, data)

    hexVerWritData = []
    index = 0
    for i in writtenData:
        if (index <> 0):
            hexVerWritData.append(hex(i))
        index = index + 1

    print "\nWrote to DeviceAddress:", hex(address), "SubAddress:", hex(writtenData[0]), \
        "Length:", "%02d Data:" % (len(writtenData) - 1), hexVerWritData



if __name__ == '__main__':
    print "import this file into your programs"
