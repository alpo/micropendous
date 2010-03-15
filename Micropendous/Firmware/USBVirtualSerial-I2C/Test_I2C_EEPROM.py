#    Purpose: Communicate over I2C with a device connected to a Micropendous
#        programmed with USBVirtualSerial-I2C firmware.  Visit Micropendous.org for more info.
#    Created: 2009-04-15 by Opendous Inc.
#    Last Edit: 2010-03-07 by Opendous Inc.
#    Released under the MIT License
import serial           # for accessing the serial port on multiple platforms
import i2cfuncs         # useful i2c functions
import time             # needed for sleep function
import sys              # needed for command-line input


def Test_I2C_EEPROM_Communication(comport):
    # open the given serial port for communication
    ser = serial.Serial(comport)
    ser.setTimeout(2000)
    ser.setWriteTimeout(2000)

    print ser               # dump all info regarding serial port being used


    # communicate with a 24AA08 I2C EEPROM
    # Why the 24AA08?  It is a cheap and simple DIP I2C device that works at all Micropendous voltages
    # connect WP to Vss/GND to disable WriteProtect
    # connect a 2k to 15k resistor between SDA and Vcc and between SCL and Vcc
    # A0, A1, A2 are unused by the device, so leave unconnected
    # device address is 1010B2B1B0, but B2 is unused in the 24AA08
    # therefore B0 and B1 determine the current block to read/write from
    # the 24AA08 is organized as 4 blocks of 256 x 8-bit bytes for 1kilobyte/8kilobit of data
    # for testing, will use block 0, so device address is 1010000 = 0x50
    TRGT_ADDRESS = 0x50

    # i2cfuncs usage:
    # ser is the Serial object which must support read() and write() functions and sends/receives data over I2C
    # TRGT_ADDRESS is the non-RW-adjusted, non-shifted, 7-bit device address from the device's datasheet
    # length, 3rd variable, is the length of data (number of bytes) to expect to be returned, 1 for writes
    # data is the tuple of data to send over I2C, which includes any sub-addresses
    # Note a 1 element tuple is created with a comma after the data (elem,)
    i2cfuncs.i2c_readprint(ser, TRGT_ADDRESS, 1, (0x05,)) # send one byte, the page address 0x05 and expect 1 byte
    i2cfuncs.i2c_write(ser, TRGT_ADDRESS, 1, (0x05,0x37)) # write one byte (value=0x37) to address 0x05
    time.sleep(0.5) # need a delay for the EEPROM to be written as data is internally buffered
    i2cfuncs.i2c_writeprint(ser, TRGT_ADDRESS, 1, (0x07,0xAB)) # write one byte (value=0xAB) to address 0x07, print information
    time.sleep(0.5) # need a delay for the EEPROM to be written as data is internally buffered
    i2cfuncs.i2c_writeprint(ser, TRGT_ADDRESS, 4, (0x08,0xDE, 0xAD, 0xBE, 0xEF)) # write 4 bytes to address 0x08
    time.sleep(0.5) # need a delay for the EEPROM to be written as data is internally buffered
    i2cfuncs.i2c_readprint(ser, TRGT_ADDRESS, 12, (0x05,)) # send page address 0x05 and nicely print 12 bytes of data starting at that address
    returnedDataList = i2cfuncs.i2c_read(ser, TRGT_ADDRESS, 12, (0x05,)) # send page address 0x05 and expect 12 bytes of data in a list
    print "\ni2c_read result = ", returnedDataList # print the list returned by i2cfuncs.i2c_read

    ser.close()             # release/close the serial port

    """
    Expected Output under Windows with an unprogrammed 24AA08:
    C:\Micropendous\Firmware\USBVirtualSerial-I2C>python Test_I2C_EEPROM.py COM21
    Serial<id=0xba72d0, open=True>(port='COM21', baudrate=9600, bytesize=8, parity='N', stopbits=1, timeout=None, xonxoff=0, rtscts=0, dsrdtr=0)

    76543210 - Hex  - Int - Read from DeviceAddress: 0x50 SubAddress: 0x5 Length: 01 Data: [255]
    00110111 - 0xff - 255

    Wrote to DeviceAddress: 0x50 SubAddress: 0x7 Length: 01 Data: ['0xab']

    Wrote to DeviceAddress: 0x50 SubAddress: 0x8 Length: 04 Data: ['0xde', '0xad', '0xbe', '0xef']

    76543210 - Hex  - Int - Read from DeviceAddress: 0x50 SubAddress: 0x5 Length: 12 Data: [55, 255, 171, 222, 173, 190, 239, 255, 255, 255, 255, 255]
    00110111 - 0x37 - 055
    11111111 - 0xff - 255
    10101010 - 0xab - 171
    11011110 - 0xde - 222
    10101101 - 0xad - 173
    10111110 - 0xbe - 190
    11101111 - 0xef - 239
    11111111 - 0xff - 255
    11111111 - 0xff - 255
    11111111 - 0xff - 255
    11111111 - 0xff - 255
    11111111 - 0xff - 255

    i2c_read result =  [55, 255, 170, 222, 173, 190, 239, 255, 255, 255, 255, 255]

    """



# if this file is the program actually being run, print usage info or run Test_I2C_EEPROM_Communication
if __name__ == '__main__':
    if (len(sys.argv) != 2):
        print "I2C EEPROM Serial Communication Example"
        print "    Usage:"
        print "      python", sys.argv[0], "<port>"
        print "        Where <port> = serial port; COM? on Windows, '/dev/ttyACM0 on Linux'"
        print "          Enumerated serial port can be found on Linux using dmesg"
        print "          look for something like  cdc_acm 2-1:1.0: ttyACM0: USB ACM device"
        print "      python", sys.argv[0], "COM5"
        exit()
    Test_I2C_EEPROM_Communication(sys.argv[1])
