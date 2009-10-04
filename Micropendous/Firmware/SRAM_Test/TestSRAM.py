#    Purpose: Test External SRAM on a Micropendous3 with random data
#        Visit www.Micropendous.org for more info.
#    Created: 2009-08-23 by Opendous Inc.
#    Last Edit: 2009-09-02 by Opendous Inc.
#    Released under the MIT License
#
# Expected Output on correct function of a Micropendous with an AT90USB647
#   Serial<id=0xbad2b0, open=True>(port='COM10', baudrate=9600, bytesize=8, ...
#   There are 61183 bytes of available SRAM in each of two banks for a total of 122366 bytes
#   Micropendous Internal SRAM Test Status: SRAM_Passed_Tests
#   It took  612.187999964 seconds to test 122366 bytes of SRAM
import serial               # for accessing the serial port on multiple platforms
import baseconvert    # for printing in binary format
import random, re, time, sys

def sendByteStream(device, data):
    s = ''
    for i in data:
        s = s + chr(i)
    device.write(s)

# Test the external SRAM of a Micropendous board
# comport    is a string - the Serial Port name to use
# testMaxBytes    is an integer - the maximum number of bytes to test or 0 for all
def TestSRAM(comport, testMaxBytes):
    ser = serial.Serial(comport)         # open the 11th serial port, COM11 (counting starts at 0) on Windows
    #ser = serial.Serial('/dev/ttyACM0') # under Linux, find serial port name with dmesg
                                         # look for something like  cdc_acm 2-1:1.0: ttyACM0: USB ACM device

    print ser             # dump all info regarding serial port being used

    # note  a = 0x61, f = 0x66, g = 0x67, i = 0x69,  s = 0x73

    # get the size of available external SRAM
    ser.write('s')
    sizeOfSRAM = int((ser.readline().strip()))
    print "\nThere are", sizeOfSRAM, "bytes of available SRAM in each of two banks", \
        "for a total of", (2 * sizeOfSRAM), "bytes"

    ser.write('t')
    print "\nMicropendous Internal SRAM Test Status:", ser.readline()

    ScriptStartTime = time.time()

    # create an array of random data bytes for each SRAM bank
    i = 0
    Bank0Array = []
    Bank1Array = []
    while (i < sizeOfSRAM):
        Bank0Array.append(random.randint(0, 255))
        Bank1Array.append(random.randint(0, 255))
        i = i + 1

    if (int(testMaxBytes) > 0):
        sizeOfSRAM = int(testMaxBytes)
    # fill each SRAM bank with corresponding data
    i = 0
    while (i < sizeOfSRAM):
        iByteMSB = ((i & 0xFF00) >> 8)
        iByteLSB = (i & 0x00FF)
        # test Bank 0
        ser.write('0')
        sendByteStream(ser, (0x67, iByteMSB, iByteLSB, Bank0Array[i]))
        #print "Bank0: ", ser.readline()
        # test Bank 1
        ser.write('1')
        sendByteStream(ser, (0x67, iByteMSB, iByteLSB, Bank1Array[i]))
        #print "Bank1: ", ser.readline()
        i = i + 1

    i = 0
    while (i < sizeOfSRAM):
        iByteMSB = ((i & 0xFF00) >> 8)
        iByteLSB = (i & 0x00FF)
        # test Bank 0
        ser.write('0')
        sendByteStream(ser, (0x69, iByteMSB, iByteLSB))
        returnedData = ser.readline()
        p = re.compile('\d+')
        DataIntVal = int((p.findall(returnedData)[2]).strip())
        if (DataIntVal != Bank0Array[i]):
            print "SRAM Bank0 does not match sent data:", returnedData.strip(), \
                "[", baseconvert.show_base(DataIntVal, 2, 8), "] should be", Bank0Array[i], \
                "[", baseconvert.show_base(Bank0Array[i], 2, 8), "]"
        #print returnedData # print the actual data returned from the Micropendous

        # test Bank 1
        ser.write('1')
        sendByteStream(ser, (0x69, iByteMSB, iByteLSB))
        returnedData = ser.readline()
        p = re.compile('\d+')
        DataIntVal = int((p.findall(returnedData)[2]).strip())
        if (DataIntVal != Bank1Array[i]):
            print "SRAM Bank1 does not match sent data:", returnedData.strip(), \
                "[", baseconvert.show_base(DataIntVal, 2, 8), "] should be", Bank1Array[i], \
                "[", baseconvert.show_base(Bank0Array[i], 2, 8), "]"
        #print returnedData # print the actual data returned from the Micropendous
        i = i + 1

    ScriptEndTime = time.time()
    print "\nIt took ", (ScriptEndTime - ScriptStartTime), "seconds to test", (2 * sizeOfSRAM), "bytes of SRAM"

    ser.close()             # release/close the serial port


if __name__ == '__main__':
    if len(sys.argv) != 3:
        print "Micropendous SRAM Test v2009-10-02 By Opendous Inc."
        print "  Test the external SRAM on a Micropendous board."
        print "    Usage:"
        print "      python", sys.argv[0], "<port> <>"
        print "        Where <port> = serial port; COM? on Windows, '/dev/ttyACM0 on Linux'"
        print "          Enumerated serial port can be found on Linux using dmesg"
        print "        Where <testMax> = maximum number of bytes to test; 0 for all"
        print "      python", sys.argv[0], "COM5  0"
        exit()
    TestSRAM(sys.argv[1], sys.argv[2])
