#! /usr/bin/env python2.7
from datetime import *

class state_entire:
    """Bulky state class. Extract your desired data format from one of the built-in modules. """
    def __init__(self):
        #Initialize testParam_name
        date = datetime.now().strftime("%Y-%m-%d")

        vol_lim = 30    # V
        cur_lim = 100   # mA
        no_lim = 0
        time_lim = 1000 # min

        # key:[printed title, value, printed order, max allowed value]
        self.state = {'usrName':['User Name','Your Name',   0,no_lim],
                'testDate':['Test Date',date,         1,no_lim],
                'anodMat':['Anod Material','Titanium',2,no_lim],
                'electro':['Electrolye','H3PO4',      3,no_lim],
                'ch1_vol':['Ch1 Vol(V)',0,            4,vol_lim],
                'ch1_cur':['Ch1 Cur(mA)',1,           5,cur_lim],
                'ch1_time':['Ch1 Time(min)',2,        6,time_lim],
                'ch2_vol':['Ch2 Vol(V)',3,            7,vol_lim],
                'ch2_cur':['Ch2 Cur(mA)',4,           8,cur_lim],
                'ch2_time':['Ch2 Time(min)',2,        9,time_lim],
                }

    def init_rwrKeys(self):
        """ Ordered list of keys to write for init Excel doc  """
        return sorted(self.state,  key=lambda key1:self.state[key1][2])

    def init_wrDict(self):
        return ([(self.state[key][0],self.state[key][1]) for key in self.init_rwrKeys()])

