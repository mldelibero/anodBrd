#!/usr/bin/env python

"""This file contains all of the functions necessary to spoof (emulate) communication with a circuit board."""

class commprot():
   """Holds the communication protocal for the anodBrd on comp"""
   def __init__(self):
       self.startByte = 0x97

       self.ch1En = 0
       self.ch2En = 0
       self.ch3En = 0
       self.ch4En = 0
       self.msgSize = 6
       self.chEnSz = 0 | (self.ch1En<<4) | (self.ch2En<<5) | (self.ch3En<<6) | (self.ch4En<<7) | self.msgSize

       self.ch1State = 3
       self.ch2State = 3
       self.ch3State = 3
       self.ch4State = 3
       self.state = (self.ch1State) | (self.ch2State<<2) | (self.ch3State<<4) | (self.ch4State<<6)

       self.ch1vol = 7
       self.ch2vol = 8
       self.ch3vol = 0
       self.ch4vol = 0

       self.ch1cur = 20
       self.ch2cur = 21
       self.ch3cur = 0
       self.ch4cur = 0

       self.stopByte = 0x68
   def get2PCmsg(self):
        """Return the formatted message that goes to the PC"""
        msg2PC = []
        msg2PC.append(self.startByte)
        msg2PC.append(self.chEnSz)
        msg2PC.append(self.ch1vol)
        msg2PC.append(self.ch1cur)
        msg2PC.append(self.ch2vol)
        msg2PC.append(self.ch2cur)
        msg2PC.append(self.stopByte)

        return msg2PC

def spoofInit(state):
    """Initiliaze all variables in spoofInit"""
    brdState = commprot()
    brdState.ch1En = state['ch1_on']
    brdState.ch2En = state['ch2_on']

    return brdState

if __name__=='__main__':
    msg = commprot()
    print "0x{0:x}".format(msg.chEnSz)
    print "0x{0:x}".format(msg.state)
