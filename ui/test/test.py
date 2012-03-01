#! /usr/bin/env python

def create_template_test():
    from init.progConfig import create_template
    from init.state import state_entire
    from init.progConfig import template_file
    bulkState = state_entire()
    create_template(template_file,bulkState.init_wrDict())

def state_test():
    """Test the modules in the state class"""

    print stateEntire.init_rwrKeys()
    print stateEntire.init_wrDict()

if __name__ == "__main__":
    create_template_test()
