import serial
import time
from xbee import ZigBee

serial_port = serial.Serial('/dev/ttyUSB0', 9600)

def print_data(data):
    """called when data is received"""
    print "received data"
    print data
    
		 

xbee = ZigBee(serial_port, callback=print_data)

print "Attempting to receive..."

while True:
    try:
        time.sleep(0.001)
        """print "blocking read..." """
	
    except KeyboardInterrupt:
        break

xbee.halt()
serial_port.close()
