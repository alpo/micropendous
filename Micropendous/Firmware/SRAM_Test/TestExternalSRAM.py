#    Purpose: Test External SRAM on a Micropendous board with random data
#        Pattern testing is done in firmware.
#        Visit www.Micropendous.org for more info.
#    Created: 2009-08-23 by Opendous Inc.
#    Last Edit: 2011-02-24 by Opendous Inc.
#    Released under the MIT License
#
# Expected Output on correct function of a Micropendous with an AT90USB1287
#    Serial<id=0xbad2b0, open=True>(port='COM5', baudrate=9600, bytesize=8, ...
#    There are 57087 bytes of available SRAM in each of two banks for a total of 114174 bytes
#    Micropendous SRAM Pattern Testing Status: External_SRAM_Passed_Pattern_Tests
#    It took 342.937999964 seconds to test 114174 bytes of external SRAM

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
    print "\nMicropendous SRAM Pattern Testing Status:", ser.readline()

    ScriptStartTime = time.time()

    # create an array of random data bytes for each SRAM bank
    i = 0
    Bank0Array = []
    Bank1Array = []
    while (i < sizeOfSRAM):
        Bank0Array.append(random.randint(0, 255))
        Bank1Array.append(random.randint(0, 255))
        i = i + 1

    # run partial test if that is what the user wants
    SRAMTestLimit = sizeOfSRAM
    if (int(testMaxBytes) > 0):
        SRAMTestLimit = int(testMaxBytes)

    # fill SRAM bank 0 with random data
    i = 0
    ser.write('0')
    while (i < SRAMTestLimit):
        iByteMSB = ((i & 0xFF00) >> 8)
        iByteLSB = (i & 0x00FF)
        sendByteStream(ser, (0x67, iByteMSB, iByteLSB, Bank0Array[i]))
        i = i + 1

    # fill SRAM bank 1 with random data
    i = 0
    ser.write('1')
    while (i < SRAMTestLimit):
        iByteMSB = ((i & 0xFF00) >> 8)
        iByteLSB = (i & 0x00FF)
        sendByteStream(ser, (0x67, iByteMSB, iByteLSB, Bank1Array[i]))
        i = i + 1

    # check that SRAM bank 0 has the correct random data
    i = 0
    ser.write('0')
    while (i < SRAMTestLimit):
        iByteMSB = ((i & 0xFF00) >> 8)
        iByteLSB = (i & 0x00FF)
        sendByteStream(ser, (0x69, iByteMSB, iByteLSB))
        returnedData = ser.readline()
        p = re.compile('\d+')
        DataIntVal = int((p.findall(returnedData)[2]).strip())
        if (DataIntVal != Bank0Array[i]):
            print "SRAM Bank0 does not match sent data:", returnedData.strip(), \
                "[", baseconvert.show_base(DataIntVal, 2, 8), "] should be", Bank0Array[i], \
                "[", baseconvert.show_base(Bank0Array[i], 2, 8), "]"
        #print returnedData # print the actual data returned from the Micropendous
        i = i + 1

    # check that SRAM bank 1 has the correct random data
    i = 0
    ser.write('1')
    while (i < SRAMTestLimit):
        iByteMSB = ((i & 0xFF00) >> 8)
        iByteLSB = (i & 0x00FF)
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
    print "\nIt took", (ScriptEndTime - ScriptStartTime), "seconds to test", (2 * SRAMTestLimit), "bytes of external SRAM"
    if (int(testMaxBytes) > 0):
        print "It will take about", ((ScriptEndTime - ScriptStartTime) * (sizeOfSRAM / SRAMTestLimit)), "seconds to test the full external SRAM"


    ser.close()             # release/close the serial port


if __name__ == '__main__':
    if len(sys.argv) != 3:
        print "Micropendous External SRAM Test v2011-02-24 By Opendous Inc."
        print "  Test the external SRAM on a Micropendous board."
        print "    Usage:"
        print "      python", sys.argv[0], "<port> <>"
        print "        Where <port> = serial port; COM? on Windows, '/dev/ttyACM0 on Linux'"
        print "          Enumerated serial port can be found on Linux using dmesg"
        print "        Where <testMax> = maximum number of bytes to test; 0 for all"
        print "      python", sys.argv[0], "COM5  0"
        exit()
    TestSRAM(sys.argv[1], sys.argv[2])
