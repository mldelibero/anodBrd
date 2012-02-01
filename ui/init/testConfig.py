#! /usr/bin/env python
'''
This file contains everything needed to inspect the user's testParam_val file format in the init phase of the program.
'''

from xlrd import open_workbook
from xlwt import Workbook
from xlutils import *
from tempfile import TemporaryFile
from collections import namedtuple

#Initialize testParam_name
testParam_name = (['','','','','','','','','',])
testParam_name[0] = 'User Name'
testParam_name[1] = 'Anod Material'
testParam_name[2] = 'Electrolyte'
testParam_name[3] = 'Ch1 Vol (V)'
testParam_name[4] = 'Ch1 Cur (mA)'
testParam_name[5] = 'Ch1 Time (min)'
testParam_name[6] = 'Ch2 Vol (V)'
testParam_name[7] = 'Ch2 Cur (mA)'
testParam_name[8] = 'Ch2 Time (min)'
#Initialize sample testParam_val
testParam_val = (['','','','','','','','','',])
testParam_val[0] = 'Michael DeLibero'
testParam_val[1] = 'Titanium'
testParam_val[2] = 'H3PO4'
testParam_val[3] = 0
testParam_val[4] = 1
testParam_val[5] = 2
testParam_val[6] = 3
testParam_val[7] = 4
testParam_val[8] = 5

vol_lim = 30    # V
cur_lim = 100   # mA
time_lim = 1440 # min (24hrs)

limits = {testParam_name[3]:vol_lim,testParam_name[4]:cur_lim,testParam_name[5]:time_lim,testParam_name[6]:vol_lim,testParam_name[7]:cur_lim,testParam_name[8]:time_lim}

#Initialize state variable
state = {testParam_name[3]:0,testParam_name[4]:1,testParam_name[5]:2,testParam_name[6]:3,testParam_name[7]:4,testParam_name[8]:5,'ch1_state':6,'ch2_state':7}


template_file = 'testConfig_template.xls'
testConfig_file = 'testConfig.xls' # Must be based from template

def create_template(file):
    """ Creat a template .xls file for the test's testParam_val output."""
    book = Workbook()
    sheet1 = book.add_sheet('test_info')

    for i in range(len(testParam_name)):
        sheet1.write(i,0,testParam_name[i])
        sheet1.write(i,1,testParam_val[i])
    
    book.save(file)
    book.save(TemporaryFile())

def matches_template(file):
    """Check to see if the test Configuration file matches the template:"""
    print "Checking for '%s' -> '%s' compatability" % (testConfig_file,template_file)
    #Test to see if the the testConfig_file exists
    try:
        open(file)
    except IOError as e:
        print "Error: File %s does not exist!" % file
        print "Rename '%s' to '%s' and fill in your desired test testParam_val." % (template_file,testConfig_file)
        return False

    #Test to see if the testConfig_file matches the template
    allGood = True
    book = open_workbook(file)
    sheet = book.sheet_by_index(0)
    names = sheet.col_values(0,0,len(testParam_name));

    for i in range(len(testParam_name)):
        if (names[i] != testParam_name[i]):
            print "%s   !=  %s" % (names[i],testParam_name[i])
            allGood = False
          
    if (allGood == True):
        print "Files are compatable\n"

    return (allGood)    
    
def check_usr_limits(file):
    """Check to see if the test's limits are below the maximums:"""
    
    print "Testing if test parameters are below maximums:"

    allGood = True
    book = open_workbook(file)
    sheet = book.sheet_by_index(0)
    usr_val= sheet.col_values(1,0,len(testParam_name));
    r_val = sheet.col_values(0,0,len(testParam_name));
    k = sorted(limits.keys())

    for row in range(3,len(testParam_name)):
        if (usr_val[row] > limits[r_val[row]]):
            print "%s : %i !< limit of %i" % (r_val[row],usr_val[row],limits[r_val[row]])
            allGood = False
    if (allGood == True):
        print "Parameters are below maximums"
        return allGood
    else:
        return False

def confirm_testParameters():
    """ Confirm the test parameters with the user."""
    print "\nConfirming test parameters:"
    print "Are all of the following correct?\n"
    for key in state.keys():
        print "%s = %i" % (key,state[key])
    return True

def checkUsr():
    """Check that the test configuration file is properly formatted with sensible parameters."""

    create_template(template_file)#OverWr old templates is desired
    if matches_template(testConfig_file) & check_usr_limits(testConfig_file) & confirm_testParameters():
        print "Initialization pases"
    else:
        print "Error: Initialization Failed!"
        return False
    
