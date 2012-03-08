#! /usr/bin/env python
from init.init import init
from loop.loop import loop
from init.state import state_entire
#Run the rest of the program if the initialization is sucessful.

print "Anod Board UI program running..."
state = state_entire().progState
print "State = ",state

state = init()
print "Initialized state = ",state
'''
state = state_entire.get_state()
state = init()
print state
'''
'''
if (state != False):
#    loop(state)
    postProcess()
    '''
