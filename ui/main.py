#! /usr/bin/env python

import sys
sys.path.append('./init')
sys.path.append('./loop')
sys.path.append('./postProc')
from init import *
from loop import *
from postProcess import *
from testConfig import state_t

#Run the rest of the program if the initialization is sucessful.
state = state_t
state = init(state)

if (state != False):
    loop(state)
    postProcess()

