import os

# variable_list = [    "m_sv",
#                      "m_vis",
#                      "pt_1",
#                      "pt_2",
#                      "jpt_2",
#                      "njets",
#                      "nbtag",
#                      "mt_1",
#                      "mt_2",
#                      "pt_tt",
#                      "mjj",
#                      "met"]

variable_list = [ "jpt_2" ]

axis_range = {"jpt_2" : [10,100] , "pt_2" : [10,0,200] , "m_vis" : [20,0,200]} 


# Execute produce_gof_inputs.cpp for all variables

for var in variable_list :

    print "****************************************************************************************"
    print "****************************************************************************************"
    print "Execute " + var + " : \n"

    # Get axis ranges (if not specified set to default value of {10,400})
    xmin , xmax = axis_range.get(var,[0,400])

    cmd = "root -l -b -q produce_gof_input.cpp+\"(\\\""+var+"\\\" , {"+str(xmin)+","+str(xmax)+"})\""
    os.system(cmd)

    # Now start to make the actual gof test
    os.environ["VAR"] = var
    os.system("source ./run_gof.sh")
