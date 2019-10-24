import ROOT as rt
import functions as f

rt.gROOT.SetBatch(rt.kTRUE)

variable_list = [  "pt_1"  , "m_vis" ]
axis_range = {"pt_1" : [10,0,100] , "pt_2" : [10,0,200] , "m_vis" : [20,0,200]} 

# open tree
inDir = "/nfs/dust/cms/user/tlenz/13TeV/2018/SM_HTauTau/HTauTau_emu/Inputs/NTuples_2017"
f0,tree_Data      = f.openTree(inDir+"/em-NOMINAL_ntuple_MuonEG.root","TauCheck")
f1,tree_DYJets    = f.openTree(inDir+"/em-NOMINAL_ntuple_DYJets.root","TauCheck")
f2,tree_WJets     = f.openTree(inDir+"/em-NOMINAL_ntuple_WJets.root" ,"TauCheck")
f3,tree_TTbar     = f.openTree(inDir+"/em-NOMINAL_ntuple_TTbar.root","TauCheck")
f4,tree_SingleTop = f.openTree(inDir+"/em-NOMINAL_ntuple_SingleTop.root","TauCheck")
f5,tree_Diboson   = f.openTree(inDir+"/em-NOMINAL_ntuple_Diboson.root","TauCheck")
f6,tree_ggH       = f.openTree(inDir+"/em-NOMINAL_ntuple_ggH.root","TauCheck")
f7,tree_VBFH      = f.openTree(inDir+"/em-NOMINAL_ntuple_VBFH.root","TauCheck")

# define selection
selection = "pt_2>30 && pt_1>30"

# define weights to apply
weight = "xsec_lumi_weight*mcweight*puweight*effweight" # checked

# create canvas
c = f.createCanvas()
c.cd()

for var in variable_list :

    # Get x-axis ranges
    nbins , xmin , xmax = axis_range.get(var,[50,0,200])

    # Create histograms
    h_Data = f.createH1(var,nbins,xmin,xmax)
    h_ZTT  = h_Data.Clone( "h_ZTT_"+var)
    h_ZLL  = h_Data.Clone( "h_ZLL_"+var)
    h_W    = h_Data.Clone( "h_W_"+var)
    h_TT   = h_Data.Clone( "h_TT_"+var)
    h_ST   = h_Data.Clone( "h_ST_"+var)
    h_VV   = h_Data.Clone( "h_VV_"+var)

    h_ZTT = f.InitHist(h_ZTT,rt.TColor.GetColor("#FFCC66"));
    h_ZLL = f.InitHist(h_ZLL,rt.TColor.GetColor("#DE5A6A"));
    h_W   = f.InitHist(h_W,rt.TColor.GetColor("#4496C8"));
    h_TT  = f.InitHist(h_TT,rt.TColor.GetColor("#9999CC"));
    h_ST  = f.InitHist(h_ST,rt.TColor.GetColor("#FFCCFF"));
    h_VV  = f.InitHist(h_VV,rt.TColor.GetColor("#6F2D35"));
    #h_QCD = f.InitHist(h_QCD,TColor::GetColor("#FFCCFF"));

    print(h_Data.GetName())

    # Draw
    tree_Data.Draw( var + " >> "+h_Data.GetName(),"("+selection+")","hist" )
    tree_DYJets.Draw( var + " >> h_ZTT_"+var , weight+"*("+selection+" && isZTT)","hist" )
    tree_DYJets.Draw( var + " >> h_ZLL_"+var , weight+"*("+selection+" && !isZTT)","hist" )
    tree_WJets.Draw( var + " >> h_W_"+var , weight+"*("+selection+")","hist" )
    tree_TTbar.Draw( var + " >> h_TT_"+var , weight+"*("+selection+")","hist" )
    tree_SingleTop.Draw( var + " >> h_ST_"+var , weight+"*("+selection+")","hist" )
    tree_Diboson.Draw( var + " >> h_VV_"+var , weight+"*("+selection+")","hist" )
    print "data = " + str(h_Data.GetSumOfWeights())
    print "ZTT  = " + str(h_ZTT.GetSumOfWeights())
    print "ZLL  = " + str(h_ZLL.GetSumOfWeights())
    print "W    = " + str(h_W.GetSumOfWeights())
    print "TT   = " + str(h_TT.GetSumOfWeights())
    print "ST   = " + str(h_ST.GetSumOfWeights())
    print "VV   = " + str(h_VV.GetSumOfWeights())
    print "Bkg  = " + str(h_ZTT.GetSumOfWeights() + h_ZLL.GetSumOfWeights() + h_W.GetSumOfWeights() + h_TT.GetSumOfWeights() + h_ST.GetSumOfWeights() + h_VV.GetSumOfWeights() )

    bkg = rt.THStack("Background","");
    bkg.Add(h_ST);
    bkg.Add(h_VV);
    bkg.Add(h_W);
    bkg.Add(h_ZLL);
    bkg.Add(h_TT);
    bkg.Add(h_ZTT);

    legend = rt.TLegend(0.65,0.7,0.89,0.92);
    legend.SetTextFont(42);
    legend.AddEntry(h_Data, "Observed", "ple");
    legend.AddEntry(h_ZTT,"Z#rightarrow #tau#tau","f");
    legend.AddEntry(h_TT,"t#bar{t}","f");
    legend.AddEntry(h_ZLL,"Z#rightarrow #mu#mu/ee","f");
    legend.AddEntry(h_W,"W+jets","f");
    legend.AddEntry(h_VV,"diboson","f");
    legend.AddEntry(h_ST,"single top","f");

    rp = rt.TRatioPlot(bkg , h_Data);
    rp.Draw();
    legend.Draw("same")

    # if h2.GetMaximum() > h1.GetMaximum():
    #     h1.SetMaximum(h2.GetMaximum()*1.1)

    # save plot
    c.Print("figures/"+var+".pdf")    

    rt.gDirectory.Delete("h1")
    rt.gDirectory.Delete("h2")



