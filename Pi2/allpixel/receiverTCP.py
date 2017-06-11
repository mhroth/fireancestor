import socket
import sys
import bibliopixel

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)


#Load driver for the AllPixel
from bibliopixel.drivers.serial_driver import *
#set number of pixels & LED type here 
driver = DriverSerial(num = 10, type = LEDTYPE.WS2812B, dev = "/dev/cu.usbmodem9", c_order = ChannelOrder.GRB)

#load the LEDStrip class
from bibliopixel.led import *
led = LEDStrip(driver)

#load channel test animation
# from bibliopixel.animation import StripChannelTest
# anim = StripChannelTest(led)


# Bind the socket to the port
server_address = ('localhost', 10000)
print >>sys.stderr, 'starting up on %s port %s' % server_address
sock.bind(server_address)

# Listen for incoming connections
sock.listen(1)

def updateLED (index = 0):
	print('received ', index)
	index = int(index)
	led.all_off()
	led.setRGB(index, 255,128,64)
	led.update()

while True:
    # Wait for a connection
    print >>sys.stderr, 'waiting for a connection'
    connection, client_address = sock.accept()
    try:
        print >>sys.stderr, 'connection from', client_address

        # Receive the data in small chunks and retransmit it
        while True:
            data = connection.recv(16)
            print >>sys.stderr, 'received "%s"' % data
            if data:
                print >>sys.stderr, 'sending data back to the client'
                connection.sendall(data)
                updateLED(data)
            else:
                print >>sys.stderr, 'no more data from', client_address
                break

    except KeyboardInterrupt:
        # Clean up the connection
        connection.close()
        led.all_off()
    	led.update()
