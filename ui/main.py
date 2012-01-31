#! /usr/bin/env python

import sys
sys.path.append('./init')
from init import *
from loop import *
from postProcess import *


if (init()):
    loop()
    postProcess()

