#! /usr/bin/env python
    
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
    check_usr_limits(testConfig_file,bulkState.listLimits())

def confirm_testParameters_test():
    from init.progConfig import confirm_testParameters
    confirm_testParameters(testConfig_file,bulkState.listOrdKeys())
    
def createState_test():
    stateP = createState(testConfig_file)
    for key in stateP.keys():
        print key, stateP[key]

if __name__ == "__main__":
    import sys
    sys.path.append('/Users/mldelibero/case/masters/anodBrd/ui')

    from init.state import state_entire
    bulkState = state_entire()
    
    from init.progConfig import testConfig_file
    from init.progConfig import template_file
    from init.progConfig import createState
    
#    state_test()
#    create_template_test()
#    matches_template_test()
#    check_usr_limits_test()
#    confirm_testParameters_test()
    createState_test()

