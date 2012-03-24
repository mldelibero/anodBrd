#! /usr/bin/env python
from init.init import init
from loop.loop import loop
from init.state import state_entire
from postProc.postProcess import postProcess


#Run the rest of the program if the initialization is sucessful.

print "Anod Board UI program running..."
state = state_entire().progState
state = init()
if (state == False):
    print "Erorr: main.py did not recieve state from init.py"
else:
    dFile = loop(state)

postProcess(dFile)
