import os
import ROOT as R

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
                  "bpt_2",
                  "beta_1",
                  "beta_2",
                  "metphi",
                  ]

variable_list = [ "m_vis" ]


axis_range = { "m_sv"  : [8 , 0  , 300],
               "m_vis" : [8 , 0  , 300],
               "pt_1"  : [8 , 10 , 150],
               "pt_2"  : [8 , 10 , 150],
               "eta_1" : [8 , -2.5 , +2.5],
               "eta_2" : [8 ,-2.4 , +2.4],
               "jpt_1" : [8 , 30 , 400],
               "jpt_2" : [8 , 30 , 400],
               "njets" : [8 , 0  , 8],
               "nbtag" : [3 , 0  , 2],
               "mt_1"  : [8 , 0 , 400],
               "mt_2"  : [8 , 0 , 400],
               "pt_tt" : [8 , 0 , 300],
               "mjj"   : [8 , 0 , 600],
               "met"   : [8 , 0 , 150],
               "dzeta" : [8 , -35 , 100],
               "phi_1" : [8 , -R.TMath.Pi() , R.TMath.Pi()],
               "phi_2" : [8 , -R.TMath.Pi() , R.TMath.Pi()],
               "jphi_1"     : [8 , -R.TMath.Pi() , R.TMath.Pi()],
               "jphi_2"     : [8 , -R.TMath.Pi() , R.TMath.Pi()],
               "mTemu"      : [8 , 0, 400],
               "mt_sv"      : [8 ,0, 800 ],
               "pt_sv"      : [8 ,0, 200 ],
               "eta_sv"     : [8 ,-4.0,4.0 ],
               "phi_sv"     : [8 , -R.TMath.Pi() , R.TMath.Pi()],
               "iso_1"      : [8 , 0, 0.15],
               "iso_2"      : [8 , 0, 0.2],
               "d0_1"       : [8 , -0.045, 0.045],
               "d0_2"       : [8 ,-0.045, 0.045 ],
               "dZ_1"       : [8 ,-0.2,0.2 ],
               "dZ_2"       : [8 , -0.2,0.2 ],
               "mtmax"      : [8 , 0, 500],
               "dphi_mumet" : [8 , -R.TMath.Pi() , R.TMath.Pi()],
               "dphi_emet"  : [8 , -R.TMath.Pi() , R.TMath.Pi()],
               "msvmet"     : [8 ,0,300 ],
               "dr_tt"      : [8 ,0,2*R.TMath.Pi() ],
               "pzetavis"   : [8 ,0,200 ],
               "pzetamiss"  : [8 , -200,200],
               "njetspt20"  : [8 , 0, 8],
               "jeta_1"     : [8 ,-4.5,4.5 ],
               "jeta_2"     : [8 ,-4.5,4.5 ],
               "jdeta"      : [8 , 0, 9],
               "bpt_1"      : [8 , 0, 200],
               "bpt_2"      : [8 , 0, 200],
               "beta_1"     : [8 , -2.5,2.5],
               "beta_2"     : [8 ,  -2.5,2.5],
               "metphi"     : [8 , -R.TMath.Pi() , R.TMath.Pi()],
               }

# Execute produce_gof_inputs.cpp for all variables

for var in variable_list :

    print "****************************************************************************************"
    print "****************************************************************************************"
    print "Execute " + var + " : \n"

    # Get axis ranges (if not specified set to default value of {10,400})
    nbins , xmin , xmax = axis_range.get(var,[8, 0,400])

    cmd = "root -l -b -q produce_gof_input.cpp+\"(\\\""+var+"\\\" , " + str(nbins) + " , {"+str(xmin)+","+str(xmax)+"})\""
    os.system(cmd)

    # Now start to make the actual gof test
    os.environ["VAR"] = var
    os.system("source ./run_gof.sh")

    cmd = "root -l -b -q ../../Plotting/plot_1d_var.cpp\"(\\\""+var+"\\\")\""
    os.system(cmd)
