#    Purpose: Communicate over SPI with an AVR programmed with SPI_AVRISP_Test Firmware
#        using a Micropendous board programmed with USBVirtualSerial-SPI firmware.
#        Visit Micropendous.org for more info.
#    Created: 2009-04-15 by Opendous Inc.
#    Last Edit: 2010-03-14 by Opendous Inc.
#    Released under the MIT License
import serial           # for accessing the serial port on multiple platforms
import time             # needed for sleep function
import sys              # needed for command-line input
import binascii         # for converting received ASCII data to a workable form
import baseconvert      # for printing in binary format


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



def Test_SPI_AVRISP_Test_Communication(comport):
    # open the given serial port for communication
    ser = serial.Serial(comport)
    ser.setTimeout(2000)
    ser.setWriteTimeout(2000)
    ser.bytesize = serial.EIGHTBITS
    ser.parity = serial.PARITY_NONE
    ser.stopbits = serial.STOPBITS_ONE

    ser.setBaudrate(9600)  # set the SPI clock speed to 125kHz (250kHz if AVR is running at 16MHz)

    print ser                # dump all info regarding serial port being used
    print "\n"

    # discard first value read
    dataToSend = (0x55,)
    returnedData = spi_transfer(ser, dataToSend)
    time.sleep(0.1)

    print "get first byte - should be 85"
    dataToSend = (0x55,)
    returnedData = spi_transfer(ser, dataToSend)
    printFormatted(dataToSend, returnedData, 1)
    time.sleep(0.1)
    print ""

    print "get upper byte"
    dataToSend = (0x55,)
    returnedDataByte1 = spi_transfer(ser, dataToSend)
    printFormatted(dataToSend, returnedDataByte1, 2)
    time.sleep(0.1)
    print ""

    print "get lower byte"
    dataToSend = (0x55,)
    returnedDataByte2 = spi_transfer(ser, dataToSend)
    printFormatted(dataToSend, returnedDataByte2, 2)
    time.sleep(0.1)
    print ""

    print "\nReceived Timer0 Value = ", ((returnedDataByte1[0] << 8) + returnedDataByte2[0]), "\n\n"

    time.sleep(0.5)

    print "get first byte - should be 85"
    dataToSend = (0x55,)
    returnedData = spi_transfer(ser, dataToSend)
    printFormatted(dataToSend, returnedData, 1)
    time.sleep(0.1)
    print ""

    print "get upper byte"
    dataToSend = (0x55,)
    returnedDataByte1 = spi_transfer(ser, dataToSend)
    printFormatted(dataToSend, returnedDataByte1, 2)
    time.sleep(0.1)
    print ""

    print "get lower byte"
    dataToSend = (0x55,)
    returnedDataByte2 = spi_transfer(ser, dataToSend)
    printFormatted(dataToSend, returnedDataByte2, 2)
    time.sleep(0.1)
    print ""

    print "\nReceived Timer0 Value = ", ((returnedDataByte1[0] << 8) + returnedDataByte2[0])


    ser.close()             # release/close the serial port


    """
    Expected Output under Windows:
    C:\Micropendous\Firmware\SPI_AVRISP_Test>python  Test_SPI_AVRISP_Test_Communication.py  COM5
    Serial<id=0xbab410, open=True>(port='COM5', baudrate=9600, bytesize=8, parity='N', stopbits=1, timeout=2000, xonxoff=0, rtscts=0, dsrdtr=0)

    get first byte - should be 170
    Data Sent (85,)  Data Returned: [170]

    get upper byte
    76543210 - Hex  - Int - Data Sent (85,)  Data Returned: [5]
    00000101 - 0x5 - 005

    get lower byte
    76543210 - Hex  - Int - Data Sent (85,)  Data Returned: [1]
    00000001 - 0x1 - 001

    Received Timer0 Value =  1281


    get first byte - should be 170
    Data Sent (85,)  Data Returned: [170]

    get upper byte
    76543210 - Hex  - Int - Data Sent (85,)  Data Returned: [18]
    00010010 - 0x12 - 018

    get lower byte
    76543210 - Hex  - Int - Data Sent (85,)  Data Returned: [0]
    00000000 - 0x0 - 000

    Received Timer0 Value =  4608
    """



# if this file is the program actually being run, print usage info or run SPI_AVRISP_Test
if __name__ == '__main__':
    if (len(sys.argv) != 2):
        print "SPI_AVRISP_Test Serial Communication Example"
        print "    Usage:"
        print "      python", sys.argv[0], "<port>"
        print "        Where <port> = serial port; COM? on Windows, '/dev/ttyACM0 on Linux'"
        print "          Enumerated serial port can be found on Linux using dmesg"
        print "          look for something like  cdc_acm 2-1:1.0: ttyACM0: USB ACM device"
        print "      python", sys.argv[0], "COM5"
        exit()
    Test_SPI_AVRISP_Test_Communication(sys.argv[1])
