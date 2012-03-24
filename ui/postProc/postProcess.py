#! /usr/bin/env python

#from init.progConfig import testConfig_file
testConfig_file = "./testConfig.xls"

import os
import re
from xlrd import open_workbook
from xlwt import Workbook
from xlutils.copy import copy
from tempfile import TemporaryFile

def postProcess(usrFile,dataFile):
    """Process all the data taken during the test."""
    print "\n//---------------------------------------"
    print "Starting Post Processing of data"
    print "//---------------------------------------"
    wrfile = createFile(dataFile)
    wr2file(wrfile,dataFile)

def createFile(dFileName,setFile):
    """Create a .xls file with the same name as the data file
    
        dFileName --> data file name
        setFile --> settings file (usrFile from init).
            The 'test_info' page will be copied into the new file
    """

    #Check to see if desired file exists
    name = re.findall("^(\./\w+-\w+)",dFileName)[0]
    num = 0
    newName = "%s.xls" % (name)
    while (os.path.exists(newName)):
        num+=1
        newName = "%s-%i.xls" % (name,num)
    
    #Copy 'test_info' page into the new file
    book_old = open_workbook(setFile)
    sheetNames = book_old.sheet_names()[0]
    sheet_old = book_old.sheet_by_name(sheetNames)
    
    wb = copy(book_old)
    wb.save(newName)
    
    #Add ch1&2 sheets and column headers
    ch1 = wb.add_sheet('ch1')
    ch2 = wb.add_sheet('ch2')
    colHeaders = [u'chEn',u'State',u'vol (V)',u'cur (mA)',u'time (s)']
    for col in range(len(colHeaders)):
        ch1.write(0,col,colHeaders[col])
        ch2.write(0,col,colHeaders[col])

    wb.save(newName)
    return newName

def wr2file(toFile,frFile):
    a = 1

if __name__=='__main__':
    usrInfoFile = testConfig_file
    datFile = "./anod_2012_3_23-0.txt"

    file = createFile(datFile,usrInfoFile)

