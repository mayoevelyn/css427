from xbee import XBee, ZigBee
import serial

PORT = '/dev/tty.usbserial-DN01F2QA'
BAUD_RATE = 9600

ser = serial.Serial(PORT, BAUD_RATE)

xbee = ZigBee(ser)

while True:
    try:
        response = xbee.wait_read_frame()
        print response
    except KeyboardInterrupt:
        break

ser.close()
