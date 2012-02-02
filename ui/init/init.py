#! /usr/bin/env python
from testConfig import *

def init(state_init):
    """ Initialize program files before running loop.py.
        
        state_init -- The state variable as it exists in the init fun
    """
    print "\n//---------------------------------------"
    print "Starting Initialization"
    print "//---------------------------------------\n"
    
    print "Checking User/Test configurations:\n"

    state_init = checkUsr(state_init)

    if(state_init != False):
        print "\n//---------------------------------------"
        print "Initialization Sucessful"
        print "//---------------------------------------"
        return state_init
    else:
        print "Error: Initialization Failed!"
        return False
    print "//---------------------------------------"
    

