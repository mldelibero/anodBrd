#! /usr/bin/env python
'''
This file contains everything needed to inspect the user's testParam_val file format in the init phase of the program.
'''

from xlrd import open_workbook
from xlwt import Workbook
from xlutils import *
from tempfile import TemporaryFile
from collections import namedtuple
from datetime import *

#Initialize testParam_name
date = datetime.now().strftime("%Y-%m-%d")

usrName = 'User Name'
testDate = 'Test Date'
anodMat = 'Anod Material'
elecro = 'Elecrolyte'
ch1_vol = 'Ch1 Vol(V)'
ch2_vol = 'Ch2 Vol(V)'
ch1_cur = 'Ch1 Cur(mA)'
ch2_cur = 'Ch2 Cur(mA)'
ch1_time = 'Ch1 Time'
ch2_time = 'Ch2 Time'

testParam = {usrName:'Michael DeLibero',testDate:date,anodMat:'Titanium',elecro:'H3PO4',ch1_vol:0,ch1_cur:1,ch1_time:2,ch2_vol:3,ch2_cur:4,ch2_time:5}

vol_lim = 30    # V
cur_lim = 100   # mA
time_lim = 1440 # min (24hrs)

limits = {ch1_vol:vol_lim,ch1_cur:cur_lim,ch1_time:time_lim,ch2_vol:vol_lim,ch2_cur:cur_lim,ch2_time:time_lim}
order = [usrName,testDate,anodMat,elecro,ch1_vol,ch1_cur,ch1_time,ch2_vol,ch2_cur,ch2_time]

#Initialize state variable
state = {ch1_vol:0,ch1_cur:1,ch1_time:2,ch2_vol:3,ch2_cur:4,ch2_time:5}

template_file = 'testConfig_template.xls'
testConfig_file = 'testConfig.xls' # Must be based from template

def create_template(file):
    """ Creat a template .xls file for the test's testParam_val output."""
    book = Workbook()
    sheet1 = book.add_sheet('test_info')

    row = 0
    for key in order:
        sheet1.write(row,0,key)
        sheet1.write(row,1,testParam[key])
        row+=1

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
        print "Rename '%s' to '%s' and fill in your desired test testParam values.\n" % (template_file,testConfig_file)
        return False

    #Test to see if the testConfig_file matches the template
    allGood = True
    book = open_workbook(file)
    sheet = book.sheet_by_index(0)
    names = sheet.col_values(0,0,len(testParam));

    for row in range(len(order)):
        if(names[row] != order[row]):
            print "%s   !=  %s" % (names[row],order[row])
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
    usr_val = sheet.col_values(1,0,len(order))
    usr_val_name = sheet.col_values(0,0,len(order))

    for row in range(4,len(order)):
        if (usr_val[row] > limits[usr_val_name[row]]):
            print "%s : %i !< limit of %i" % (usr_val_name[row],usr_val[row],limits[usr_val_name[row]])
            allGood = False

    if (allGood == True):
        print "Parameters are below maximums"
        return allGood
    else:
        return False

def confirm_testParameters(file):
    """ Confirm the test parameters with the user."""
    print "\nConfirming test parameters:"
    print "Are all of the following correct?\n"

    allGood = True
    book = open_workbook(file)
    sheet = book.sheet_by_index(0)
    usr_val = sheet.col_values(1,0,len(order))
    usr_val_name = sheet.col_values(0,0,len(order))

    for row in range(len(usr_val)):
        print "%s = %s" % (usr_val_name[row],usr_val[row])
    
    
    valid = False
    while (valid == False):
        ans = raw_input('\n(y/n)\n')

        if ((ans == 'y') or (ans == 'Y')):
            valid = True
        elif ((ans == 'n') or (ans == 'N')):
            print "Fix your parameters in %s and run this script again." % (template_file)
            valid = True
            return False
        else:
            print "Invalid Answer"

    return True

def checkUsr():
    """Check that the test configuration file is properly formatted with sensible parameters."""

    create_template(template_file)#OverWr old templates is desired
    if (matches_template(testConfig_file) and check_usr_limits(testConfig_file) and confirm_testParameters(testConfig_file)):
        print "Initialization Sucessful\n"
    else:
        print "Error: Initialization Failed!"
        return False
