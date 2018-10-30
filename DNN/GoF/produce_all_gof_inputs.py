import os

variable_list = [ "m_sv",
                  "m_vis",
                  "pt_1",
                  "pt_2",
                  "eta_1",
                  "eta_2",
                  "jpt_1",
                  "jpt_2",
                  "njets",
                  "nbtag",
                  "mt_1",
                  "mt_2",
                  "pt_tt",
                  "mjj",
                  "met",
                  "dzeta"
                  ]

# variable_list = [ "m_sv" ]


axis_range = { "m_sv"  : [0  , 300],
               "m_vis" : [0  , 300],
               "pt_1"  : [10 , 150],
               "pt_2"  : [10 , 150],
               "eta_1" : [-2.5 , +2.5],
               "eta_2" : [-2.4 , +2.4],
               "jpt_1" : [30 , 400],
               "jpt_2" : [30 , 400],
               "njets" : [0  , 8],
               "nbtag" : [0  , 8],
               "mt_1"  : [0 , 150],
               "mt_2"  : [0 , 1500],
               "pt_tt" : [0 , 300],
               "mjj"   : [0 , 600],
               "met"   : [0 , 150],
               "dzeta" : [-35 , 100],
               }

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

    # cmd = "root -l -b -q ../../Plotting/plot_1d_var.cpp\"(\\\""+var+"\\\")\""
    # os.system(cmd)
