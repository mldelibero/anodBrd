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

from test.test import state_entire

bulkState = state_entire()

template_file = 'testConfig_template.xls'
testConfig_file = 'testConfig.xls' # Must be based from template

if __name__ == "__main__":
    create_template(template_file,bulkState.init_wrDict)

def create_template(file,template):
    """ Creat a template .xls file for the test's testParam_val output."""
    book = Workbook()
    sheet1 = book.add_sheet('test_info')

    row = 0
    for set in template:
        sheet1.write(row,0,template[0])
        sheet1.write(row,1,template[1])
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
    usr_val_name = sheet.col_values(0,0,len(testParam));

    for row in range(len(order)):
        if(usr_val_name[row] != order[row]):
            print "%s   !=  %s" % (usr_val_name[row],order[row])
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

def initState(file,state_inSt):
    """ Initilize the state variable.

        state_inSt is the state variable as it exists in the initState fun
    """

    book = open_workbook(file)
    sheet = book.sheet_by_index(0)
    usr_val = sheet.col_values(1,0,len(order))
    usr_val_name = sheet.col_values(0,0,len(order))
    usr_dic = dict(zip(usr_val_name,usr_val))

    print usr_dic[ch1_vol]
    print type(usr_dic[ch1_vol])
    print type(state_inSt.ch1_vol)

    #state_inSt = state_inSt._replace(ch1_vol=usr_dic[ch1_vol],ch1_cur = usr_dic[ch1_cur],ch1_time = usr_dic[ch1_time],ch2_vol = usr_dic[ch2_vol],ch2_time = usr_dic[)

    print state_inSt
    return state_inSt

def checkUsr(state_checkUsr):
    """Check that the test configuration file is properly formatted with sensible parameters.
        state_checkUsr is the state variable as it exists in the checkUsr fun.
    """

    create_template(template_file)#OverWr old templates is desired
#    if (matches_template(testConfig_file) and check_usr_limits(testConfig_file) and confirm_testParameters(testConfig_file)):
    if (matches_template(testConfig_file) and check_usr_limits(testConfig_file)):
        state_checkUsr = initState(testConfig_file,state_checkUsr)
        return state_checkUsr
    else:
        return False
