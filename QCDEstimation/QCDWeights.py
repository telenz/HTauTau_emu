import ROOT as rt
import ConfigParser

def main():

    rt.gStyle.SetOptFit(1);
    rt.gROOT.SetBatch(rt.kTRUE)

    # read in config ------------------------------------------------------------------------------------------------------------------------------------------
    config = ConfigParser.ConfigParser()
    config.read('settings.cfg')

    inDir = config.get('INPUT','input_directory')

    preselection = config.get('SELECTION', 'preselection')
    selection_muon_antiiso = config.get('SELECTION','selection_muon_antiiso')
    selection_os = config.get('SELECTION','selection_os')
    selection_ss = config.get('SELECTION','selection_ss')

    jet_selection = dict(config.items('JET_BINS'))

    weights_common = config.get('COMMON_WEIGHTS', 'weights_common')
    weights_processes = dict(config.items('WEIGHTS_PER_PROCESS'))

    nbins = int(config.get('OUTPUT','nbins'))
    xmin = float(config.get('OUTPUT','xmin'))
    xmax = float(config.get('OUTPUT','xmax'))

    outFile = rt.TFile(config.get('OUTPUT','outFile'), "RECREATE")


    # read in trees ---------------------------------------------------------------------------------------------------------------------------------------------
    f_Data,t_Data = openTree(inDir+"em-NOMINAL_ntuple_MuonEG.root","TauCheck")
    f_DY,t_DY = openTree(inDir+"em-NOMINAL_ntuple_DYJets.root","TauCheck")
    f_SingleTop,t_SingleTop = openTree(inDir+"em-NOMINAL_ntuple_SingleTop.root","TauCheck")
    f_TTbar,t_TTbar = openTree(inDir+"em-NOMINAL_ntuple_TTbar.root","TauCheck")
    f_WJets,t_WJets = openTree(inDir+"em-NOMINAL_ntuple_WJets.root","TauCheck")
    f_Diboson,t_Diboson = openTree(inDir+"em-NOMINAL_ntuple_Diboson.root","TauCheck")
    bg_trees = {"dy" : t_DY, "singletop": t_SingleTop, "ttbar" : t_TTbar, "wjets": t_WJets, "diboson": t_Diboson}

    # -----------------------------------------------------------------------------------------------------------------------------------------------------------
    # OS/SS transfer factors as function of DeltaR(e,mu) & njets

    h_os = rt.TH1F("hist_os","hist_os",nbins,xmin,xmax)
    h_ss = rt.TH1F("hist_ss","hist_ss",nbins,xmin,xmax)

    h_os_bg = rt.TH1F("hist_os_bg","hist_os_bg",nbins,xmin,xmax)
    h_ss_bg = rt.TH1F("hist_ss_bg","hist_ss_bg",nbins,xmin,xmax)
    h_os_bg_total = rt.TH1F("hist_os_bg_total","hist_os_bg_total",nbins,xmin,xmax)
    h_ss_bg_total = rt.TH1F("hist_ss_bg_total","hist_ss_bg_total",nbins,xmin,xmax)
    h_os_bg_total = None
    h_ss_bg_total = None

    c = rt.TCanvas("c", "canvas", 800, 800)

    for jetbin in jet_selection:

        t_Data.Draw("dr_tt>>hist_os",preselection + "&&" + selection_muon_antiiso + "&&" + selection_os + "&&" + jet_selection[jetbin])
        t_Data.Draw("dr_tt>>hist_ss",preselection + "&&" + selection_muon_antiiso + "&&" + selection_ss + "&&" + jet_selection[jetbin])

        for bg in bg_trees:
            bg_trees[bg].Draw("dr_tt>>hist_os_bg",weights_common + weights_processes[bg] + "(" + preselection + "&&" + selection_muon_antiiso + "&&" + selection_os + "&&" + jet_selection[jetbin] + ")")
            bg_trees[bg].Draw("dr_tt>>hist_ss_bg",weights_common + weights_processes[bg] + "(" + preselection + "&&" + selection_muon_antiiso + "&&" + selection_ss + "&&" + jet_selection[jetbin] + ")")
            if h_os_bg_total is None :
               h_os_bg_total = h_os_bg.Clone()
               h_ss_bg_total = h_ss_bg.Clone()
            else :
               h_os_bg_total.Add(h_os_bg)
               h_ss_bg_total.Add(h_ss_bg)
               h_ss_bg_total.Draw("same")

        print "BG / OS DATA = " + str(h_os_bg_total.Integral()/h_os.Integral()) + " in category " + str(jetbin)
        print "BG / SS DATA = " + str(h_ss_bg_total.Integral()/h_ss.Integral()) + " in category " + str(jetbin)

        h_os.Add(h_os_bg_total,-1.)
        h_ss.Add(h_ss_bg_total,-1.)

        ratio = h_os.Clone()
        ratio.Divide(h_os, h_ss, 1., 1., "B")
        ratio.Fit("pol1")
        ratio.Draw("E")

        bins = []
        n = xmax/0.05
        for i in range(0,int(n+1)) :
            bins.append(0.05*i)
        grint  =  rt.TGraphErrors(len(bins)-1);
        for k in range(0,len(bins)-1) :
           grint.SetPoint(k, bins[k], 0);
        (rt.TVirtualFitter.GetFitter()).GetConfidenceIntervals(grint,0.68);
        grint.SetLineColor(rt.kBlue);

        grint_Up = rt.TGraphErrors(len(bins)-1);
        grint_Down = rt.TGraphErrors(len(bins)-1);
        for k in range(0,len(bins)-1) :
           x = rt.Double(0)
           value = rt.Double(0)
           grint.GetPoint(k,x,value);
           value_up = value + grint.GetErrorYhigh(k);
           value_down = value - grint.GetErrorYhigh(k);
           grint_Up.SetPoint(k, bins[k], value_up );
           grint_Down.SetPoint(k, bins[k], value_down );

        grint_Up.Draw("same")
        grint_Down.Draw("same")

        outFile.cd()
        transfer_function = ratio.GetFunction("pol1");
        transfer_function.SetName("OS_SS_transfer_factors_" + jetbin)
        grint_Up.SetName("OS_SS_transfer_factors_" + jetbin + "_UP")
        grint_Down.SetName("OS_SS_transfer_factors_" + jetbin + "_DOWN")
        grint_Up.Write()
        grint_Down.Write()
        transfer_function.Write()
        c.Print("figures/transfer_factor_drtt_"+ jetbin + ".pdf")


def openTree(filename,treename):
    f1 = rt.TFile(filename)
    tree = f1.Get(treename)
    return(f1,tree)


if __name__ == '__main__':
    main()
