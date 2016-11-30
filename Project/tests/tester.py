#! /usr/bin/python

from xbee import ZigBee, XBee
import serial

def main():
    # Open serial port
    ser = serial.Serial('/dev/ttyUSB0', 9600)
    
    # Create XBee Series 2 object
    xbee = ZigBee(ser)
    
    DEST_ADDR_LONG = "\x00\x13\xA2\x00\x40\xE3\xCD\x1E"
    DEST_ADDR = "\xFF\xFE"
    
    try:
        # Send packet
        #xbee.send('tx', dest_addr_long=DEST_ADDR_LONG, dest_addr=DEST_ADDR, data='16,1') # get valve reading for zone 1
        #xbee.send('tx', dest_addr_long=DEST_ADDR_LONG, dest_addr=DEST_ADDR, data='17,1') # open valve for zone 1
        #xbee.send('tx', dest_addr_long=DEST_ADDR_LONG, dest_addr=DEST_ADDR, data='18,1') # close valve for zone 1
        #xbee.send('tx', dest_addr_long=DEST_ADDR_LONG, dest_addr=DEST_ADDR, data='19,1') # toggle valve for zone 1
        #xbee.send('tx', dest_addr_long=DEST_ADDR_LONG, dest_addr=DEST_ADDR, data='22,1') # get sensor readings for zone 1
        #xbee.send('tx', dest_addr_long=DEST_ADDR_LONG, dest_addr=DEST_ADDR, data='23,1') # get sensor readings for all zones
        xbee.send('tx', dest_addr_long=DEST_ADDR_LONG, dest_addr=DEST_ADDR, data='25,1,23,13,1') # set schedule for zone 1
        
        # Wait for response
        response = xbee.wait_read_frame()
        print response
    except KeyboardInterrupt:
        pass
        
    print "Attempting to receive..."

    while True:
        try:
            response = xbee.wait_read_frame()
            print response
        
        except KeyboardInterrupt:
            break
    
    ser.close()
    
if __name__ == '__main__':
    main()
