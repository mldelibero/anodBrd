#! /usr/bin/env python
'''
This program loads the user data from the usr_info.xls file.
The data must be in a specific format or the test will fail.
'''
# grab the first sheet of the excel file.
import xlrd
from xlrd import open_workbook
import xlwt
from xlwt import Workbook
import xlutils
from tempfile import TemporaryFile

#Initialize rheaders
rheaders = (['','','','','','','','','',])
rheaders[0] = 'User Name'
rheaders[1] = 'Anod Material'
rheaders[2] = 'Electrolyte'
rheaders[3] = 'Ch1 Cur (mA)'
rheaders[4] = 'Ch1 Time (min)'
rheaders[5] = 'Ch1 Vol (V)'
rheaders[6] = 'Ch2 Cur (mA)'
rheaders[7] = 'Ch2 Time (min)'
rheaders[8] = 'Ch2 Vol (V)'
#Initialize sample data
data = (['','','','','','','','','',])
data[0] = 'Michael DeLibero'
data[1] = 'Titanium'
data[2] = 'H3PO4'
data[3] = '0'
data[4] = '0'
data[5] = '0'
data[6] = '0'
data[7] = '0'
data[8] = '0'

vol_lim = 30    # V
cur_lim = 100   # mA
time_lim = 1440 # 24hrs

limits = {rheaders[3]:vol_lim,rheaders[4]:cur_lim,rheaders[5]:time_lim,rheaders[6]:vol_lim,rheaders[7]:cur_lim,rheaders[8]:time_lim}

save_file = 'usr_info_template.xls'
usr_file = 'usr_info.xls' # Must be based from template

#function to creat an excel file of approp format
#warning! this will overwrite your Current file
def create_template(file):
    book = Workbook()
    sheet1 = book.add_sheet('usr_info')

    for i in range(len(rheaders)):
        sheet1.write(i,0,rheaders[i])
        sheet1.write(i,1,data[i])
    
    book.save(file)
    book.save(TemporaryFile())

#function to check if the file matches the template
def matches_template(file):
    allGood = True
    book = open_workbook(file)
    sheet = book.sheet_by_index(0)
    names = sheet.col_values(0,0,len(rheaders));

    for i in range(len(rheaders)):
        print "%s :       %s   :" % (names[i],rheaders[i]),
        test = names[i] == rheaders[i]
        print test
        if (test == False):
            allGood = test
          
    return allGood        

#function checks limits on user's requested settings
def check_usr_limits(file):
    allGood = True
    book = open_workbook(file)
    sheet = book.sheet_by_index(0)
    usr_val= sheet.col_values(1,0,len(rheaders));
    k = sorted(limits.keys())

    for i in range(3,len(usr_val)):
        print k[i-3]

    '''
    for i in range(3,len(usr_val)):
        test = (usr_val[i] <= limits[k[i-3]])
        print "User %s: %s  <= Limit: %s --- %s" % (k[i-3],usr_val[i],limits[k[i-3]],test)
        if (test == False):
            allGood = test
    '''   

check_usr_limits(usr_file)
#print "all tests: &s " % check_usr_limits(usr_file)
#print "all tests: %s " % matches_template(usr_file)
#create_template(save_file)
