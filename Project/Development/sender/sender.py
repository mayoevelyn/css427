#! /usr/bin/python

from xbee import ZigBee, XBee
import serial

def main():
    try:
        
        # Open serial port
        ser = serial.Serial('/dev/ttyUSB0', 9600)
        
        # Create XBee Series 2 object
        xbee = ZigBee(ser)
        
        DEST_ADDR_LONG = "\x00\x13\xA2\x00\x40\xE3\xCD\x1E"
        DEST_ADDR = "\xFF\xFE"
        
        # Send packet
        xbee.send('tx', dest_addr_long=DEST_ADDR_LONG, dest_addr=DEST_ADDR, data='WHOS A GOOD BOY')
        
        # Wait for response
        response = xbee.wait_read_frame()
        print response
    except KeyboardInterrupt:
        pass
    finally:
        ser.close()
    
if __name__ == '__main__':
    main()
