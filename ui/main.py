#! /usr/bin/env python

import sys
sys.path.append('./init')
sys.path.append('./loop')
sys.path.append('./postProc')
from init import *
from loop import *
from postProcess import *

#Run the rest of the program if the initialization is sucessful.
if (init()):
    loop()
    postProcess()

