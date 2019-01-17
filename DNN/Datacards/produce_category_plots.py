import os
import argparse
import sys
import ROOT as R
from joblib import Parallel, delayed
import multiprocessing

parser = argparse.ArgumentParser()
parser.add_argument('config', help='please specify config name')
args = parser.parse_args()
config_name = args.config

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
    os.system(cmd)


# Make datacard (root-files)
print '-----------------------------------------------------------------------'
print "\nMake datacards : \n"
cmd = "root -l -b -q make_histograms.cpp+\"(\\\"" + config_name + "\\\")\""
os.system(cmd)

# Make pre-fit plots
print '-----------------------------------------------------------------------'
print "\nMake pre-fit plots \n"
#for category in category_list:
#    make_category_plots(category)
num_cores=20
Parallel(n_jobs=num_cores)(delayed(make_category_plots)(i) for i in category_list)

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
cmd = "cp output/2017/htt_em.inputs-sm-Run2017-ML.root output/2017/htt_em.inputs-sm-Run2017-ML_save.root"
os.system(cmd)
cmd = "python fix_em_shapes.py output/2017/htt_em.inputs-sm-Run2017-ML_save.root output/2017/htt_em.inputs-sm-Run2017-ML.root"
os.system(cmd)
os.system("source ./measure_signal_strength.sh")
os.system("source ./measure_inclusive_signal_strength.sh")
#os.system("source ./plot_impacts_fit.sh")
cmd = "mv output/2017/htt_em.inputs-sm-Run2017-ML_save.root output/2017/htt_em.inputs-sm-Run2017-ML.root"
os.system(cmd)

# Make post-fit plots
print '-----------------------------------------------------------------------'
print '/nMake post-fit plots \n'
cmd = "root -l -b -q ../../Plotting/make_postfit_plots.cpp\"(true,true,\\\"output\\\",\\\""+era+"\\\","+str(embedded)+")\""
os.system(cmd)
cmd = "root -l -b -q ../../Plotting/make_postfit_plots.cpp\"(true,false,\\\"output\\\",\\\""+era+"\\\","+str(embedded)+")\""
os.system(cmd)
