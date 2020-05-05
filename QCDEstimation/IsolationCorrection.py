import ROOT as rt
import QCDWeights as qcdweights
from array import array
import json
import ConfigParser
import argparse

def main() :
    
    rt.gStyle.SetPaintTextFormat("4.2f")
    
    parser = argparse.ArgumentParser()
    parser.add_argument('-e', dest='era', help='ERA' ,choices = ['2016','2017','2018'], default = '2016')
    args = parser.parse_args()

    era=args.era
    rt.gStyle.SetPadTopMargin(0.05);
    rt.gStyle.SetPadBottomMargin(0.16);
    rt.gStyle.SetPadLeftMargin(0.14);
    rt.gStyle.SetPadRightMargin(0.18);

    rt.gROOT.SetBatch(rt.kTRUE)
    rt.gStyle.SetOptStat(0);
    config = ConfigParser.ConfigParser()
    config.read('settings.cfg')

    if (era=="2018"): 
        inDir = config.get('INPUT','input_directory_2018')
    elif (era=="2017"):
        inDir = config.get('INPUT','input_directory_2017')
    elif  (era=="2016"):
        inDir = config.get('INPUT','input_directory_2016')
    
    f_Data,t_Data = qcdweights.openTree(inDir+"em-NOMINAL_ntuple_MuonEG.root","TauCheck")
    f_DY,t_DY = qcdweights.openTree(inDir+"em-NOMINAL_ntuple_DYJets.root","TauCheck")
    f_SingleTop,t_SingleTop = qcdweights.openTree(inDir+"em-NOMINAL_ntuple_SingleTop.root","TauCheck")
    f_TTbar,t_TTbar = qcdweights.openTree(inDir+"em-NOMINAL_ntuple_TTbar.root","TauCheck")
    f_WJets,t_WJets = qcdweights.openTree(inDir+"em-NOMINAL_ntuple_WJets.root","TauCheck")
    f_Diboson,t_Diboson = qcdweights.openTree(inDir+"em-NOMINAL_ntuple_Diboson.root","TauCheck")
    bg_trees = {"dy" : t_DY, "singletop": t_SingleTop, "ttbar" : t_TTbar, "wjets": t_WJets, "diboson": t_Diboson}

    preselection = config.get('SELECTION', 'preselection')
    selection_muon = dict(config.items('ISO_SELECTION_MUON'))


   # weights for background processes
    weights_common = config.get('COMMON_WEIGHTS', 'weights_common')
    weights_processes = dict(config.items('WEIGHTS_PER_PROCESS'))

    binsx = json.loads(config.get('OUTPUT','binsx_iso'))
    binsy = json.loads(config.get('OUTPUT','binsy_iso'))

    ratios={"iso" : None , "antiiso" : None}

    for selection in selection_muon :
        h_OS = rt.TH2F("OS","OS",len(binsx)-1,array('d',binsx),len(binsx)-1,array('d',binsy))
        h_SS = rt.TH2F("SS","SS",len(binsx)-1,array('d',binsx),len(binsx)-1,array('d',binsy))

        h_OS_bg = rt.TH2F("OS_bg","OS_bg",len(binsx)-1,array('d',binsx),len(binsx)-1,array('d',binsy))
        h_SS_bg = rt.TH2F("SS_bg","SS_bg",len(binsx)-1,array('d',binsx),len(binsx)-1,array('d',binsy))
        h_OS_bg_total = rt.TH2F("hist_os_bg_total","hist_os_bg_total",len(binsx)-1,array('d',binsx),len(binsx)-1,array('d',binsy))
        h_SS_bg_total = rt.TH2F("hist_ss_bg_total","hist_ss_bg_total",len(binsx)-1,array('d',binsx),len(binsx)-1,array('d',binsy))
        h_OS_bg_total = None
        h_SS_bg_total = None
        t_Data.Draw("pt_1:pt_2>>OS",preselection + "&&" + config.get('SELECTION','selection_electron_antiiso') + "&&" + config.get('SELECTION','selection_os') + "&&" + selection_muon[selection])
        t_Data.Draw("pt_1:pt_2>>SS",preselection + "&&" + config.get('SELECTION','selection_electron_antiiso') + "&&" + config.get('SELECTION','selection_ss') + "&&" + selection_muon[selection])

        for bg in bg_trees:
            bg_trees[bg].Draw("pt_1:pt_2>>OS_bg",weights_common + weights_processes[bg] + "(" + preselection + "&&" + config.get('SELECTION','selection_electron_antiiso') + "&&" + config.get('SELECTION','selection_os') + "&&" + selection_muon[selection]  + ")")
            bg_trees[bg].Draw("pt_1:pt_2>>SS_bg",weights_common + weights_processes[bg] + "(" + preselection + "&&" + config.get('SELECTION','selection_electron_antiiso') + "&&" + config.get('SELECTION','selection_ss') + "&&" + selection_muon[selection] + ")")
            if h_OS_bg_total is None :
               h_OS_bg_total = h_OS_bg.Clone()
               h_SS_bg_total = h_SS_bg.Clone()
            else :
                 h_OS_bg_total.Add(h_OS_bg)
                 h_SS_bg_total.Add(h_SS_bg)


        h_OS.Add(h_OS_bg_total,-1.)
        h_SS.Add(h_SS_bg_total,-1.)

        ratio = h_OS.Clone()
        ratio.GetXaxis().SetTitle("p_{T} muon [GeV]")
        ratio.GetYaxis().SetTitle("p_{T} electron [GeV]")
        ratio.GetXaxis().SetTitleOffset(1.2)
        ratio.GetZaxis().SetTitleOffset(1.2)
        ratio.SetTitle("")
        ratio.GetZaxis().SetTitle("OS/SS transfer factor")
        ratio.Divide(h_OS, h_SS)
        ratios.update({selection : ratio})

        del h_OS
        del h_SS
        del h_OS_bg
        del h_SS_bg
        del h_OS_bg_total
        del h_SS_bg_total

    iso_correction = ratios["iso"].Clone()
    iso_correction.Divide(ratios["iso"], ratios["antiiso"])

    c = rt.TCanvas("c","c",800,800)
    iso_correction.Draw("textcolz")
    
    if (era=="2018"):
        fout = rt.TFile(config.get('OUTPUT','outFile_2018'),"UPDATE")
    elif (era=="2017"):
        fout = rt.TFile(config.get('OUTPUT','outFile_2017'),"UPDATE")
    elif (era=="2016"):
        fout = rt.TFile(config.get('OUTPUT','outFile_2016'),"UPDATE")
                
    fout.cd()
    iso_correction.SetName("IsolationCorrection")
    iso_correction.Write()
    c.Print("figures_"+era+"/iso_correction.pdf")

if __name__ == '__main__':
   main()
