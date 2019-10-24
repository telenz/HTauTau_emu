# First you will need to import some modules
import ROOT as rt
import functions as f

# Plots won't pop up during creation
rt.gROOT.SetBatch(rt.kTRUE)

# Variable list and definition of x-axis ranges
variable_list = [  "pt_1"  ]
axis_range = {"pt_1" : [10,0,100] } 

# open tree
input_dir = "/nfs/dust/cms/user/tlenz/13TeV/2018/SM_HTauTau/HTauTau_emu/Inputs/NTuples_2017/"
f0,tree_Data   = f.openTree(input_dir+"/em-NOMINAL_ntuple_MuonEG.root","TauCheck")
f1,tree_DYJets = f.openTree(input_dir+"/em-NOMINAL_ntuple_DYJets.root","TauCheck")

# Definition of  selection
selection = "pt_2>13 && pt_1>10 && iso_1<0.15 && iso_2<0.2 && nbtag==0"

# Definition of weights to be applied
weight = "xsec_lumi_weight*mcweight*puweight*effweight*trigger_filter_weight"

# Create canvas
c = f.createCanvas()
c.cd()

for var in variable_list :

    # Get x-axis ranges
    nbins , xmin , xmax = axis_range.get(var,[50,0,200])

    # Create histograms
    h_Data = f.createH1(var,nbins,xmin,xmax)
    h_ZTT  = h_Data.Clone( "h_ZTT_"+var)

    h_ZTT = f.InitHist(h_ZTT,rt.TColor.GetColor("#FFCC66"));

    # Draw
    tree_Data.Draw( var + " >> "+h_Data.GetName(),"("+selection+")","hist" )
    tree_DYJets.Draw( var + " >> h_ZTT_"+var , weight+"*("+selection+" && isZTT)","hist" )
    print "data = " + str(h_Data.GetSumOfWeights())
    print "ZTT  = " + str(h_ZTT.GetSumOfWeights())
    print "Bkg  = " + str(h_ZTT.GetSumOfWeights() )

    bkg = rt.THStack("Background","");
    bkg.Add(h_ZTT);

    legend = rt.TLegend(0.65,0.7,0.89,0.92);
    legend.SetTextFont(42);
    legend.AddEntry(h_Data, "Observed", "ple");
    legend.AddEntry(h_ZTT,"Z#rightarrow #tau#tau","f");

    rp = rt.TRatioPlot(bkg , h_Data);
    rp.Draw();
    legend.Draw("same")

    # save plot
    c.Print("figures/"+var+".pdf")    

    rt.gDirectory.Delete("h1")
    rt.gDirectory.Delete("h2")



