#! /usr/bin/env python

import sys
sys.path.append('./init')
from init.testConfig  import *

if __name__ == "__main__":
    import sys

testConfig.create_template(template_file,bulkState.init_wrDict)

'''    
#Run the rest of the program if the initialization is sucessful.
state = state_t
state = init(state)
'''
'''
print "main time"
print state.ch1_time
print state.ch1_time

if (state != False):
    loop(state)
    postProcess()
'''
