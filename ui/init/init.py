#! /usr/bin/env python
from state import state_entire
from progConfig import checkUsrParams
from progConfig import testConfig_file
from progConfig import checkUsrParams
from progConfig import createState
verbState = state_entire;

def init():
    """ Initialize program files before running loop.py.
        
        state_init -- The state variable as it exists in the init fun
    """
    print "\n//---------------------------------------"
    print "Starting Initialization"
    print "//---------------------------------------\n"
    
    print "Checking User/Test configurations:\n"

    if (checkUsrParams(testConfig_file,verbState)):
        print "\n//---------------------------------------"
        print "Initialization Sucessful"
        print "//---------------------------------------"
        return (createState(testConfig_file))
    else:
        print "Error: Initialization Failed!"
        return False
    print "//---------------------------------------"
    
if __name__ == "__main__":
    init();
