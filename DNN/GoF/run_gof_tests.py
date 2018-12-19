import os
import ROOT as R
import argparse
import sys
from joblib import Parallel, delayed
import multiprocessing

parser = argparse.ArgumentParser()
parser.add_argument('-e', dest='era', help='ERA' ,choices = ['2016','2017'], default = '2016')
parser.add_argument('-emb', dest='embedded',   help='embedded samples used' , action='store_true')
args = parser.parse_args()

era=args.era
if args.embedded == True:
    embedded=1
    embedded_c='true'
else :
    embedded=0
    embedded_c='false'

# some settings
output_directory = "/nfs/dust/cms/user/tlenz/13TeV/2017/SM_HTauTau/HTauTau_emu/DNN/GoF/output/" + era +"/var_1d/"
#output_directory = "/nfs/dust/cms/user/mameyer/SM_HiggsTauTau/HTauTau_emu/DNN/GoF/output/" + era +"/var_1d/"

config_filename_in = "config_for_gof_2016_with_placeholders.cfg"
if era == "2017" :
    print "era is 2017"
    config_filename_in = "config_for_gof_2017_with_placeholders.cfg"

variable_list = [ "m_sv",
                  "pt_sv",
                  "mt_sv",
                  "m_vis",
                  "pt_1",
                  "pt_2",
                  "jpt_1",
                  "jpt_2",
                  "njets",
                  "mt_1",
                  "mt_2",
                  "pt_tt",
                  "mjj",
                  "met",
                  "dzeta",
                  "mTemu",
                  "mtmax",
                  "dphi_mumet",
                  "dphi_emet",
                  "dr_tt",
                  "jeta_1",
                  "jeta_2",
                  "jdeta",
                  "mTdileptonMET",
                  "pt_ttjj",
                  "dijetpt",
                  "pt_vis",
                  ]

#variable_list = [ "jeta_1"]

# Execute produce_gof_inputs.cpp for all variables

def process_vars(var):
    

    print "****************************************************************************************"
    print "****************************************************************************************"
    print "Execute " + var + " : \n"

    # Produce the root-files (datacard input)
    # change config file
    with open(config_filename_in) as f:
        newText=f.read().replace('output_file_suffix =', 'output_file_suffix = ' + var)
        newText=newText.replace('em_inclusive_variable = ', 'em_inclusive_variable = ' + var)
        newText=newText.replace('use_embedded = ', 'use_embedded = ' + embedded_c)
    with open("config_for_gof_"+era+"_"+var+".cfg", "w") as f:
        f.write(newText)
    cmd = "root -l -b -q make_histograms.cpp\"(\\\""+"config_for_gof_"+era+"_"+var+".cfg"+"\\\")\""
    os.system(cmd)

    # Now start to make the actual gof test
    os.environ["VAR"] = var
    os.environ["ERA"] = era
    os.environ["EMB"] = str(embedded)
    os.environ["INPUT_FOLDER"] = 'var_1d'
    os.system("source ./run_gof.sh")

    # Plotting
    cmd="root -l -b -q ../../Plotting/plot_1d_var.cpp\"(\\\""+var+"\\\",\\\"em_inclusive\\\",false,false,\\\"" + output_directory + "\\\",\\\"" + era + "\\\"," + str(embedded) + ")\""
    os.system(cmd)
    cmd="root -l -b -q ../../Plotting/plot_1d_var.cpp\"(\\\""+var+"\\\",\\\"em_inclusive\\\",false,true,\\\"" + output_directory + "\\\",\\\"" + era + "\\\"," + str(embedded)+ ")\""
    os.system(cmd)

    os.system("rm config_for_gof_"+era+"_"+var+".cfg")


num_cores = multiprocessing.cpu_count()
print "available number of cores = " + str(num_cores)

num_cores = 16

results = Parallel(n_jobs=num_cores)(delayed(process_vars)(i) for i in variable_list)
