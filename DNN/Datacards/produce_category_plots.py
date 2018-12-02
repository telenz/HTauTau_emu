import os
import ROOT as R

cmd = "root -l -b -q make_histograms.cpp+\"(\\\"config.cfg\\\")\""
os.system(cmd)

category_list = ["em_vv" , "em_ztt" , "em_ss" , "em_tt" , "em_misc" , "em_st" , "em_qqh" , "em_ggh" ]

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

# Measure stage0 signal strength constraint
os.environ["ERA"] = '2016'
os.system("source ./measure_signal_strength.sh")
os.system("source ./measure_inclusive_signal_strength.sh")

# Make popst-fit plots
cmd = "root -l -b -q ../../Plotting/make_postfit_plots.cpp\"(true,true)\""
os.system(cmd)
