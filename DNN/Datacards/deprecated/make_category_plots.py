import os
import ROOT as R

category_list = ["em_tt" , "em_ztt" , "em_ss" , "em_st" , "em_misc" , "em_qqh" , "em_ggh" , "em_vv"]
#category_list = ["em_tt" , "em_ztt" ]


cmd = "root -l -b -q make_datacard.cpp+"
os.system(cmd)

for category in category_list :

    print "****************************************************************************************"
    print "****************************************************************************************"
    print "Execute " + category + " : \n"
    # Now start to make the actual gof test
    cmd = "root -l -b -q ../../Plotting/plot_1d_var.cpp\"(\\\"ML\\\",\\\"" + category    + "\\\", true , true , \\\"output/2016/\\\")\""
    os.system(cmd)
    cmd = "root -l -b -q ../../Plotting/plot_1d_var.cpp\"(\\\"ML\\\",\\\"" + category    + "\\\", true , false , \\\"output/2016/\\\")\""
    os.system(cmd)

# Print confusione matrices
os.system("python ../../Plotting/Confusion.py -c \"em\" ")
