#! /usr/bin/env python

def loop():
    """ Run the main loop.

    This consists of:
        1. Ending each test if time > time_test
        2. Retrieve data from the USB buffer
        3. Error checking
        4. Format data
        5. Output heartbeat
        6. sleep
    """
    
    print "\n//---------------------------------------"
    print "Starting Main Loop"
    print "//---------------------------------------"

    chkTime()
    getData()
    formData()
    chkErr()
    beat()
    sleep()

    print "\n//---------------------------------------"
    print "Main Loop Done"
    print "//---------------------------------------"

def chkTime():
    print "time"
def getData():
    print "getting Data"
def formData():
    print "formatting Data"
def chkErr():
    print "Checking for errors"
def beat():
    print "Output Heartbeat"
def sleep():
    print "sleeping for a bit"
