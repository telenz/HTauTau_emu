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

category_list = ["em_ztt","em_vv","em_ss","em_tt","em_misc","em_st","em_qqh","em_ggh","em_qqh_unrolled","em_ggh_unrolled"]

# Find settings in config
print ''
print 'Settings from ' + config_name + " : "
config_file = file(config_name)
for line in config_file:
    line = line.strip()
    line = line.replace(" ", "")
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
print ''

def make_category_plots(category):
    # Plot log and non-log
    cmd = "root -l -b -q ../../Plotting/plot_1d_var.cpp\"(\\\"ML\\\",\\\"" + str(category) + "\\\",true,true,\\\"output/" + era + "/\\\",\\\"" + era + "\\\"," + str(embedded)+")\""
    #os.system(cmd)
    cmd = "root -l -b -q ../../Plotting/plot_1d_var.cpp\"(\\\"ML\\\",\\\"" + category + "\\\", true , false , \\\"output/" + era + "/\\\",\\\"" + era + "\\\"," + str(embedded)+")\""
    #os.system(cmd)


# Make datacard (root-files)
def make_datacard(cat):
    with open(config_name) as f:
         newText=f.read().replace('categories=', 'categories='+ cat)
         newText=newText.replace('output_file_suffix = ML','output_file_suffix = ML__'+ cat)
         config_name_new = "config_for_datacard_"+era+"__"+cat+".cfg"
         with open(config_name_new, "w") as f:
              f.write(newText)

    print '-----------------------------------------------------------------------'
    print "\nMake datacards : \n"
    cmd = "root -l -b -q make_histograms.cpp+\"(\\\"" + config_name_new + "\\\")\""
    os.system(cmd)

num_cores = multiprocessing.cpu_count()
print "available number of cores = " + str(num_cores)

num_cores = 8

results = Parallel(n_jobs=num_cores)(delayed(make_datacard)(i) for i in category_list)

print '-----------------------------------------------------------------------'
cmd = "hadd -f output/"+era+"/htt_em.inputs-sm-Run"+era+"-ML.root output/"+era+"/*__*.root"
os.system(cmd)
cmd = "rm output/"+era+"/*__*.root"
os.system(cmd)

# Make pre-fit plots
print '-----------------------------------------------------------------------'
print "\nMake pre-fit plots \n"
#for category in category_list:
#    make_category_plots(category)
#num_cores=20
#Parallel(n_jobs=num_cores)(delayed(make_category_plots)(i) for i in category_list)

# Print confusione matrices
print '-----------------------------------------------------------------------'
print "\nPlot confusion matrices \n"
if embedded_c == 'true':
    cmd = "python ../../Plotting/Confusion.py -e \""+era+"\" -emb "
else:
    cmd = "python ../../Plotting/Confusion.py -e \""+era+"\""
#os.system(cmd)

# Measure stage0/inclusive signal strength constraint
print '-----------------------------------------------------------------------'
print '/nMeasure signal strength constraint \n'
os.environ["ERA"] = str(era)
os.environ["EMB"] = str(embedded)
cmd = "cp output/"+era+"/htt_em.inputs-sm-Run"+era+"-ML.root output/"+era+"/htt_em.inputs-sm-Run"+era+"-ML_save.root"
os.system(cmd)
cmd = "python fix_em_shapes.py output/"+era+"/htt_em.inputs-sm-Run"+era+"-ML_save.root output/"+era+"/htt_em.inputs-sm-Run"+era+"-ML.root"
os.system(cmd)
os.system("source ./measure_signal_strength.sh")
os.system("source ./measure_inclusive_signal_strength.sh")
#os.system("source ./plot_impacts_fit.sh")
cmd = "mv output/"+era+"/htt_em.inputs-sm-Run"+era+"-ML_save.root output/"+era+"/htt_em.inputs-sm-Run"+era+"-ML.root"
os.system(cmd)

# Make post-fit plots
print '-----------------------------------------------------------------------'
print '/nMake post-fit plots \n'
cmd = "root -l -b -q ../../Plotting/make_postfit_plots.cpp\"(true,true,\\\"output\\\",\\\""+era+"\\\","+str(embedded)+")\""
os.system(cmd)
cmd = "root -l -b -q ../../Plotting/make_postfit_plots.cpp\"(true,false,\\\"output\\\",\\\""+era+"\\\","+str(embedded)+")\""
os.system(cmd)