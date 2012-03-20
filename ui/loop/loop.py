#! /usr/bin/env python
from time import time
import serial

dev = '/dev/tty.usbserial-000013FD'
baud = 2000000
tout = 1
ser = serial.Serial(dev,baud,timeout=tout)

def loop(state_loop):
    """ Run the main loop.

        state_loop is the state variable as it exists in the loop fun.

    This consists of:

        1. Ending each test if time > time_test
        2. Retrieve data from the USB buffer
        3. Error checking
        4. Format data
        5. Output heartbeat
        6. Display Data if necessary
        7. sleep
    """
    
    print "\n//---------------------------------------"
    print "Starting Main Loop"
    print "//---------------------------------------"

    if (state_loop['ch1_on'] == 1):
        print "ch1 running for %s minutes" % (state_loop['ch1_time']/60)
    else:
        print "ch1 --> OFF"
    if (state_loop['ch2_on'] == 1):
        print "ch2 running for %s minutes" % (state_loop['ch2_time']/60)
    else:
        print "ch2 --> OFF"

    state_loop['stTime'] = time()
    while (state_loop['ch1_on'] or state_loop['ch2_on']):
        chkTime(state_loop)
        getData()
        formData()
        chkErr()
        beat()
        sleep()

    print "\n//---------------------------------------"
    print "Main Loop Done"
    print "//---------------------------------------"

def chkTime(state_loop):
    """Check the time and end test if the are past their time limits."""
    curTime = time()
    diff= curTime-state_loop['stTime']
    print diff
#    print state_loop['ch1_time']
#    print state_loop['ch2_time']

    if (diff > state_loop['ch1_time']):
        if (state_loop['ch1_on'] == 1):
            state_loop['ch1_on'] = 0
            print "Turning off Channel 1"
        
    if (diff > state_loop['ch2_time']):
        if (state_loop['ch2_on'] == 1):
            state_loop['ch2_on'] = 0
            print "Turning off Channel 2"
    #print state_time['ch1_time']
    #print state_time['ch2_time']

def getData():
    """Retrieve data from the serial port"""
    #print "getting Data"
    for a in range(101):
        y = ser.read(1)
        print "0x{0:x}".format(ord(y))
        y = ser.read(1)
        print "0x{0:x}".format(ord(y))
        y = ser.read(1)
        print "0x{0:x}".format(ord(y))
        print 83
    ser.close()
def formData():
    """format data gotten from getData()"""
    #print "formatting Data"
def chkErr():
    """Check for errors in data"""
    #print "Checking for errors"
def beat():
    """Output heartbeat at a frequency"""
    #print "Output Heartbeat"
def sleep():
    """Sleep for the remaining loop time"""
    #print "sleeping for a bit"
