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

#variable_list = [ "m_sv" ]


axis_range = { "m_sv"  : [8 , 0  , 300],
               "m_vis" : [8 , 0  , 300],
               "pt_1"  : [8 , 10 , 150],
               "pt_2"  : [8 , 10 , 150],
               "eta_1" : [8 , -2.5 , +2.5],
               "eta_2" : [8 ,-2.4 , +2.4],
               "jpt_1" : [8 , 30 , 400],
               "jpt_2" : [8 , 30 , 400],
               "njets" : [8 , 0  , 8],
               "nbtag" : [8 , 0  , 8],
               "mt_1"  : [8 , 0 , 150],
               "mt_2"  : [8 , 0 , 150],
               "pt_tt" : [8 , 0 , 300],
               "mjj"   : [8 , 0 , 600],
               "met"   : [8 , 0 , 150],
               "dzeta" : [8 , -35 , 100],
               }

# Execute produce_gof_inputs.cpp for all variables

for var in variable_list :

    print "****************************************************************************************"
    print "****************************************************************************************"
    print "Execute " + var + " : \n"

    # Get axis ranges (if not specified set to default value of {10,400})
    nbins , xmin , xmax = axis_range.get(var,[8, 0,400])

    cmd = "root -l -b -q produce_gof_input.cpp+\"(\\\""+var+"\\\" , " + str(nbins) + " , {"+str(xmin)+","+str(xmax)+"})\""
    os.system(cmd)

    # Now start to make the actual gof test
    os.environ["VAR"] = var
    os.system("source ./run_gof.sh")

    cmd = "root -l -b -q ../../Plotting/plot_1d_var.cpp\"(\\\""+var+"\\\")\""
    os.system(cmd)
