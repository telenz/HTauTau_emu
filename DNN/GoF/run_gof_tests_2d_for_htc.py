import os
import ROOT as R
import argparse
import sys
from joblib import Parallel, delayed
import multiprocessing

parser = argparse.ArgumentParser()
parser.add_argument('-var', dest='var', help='variable to process', default = 'm_vis:m_vis')
parser.add_argument('-e', dest='era', help='ERA' ,choices = ['2016','2017'], default = '2016')
parser.add_argument('-emb', dest='embedded',   help='embedded samples used' , action='store_true')
args = parser.parse_args()

era=args.era
variable=args.var
if args.embedded == True:
    embedded=1
    embedded_c='true'
else :
    embedded=0
    embedded_c='false'

# some settings
output_directory = "/nfs/dust/cms/user/tlenz/13TeV/2017/SM_HTauTau/HTauTau_emu/DNN/GoF/output/" + era +"/var_2d/"

config_filename_in = "/nfs/dust/cms/user/tlenz/13TeV/2017/SM_HTauTau/HTauTau_emu/DNN/GoF/config_for_gof_2016_with_placeholders_2d.cfg"
if era == "2017" :
    print "era is 2017"
    config_filename_in = "/nfs/dust/cms/user/tlenz/13TeV/2017/SM_HTauTau/HTauTau_emu/DNN/GoF/config_for_gof_2017_with_placeholders_2d.cfg"

# Execute produce_gof_inputs.cpp for all variables

def process_vars(var):
    

    print "****************************************************************************************"
    print "****************************************************************************************"
    print "Execute " + var + " : \n"

    var1 = var.split(":")[0]
    var2 = var.split(":")[1]
    
    var12 = var1+'-'+var2

    # Produce the root-files (datacard input)
    # change config file
    with open(config_filename_in) as f:
        newText=f.read().replace('output_file_suffix =', 'output_file_suffix = ' + var12)
        newText=newText.replace('em_inclusive_variable = ', 'em_inclusive_variable = ' + var)
        newText=newText.replace('use_embedded = ', 'use_embedded = ' + embedded_c)
    with open("config_for_gof_"+era+"_"+var12+".cfg", "w") as f:
        f.write(newText)
    cmd = "root -l -b -q /nfs/dust/cms/user/tlenz/13TeV/2017/SM_HTauTau/HTauTau_emu/DNN/GoF/make_histograms.cpp\"(\\\""+"config_for_gof_"+era+"_"+var12+".cfg"+"\\\")\""
    os.system(cmd)

    # Now start to make the actual gof test
    os.environ["VAR"] = var12
    os.environ["ERA"] = era
    os.environ["EMB"] = str(embedded)
    os.environ["INPUT_FOLDER"] = 'var_2d'
    os.system("source /nfs/dust/cms/user/tlenz/13TeV/2017/SM_HTauTau/HTauTau_emu/DNN/GoF/run_gof.sh")

    # Plotting
    cmd="root -l -b -q /nfs/dust/cms/user/tlenz/13TeV/2017/SM_HTauTau/HTauTau_emu/Plotting/plot_1d_var.cpp\"(\\\""+var12+"\\\",\\\"em_inclusive\\\",false,false,\\\"" + output_directory + "\\\",\\\"" + era + "\\\"," + str(embedded) + ")\""
    os.system(cmd)
    cmd="root -l -b -q /nfs/dust/cms/user/tlenz/13TeV/2017/SM_HTauTau/HTauTau_emu/Plotting/plot_1d_var.cpp\"(\\\""+var12+"\\\",\\\"em_inclusive\\\",false,true,\\\"" + output_directory + "\\\",\\\"" + era + "\\\"," + str(embedded)+ ")\""
    os.system(cmd)

    os.system("rm config_for_gof_"+era+"_"+var12+".cfg")


process_vars(variable)
