import os

variable_list = [    "m_sv",
                     "m_vis",
                     "pt_vis",
                     "pt_1",
                     "pt_2",
                     "jpt_2",
                     "jm_1",
                     "jm_2",
                     "bpt_1",
                     "bpt_2",
                     "bcsv_1",
                     "bcsv_2",
                     "njets",
                     "nbtag",
                     "mt_1",
                     "mt_2",
                     "pt_tt",
                     "mjj",
                     "dijetpt",
                     "met"]

# Execute produceGoFInputs.C for all variables
for var in variable_list :
    print "****************************************************************************************"
    print "****************************************************************************************"
    print "Execute " + var + "\n"
    cmd = "root -l -b -q produce_gof_input.cpp+\"(\\\"" + var+ "\\\")\""
    os.system(cmd)
