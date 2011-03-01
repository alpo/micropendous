import serial
ser = serial.Serial("/dev/ttyACM1") # COM5 or similar under Windows

cmd = 'IC64'
ser.write((cmd + "\n"))
print "Command " + cmd + " returned: " + ser.readline()

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
