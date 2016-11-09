#! /usr/bin/python

from xbee import ZigBee
import serial, string

def addData(indata):
    
    #parameters = indata['rf_data'].split(',')
    """if parameters.__len__() != 3:
        return -1

    param1 = parameters[0]
    param2 = parameters[1]
    param3 = parameters[2]

    print param1 + " " + param2 + " " + param3"""
    print str(indata)

    #retval = int(param1) + int(param2) + int(param3)
    #return retval
    return -1

def main():
    # Open serial port
    ser = serial.Serial('/dev/ttyUSB0', 9600)

    # Create XBee Series 2 object
    xbee = ZigBee(ser)
            
    DEST_ADDR_LONG = "\x00\x13\xA2\x00\x40\xE3\xCD\x0F"
    DEST_ADDR = "\xFF\xFE"
    
    while True:
        try:             
            # Receive initial data from arduino
            indata = xbee.wait_read_frame()

            #perform calculation
            result = addData(indata)            

            # Send packet
            outdata = str(result)
            xbee.send('tx', dest_addr_long=DEST_ADDR_LONG, dest_addr=DEST_ADDR, data=outdata)
            
            # Wait for response
            response = xbee.wait_read_frame()
            print response
            
        except KeyboardInterrupt:            
            break        
            
    ser.close()
    

    
if __name__ == '__main__':
    main()







