#! /usr/bin/env python

'''
These programs take care of writing the data collected
to a file.
'''
from xlrd import open_workbook
from xlwt import Workbook
from xlutils.copy import copy
from tempfile import TemporaryFile
from dataFormat import * 


dataFile = 'test_file.xls' #file name to be created/written
usrFile = 'usr_info.xls'

def createFile(fileCreated,usrInfoFile,numRows):
    """Create an .xls file formated to take data from anod tests.
    
        fileCreated  -- The name of file to be created
        userInfoFile -- .xls File that has user and test information
                     -- Create with usrData.py->createtemplate(file) and then edit
        numRows      -- Number of rows of data in usrInfoFile

    The first sheet of the fileCreated will be identical to the first sheet of usrInfoFile.
    """
   
   # Initialize file
    wbook = Workbook() # wbook - write book
    wbook.add_sheet('test_info')
    wbook.add_sheet('Ch1_data')
    wbook.add_sheet('Ch2_data')

    # copy 1st sheet of usrInfoFile to 1st sheet of fileCreated
    rbook = open_workbook(usrInfoFile) # rbook - read book
    rsheet = rbook.sheet_by_index(0)
    
    for col in range(2):
        for row in range(numRows):
            num = rsheet.cell(row,col).value
            wbook.get_sheet(0).write(row,col,num)

    # Pre-format data-sheets
    col = 0
    row = 0
    wbook.get_sheet(1).write(row,col,data[0])
    wbook.get_sheet(2).write(row,col,data[0])
    num = (len(data)+1)/2
    for col in range(1,num):
        wbook.get_sheet(1).write(row,col,data[col])
        wbook.get_sheet(2).write(row,col,data[col+4])
        
    # save file
    wbook.save(fileCreated)
    wbook.save(TemporaryFile())

def writeData(workingFile,dataList,row):
    """ Write 'data' to 'workingFile.xls'

        workingFile -- file to be written to
                    -- create this file with createFile(fileCreated)
        data        -- pre-formatted data to be written to workingFile
    """

    book = copy(open_workbook(workingFile))
    sheet1 = book.get_sheet(1)
    sheet2 = book.get_sheet(2)

    for data in dataList:
        sheet1.write(row,0,data.timestamp)
        sheet1.write(row,1,data.ch1_vol)
        sheet1.write(row,2,data.ch1_cur)
        sheet1.write(row,3,data.ch1_state)
        sheet1.write(row,4,data.ch1_on)

        sheet2.write(row,0,data.timestamp)
        sheet2.write(row,1,data.ch2_vol)
        sheet2.write(row,2,data.ch2_cur)
        sheet2.write(row,3,data.ch2_state)
        sheet2.write(row,4,data.ch2_on)

        row +=1

    #save workbook over old filename
    book.save(workingFile)
    book.save(TemporaryFile())

    #allow for incrementing rows upon sucess
    return (row+1)

createFile(dataFile,usrFile,9)

def testWrData():
    t = tup(0,1,2,3,4,5,6,7,8)
    tList = [t for i in range(65000)]

    from time import *
    curTtime = time() 
    writeData(dataFile,tList,1)
    nextTime = time()

    print nextTime - curTtime

