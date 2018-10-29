import os

variable_list = [ "pt_1" , "pt_2" , "m_vis" , "m_sv"]

# Execute produceGoFInputs.C for all variables
for var in variable_list :
    print "Execute " + var + "\n"
    cmd = "root -l -b -q produce_gof_input.cpp+\"(\\\"" + var+ "\\\")\""
    
    print "cmd  = " + cmd
    
    os.system(cmd)
