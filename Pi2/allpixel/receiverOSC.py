#!/usr/bin/python

from OSC import OSCServer
import socket
import sys
import bibliopixel
from time import sleep
import time

#Load driver for the AllPixel
from bibliopixel.drivers.serial_driver import *
#set number of pixels & LED type here 
driver = DriverSerial(num = 30, type = LEDTYPE.WS2812B, dev = "/dev/tty.usbmodem23451", c_order = ChannelOrder.GRB)

#load the LEDStrip class
from bibliopixel.led import *
led = LEDStrip(driver)

#load channel test animation
# from bibliopixel.animation import StripChannelTest
# anim = StripChannelTest(led)
port = 5005
server = OSCServer( ("localhost", port) )
server.timeout = 0
run = True
print ('OSC listening on port ', port)

def fillRGB (path, tags, args, source):
	r = args[0]
	g = args[1]
	b = args[2]
	start = args[3]
	end = args[4]
	led.fillRGB(r,g,b,start,end)
	led.update()

def singleLED (path, tags, args, source):
	r = args[0]
	g = args[1]
	b = args[2]
	index = args[3]
	led.setRGB(index, r,g,b)

def multipleLED (path, tags, args, source):
	r = args[0]
	g = args[1]
	b = args[2]
	for i in range(3, len(args)):
		index = args[i]
		led.setRGB(index, r,g,b)

def update (path, tags, args, source):
	led.update()

def allOff (path, tags, args, source):
	led.all_off()

server.addMsgHandler( "/allOff", allOff)
server.addMsgHandler( "/update", update)
server.addMsgHandler( "/singleLED", singleLED)
server.addMsgHandler( "/multipleLED", multipleLED)
server.addMsgHandler( "/fillRGB", fillRGB)

def each_frame():
    server.timed_out = False
    while not server.timed_out:
        server.handle_request()

while run:
    sleep(1)
    each_frame()

led.all_off()
led.update()
