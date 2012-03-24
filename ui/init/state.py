#! /usr/bin/env python2.7
from datetime import *

class state_entire:
    """Bulky state class. Extract your desired data format from one of the built-in modules. """
    def __init__(self):
        #Initialize testParam_name
        self.date = datetime.now().strftime("%Y-%m-%d")

        #{V,mA,min,NA}
        self.absLim = {'vol':30,'cur':100,'time':1440,'no':-1}
        
        self.runMode = {'normal':1, #Normal operation w/ brd attached
                        'spoofBrd':2, #spoofed operation wo brd
                        'calib':3 #calibrating brd
                  }
        # key:[printed title, value, printed order, max allowed value]
        self.state = {
                'usrName':['User Name','Your Name',0,self.absLim['no']],
                'testDate':['Test Date',self.date,      1,self.absLim['no']],
                'anodMat':['Anod Material','Ti',   2,self.absLim['no']],
                'electro':['Electrolye','H3PO4',   3,self.absLim['no']],
                'ch1_vol':['Ch1 Vol(V)',0,         4,self.absLim['vol']],
                'ch1_cur':['Ch1 Cur(mA)',1,        5,self.absLim['cur']],
                'ch1_time':['Ch1 Time(min)',2,     6,self.absLim['time']],
                'ch2_vol':['Ch2 Vol(V)',3,         7,self.absLim['vol']],
                'ch2_cur':['Ch2 Cur(mA)',4,        8,self.absLim['cur']],
                'ch2_time':['Ch2 Time(min)',2,     9,self.absLim['time']],
                }
        self.maxSamps = 60 # Maximum allowed samples
        self.sampPer = 1000 # Sample period (runTime(s) / maxSamps
        self.progState = {
                'runMode':self.runMode['normal'],
                'sampPer':1000,
                'stTime':0,
                'ch1_vol':30,
                'ch1_cur':100,
                'ch1_time':2,
                'ch1_on':1,
                'ch2_vol':30,
                'ch2_cur':100,
                'ch2_time':1,
                'ch2_on':1,
                }

    def init_rwrKeys(self):
        """ Ordered list of keys to write for init Excel doc  """
        return sorted(self.state,  key=lambda key1:self.state[key1][2])

    def init_wrDict(self):
        return ([(self.state[key][0],self.state[key][1]) for key in self.init_rwrKeys()])

    def listLimits(self):
        return ([(self.state[key][0],self.state[key][3]) for key in self.init_rwrKeys()])

    def listOrdKeys(self):
        """ List the dictionary keys in the correct order."""
        return self.init_rwrKeys()
    def getState(self):
        """ Return the initialized state used by the program."""
        """ The user file should already be populated and checked."""
        print self.state.keys()

if __name__=='__main':
    st = state()
