#! /usr/bin/env python
from time import time
import serial
from init.state import state_entire
from spoof import commprot
from datetime import datetime
import os

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

    runModes = state_entire().runMode
    
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

    linit = loop_init(state,runModes)
    ser = linit[0]
    wrFile = linit[1]

    protocol = commprot()

    while (state['ch1_on'] or state['ch2_on']):
        chkTime(state)
        data = getData(state,runModes,protocol,ser)
        forData = formData(data)
        chkErr()
        wrData(forData,wrFile)
        beat()
        sleep()

    if state['runMode'] != runModes['spoofBrd']:
        ser.close()
    print "\n//---------------------------------------"
    print "Main Loop Done"
    print "//---------------------------------------"
def loop_init(state,modes):
    if (state['runMode'] == modes['spoofBrd']):
        ser = 0
    else:
        dev = '/dev/tty.usbserial-000013FD'
        baud = 2000000
        tout = 1
        ser = serial.Serial(dev,baud,timeout=tout)

    date = datetime.now().timetuple()
    date  = "%s_%s_%s" % (date[0],date[1],date[2])
    filepath = "./anod_%s-%i.txt" % (date,0)
     
    i = 0
    while(os.path.exists(filepath)):
        filepath = "./anod_%s-%i.txt" % (date,i)
        i+=1        

    file = open(filepath,'w')
    return [ser,file]

def chkTime(state):
    """Check the time and end test if the are past their time limits."""
    curTime = time()
    diff= curTime-state['stTime']
#    print diff

    if (diff > state['ch1_time']):
        if (state['ch1_on'] == 1):
            state['ch1_on'] = 0
            print "Turning off Channel 1"
        
    if (diff > state['ch2_time']):
        if (state['ch2_on'] == 1):
            state['ch2_on'] = 0
            print "Turning off Channel 2"

def getData(state,modes,prot,sport):
    """Retrieve data from the serial port"""
    
    if state['runMode'] == modes['spoofBrd']:
        msgIn = prot.get2PCmsg() # get spoofed message
    else:
        for a in range(101):
            msgIn = sport.read(3)
    
    return msgIn

def formData(unform):
   """format data gotten from getData()"""
   form = unform
   return form
    
    #print "formatting Data"
def chkErr():
    """Check for errors in data"""
    #print "Checking for errors"
def wrData(data,file):
    """Write data to the file"""
    for d in data:
        file.write(str(d) + ",")
    file.write("\n")
def beat():
    """Output heartbeat at a frequency"""
    #print "Output Heartbeat"
def sleep():
    """Sleep for the remaining loop time"""
    #print "sleeping for a bit"
