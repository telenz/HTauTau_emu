import os
import ROOT as R
import argparse
import sys
from joblib import Parallel, delayed
import multiprocessing

parser = argparse.ArgumentParser()
parser.add_argument('-e', dest='era', help='ERA' ,choices = ['2016','2017','2018'], default = '2016')
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
#output_directory = "/nfs/dust/cms/user/tlenz/13TeV/2018/SM_HTauTau/HTauTau_emu/DNN/GoF/output/" + era +"/var_1d/"
output_directory = "/nfs/dust/cms/user/mameyer/SM_HiggsTauTau/HTauTau_emu/DNN/GoF/output/" + era +"/var_1d/"

config_filename_in = "config_for_gof_2016_with_placeholders.cfg"
if era == "2017" :
    print "era is 2017"
    config_filename_in = "config_for_gof_2017_with_placeholders.cfg"
if era == "2018" :
    print "era is 2018"
    config_filename_in = "config_for_gof_2018_with_placeholders.cfg"

#variable_list = [ "m_sv",
#                  "m_vis",
#                  "pt_1",
#                  "pt_2",
#                  "jpt_1",
#                  "jpt_2",
#                  "met",
#                  ]

#variable_list = [ "ME_vbf_vs_Z",
#                  "ME_ggh_vs_Z",
#                  "ME_vbf_vs_ggh",
#                  "ME_q2v1",
#                  "ME_q2v2",
#                  "ME_costheta1",
#                  "ME_costheta2",
#                  "ME_costhetastar",
#                  "ME_phi1",
#                  "ME_phi",
#]
#variable_list = [ "ME_vbf_vs_ggh",
#]

#variable_list = [ "pt_1",
#              "pt_2",
#]

#variable_list = [ "ME_q2v1",
#                  "ME_q2v2",
#]

#variable_list = [ "ME_costheta1",
#                  "ME_costheta2",
#                  "ME_costhetastar"
#]

#variable_list = [ "ME_phi1",
#                  "ME_phi",
#]
#variable_list = [ "nbtag",
#                  "bpt_1"
#]
#pruned list from Simon (core set + extended set), based on selection without b jet veto, pt_tt not in extended set for emu, but in other channels
variable_list = [ "m_vis",
                  "pt_vis",
                  "pt_2",
                  "m_sv",
                  "jpt_1",
                  "dijetpt",
                  "mjj",
                  "njets",
                  "pt_1",
                  "ME_q2v1",
                  "ME_q2v2",
                  "pt_tt",
                  "jpt_2",
                  "dr_tt",
                  "mTdileptonMET",
                  "jdeta", 
                  "met",
                  "iso_1",
                  "iso_2", 
                  "nbtag",
                  "bpt_1",
                  "dzeta",
                  "mTemu",
                  "mt_1",
                  "mt_2"                   
]
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
    cmd="root -l -b -q ../../Plotting/plot_1d_var.cpp\"(\\\""+var+"\\\",\\\"em_inclusive\\\",true,false,\\\"" + output_directory + "\\\",\\\"" + era + "\\\"," + str(embedded) + ", false)\""
    os.system(cmd)
    cmd="root -l -b -q ../../Plotting/plot_1d_var.cpp\"(\\\""+var+"\\\",\\\"em_inclusive\\\",true,true,\\\"" + output_directory + "\\\",\\\"" + era + "\\\"," + str(embedded)+ ", false)\""
    os.system(cmd)
    cmd="root -l -b -q ../../Plotting/plot_1d_var.cpp\"(\\\""+var+"\\\",\\\"em_inclusive\\\",true,false,\\\"" + output_directory + "\\\",\\\"" + era + "\\\"," + str(embedded) + ", true)\""
    os.system(cmd)
    cmd="root -l -b -q ../../Plotting/plot_1d_var.cpp\"(\\\""+var+"\\\",\\\"em_inclusive\\\",true,true,\\\"" + output_directory + "\\\",\\\"" + era + "\\\"," + str(embedded)+ ", true)\""
    os.system(cmd)

    cmd="root -l -b -q ../../Plotting/make_postfit_plots_GoF.cpp\"(\\\""+var+"\\\",true,false,\\\"" + output_directory + "\\\",\\\"" + era + "\\\"," + str(embedded) + ", false, true)\""
    os.system(cmd)
    cmd="root -l -b -q ../../Plotting/make_postfit_plots_GoF.cpp\"(\\\""+var+"\\\",true,true,\\\"" + output_directory + "\\\",\\\"" + era + "\\\"," + str(embedded)+ ", false, true)\""
    os.system(cmd)
    cmd="root -l -b -q ../../Plotting/make_postfit_plots_GoF.cpp\"(\\\""+var+"\\\",true,false,\\\"" + output_directory + "\\\",\\\"" + era + "\\\"," + str(embedded) + ", true, true)\""
    os.system(cmd)
    cmd="root -l -b -q ../../Plotting/make_postfit_plots_GoF.cpp\"(\\\""+var+"\\\",true,true,\\\"" + output_directory + "\\\",\\\"" + era + "\\\"," + str(embedded)+ ", true, true)\""
    os.system(cmd)
    
    cmd="root -l -b -q ../../Plotting/make_postfit_plots_GoF.cpp\"(\\\""+var+"\\\",true,false,\\\"" + output_directory + "\\\",\\\"" + era + "\\\"," + str(embedded) + ", false, false)\""
    os.system(cmd)
    cmd="root -l -b -q ../../Plotting/make_postfit_plots_GoF.cpp\"(\\\""+var+"\\\",true,true,\\\"" + output_directory + "\\\",\\\"" + era + "\\\"," + str(embedded)+ ", false, false)\""
    os.system(cmd)
    cmd="root -l -b -q ../../Plotting/make_postfit_plots_GoF.cpp\"(\\\""+var+"\\\",true,false,\\\"" + output_directory + "\\\",\\\"" + era + "\\\"," + str(embedded) + ", true, false)\""
    os.system(cmd)
    cmd="root -l -b -q ../../Plotting/make_postfit_plots_GoF.cpp\"(\\\""+var+"\\\",true,true,\\\"" + output_directory + "\\\",\\\"" + era + "\\\"," + str(embedded)+ ", true, false)\""
    os.system(cmd)

    os.system("rm config_for_gof_"+era+"_"+var+".cfg")


num_cores = multiprocessing.cpu_count()
print "available number of cores = " + str(num_cores)

num_cores = num_cores/2

print "number of cores that will be used = " + str(num_cores)

results = Parallel(n_jobs=num_cores)(delayed(process_vars)(i) for i in variable_list)
