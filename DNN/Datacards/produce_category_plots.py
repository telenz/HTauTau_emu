import os
import argparse
import sys
import ROOT as R
from joblib import Parallel, delayed
import multiprocessing

config_name = 'config_2016.cfg'

# Find settings in config
print ''
print 'Settings from ' + config_name + " : "
config_file = file(config_name)
for line in config_file:
    line = line.strip()
    line = line.replace(" ", "")
    if line.startswith('categories=') :
        categories_comma_separated = line.split("=")[1]
        category_list = categories_comma_separated.split(",")
    if line.startswith('era=') :
        era = line.split("=")[1]
    if line.startswith('use_embedded=') :
        embedded_c = line.split("=")[1]

if embedded_c == 'true':
    embedded=1
else :
    embedded=0

print ''
print 'Era is : ' + era
print 'Use embedded : ' + embedded_c
print 'Categories are : '
for cat in category_list:
    print cat
print ''

def make_category_plots(category):
    # Plot log and non-log
    cmd = "root -l -b -q ../../Plotting/plot_1d_var.cpp\"(\\\"ML\\\",\\\"" + str(category) + "\\\",true,true,\\\"output/" + era + "/\\\",\\\"" + era + "\\\"," + str(embedded)+")\""
    os.system(cmd)
    cmd = "root -l -b -q ../../Plotting/plot_1d_var.cpp\"(\\\"ML\\\",\\\"" + category + "\\\", true , false , \\\"output/" + era + "/\\\",\\\"" + era + "\\\"," + str(embedded)+")\""
    #os.system(cmd)


# Make datacard (root-files)
print '-----------------------------------------------------------------------'
print "\nMake datacards : \n"
cmd = "root -l -b -q make_histograms.cpp+\"(\\\"" + config_name + "\\\")\""
os.system(cmd)

# Make pre-fit plots
print '-----------------------------------------------------------------------'
print "\nMake pre-fit plots \n"
for category in category_list:
    make_category_plots(category)
#num_cores=2
#Parallel(n_jobs=num_cores)(delayed(make_category_plots)(i) for i in category_list)

# Print confusione matrices
print '-----------------------------------------------------------------------'
print "\nPlot confusion matrices \n"
if embedded_c == 'true':
    cmd = "python ../../Plotting/Confusion.py -e \""+era+"\" -emb "
else:
    cmd = "python ../../Plotting/Confusion.py -e \""+era+"\""
os.system(cmd)

# Measure stage0/inclusive signal strength constraint
print '-----------------------------------------------------------------------'
print '/nMeasure signal strength constraint \n'
os.environ["ERA"] = str(era)
os.environ["EMB"] = str(embedded)
os.system("source ./measure_signal_strength.sh")
os.system("source ./measure_inclusive_signal_strength.sh")

# Make post-fit plots
print '-----------------------------------------------------------------------'
print '/nMake post-fit plots \n'
cmd = "root -l -b -q ../../Plotting/make_postfit_plots.cpp\"(true,true,\\\"output\\\",\\\""+era+"\\\","+str(embedded)+")\""
os.system(cmd)
cmd = "root -l -b -q ../../Plotting/make_postfit_plots.cpp\"(true,false,\\\"output\\\",\\\""+era+"\\\","+str(embedded)+")\""
os.system(cmd)
