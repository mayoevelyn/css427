import serial
from xbee import ZigBee

def addData(data):
    splitData = data.split(',')

    if len(splitData) == 2:
        arg1 = float(splitData[0])
        arg2 = float(splitData[1])
        return arg1 + arg2

    else:
        arg1 = float(splitData[0])
        arg2 = float(splitData[1])
        arg3 = float(splitData[2])        
        return arg1 + arg2 + arg3

    
            

def main():
    serial_port = serial.Serial('/dev/ttyUSB0', 9600)
    xbee = ZigBee(serial_port)

    DEST_ADDR_LONG = "\x00\x13\xA2\x00\x40\xE3\xCD\x1E"
    DEST_ADDR = "\xFF\xFE"

    print "Attempting to receive..."

    while True:
        try:
            # Receive data
            indata = xbee.wait_read_frame()            

            rfdata = indata['rf_data']
            
            result = addData(rfdata)
            #print(addData(rfdata))
            print result
            strResult = str(result)

            # Send back response# Send packet
            #xbee.send('tx', dest_addr_long=DEST_ADDR_LONG, dest_addr=DEST_ADDR, data='good boy')
            xbee.send('tx', dest_addr_long=DEST_ADDR_LONG, dest_addr=DEST_ADDR, data=strResult)
            
        
            # Wait for response
            """response = xbee.wait_read_frame()
            print response"""

            ack = xbee.wait_read_frame()
            print ack
            
        except KeyboardInterrupt:
            break

    serial_port.close()

if __name__ == '__main__':
    main()
