#! /usr/bin/env python
'''
This file contains everything needed to inspect the user's testParam_val file format in the init phase of the program.
'''

from xlrd import open_workbook
from xlwt import Workbook
from tempfile import TemporaryFile
from collections import namedtuple
from state import state_entire

bulkState = state_entire()

template_file = 'testConfig_template.xls'
testConfig_file = 'testConfig.xls' # Must be based from template

def create_template(templFile,template):
    """ Create a template .xls file for the test's testParam_val output
    templFile -> filename to save the template
    template -> ordered list of Name, value to write in the templFile
    """

    book = Workbook()
    sheet1 = book.add_sheet('test_info')

    row = 0
    for set in template:
        sheet1.write(row,0,set[0])
        sheet1.write(row,1,set[1])
        row+=1

    book.save(templFile)
    book.save(TemporaryFile())

def matches_template(usrFile,template):
    """Check to see if the test Configuration file matches the template:"""
    print "Checking for '%s' -> '%s' compatability" % (usrFile,template_file)
    #Test to see if the the testConfig_file exists
    try:
        open(usrFile)
    except IOError as e:
        print "Error: File %s does not exist!" % usrFile
        return False

    #Test to see if the testConfig_file matches the template
    allGood = True
    book = open_workbook(usrFile)
    sheet = book.sheet_by_index(0)
    usr_val_name = sheet.col_values(0,0,len(template));

    for row in range(len(template)):
        if(usr_val_name[row] != template[row][0]):
            print "%s   !=  %s" % (usr_val_name[row],template[row])
            allGood = False
          
    if (allGood == True):
        print "-->Files are compatable\n"

    return (allGood)    

def check_usr_limits(usrFile,limits):
    """Check to see if the test's limits are below the maximums:"""
    
    print "Testing if test parameters are below maximums:"

    allGood = True
    book = open_workbook(usrFile)
    sheet = book.sheet_by_index(0)
    usr_val = sheet.col_values(1,0,len(limits))
    usr_val_name = sheet.col_values(0,0,len(limits))

    for row in range(4,len(limits)):
        if (usr_val[row] > limits[row][1]):
            print "%s : %i !< limit of %i" % (usr_val_name[row],usr_val[row],limits[row][1])
            allGood = False

    if (allGood == True):
        print "-->Parameters are below maximums"
        return allGood
    else:
        return False

def confirm_testParameters(usrFile,keys):
    """ Confirm the test parameters with the user."""
    print "\nConfirming test parameters:"
    print "Is all of the following correct?\n"

    allGood = True
    book = open_workbook(usrFile)
    sheet = book.sheet_by_index(0)
    usr_val = sheet.col_values(1,0,len(keys))
    usr_val_name = sheet.col_values(0,0,len(keys))

    for row in range(len(usr_val)):
        print "%s = %s" % (usr_val_name[row],usr_val[row])
    
    valid = False
    while (valid == False):
        ans = raw_input('\n(y/n)\n')

        if ((ans == 'y') or (ans == 'Y')):
            valid = True
        elif ((ans == 'n') or (ans == 'N')):
            print "\nFix your parameters in %s and run this script again." % (usrFile)
            valid = True
            return False
        else:
            print "Invalid Answer"

    return True

def createState(usrFile):
    """ Create and initialize state to be used by program."""
    """ usrFile -- should contain the user's configurations.
    Make sure to run matches_template, check_usr_limits, and confirm_testParameters on usrFile before running this function."""
    from time import time
    stateP = bulkState.progState
    stateP['stTime'] = time() # Get time is (s) since epoch

    book = open_workbook(usrFile)
    sheet = book.sheet_by_index(0)
    usr_val = sheet.col_values(1,0,len(bulkState.state.keys()))
    usr_val_name = sheet.col_values(0,0,len(bulkState.state.keys()))
    usr_dic = dict(zip(usr_val_name,usr_val))
   
    for key in stateP.keys():
        try:
            stateP[key] = usr_dic[bulkState.state[key][0]]
        except KeyError as e:
            a = 1

    stateP['ch1_time'] *= 60
    stateP['ch2_time'] *= 60

    if stateP['ch1_time'] > 0:
        stateP['ch1_on'] = 1
    if stateP['ch2_time'] > 0:
        stateP['ch2_on'] = 1

    
    norm = int(bulkState.runMode['normal'])
    spoof = int(bulkState.runMode['spoofBrd'])
    calib = int(bulkState.runMode['calib'])
            
    valid = False
    while (valid == False):
        print "Select Run Mode:\n"
        print "[%i] Normal" % norm
        print "[%i] Spoofed" % spoof
        print "[%i] Calibration" % calib

        ans = int(raw_input(''))

        if ((ans == norm) or (ans == spoof) or (ans == calib)):
            valid = True
        else:
            print "Invalid Selection!\n"

    bulkState.progState['runMode'] = int(ans)

    #Set sample rate
    tlen = max(stateP['ch1_time'],stateP['ch2_time'])
    stateP['sampPer'] = tlen / bulkState.maxSamps
    print "progConfig"
    print "maxtime: ", max(stateP['ch1_time'],stateP['ch2_time'])
    print "tlen: ",tlen
    print "sampPer: ", stateP['sampPer']
    
    return stateP

def checkUsrParams(usrFile,state_checkUsr):
    """Check that the test configuration file is properly formatted with sensible parameters.
        state_checkUsr is the state variable as it exists in the checkUsr fun.
    """
    '''
    create_template()
    matches_template()
    check_usr_limits()
    confirm_testParameters()
    '''
   
    create_template(template_file,bulkState.init_wrDict())

    if (matches_template(usrFile,bulkState.init_wrDict())):
        if (check_usr_limits(usrFile,bulkState.listLimits())):
            if (confirm_testParameters(usrFile,bulkState.listOrdKeys())):
                return True
            else:
                return False
        else:
            return False
    else:
        return False

