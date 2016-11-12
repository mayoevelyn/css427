#! /usr/bin/python

from xbee import ZigBee, XBee
import serial

def main():
    try:
        
        # Open serial port
        ser = serial.Serial('/dev/ttyUSB0', 9600)
        
        # Create XBee Series 2 object
        xbee = ZigBee(ser)
        
        # Send packet
        xbee.send('at', frame_id='A', command='DH')
        
        # Wait for response
        response = xbee.wait_read_frame()
        print response
    except KeyboardInterrupt:
        pass
    finally:
        ser.close()
    
if __name__ == '__main__':
    main()
