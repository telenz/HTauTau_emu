import os
import ROOT as R

directory = "/nfs/dust/cms/user/tlenz/13TeV/2017/SM_HTauTau/HTauTau_emu/DNN/GoF/output/2016/var_1d/"

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
                  "bpt_1",
                  "beta_1",
                  "metphi",
                  "d0_1_cal",
                  "dZ_1_cal",
                  "d0_2_cal",
                  "dZ_2_cal"
                  ]

#variable_list = [ "m_sv" ]

axis_range = { "m_sv"  : [12 , 0  , 4000],
               "m_vis" : [12 , 0  , 800],
               "pt_1"  : [12 , 10 , 500],
               "pt_2"  : [12 , 10 , 400],
               "eta_1" : [12 , -2.5 , +2.5],
               "eta_2" : [12 ,-2.4 , +2.4],
               "jpt_1" : [12 , 30 , 900],
               "jpt_2" : [12 , 30 , 500],
               "njets" : [12 , 0  , 11],
               "nbtag" : [12, 0  , 2],
               "mt_1"  : [12 , 0 , 700],
               "mt_2"  : [12 , 0 , 500],
               "pt_tt" : [12 , 0 , 700],
               "mjj"   : [12, 0 , 3000],
               "met"   : [12 , 0 , 600],
               "dzeta" : [12 , -35 , 300],
               "phi_1" : [12 , -R.TMath.Pi() , R.TMath.Pi()],
               "phi_2" : [12 , -R.TMath.Pi() , R.TMath.Pi()],
               "jphi_1"     : [12 , -R.TMath.Pi() , R.TMath.Pi()],
               "jphi_2"     : [12 , -R.TMath.Pi() , R.TMath.Pi()],
               "mTemu"      : [12 , 0, 600],
               "mt_sv"      : [12 ,0, 3500 ],
               "pt_sv"      : [12 ,0, 600 ],
               "eta_sv"     : [12 ,-5.5,5.5 ],
               "phi_sv"     : [12 , -R.TMath.Pi() , R.TMath.Pi()],
               "iso_1"      : [12 , 0, 0.15],
               "iso_2"      : [12 , 0, 0.2],
               "d0_1"       : [12 , -0.045, 0.045],
               "d0_2"       : [12 ,-0.045, 0.045 ],
               "dZ_1"       : [12 ,-0.2,0.2 ],
               "dZ_2"       : [12 , -0.2,0.2 ],
               "mtmax"      : [12 , 0, 600],
               "dphi_mumet" : [12 , 0 , R.TMath.Pi()],
               "dphi_emet"  : [12 , 0 , R.TMath.Pi()],
               "msvmet"     : [12 ,0,500 ],
               "dr_tt"      : [12 ,0,2*R.TMath.Pi() ],
               "pzetavis"   : [12 ,0,400 ],
               "pzetamiss"  : [12 , -300,300],
               "njetspt20"  : [12 , 0, 13],
               "jeta_1"     : [12 ,-4.5,4.5 ],
               "jeta_2"     : [12 ,-4.5,4.5 ],
               "jdeta"      : [12 , 0, 9],
               "bpt_1"      : [12 , 0, 550],
               "beta_1"     : [12 , -2.5,2.5],
               "metphi"     : [12 , -R.TMath.Pi() , R.TMath.Pi()],
               "d0_1_cal"   : [12 , -0.05 , +0.05],
               "dZ_1_cal"   : [12 , -0.20 , +0.20],
               "d0_2_cal"   : [12 , -0.05 , +0.05],
               "dZ_2_cal"   : [12 , -0.20 , +0.20],
               }

# Execute produce_gof_inputs.cpp for all variables

for var in variable_list :

    print "****************************************************************************************"
    print "****************************************************************************************"
    print "Execute " + var + " : \n"

    # Get axis ranges (if not specified set to default value of {10,400})
    nbins , xmin , xmax = axis_range.get(var,[8, 0,400])

    # Produce the root-files (datacard input)
    cmd = "root -l -b -q produce_gof_input.cpp+\"(\\\"em_inclusive\\\",false,\\\""+var+"\\\" , " + str(nbins) + " , {"+str(xmin)+","+str(xmax)+"})\""
    os.system(cmd)

    # Now start to make the actual gof test
    os.environ["VAR"] = var
    os.system("source ./run_gof.sh")

    # Plotting
    cmd="root -l -b -q ../../Plotting/plot_1d_var.cpp\"(\\\""+var+"\\\",\\\"em_inclusive\\\",false,false,\\\"" + directory + "\\\")\""
    os.system(cmd)
    cmd="root -l -b -q ../../Plotting/plot_1d_var.cpp\"(\\\""+var+"\\\",\\\"em_inclusive\\\",false,true,\\\"" + directory + "\\\")\""
    os.system(cmd)
