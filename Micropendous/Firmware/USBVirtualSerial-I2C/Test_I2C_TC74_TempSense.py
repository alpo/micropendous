#    Purpose: Communicate over I2C with a TC74 Temperature Sensor device connected to a Micropendous
#        programmed with USBVirtualSerial-I2C firmware.  Visit Micropendous.org for more info.
#    Created: 2009-04-15 by Opendous Inc.
#    Last Edit: 2010-03-10 by Opendous Inc.
#    Released under the MIT License
import serial           # for accessing the serial port on multiple platforms
import i2cfuncs         # useful i2c functions
import time             # needed for sleep function
import sys              # needed for command-line input


def Test_I2C_Communication(comport):
    # open the given serial port for communication
    ser = serial.Serial(comport)
    ser.setTimeout(2)
    ser.setWriteTimeout(2)

    print ser               # dump all info regarding serial port being used


    # communicate with a TC74 I2C Temperature Sensor
    # connect a 4.7k resistor between SDA and Vcc and between SCL and Vcc
    # device address is 1001101rw, where rw is read/^write bit
    # proper, 7-bit non-read/write-adjusted device address from datasheet is 1001101 = 0x4D
    TRGT_ADDRESS = 0x48

    # i2cfuncs usage:
    # ser is the Serial object which must support read() and write() functions and sends/receives data over I2C
    # TRGT_ADDRESS is the non-RW-adjusted, non-shifted, 7-bit device address from the device's datasheet
    # length, 3rd variable, is the length of data (number of bytes) to expect to be returned, 1 for writes
    # data is the tuple of data to send over I2C, which includes any sub-addresses
    # Note a 1 element tuple is created with a comma after the data (elem,)

    print "\n\nConfiguration Register:"
    i2cfuncs.i2c_readprint(ser, TRGT_ADDRESS, 1, (0x01,)) # send one byte, register address 0x01 and expect 1 byte, the status byte
    time.sleep(0.5)
    print "\n\nCurrent Temperature:"
    i2cfuncs.i2c_readprint(ser, TRGT_ADDRESS, 1, (0x00,)) # send one byte, register address 0x00 and expect 1 byte, the temperature


    ser.close()             # release/close the serial port

    """
    Expected Output under Windows:
    C:\Micropendous\Firmware\USBVirtualSerial-I2C>python  Test_I2C_TC74_TempSense.py  COM21
    Serial<id=0xba72d0, open=True>(port='COM21', baudrate=9600, bytesize=8, parity='N', stopbits=1, timeout=None, xonxoff=0, rtscts=0, dsrdtr=0)

    Current Temperature:

    76543210 - Hex- Int - Read from DeviceAddress: 0x4D SubAddress: 0x0 Length: 01 Data: ['\x1b']
    00011011 - 1b - 027


    Configuration Register:

    76543210 - Hex- Int - Read from DeviceAddress: 0x4D SubAddress: 0x1 Length: 01 Data: ['@']
    01000000 - 40 - 064
    """



# if this file is the program actually being run, print usage info or run Test_I2C_Communication
if __name__ == '__main__':
    if (len(sys.argv) != 2):
        print "TC74 Temperature Sensor I2C Serial Communication Example"
        print "    Usage:"
        print "      python", sys.argv[0], "<port>"
        print "        Where <port> = serial port; COM? on Windows, '/dev/ttyACM0 on Linux'"
        print "          Enumerated serial port can be found on Linux using dmesg"
        print "          look for something like  cdc_acm 2-1:1.0: ttyACM0: USB ACM device"
        print "      python", sys.argv[0], "COM5"
        exit()
    Test_I2C_Communication(sys.argv[1])
