#    Purpose: Communicate over I2C with a device connected to a Micropendous
#        programmed with USBVirtualSerial_I2C firmware.  Visit Micropendous.org for more info.
#    Created: 2009-04-15 by Opendous Inc.
#    Last Edit: 2009-10-04 by Opendous Inc.
#    Released under the MIT License
import serial           # for accessing the serial port on multiple platforms
import i2cfuncs         # useful i2c functions
import time             # needed for sleep function
import sys              # needed for command-line input


def Test_I2C_EEPROM_Communication(comport):
    # open the given serial port for communication
    ser = serial.Serial(comport)

    print ser               # dump all info regarding serial port being used


    # communicate with a 24AA08 I2C EEPROM
    # connect WP to Vss/GND to disable WriteProtect
    # connect a 4k to 15k resistor between SDA and Vcc and between SCL and Vcc
    # A0,A1, A2 are unused by the device, so leave unconnected
    # device address is 1010B2B1B0, but B2 is unused in the 24AA08
    # therefore B0 and B1 determine the current block to read/write from
    # the 24AA08 is organized as 4 blocks of 256 x 8-bit bytes for 1kilobyte/8kilobit of data
    # for testing, will use block 0, so device address is 1010000 = 0x50
    TRGT_ADDRESS = 0x50

    # i2cfuncs usage:
    # ser is the Serial object which must support read() and write() functions
    # TRGT_ADDRESS is the non-RW-adjusted, non-shifted, 7-bit device address from its' datasheet
    # length, 3rd variable, is the length of data (number of bytes) to expect to be returned
    # data is the tuple of data to send over I2C, which includes any sub-addresses
    # Note a 1 element tuple is created with a comma after the data (elem,)
    i2cfuncs.i2c_readprint(ser, TRGT_ADDRESS, 1, (0x05,)) # send one byte, the page address 5 and expect one byte
    i2cfuncs.i2c_writeprint(ser, TRGT_ADDRESS, 1, (0x05,0x42)) # write one byte (value=0x77) to address 0x05
    time.sleep(0.5) # need a delay for the EEPROM value above to be written when using software-based I2C firmware
    i2cfuncs.i2c_writeprint(ser, TRGT_ADDRESS, 1, (0x07,0x47)) # write one byte (value=0x79) to address 0x07
    time.sleep(0.5) # need a delay for the EEPROM value above to be written when using software-based I2C firmware
    i2cfuncs.i2c_readprint(ser, TRGT_ADDRESS, 5, (0x05,)) # send one byte, the page address 5 and expect 5 bytes of data
    # this last call is an example of multi-byte read - just tell i2cfuncs that you expect more data

    """
    Expected Output under Windows:
    C:\Micropendous\Firmware\USBVirtualSerial-I2C>python serialpy.py
    Serial<id=0xba60d0, open=True>(port='COM3', baudrate=9600, bytesize=8, parity='N', stopbits=1, timeout=None, xonxoff=0, rtscts=0, dsrdtr=0)

    76543210 - Hex- Int - Read from DeviceAddress: 0x50 SubAddress: 0x5 Length: 01 Data: ['c']
    01100011 - 77 - 099

    Wrote to DeviceAddress: 0x50 SubAddress: 0x77 Length: 02 Data: 0x5,0x42,

    Wrote to DeviceAddress: 0x50 SubAddress: 0x79 Length: 02 Data: 0x7,0x47,

    76543210 - Hex- Int - Read from DeviceAddress: 0x50 SubAddress: 0x5 Length: 05 Data: ['B', '\xff', 'G', '\xff', '\xff']
    01110111 - 42 - 66
    11111111 - ff - 255
    01111001 - 47 - 71
    11111111 - ff - 255
    11111111 - ff - 255

    """
    ser.close()             # release/close the serial port



# if this file is the program actually being run, print usage info or run SerialSendReceive
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