#! /usr/bin/env python

import xlrd

book = xlrd.open_workbook("usr_info.xls")
sheet = book.sheet_by_index(0)

r = sheet.row(0)
c = sheet.col(0)

for i in range (0,9):
    print sheet.col(0)[i].value 
    print sheet.col(1)[i].value

