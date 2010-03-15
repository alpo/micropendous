#    Purpose: Plot data from a device loaded with USBVirtualSerial_ADC firmware
#        Visit www.Micropendous.org for more info.
#    Created: 2009-06-19 by Opendous Inc.
#    Last Edit: 2009-10-04 by Opendous Inc.
#    Released under the MIT License
import time
import matplotlib
import numpy as np
matplotlib.use('WXAgg') # do this before importing pylab
import serial
import sys, string, re
import matplotlib.pyplot as plt
import wx

global ser
global SAMPLE_SIZE
global starttimeFPS
global plotdata1
global plotdata2
global ax, fig, adcData


# function that gets 'datapoints' number of ADC data points from serial port ser
def get_ADC_Data(datapoints):
    global ser
    global SAMPLE_SIZE
    global starttimeFPS
    global plotdata1
    global plotdata2
    global ax, fig, adcData
    i = 0
    dataY1 = []
    dataX1 = []
    dataY2 = []
    dataX2 = []
    dataX1prev = 0
    dataX2prev = 0
    while (i < datapoints):
        ser.write('0')
        text = ser.readline()
        p = re.compile('\d+')
        values = p.findall(text)
        dataY1 = dataY1 + [int(values[0])]
        dataX1 = dataX1 + [(int(values[1]) - dataX1prev)]
        dataX1prev = int(values[1])
        ser.write('1')
        text = ser.readline()
        p = re.compile('\d+')
        values = p.findall(text)
        dataY2 = dataY2 + [int(values[0])]
        dataX2 = dataX2 + [(int(values[1]) - dataX2prev)]
        dataX2prev = int(values[1])
        i = i + 1
    return [dataX1, dataY1, dataX2, dataY2]



def update_plotdata(idleevent):
    global ser
    global SAMPLE_SIZE
    global starttimeFPS
    global plotdata1
    global plotdata2
    global ax, fig, adcData
    starttime = time.time()
    adcData = get_ADC_Data(SAMPLE_SIZE)
    endtime = time.time()
    plotdata1.set_xdata(np.arange(0, len(adcData[1]), 1))
    plotdata1.set_ydata(adcData[1])
    plotdata2.set_xdata(np.arange(0, len(adcData[3]), 1))
    plotdata2.set_ydata(adcData[3])
    endtimeFPS = time.time()
    title = "ADC Sampling at " + str(int(SAMPLE_SIZE / (endtime - starttime))) + " SPS" + \
        ", " + str(1 / (endtimeFPS - starttimeFPS)) + " FPS"
    ax.set_title(title)
    fig.canvas.draw_idle()                 # redraw the canvas
    starttimeFPS = time.time()

	
# function which plots ADC data from USBVirtualSerial_ADC
# comport = serial port name string
# SAMPLE_SIZE = integer, the number of ADC sample points to show per plot
def PlotADCData(comport, sampleSize):
    global ser
    global SAMPLE_SIZE
    global starttimeFPS
    global plotdata1
    global plotdata2
    global ax, fig, adcData

    SAMPLE_SIZE = sampleSize
    # open the given serial port for communication
    ser = serial.Serial(comport)

    print ser            # dump all info regarding serial port being used

    starttimeFPS = time.time()

    fig = plt.figure()

    ax = fig.add_subplot(111)

    adcData = get_ADC_Data(SAMPLE_SIZE)
    plotdata1, = ax.plot(np.arange(0, (len(adcData[1]) + 2), 1), (adcData[1] + [0] + [1023]), 'r')
    plotdata2, = ax.plot(np.arange(0, (len(adcData[3]) + 2), 1), (adcData[3] + [0] + [1023]), 'b')
    ax.set_ylabel('ADC Value')
    ax.set_xlabel('TimeStamp')
    ax.set_title('ADC Sampling at ? SPS, ? FPS')

    wx.EVT_IDLE(wx.GetApp(), update_plotdata)
    plt.show()

    ser.close()             # release/close the serial port


# if this file is the program actually being run, print usage info or run SerialSendReceive
if __name__ == '__main__':
    if (len(sys.argv) != 3):
        print "Plot ADC Data from USBVirtualSerial_ADC"
        print "    Usage:"
        print "      python", sys.argv[0], "<port> <plotPoints>"
        print "        Where <port> = serial port; COM? on Windows, '/dev/ttyACM0 on Linux'"
        print "          Enumerated serial port can be found on Linux using dmesg"
        print "          look for something like  cdc_acm 2-1:1.0: ttyACM0: USB ACM device"
        print "        Where <plotPoints> = the number of points per plot/frame, 128 is a good value"
        print "      python", sys.argv[0], "COM5  128"
        exit()
    PlotADCData(sys.argv[1], int(sys.argv[2]))
