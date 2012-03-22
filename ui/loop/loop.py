#! /usr/bin/env python
from time import time
import serial
from init.state import state_entire
from loop.spoof import commprot


def loop(state):
    """ Run the main loop.

        state is the state variable as it exists in the loop fun.

    This consists of:

        1. Ending each test if time > time_test
        2. Retrieve data from the USB buffer
        3. Error checking
        4. Format data
        5. Output heartbeat
        6. Display Data if necessary
        7. sleep
    """

    runModes = state_entire.runMode
    
    print "\n//---------------------------------------"
    print "Starting Main Loop"
    print "//---------------------------------------"

    if (state['ch1_on'] == 1):
        print "ch1 running for %s minutes" % (state['ch1_time']/60)
    else:
        print "ch1 --> OFF"
    if (state['ch2_on'] == 1):
        print "ch2 running for %s minutes" % (state['ch2_time']/60)
    else:
        print "ch2 --> OFF"

    state['stTime'] = time()

    ser = loop_init(state)
    protocol = commprot()

    while (state['ch1_on'] or state['ch2_on']):
        chkTime(state)
        getData()
        formData()
        chkErr()
        beat()
        sleep()

    print "\n//---------------------------------------"
    print "Main Loop Done"
    print "//---------------------------------------"
def loop_init(state):
    if state.progState['runMode'] == state.runMode['spoofBrd']:
        ser = 0
    else:
        dev = '/dev/tty.usbserial-000013FD'
        baud = 2000000
        tout = 1
        ser = serial.Serial(dev,baud,timeout=tout)

    return ser

def chkTime(state):
    """Check the time and end test if the are past their time limits."""
    curTime = time()
    diff= curTime-state['stTime']
    print diff

    if (diff > state['ch1_time']):
        if (state['ch1_on'] == 1):
            state['ch1_on'] = 0
            print "Turning off Channel 1"
        
    if (diff > state['ch2_time']):
        if (state['ch2_on'] == 1):
            state['ch2_on'] = 0
            print "Turning off Channel 2"

def getData(state,modes,protocol):
    """Retrieve data from the serial port"""
    
    if state['runMode'] == modes['spoofBrd']:
        msgIn = protocol.get2PCmsg() # get spoofed message

    else:
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
