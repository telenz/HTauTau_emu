import os
import ROOT as R

category_list = ["em_tt" , "em_ztt"]


cmd = "root -l -b -q make_datacards.cpp+"
os.system(cmd)

for category in category_list :

    print "****************************************************************************************"
    print "****************************************************************************************"
    print "Execute " + category + " : \n"
    # Now start to make the actual gof test
    cmd = "root -l -b -q ../../Plotting/plot_1d_var.cpp\"(\\\"ML\\\",\\\"" + category    + "\\\", \\\"output_datacards/2016/\\\")\""
    os.system(cmd)
