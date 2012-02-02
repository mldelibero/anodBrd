#! /usr/bin/env python

def loop(state_loop):
    """ Run the main loop.

        state_loop is the state variable as it exists in the loop fun.

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

    chkTime(state_loop)
    getData()
    formData()
    chkErr()
    beat()
    sleep()

    print "\n//---------------------------------------"
    print "Main Loop Done"
    print "//---------------------------------------"

def chkTime(state_time):
    """Check the time and end test if the are past their time limits."""
    print state_time.ch1_time
    print state_time.ch2_time
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
