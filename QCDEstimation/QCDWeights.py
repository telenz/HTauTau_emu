import ROOT as rt
import ConfigParser
from array import array
import argparse
import numpy as np


def main():

    rt.gStyle.SetOptFit(1);
    rt.gROOT.SetBatch(rt.kTRUE)

    parser = argparse.ArgumentParser()
    parser.add_argument('-e', dest='era', help='ERA' ,choices = ['2016','2017','2018'], default = '2016')
    args = parser.parse_args()

    era=args.era

    # read in config ------------------------------------------------------------------------------------------------------------------------------------------
    config = ConfigParser.ConfigParser()
    config.read('settings.cfg')

    if (era=="2018"): 
        inDir = config.get('INPUT','input_directory_2018')
    elif (era=="2017"):
        inDir = config.get('INPUT','input_directory_2017')
    elif  (era=="2016"):
        inDir = config.get('INPUT','input_directory_2016')

    preselection = config.get('SELECTION', 'preselection')
    selection_muon_antiiso = config.get('SELECTION','selection_muon_antiiso')
    selection_electron_antiiso = config.get('SELECTION','selection_electron_antiiso')
    selection_electron_iso = config.get('ISO_SELECTION_ELECTRON','iso_ele')
    selection_os = config.get('SELECTION','selection_os')
    selection_ss = config.get('SELECTION','selection_ss')

    jet_selection = dict(config.items('JET_BINS'))

    weights_common = config.get('COMMON_WEIGHTS', 'weights_common')
    weights_processes = dict(config.items('WEIGHTS_PER_PROCESS'))

    nbins = int(config.get('OUTPUT','nbins'))
    xmin = float(config.get('OUTPUT','xmin'))
    xmax = float(config.get('OUTPUT','xmax'))

    if (era=="2018"):
        outFile = rt.TFile(config.get('OUTPUT','outFile_2018'), "RECREATE")
    elif (era=="2017"):
        outFile = rt.TFile(config.get('OUTPUT','outFile_2017'), "RECREATE")
    elif (era=="2016"):
        outFile = rt.TFile(config.get('OUTPUT','outFile_2016'), "RECREATE")    


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



    c = rt.TCanvas("c", "canvas", 800, 800)

    for jetbin in jet_selection:
        bins = [0.3,0.9,1.5,2.0,2.4,2.8,3.2,3.6,4.0,4.4,4.8,6.0]  #v1
        nbins =11
        if (jetbin=="njet0") :
            bins = [0.3,1.2,2.0,2.6,3.2,3.8,4.4,5.0,6.0] #v1
            nbins=8
        h_os = rt.TH1F("hist_os","hist_os",nbins,array('d',bins))
        h_ss = rt.TH1F("hist_ss","hist_ss",nbins,array('d',bins))
        h_os_validation = rt.TH1F("hist_os_validation","hist_os_validation",nbins,array('d',bins))
        h_ss_validation = rt.TH1F("hist_ss_validation","hist_ss_validation",nbins,array('d',bins))

        h_os_bg = rt.TH1F("hist_os_bg","hist_os_bg",nbins,array('d',bins))
        h_ss_bg = rt.TH1F("hist_ss_bg","hist_ss_bg",nbins,array('d',bins))
        h_os_bg_validation = rt.TH1F("hist_os_bg_validation","hist_os_bg_validation",nbins,array('d',bins))
        h_ss_bg_validation = rt.TH1F("hist_ss_bg_validation","hist_ss_bg_validation",nbins,array('d',bins))
        h_os_bg_total = rt.TH1F("hist_os_bg_total","hist_os_bg_total",nbins,array('d',bins))
        h_ss_bg_total = rt.TH1F("hist_ss_bg_total","hist_ss_bg_total",nbins,array('d',bins))
        h_os_bg_total_validation = rt.TH1F("hist_os_bg_total_validation","hist_os_bg_total_validation",nbins,array('d',bins))
        h_ss_bg_total_validation = rt.TH1F("hist_ss_bg_total_validation","hist_ss_bg_total_validation",nbins,array('d',bins))

        h_os_bg_total = None
        h_ss_bg_total = None
        h_os_bg_total_validation = None
        h_ss_bg_total_validation = None
        
        t_Data.Draw("dr_tt>>hist_os",preselection + "&&" + selection_muon_antiiso + "&&" + selection_electron_iso + "&&" + selection_os + "&&" + jet_selection[jetbin])
        t_Data.Draw("dr_tt>>hist_ss",preselection + "&&" + selection_muon_antiiso + "&&" + selection_electron_iso + "&&" + selection_ss + "&&" + jet_selection[jetbin])
        t_Data.Draw("dr_tt>>hist_os_validation",preselection + "&&" + selection_muon_antiiso + "&&" + selection_electron_antiiso + "&&" + selection_os + "&&" + jet_selection[jetbin])
        t_Data.Draw("dr_tt>>hist_ss_validation",preselection + "&&" + selection_muon_antiiso + "&&" + selection_electron_antiiso + "&&" + selection_ss + "&&" + jet_selection[jetbin])
        print "os: data: integral " + str(h_os.Integral())
        print "ss: data: integral " + str(h_ss.Integral())
        for bg in bg_trees:
            bg_trees[bg].Draw("dr_tt>>hist_os_bg",weights_common + weights_processes[bg] + "(" + preselection + "&&" + selection_muon_antiiso + "&&" + selection_electron_iso + "&&" + selection_os + "&&" + jet_selection[jetbin] + ")")
            bg_trees[bg].Draw("dr_tt>>hist_ss_bg",weights_common + weights_processes[bg] + "(" + preselection + "&&" + selection_muon_antiiso + "&&" + selection_electron_iso + "&&" + selection_ss + "&&" + jet_selection[jetbin] + ")")
            bg_trees[bg].Draw("dr_tt>>hist_os_bg_validation",weights_common + weights_processes[bg] + "(" + preselection + "&&" + selection_muon_antiiso + "&&" + selection_electron_antiiso + "&&" + selection_os + "&&" + jet_selection[jetbin] + ")")
            bg_trees[bg].Draw("dr_tt>>hist_ss_bg_validation",weights_common + weights_processes[bg] + "(" + preselection + "&&" + selection_muon_antiiso + "&&" + selection_electron_antiiso + "&&" + selection_ss + "&&" + jet_selection[jetbin] + ")")
            print "os: background: integral " + bg + " " + str(h_os_bg.Integral())
            print "ss: background: integral " + bg + " " + str(h_ss_bg.Integral())
            if h_os_bg_total is None :
               h_os_bg_total = h_os_bg.Clone()
               h_ss_bg_total = h_ss_bg.Clone()
               h_os_bg_total_validation = h_os_bg_validation.Clone()
               h_ss_bg_total_validation = h_ss_bg_validation.Clone()
            else :
               h_os_bg_total.Add(h_os_bg)
               h_ss_bg_total.Add(h_ss_bg)
               h_os_bg_total_validation.Add(h_os_bg_validation)
               h_ss_bg_total_validation.Add(h_os_bg_validation)
        
        print "----------------------------------------------------------------------------------------"       
        print "BG / OS DATA = " + str(h_os_bg_total.Integral()/h_os.Integral()) + " in category " + str(jetbin)
        print "BG / SS DATA = " + str(h_ss_bg_total.Integral()/h_ss.Integral()) + " in category " + str(jetbin)

        print "BG / OS DATA (validation region) = " + str(h_os_bg_total_validation.Integral()/h_os_validation.Integral()) + " in category " + str(jetbin)
        print "BG / SS DATA (validation region) = " + str(h_ss_bg_total_validation.Integral()/h_ss_validation.Integral()) + " in category " + str(jetbin)
        print "----------------------------------------------------------------------------------------" 
        
        print "Data before subtraction : " + str(h_os.Integral())
        print "BG before subtraction : " + str(h_os_bg_total.Integral())
        h_os.Add(h_os_bg_total,-1.)
        h_ss.Add(h_ss_bg_total,-1.)
        print "Data after subtraction : " + str(h_os.Integral())
        h_os_validation.Add(h_os_bg_total_validation,-1.)
        h_ss_validation.Add(h_ss_bg_total_validation,-1.)

        ratio = h_os.Clone()
        ratio.Divide(ratio, h_ss)
        f1 = rt.TF1("f1", "[2]*((x-3)^2-3)+[1]*(x-3)+[0]",0.3,6.0 )
        #f1 = rt.TF1("f1", "[0]*(1-exp(0.2*(x-3)/[0]))+[2] + [3] *(x-3)",0.3,6.0 )
        f1.SetParameter(0,0.3)
        f1.SetParameter(1,0.7)
        f1.SetParameter(2,2.3)
        ratio.Fit("f1")
        ratio.GetXaxis().SetTitle("#DeltaR(e,#mu)")
        ratio.SetTitle("")
        ratio.GetYaxis().SetTitle("OS/SS transfer factor")
        ratio.Draw("E")
            
        p0_err  = f1.GetParError(0)
        p0  = f1.GetParameter(0)
        p1_err  = f1.GetParError(1)
        p1  = f1.GetParameter(1)
        p2_err  = f1.GetParError(2)
        p2  = f1.GetParameter(2)
        
        x =  np.arange(0.3,6.0,0.01)
        
        y = (p0+p0_err) + p1*(x-3) + p2*((x-3)*(x-3)-3)
        #y = (p0+p0_err)*(1-rt.TMath.Exp(p1*(x-3)))+p2
        f_0_up = rt.TGraph(x.size, x.astype(np.double),y.astype(np.double))
        y = (p0-p0_err) + p1*(x-3) + p2*((x-3)*(x-3)-3)
        #y = (p0-p0_err)*(1-rt.TMath.Exp(p1*(x-3)))+p2
        f_0_down = rt.TGraph(x.size, x.astype(np.double),y.astype(np.double))
        
        y = p0 + (p1+p1_err)*(x-3) + p2*((x-3)*(x-3)-3)
        #y = p0*(1-rt.TMath.Exp((p1-p1_err)*(x-3)))+p2
        f_1_up = rt.TGraph(x.size, x.astype(np.double),y.astype(np.double))
        #y = p0*(1-rt.TMath.Exp((p1+p1_err)*(x-3)))+p2
        y = p0 + (p1-p1_err)*(x-3)+ p2*((x-3)*(x-3)-3)
        f_1_down = rt.TGraph(x.size, x.astype(np.double),y.astype(np.double))
    
        y = p0 + p1*(x-3) + (p2-p2_err)*((x-3)*(x-3)-3)
        #y = p0*(1-rt.TMath.Exp(p1*(x-3)))+(p2-p2_err)
        f_2_up = rt.TGraph(x.size, x.astype(np.double),y.astype(np.double))
        y = p0 + p1*(x-3) + (p2+p2_err)*((x-3)*(x-3)-3)
        #y = p0*(1-rt.TMath.Exp(p1*(x-3)))+(p2+p2_err)
        f_2_down = rt.TGraph(x.size, x.astype(np.double),y.astype(np.double))
    
        #bins = []
        #n = xmax/0.05
        #for i in range(0,int(n+1)) :
        #    bins.append(0.05*i)
        #grint  =  rt.TGraphErrors(len(bins)-1);
        #for k in range(0,len(bins)-1) :
        #   grint.SetPoint(k, bins[k], 0);
        #(rt.TVirtualFitter.GetFitter()).GetConfidenceIntervals(grint,0.68);
        #grint.SetLineColor(rt.kBlue);

        #grint_Up = rt.TGraphErrors(len(bins)-1);
        #grint_Down = rt.TGraphErrors(len(bins)-1);
        #for k in range(0,len(bins)-1) :
        #   x = rt.Double(0)
        #   value = rt.Double(0)
        #   grint.GetPoint(k,x,value);
        #   value_up = value + grint.GetErrorYhigh(k);
        #   value_down = value - grint.GetErrorYhigh(k);
        #   grint_Up.SetPoint(k, bins[k], value_up );
        #   grint_Down.SetPoint(k, bins[k], value_down );
        f_0_up.SetLineColor(rt.kGreen+1)
        f_0_down.SetLineColor(rt.kGreen+1)    
        f_1_up.SetLineColor(rt.kOrange+1)
        f_1_down.SetLineColor(rt.kOrange+1) 
        f_2_up.SetLineColor(rt.kBlue+1)
        f_2_down.SetLineColor(rt.kBlue+1) 
        f_0_up.Draw("same")
        f_0_down.Draw("same")
        f_1_up.Draw("same")
        f_1_down.Draw("same")
        f_2_up.Draw("same")
        f_2_down.Draw("same")

        outFile.cd()
        transfer_function = ratio.GetFunction("f1");
        transfer_function.SetName("OS_SS_transfer_factors_" + jetbin)
        f_0_up.SetName("OS_SS_transfer_factors_Par0_" + jetbin + "_UP")
        f_0_down.SetName("OS_SS_transfer_factors_Par0_" + jetbin + "_DOWN")
        f_0_up.Write()
        f_0_down.Write()
        f_1_up.SetName("OS_SS_transfer_factors_Par1_" + jetbin + "_UP")
        f_1_down.SetName("OS_SS_transfer_factors_Par1_" + jetbin + "_DOWN")
        f_1_up.Write()
        f_1_down.Write()
        f_2_up.SetName("OS_SS_transfer_factors_Par2_" + jetbin + "_UP")
        f_2_down.SetName("OS_SS_transfer_factors_Par2_" + jetbin + "_DOWN")
        f_2_up.Write()
        f_2_down.Write()
        transfer_function.Write()
        c.Print("figures_"+era+"/transfer_factor_drtt_"+ jetbin + ".pdf")

        # validation factors -----------------------------------------------------------------------------------------------------------------------------------------------------------
        ratio_validation = h_os_validation.Clone()
        ratio_validation.Divide(ratio_validation, h_ss_validation)
        ratio_validation.Fit("f1")
        ratio_validation.Draw("E")

        grint_validation  =  rt.TGraphErrors(len(bins)-1);
        for k in range(0,len(bins)-1) :
            grint_validation.SetPoint(k, bins[k], 0);
        (rt.TVirtualFitter.GetFitter()).GetConfidenceIntervals(grint_validation,0.68);
        grint_validation.SetLineColor(rt.kBlue);

        grint_validation_Up = rt.TGraphErrors(len(bins)-1);
        grint_validation_Down = rt.TGraphErrors(len(bins)-1);
        for k in range(0,len(bins)-1) :
            x = rt.Double(0)
            value = rt.Double(0)
            grint_validation.GetPoint(k,x,value);
            value_up = value + grint_validation.GetErrorYhigh(k);
            value_down = value - grint_validation.GetErrorYhigh(k);
            grint_validation_Up.SetPoint(k, bins[k], value_up );
            grint_validation_Down.SetPoint(k, bins[k], value_down );

        grint_validation_Up.Draw("same")
        grint_validation_Down.Draw("same")

        outFile.cd()
        transfer_function_validation = ratio_validation.GetFunction("f1");
        transfer_function_validation.SetName("OS_SS_transfer_factors_validation_" + jetbin)
        grint_validation_Up.SetName("OS_SS_transfer_factors_validation_" + jetbin + "_UP")
        grint_validation_Down.SetName("OS_SS_transfer_factors_validation_" + jetbin + "_DOWN")
        grint_validation_Up.Write()
        grint_validation_Down.Write()
        transfer_function_validation.Write()
        c.Print("figures_"+era+"/transfer_factor_drtt_validation_"+ jetbin + ".pdf")

        del h_os
        del h_ss
        del h_os_validation
        del h_ss_validation

        del h_os_bg
        del h_ss_bg
        del h_os_bg_validation
        del h_ss_bg_validation
        del h_os_bg_total
        del h_ss_bg_total
        del h_os_bg_total_validation
        del h_ss_bg_total_validation

def openTree(filename,treename):
    f1 = rt.TFile(filename)
    tree = f1.Get(treename)
    return(f1,tree)
    





if __name__ == '__main__':
    main()
