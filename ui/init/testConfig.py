#! /usr/bin/env python
'''
This file contains everything needed to inspect the user's data file format in the init phase of the program.
'''

from xlrd import open_workbook
from xlwt import Workbook
from xlutils import *
from tempfile import TemporaryFile

#Initialize rheaders
rheaders = (['','','','','','','','','',])
rheaders[0] = 'User Name'
rheaders[1] = 'Anod Material'
rheaders[2] = 'Electrolyte'
rheaders[3] = 'Ch1 Vol (V)'
rheaders[4] = 'Ch1 Cur (mA)'
rheaders[5] = 'Ch1 Time (min)'
rheaders[6] = 'Ch2 Vol (V)'
rheaders[7] = 'Ch2 Cur (mA)'
rheaders[8] = 'Ch2 Time (min)'
#Initialize sample data
data = (['','','','','','','','','',])
data[0] = 'Michael DeLibero'
data[1] = 'Titanium'
data[2] = 'H3PO4'
data[3] = '0'
data[4] = '1'
data[5] = '2'
data[6] = '3'
data[7] = '4'
data[8] = '5'

vol_lim = 30    # V
cur_lim = 100   # mA
time_lim = 1440 # min (24hrs)

limits = {rheaders[3]:vol_lim,rheaders[4]:cur_lim,rheaders[5]:time_lim,rheaders[6]:vol_lim,rheaders[7]:cur_lim,rheaders[8]:time_lim}

template_file = 'testConfig_template.xls'
testConfig_file = 'testConfig.xls' # Must be based from template

def create_template(file):
    """ Creat a template .xls file for the test's data output."""
    book = Workbook()
    sheet1 = book.add_sheet('test_info')

    for i in range(len(rheaders)):
        sheet1.write(i,0,rheaders[i])
        sheet1.write(i,1,data[i])
    
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
        print "Rename '%s' to '%s' and fill in your desired test data." % (template_file,testConfig_file)
        return False

    #Test to see if the testConfig_file matches the template
    allGood = True
    book = open_workbook(file)
    sheet = book.sheet_by_index(0)
    names = sheet.col_values(0,0,len(rheaders));

    for i in range(len(rheaders)):
        if (names[i] != rheaders[i]):
            print "%s   !=  %s" % (names[i],rheaders[i])
            allGood = False
          
    if (allGood == True):
        print "Files are compatable\n"

    return (allGood)    
    
def check_usr_limits(file):
    """\nCheck to see if the test's limits are below the maximums:\n"""
    
    print "Testing if test parameters are below maximums:\n"

    allGood = True
    book = open_workbook(file)
    sheet = book.sheet_by_index(0)
    usr_val= sheet.col_values(1,0,len(rheaders));
    r_val = sheet.col_values(0,0,len(rheaders));
    k = sorted(limits.keys())

    for row in range(0,len(rheaders)):
        print usr_val[row],type(usr_val[row])
    return allGood

def checkUsr():
    """Check that the test configuration file is properly formatted with sensible parameters."""

    create_template(template_file)#OverWr old templates is desired
    if (matches_template(testConfig_file)):
        check_usr_limits(testConfig_file)
    else:
        print "Error: Initialization Failed!"
        return False
    
