#    Purpose: Test the throughput of a Micropendous board loaded with Virtual Serial Port loopback firmware
#        Visit www.Micropendous.org/Serial for more info.
#    Created: 2008-09-30 by Opendous Inc.
#    Last Edit: 2009-10-03 by Opendous Inc.
#    Released under the MIT License
import serial             # for accessing the serial port on multiple platforms
import time, sys

# test Serial Port throughput by sending and receiving transfersToComplete number
# of transferSize sized strings
# comport    is a string - the Serial Port name to use
# transferSize    is an integer - the string lenght
# transfersToComplete    is an integer - the number of transfers to test with
def SerialThroughputTest(comport, transferSize, transfersToComplete):

    ser = serial.Serial(comport)         # open serial port for communication

    print ser             # dump all info regarding serial port being used

    ScriptStartTime = time.time()

    # create a transferSize length string
    i = 0
    s = ''
    while (i < (transferSize - 1)):
        s = s + 'A'
        i = i + 1
    # want last letter to be X
    s = s + 'X'

    # complete a series of transfers
    i = 0
    while (i < transfersToComplete):
        # send one string to device then get back the string
        ser.write(s)
        receivedData = ser.read(transferSize)
        #print receivedData
        i = i + 1

    ScriptEndTime = time.time()
    print "\nIt took", (ScriptEndTime - ScriptStartTime), "seconds to transfer", \
        (transfersToComplete * transferSize), "bytes for a throughput of", \
        (((transfersToComplete * transferSize) / (ScriptEndTime - ScriptStartTime)) / 1000), "kbytes/second"
    # be careful using readline as it will block until a newline character is received

    ser.close()             # release/close the serial port


# if this file is the program actually being run, print usage info or run SerialThroughputTest
if __name__ == '__main__':
    if (len(sys.argv) != 4):
        print "Serial Communication Throughput Testing"
        print "    Usage:"
        print "      python", sys.argv[0], "<port> <transferSize> <testSize>"
        print "        Where <port> = serial port; COM? on Windows, '/dev/ttyACM0 on Linux'"
        print "           Enumerated serial port can be found on Linux using dmesg"
        print "           look for something like  cdc_acm 2-1:1.0: ttyACM0: USB ACM device"
        print "        Where <transferSize> = the size of each transfer, this value should match"
        print "           CDC_TXRX_EPSIZE in Descriptors.h for maximum throughput"
        print "        Where <testSize> = how many transfers to complete, 100 is usually enough"
        print " "
        print "      python", sys.argv[0], "COM5  64  100"
        exit()
    SerialThroughputTest(sys.argv[1], int(sys.argv[2]), int(sys.argv[3]))