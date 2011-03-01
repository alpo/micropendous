# Purpose: Use userial (www.tty1.net/userial) to communicate with an I2C device
# Released to the Public Domain by Opendous Inc. 2011-03-01
import serial    # PySerial (http://pyserial.sourceforge.net/) for accessing the serial port on multiple platforms
import sys       # command-line argument handling


def userialI2CScan(comport):
    # open the given serial port for communication
    ser = serial.Serial(comport) # COM5 or similar under Windows

    # 100kHz I2C = 64, 400kHz I2C = 190
    cmd = 'IC64'
    ser.write((cmd + "\n"))
    print "Command " + cmd + " returned: " + ser.readline()

    # userial command usage instructions can be found in userial_user_guide.pdf

    cmd = 'IS90W00S91R01P'
    ser.write((cmd + "\n"))
    print "Command " + cmd + " returned: " + ser.readline()

    cmd = 'IS90W01S91R01P'
    ser.write((cmd + "\n"))
    print "Command " + cmd + " returned: " + ser.readline()

    cmd = 'ISA0W00SA1R01P'
    ser.write((cmd + "\n"))
    print "Command " + cmd + " returned: " + ser.readline()

    cmd = 'ISA0W01SA1R01P'
    ser.write((cmd + "\n"))
    print "Command " + cmd + " returned: " + ser.readline()


    ser.close()

# if this file is the program actually being run, print usage info or run SerialSendReceive
if __name__ == '__main__':
    if (len(sys.argv) != 2):
        print "Use userial (www.tty1.net/userial) to Communicate with an I2C Device"
        print "    Usage:"
        print "      python", sys.argv[0], "<port>"
        print "        Where <port> = serial port; COM? on Windows, '/dev/ttyACM0 on Linux'"
        print "          Enumerated serial port can be found in Linux using dmesg"
        print "          look for something like  cdc_acm 2-1:1.0: ttyACM0: USB ACM device"
        print "      python", sys.argv[0], "COM5"
        exit()
    userialI2CScan(sys.argv[1])

