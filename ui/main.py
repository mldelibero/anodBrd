#! /usr/bin/env python

import sys
sys.path.append('./init')
sys.path.append('./loop')
sys.path.append('./postProc')
from init import *
from loop import *
from postProcess import *


if (init()):
    loop()
    postProcess()

