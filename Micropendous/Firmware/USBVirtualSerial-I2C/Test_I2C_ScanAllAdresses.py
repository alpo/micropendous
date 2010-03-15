#    Purpose: Attempt communication over I2C at all possible addresses with
#        devices connected to a Micropendous board programmed with
#        USBVirtualSerial-I2C firmware.  Visit Micropendous.org for more info.
#    Created: 2009-04-15 by Opendous Inc.
#    Last Edit: 2010-03-10 by Opendous Inc.
#    Released under the MIT License
import serial           # for accessing the serial port on multiple platforms
import i2cfuncs         # useful i2c functions
import time             # needed for sleep function
import sys              # needed for command-line input


def Test_I2C_Communication(comport, likelyRegister):
    # open the given serial port for communication
    ser = serial.Serial(comport)
    ser.setTimeout(2000)
    ser.setWriteTimeout(2000)

    print ser               # dump all info regarding serial port being used

    # Search for devices across all I2C addresses
    i = 0
    while (i < 128):
        i2cfuncs.i2c_readprint(ser, i, 1, (int(likelyRegister),)) # send one byte, the likely register address and expect 1 byte
        time.sleep(0.15)
        i = i + 1

    ser.close()             # release/close the serial port



# if this file is the program actually being run, print usage info or run Test_I2C_Communication
if __name__ == '__main__':
    if (len(sys.argv) != 3):
        print "I2C Address Scan for Micropendous USBVirtualSerial_I2C"
        print "    Usage:"
        print "      python", sys.argv[0], "<port> <likelyRegister>"
        print "        Where <port> = serial port; COM? on Windows, '/dev/ttyACM0 on Linux'"
        print "          Enumerated serial port can be found on Linux using dmesg"
        print "          look for something like  cdc_acm 2-1:1.0: ttyACM0: USB ACM device"
        print "        Where <likelyRegister> = an 8-bit register address in your device (in integer form)"
        print "          likely to have a value. Choose a status register. For most devices this will be register 0"
        print "      python", sys.argv[0], "COM5  0"
        exit()
    if ((int(sys.argv[2]) > 255) or (int(sys.argv[2]) < 0)):
        print "Selected I2C Register is not correct, it must be >= 0 and <= 255"
        print "Run   python", sys.argv[0], "   for help."
        exit()
    Test_I2C_Communication(sys.argv[1], sys.argv[2])
