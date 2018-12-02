import os
import ROOT as R

cmd = "root -l -b -q make_histograms.cpp+\"(\\\"config.cfg\\\")\""
os.system(cmd)

category_list = ["em_vv" , "em_ztt" ]

for category in category_list :

    print "****************************************************************************************"
    print "****************************************************************************************"
    print "Execute " + category + " : \n"
    # Now start to make the actual gof test
    cmd = "root -l -b -q ../../Plotting/plot_1d_var.cpp\"(\\\"ML\\\",\\\"" + category    + "\\\", true , true , \\\"output/2016/\\\")\""
    #os.system(cmd)
    cmd = "root -l -b -q ../../Plotting/plot_1d_var.cpp\"(\\\"ML\\\",\\\"" + category    + "\\\", true , false , \\\"output/2016/\\\")\""
    #os.system(cmd)

# Print confusione matrices
#os.system("python ../../Plotting/Confusion.py -c \"em\" ")
