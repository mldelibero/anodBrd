#! /usr/bin/env python
from init.init import init
from loop.loop import loop
from init.state import state_entire
#Run the rest of the program if the initialization is sucessful.

print "Anod Board UI program running..."
state = state_entire().progState
state = init()
print "state from init\n"
print state
if (state == False):
    print "Erorr: main.py did not recieve state from init.py"
else:
    loop(state)


'''
if (state != False):
#    loop(state)
    postProcess()
'''

