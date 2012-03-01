#! /usr/bin/env python
from progConfig import *



'''
from testConfig import *
class stateClass:

state = {'usrName':['User Name','Your Name',0,no_lim],
         'testDate':['Test Date',date,1,no_lim],
         'anodMat':['Anod Material','Titanium',2,no_lim],
         'elecro':['Electrolye','H3PO4',3,no_lim],
         'ch1_vol':['Ch1 Vol(V)',0,4,vol_lim],
         'ch1_cur':['Ch1 Cur(mA)',1,5,cur_lim],
         'ch1_time':['Ch1 Time(min)',2,6,time_lim],
         'ch2_vol':['Ch2 Vol(V)',3,7,vol_lim],
         'ch2_cur':['Ch2 Cur(mA)',4,8,cur_lim],
         'ch2_time':['Ch2 Time(min)',2,9,time_lim],
         }

def init(state_init):
    """ Initialize program files before running loop.py.
        
        state_init -- The state variable as it exists in the init fun
    """
    print "\n//---------------------------------------"
    print "Starting Initialization"
    print "//---------------------------------------\n"
    
    print "Checking User/Test configurations:\n"

    state_init = checkUsr(state_init)

    if(state_init != False):
        print "\n//---------------------------------------"
        print "Initialization Sucessful"
        print "//---------------------------------------"
        return state_init
    else:
        print "Error: Initialization Failed!"
        return False
    print "//---------------------------------------"
    
'''
