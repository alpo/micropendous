#    Purpose: Communicate over SPI with a 23K256 SRAM device connected to a Micropendous
#        programmed with USBVirtualSerial-SPI firmware.  Visit Micropendous.org for more info.
#        NOTE: The 23K256 runs at 3.6V max so DO NOT connect it to a 5V USB AVR board
#    Created: 2009-04-15 by Opendous Inc.
#    Last Edit: 2011-02-25 by Opendous Inc.
#    Released under the MIT License
import serial           # for accessing the serial port on multiple platforms
import time             # needed for sleep function
import sys              # needed for command-line input
import binascii         # for converting received ASCII data to a workable form
import baseconvert      # for printing in binary format
import random


# spi_transfer: read and write data to/from a SPI device
#    device      - an object supporting read() and write() operations which sends bytes to a physical device over SPI
#    data           - raw data tuple of bytes to send over SPI - data width equal to number of bytes
#    @return    - return a list of the returned data bytes as 8-bit integers (0...255)
# Note that a 1-element tuple is created with a comma:  (0x15,)
def spi_transfer(device, data):
    # send data over SPI
    count = 0
    s = ""
    for i in data:
        s = s + chr(i)
        count = count + 1
    device.write(s)

    # read data returned over SPI and copy into a list of integers
    index = 0
    returnedData = []
    while (index < count):
        returnedData.append(int((binascii.hexlify((device.read(1)).strip("\ x"))), 16))
        index = index + 1

    return returnedData


# printFormatted: print data nicely formatted
#    dataSent        - the tuple of data bytes that was sent
#    dataReceived     - the list of integers that was received
#    printLevel  - amount of information to print, 0 = none, 1 = summary, 2 = complete
def printFormatted(dataSent, dataReceived, printLevel):
    if (printLevel == 1):
        print "Data Sent", dataSent, " Data Returned:", dataReceived
    if (printLevel > 1):
        print "76543210 - Hex  - Int - Data Sent", dataSent, " Data Returned:", dataReceived
        for i in range(len(dataReceived)):
            print baseconvert.show_base(dataReceived[i], 2, 8), \
                "-", hex(dataReceived[i]), "-", \
                '%03d' % dataReceived[i]



def Test_SPI_SRAM_23K256_Communication(comport):
    # open the given serial port for communication
    ser = serial.Serial(comport)
    ser.setTimeout(2000)
    ser.setWriteTimeout(2000)
    ser.bytesize = serial.EIGHTBITS
    ser.parity = serial.PARITY_NONE
    ser.stopbits = serial.STOPBITS_ONE

    ser.setBaudrate(57600)  # set the SPI clock speed to 500kHz (1MHz if AVR is running at 16MHz)

    randByte = random.randint(0, 255)

    print ser                # dump all info regarding serial port being used
    print "\n"

    print "write to status register to disable HOLD function"
    dataToSend = (0x01, 0x01)
    returnedData = spi_transfer(ser, dataToSend)
    printFormatted(dataToSend, returnedData, 1)
    print ""

    print "read status register (2nd byte returned should be 0x01)"
    dataToSend = (0x05, 0x00)
    returnedData = spi_transfer(ser, dataToSend)
    printFormatted(dataToSend, returnedData, 2)
    print ""

    print "read data (the 4th byte below) at address 0b0101010101010101 = 0x5555"
    dataToSend = (0x03, 0x55, 0x55, 0x00)
    returnedData = spi_transfer(ser, dataToSend)
    printFormatted(dataToSend, returnedData, 2)
    print ""

    print "write to address 0b0101010101010101 = 0x5555 data byte", randByte
    dataToSend = (0x02, 0x55,0x55, randByte)
    returnedData = spi_transfer(ser, dataToSend)
    printFormatted(dataToSend, returnedData, 1)
    print ""

    print "read data (the 4th byte below) at address 0b0101010101010101 = 0x5555 which should be", randByte
    dataToSend = (0x03, 0x55, 0x55, 0x00)
    returnedData = spi_transfer(ser, dataToSend)
    printFormatted(dataToSend, returnedData, 2)
    print ""


    ser.close()             # release/close the serial port


    """
    Expected Output under Windows with a 23K256 Device:
    C:\Micropendous\Firmware\USBVirtualSerial-SPI>python  Test_SPI_SRAM_23K256.py  COM5
	Serial<id=0xbb3090, open=True>(port='COM5', baudrate=57600, bytesize=8, parity='N', stopbits=1, timeout=2000, xonxoff=0, rtscts=0, dsrdtr=0)

	write to status register to disable HOLD function
	Data Sent (1, 1)  Data Returned: [255, 255]

	read status register (2nd byte returned should be 0x01)
	76543210 - Hex  - Int - Data Sent (5, 0)  Data Returned: [255, 1]
	11111111 - 0xff - 255
	00000001 - 0x1 - 001

	read data (the 4th byte below) at address 0b0101010101010101 = 0x5555
	76543210 - Hex  - Int - Data Sent (3, 85, 85, 0)  Data Returned: [127, 255, 255, 12]
	01111111 - 0x7f - 127
	11111111 - 0xff - 255
	11111111 - 0xff - 255
	00001100 - 0xc - 012

	write to address 0b0101010101010101 = 0x5555 data byte 175
	Data Sent (2, 85, 85, 175)  Data Returned: [255, 255, 255, 255]

	read data (the 4th byte below) at address 0b0101010101010101 = 0x5555 which should be 175
	76543210 - Hex  - Int - Data Sent (3, 85, 85, 0)  Data Returned: [255, 255, 255, 175]
	11111111 - 0xff - 255
	11111111 - 0xff - 255
	11111111 - 0xff - 255
	10101111 - 0xaf - 175
    """



# if this file is the program actually being run, print usage info or run Test_I2C_EEPROM_Communication
if __name__ == '__main__':
    if (len(sys.argv) != 2):
        print "SPI SRAM 23K256 Serial Communication Example"
        print "    Usage:"
        print "      python", sys.argv[0], "<port>"
        print "        Where <port> = serial port; COM? on Windows, '/dev/ttyACM0 on Linux'"
        print "          Enumerated serial port can be found on Linux using dmesg"
        print "          look for something like  cdc_acm 2-1:1.0: ttyACM0: USB ACM device"
        print "      python", sys.argv[0], "COM5"
        exit()
    Test_SPI_SRAM_23K256_Communication(sys.argv[1])
