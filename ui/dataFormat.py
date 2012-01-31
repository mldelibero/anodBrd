#! /usr/bin/env python
from collections import namedtuple

first   = 'timestamp'
second  = 'ch1_vol'
third   = 'ch1_cur'
fourth  = 'ch1_state'
fifth   = 'ch1_on'
sixth   = 'ch2_vol'
seventh = 'ch2_cur'
eigth   = 'ch2_state'
ninth   = 'ch2_on'


data = [first,second,third,fourth,fifth,sixth,seventh,eigth,ninth]
tup = namedtuple('tup',data)
