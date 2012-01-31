#! /usr/bin/env python

'''
These programs take care of writing the data collected
to a file.
'''
from xlrd import open_workbook
from xlwt import Workbook
from xlutils import *
from tempfile import TemporaryFile
'''
from xlrd import *
from xlwt import *
from xlutils import *
from tempfile import *
from tempfile import TemporaryFile
'''
name = 'test_file.xls' #file name to be created/written
usrInfoFile = 'usr_info.xls'

def createFile(fileCreated,usrInfoFile):
    # Initialize file
    book = Workbook()
    book.add_sheet('test_info')
    book.add_sheet('Ch1_data')
    book.add_sheet('Ch2_data')

    sheet_info = book.get_sheet(0)
    
    #Copy Over the User information to the output file
    book_usrInfo = open_workbook(usrInfoFile)
    sheet_usrInfo = book_usrInfo.sheet_by_index(0)

    sheet_info.write(0,0,sheet_usrInfo)
#    sheet_info = sheet_usrInfo

    sheet_info.write(0,0,"hee")
    '''    
    print "Stuuuf"
    print sheet_usrInfo.col_values(0,0,10)
    print sheet_info.col_values(0,0,10)
    '''    
    # save file
    book.save(fileCreated)
    book.save(TemporaryFile())


createFile(name,usrInfoFile)


