# Purpose: Use userial (www.tty1.net/userial) to scan for I2C devices
# Released to the Public Domain by Opendous Inc. 2011-03-01
import serial    # PySerial (http://pyserial.sourceforge.net/) for accessing the serial port on multiple platforms
import sys       # command-line argument handling
import string    # userial command formatting


def userialI2CScan(comport):
    # open the given serial port for communication
    ser = serial.Serial(comport) # COM5 or similar under Windows

    print ser, "\n"

    cmd = 'V'
    ser.write((cmd + "\n"))
    print "Command " + cmd + " returned: " + ser.readline()

    # 100kHz I2C
    cmd = 'IC64'
    ser.write((cmd + "\n"))
    print "Command " + cmd + " returned: " + ser.readline()

    # I2C Bus Clear
    cmd = 'IX'
    ser.write((cmd + "\n"))
    print "Command " + cmd + " returned: " + ser.readline()

    # Reset I2C devices
    cmd = 'IS00W06P'
    ser.write((cmd + "\n"))
    print "Command " + cmd + " returned: " + ser.readline()

    # from device 0x50 try to read (0x50 + 1) a byte
    cmd = 'IS51R01P'
    ser.write((cmd + "\n"))
    print "Command " + cmd + " returned: " + ser.readline()

    # try to read a byte from device 0x50 at address 0x01
    cmd = 'IS50W01S51R01P'
    ser.write((cmd + "\n"))
    print "Command " + cmd + " returned: " + ser.readline()

    print "\n\nAnd now for the I2C bus scan results that did not return a Nack:"

    # at every possible device address attempt a read
    i = 1
    cmd = []
    while (i < 256):
        if (0 < i < 16):
            cmd = "IS0" + string.strip(hex(i), "0x") + "R01P\n"
        elif ((i%16 == 0)):
            cmd = "IS" + string.strip(hex(i), "0x") + "0R01P\n"
        else:
            cmd = "IS" + string.strip(hex(i), "0x") + "R01P\n"
        ser.write(cmd)
        retrnd = ser.readline()
        if (("Nack" not in retrnd)):
            print "\n\nCommand at address " + hex(i) + ":", cmd
            print "Returned:", retrnd

        i = i + 2


    # at every possible device address attempt a read from register 0x00
    i = 2
    cmd = []
    retrnd = []
    while (i < 128):
        hexStr0 = string.strip(hex(i), "0x")
        hexStr1 = string.strip(hex((i+1)), "0x")
        if (0 <= i < 16):
            cmd2 = "IS0" + hexStr0 + "W00S0" + hexStr1 + "R01P\n"
        elif ((i%16 == 0)):
            cmd2 = "IS" + hexStr0 + "0W00S" + hexStr1 + "R01P\n"
        else:
            cmd2 = "IS" + hexStr0 + "W00S" + hexStr1 + "R01P\n"
        ser.write(cmd2)
        retrnd = ser.readline()
        if (("Nack" not in retrnd)):
            print "\n\nCommand at address " + hex(i) + ":", cmd2
            print "Returned:", retrnd

        i = i + 2

    # the above loop fails for device 0x00
    ser.write('IS00W01S01R01P\n') 
    retrnd = ser.readline()
    if (("Nack" not in retrnd)):
        print "\n\nCommand:", "IS00W01S01R01P\n"
        print "Returned:", retrnd


    ser.close()			 # release/close the serial port


# if this file is the program actually being run, print usage info or run SerialSendReceive
if __name__ == '__main__':
    if (len(sys.argv) != 2):
        print "userial (www.tty1.net/userial) I2C Device Bus Scan"
        print "    Usage:"
        print "      python", sys.argv[0], "<port>"
        print "        Where <port> = serial port; COM? on Windows, '/dev/ttyACM0 on Linux'"
        print "          Enumerated serial port can be found in Linux using dmesg"
        print "          look for something like  cdc_acm 2-1:1.0: ttyACM0: USB ACM device"
        print "      python", sys.argv[0], "COM5"
        exit()
    userialI2CScan(sys.argv[1])

