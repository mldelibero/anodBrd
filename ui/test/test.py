#! /usr/bin/env python

from init.state import state_entire
from init.progConfig import testConfig_file
from init.progConfig import template_file

bulkState = state_entire()
    
def state_test():
    """Test the modules in the state class"""
    print stateEntire.init_rwrKeys()
    print stateEntire.init_wrDict()

def create_template_test():
    from init.progConfig import create_template
    create_template(template_file,bulkState.init_wrDict())

def matches_template_test():
    from init.progConfig import matches_template
    matches_template(testConfig_file,bulkState.init_wrDict())

def check_usr_limits_test():
    from init.progConfig import check_usr_limits
    

if __name__ == "__main__":
    import sys
    sys.path.append('/Users/mldelibero/case/masters/anodBrd/ui')
    matches_template_test()
