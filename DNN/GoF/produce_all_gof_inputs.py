import os
import ROOT as R
import argparse
import sys

parser = argparse.ArgumentParser()
parser.add_argument('-e', dest='era', help='ERA' ,choices = ['2016','2017'], default = '2016')
parser.add_argument('-emb', dest='embedded',   help='embedded samples used' , action='store_true')
args = parser.parse_args()

era=args.era
if args.embedded == True:
    embedded=1
else :
    embedded=0

if era is "2016" and embedded :
    print "no embedded samples available for this era yet. Exit."
    print ""
    sys.exit()

# some settings
output_directory = "/nfs/dust/cms/user/tlenz/13TeV/2017/SM_HTauTau/HTauTau_emu/DNN/GoF/output/" + era +"/var_1d/"

dnn_ntuples_directory = "../../Inputs/NTuples_2016_tighter_cuts"
if era == "2017" :
    print "era is 2017"
    dnn_ntuples_directory = "../../Inputs/NTuples_2017_tighter_cuts"

variable_list = [ "m_sv",
                  "m_vis",
                  "pt_1",
                  "pt_2",
                  "eta_1",
                  "eta_2",
                  "phi_1",
                  "phi_2",
                  "jpt_1",
                  "jpt_2",
                  "jphi_1",
                  "jphi_2",
                  "njets",
                  "nbtag",
                  "mt_1",
                  "mt_2",
                  "pt_tt",
                  "mjj",
                  "met",
                  "dzeta",
                  "mTemu",
                  "mt_sv",
                  "pt_sv",
                  "eta_sv",
                  "phi_sv",
                  "iso_1",
                  "iso_2",
                  "d0_1",
                  "d0_2",
                  "dZ_1",
                  "dZ_2",
                  "mtmax",
                  "dphi_mumet",
                  "dphi_emet",
                  "msvmet",
                  "dr_tt",
                  "pzetavis",
                  "pzetamiss",
                  "njetspt20",
                  "jeta_1",
                  "jeta_2",
                  "jdeta",
                  #"bpt_1",
                  #"beta_1",
                  "metphi",
                  "d0_1_cal",
                  "dZ_1_cal",
                  "d0_2_cal",
                  "dZ_2_cal",
                  "mTdileptonMET",
                  "pt_ttjj",
                  "dijetphi",
                  "dijetpt",
                  "pt_vis",
                  ]

#variable_list = [ "pt_vis" ]

axis_range = { "m_sv"  : [8 , 0  , 300],
               "m_vis" : [8 , 0  , 300],
               "pt_1"  : [8 , 10 , 400],
               "pt_2"  : [8 , 10 , 300],
               "eta_1" : [8 , -2.5 , +2.5],
               "eta_2" : [8 ,-2.4 , +2.4],
               "jpt_1" : [8 , 30 , 600],
               "jpt_2" : [8 , 30 , 300],
               "njets" : [10 , 0  , 10],
               "nbtag" : [5 , 0  , 5],
               "mt_1"  : [8 , 0 , 300],
               "mt_2"  : [8 , 0 , 300],
               "pt_tt" : [8 , 0 , 600],
               "mjj"   : [8 , 0 , 1000],
               "met"   : [8 , 0 , 400],
               "dzeta" : [8 , -35 , 300],
               "phi_1" : [8 , -R.TMath.Pi() , R.TMath.Pi()],
               "phi_2" : [8 , -R.TMath.Pi() , R.TMath.Pi()],
               "jphi_1"     : [8 , -R.TMath.Pi() , R.TMath.Pi()],
               "jphi_2"     : [8 , -R.TMath.Pi() , R.TMath.Pi()],
               "mTemu"      : [8 , 0, 600],
               "mt_sv"      : [8 ,0, 3500 ],
               "pt_sv"      : [8 ,0, 400 ],
               "eta_sv"     : [8 ,-5.5,5.5 ],
               "phi_sv"     : [8 , -R.TMath.Pi() , R.TMath.Pi()],
               "iso_1"      : [8 , 0, 0.15],
               "iso_2"      : [8 , 0, 0.2],
               "d0_1"       : [8 , -0.045, 0.045],
               "d0_2"       : [8 ,-0.045, 0.045 ],
               "dZ_1"       : [8 ,-0.2,0.2 ],
               "dZ_2"       : [8 , -0.2,0.2 ],
               "mtmax"      : [8 , 0, 300],
               "dphi_mumet" : [8 , 0 , R.TMath.Pi()],
               "dphi_emet"  : [8 , 0 , R.TMath.Pi()],
               "msvmet"     : [8 , 0, 400 ],
               "dr_tt"      : [8 , 0,2*R.TMath.Pi() ],
               "pzetavis"   : [8 , 0 , 300 ],
               "pzetamiss"  : [8 , -50,300],
               "njetspt20"  : [10 , 0, 10],
               "jeta_1"     : [8 ,-4.5,4.5 ],
               "jeta_2"     : [8 ,-4.5,4.5 ],
               "jdeta"      : [8 , 0, 9],
               "bpt_1"      : [8 , 0, 550],
               "beta_1"     : [8 , -2.5,2.5],
               "metphi"     : [8 , -R.TMath.Pi() , R.TMath.Pi()],
               "d0_1_cal"   : [8 , -0.05 , +0.05],
               "dZ_1_cal"   : [8 , -0.20 , +0.20],
               "d0_2_cal"   : [8 , -0.05 , +0.05],
               "dZ_2_cal"   : [8 , -0.20 , +0.20],
               "mTdileptonMET" : [8 , 0 , 200],
               "pt_ttjj" : [8,0,1000],
               "dijetphi" : [8,0,1000] ,
               "dijetpt" : [8,0,1000],
               "pt_vis"     : [8 ,0, 400 ],
               }

# Execute produce_gof_inputs.cpp for all variables

for var in variable_list :

    print "****************************************************************************************"
    print "****************************************************************************************"
    print "Execute " + var + " : \n"

    # Get axis ranges (if not specified set to default value of {10,400})
    nbins , xmin , xmax = axis_range.get(var,[8, 0,400])

    # Produce the root-files (datacard input)
    cmd = "root -l -b -q produce_gof_input.cpp+\"(\\\"em_inclusive\\\",false,\\\""+var+"\\\" , " + str(nbins) + " , {"+str(xmin)+","+str(xmax)+"} , \\\"pt_2:m_vis\\\" , \\\"" + dnn_ntuples_directory + "\\\",\\\""+ era + "\\\"," + str(embedded) + ")\""
    os.system(cmd)

    # Now start to make the actual gof test
    os.environ["VAR"] = var
    os.environ["ERA"] = era
    os.environ["EMB"] = str(embedded)
    os.system("source ./run_gof.sh")

    # Plotting
    cmd="root -l -b -q ../../Plotting/plot_1d_var.cpp\"(\\\""+var+"\\\",\\\"em_inclusive\\\",false,false,\\\"" + output_directory + "\\\",\\\"" + era + "\\\"," + str(embedded) + ")\""
    os.system(cmd)
    cmd="root -l -b -q ../../Plotting/plot_1d_var.cpp\"(\\\""+var+"\\\",\\\"em_inclusive\\\",false,true,\\\"" + output_directory + "\\\",\\\"" + era + "\\\"," + str(embedded)+ ")\""
    os.system(cmd)
