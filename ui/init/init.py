#! /usr/bin/env python
from testConfig import *

def init():
    """ Initialize program files before running loop.py.
        
        1. Check usr/test info file for existance and formatting.    
    """
    print "\n//---------------------------------------"
    print "Starting Initialization"
    print "//---------------------------------------\n"
    
    print "Checking User/Test configurations:\n"

    if  checkUsr():
        print "\n//---------------------------------------"
        print "Initialization Sucessful"
        print "//---------------------------------------"
        return True
    else:
        print "Error: Initialization Failed!"
        return False
    print "//---------------------------------------"
    

