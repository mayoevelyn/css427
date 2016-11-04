import serial
from xbee import ZigBee, XBee

serial_port = serial.Serial('/dev/ttyUSB0', 9600)

#xbee = XBee(serial_port)
xbee = ZigBee(serial_port)

print "Attempting to receive..."

while True:
    try:
        response = xbee.wait_read_frame()
        print response
	
    except KeyboardInterrupt:
        break

serial_port.close()
