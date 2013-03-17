#    Purpose: Write then read characters from a serial port loopback device
#        Visit www.Micropendous.org/Serial for more info.
#    Created: 2009-09-31 by Opendous Inc.
#    Last Edit: 2009-10-02 by Opendous Inc.
#    Released to the Public Domain
import serial            # PySerial for accessing the serial port on multiple platforms
import sys               # command-line argument handling

# function which sends then reads data from the given serial port
# comport should be a string
def SerialSendReceive(comport):
    # open the given serial port for communication
    ser = serial.Serial(comport)
    ser.setTimeout(1000)
    ser.setWriteTimeout(1000)

    print ser                            # dump all info regarding serial port being used

    ser.write('www.Micropendous.org')    # send these characters to the serial port
    print ser.read(20)                   # read the above characters back, assuming loopback

    ser.write('www.Micropendous.org\n')  # send this line to the serial port
    print ser.readline()                 # read the returned line

    # be careful using readline() as it will block until a newline character is received

    ser.close()                          # release/close the serial port


# if this file is the program actually being run, print usage info or run SerialSendReceive
if __name__ == '__main__':
    if (len(sys.argv) != 2):
        print "Serial Communication Example"
        print "    Usage:"
        print "      python", sys.argv[0], "<port>"
        print "        Where <port> = serial port; COM? on Windows, '/dev/ttyACM0 on Linux'"
        print "          Enumerated serial port can be found on Linux using dmesg"
        print "          look for something like  cdc_acm 2-1:1.0: ttyACM0: USB ACM device"
        print "      python", sys.argv[0], "COM5"
        exit()
    SerialSendReceive(sys.argv[1])


