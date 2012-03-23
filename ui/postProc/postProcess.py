#! /usr/bin/env python

#from writeData import createFile
#from init.progConfig import testConfig_file
testConfig_file = "./testConfig.xls"

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
    name = dFileName
    
    a = 1
    return a
def wr2file(toFile,frFile):
    a = 1

if __name__=='__main__':
    usrInfoFile = testConfig_file
    print "testConfigFile",testConfig_file
    newFile = "./anod_2012_3_23-0.txt"
    numRows = 6

    createFile(newFile,usrInfoFile,numRows)

