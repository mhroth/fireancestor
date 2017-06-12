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
driver = DriverSerial(num = 30, type = LEDTYPE.WS2812B, dev = "/dev/cu.usbmodem23451", c_order = ChannelOrder.GRB)

#load the LEDStrip class
from bibliopixel.led import *
led = LEDStrip(driver)

#load channel test animation
# from bibliopixel.animation import StripChannelTest
# anim = StripChannelTest(led)

server = OSCServer( ("localhost", 5005) )
server.timeout = 0
run = True

def printIt(path, tags, args, source):
    if (args[0]==0):
        print("Direction: Reverse")
    else:
        print("Direction: Forward")

def fillRGB (path, tags, args, source):
	print('received fillRGB', args)
	r = args[0]
	g = args[1]
	b = args[2]
	start = args[3]
	end = args[4]
	led.all_off()
	led.fillRGB(r,g,b,start,end)
	led.update()

def updateLED (path, tags, args, source):
	print('received updateLED', args)
	index = args[0]
	led.all_off()
	led.setRGB(index, 255,128,64)
	led.update()

server.addMsgHandler( "/led", updateLED)
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
