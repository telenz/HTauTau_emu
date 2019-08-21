import ROOT as rt
import QCDWeights as qcdweights
from array import array
import json
import ConfigParser

def main() :

    rt.gROOT.SetBatch(rt.kTRUE)
    rt.gStyle.SetOptStat(0);
    config = ConfigParser.ConfigParser()
    config.read('settings.cfg')

    inDir = config.get('INPUT','input_directory')

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
    h_OS = rt.TH2F("OS","OS",len(binsx)-1,array('d',binsx),len(binsx)-1,array('d',binsy))
    h_SS = rt.TH2F("SS","SS",len(binsx)-1,array('d',binsx),len(binsx)-1,array('d',binsy))

    h_OS_bg = rt.TH2F("OS_bg","OS_bg",len(binsx)-1,array('d',binsx),len(binsx)-1,array('d',binsy))
    h_SS_bg = rt.TH2F("SS_bg","SS_bg",len(binsx)-1,array('d',binsx),len(binsx)-1,array('d',binsy))
    h_OS_bg_total = rt.TH2F("hist_os_bg_total","hist_os_bg_total",len(binsx)-1,array('d',binsx),len(binsx)-1,array('d',binsy))
    h_SS_bg_total = rt.TH2F("hist_ss_bg_total","hist_ss_bg_total",len(binsx)-1,array('d',binsx),len(binsx)-1,array('d',binsy))
    h_OS_bg_total = None
    h_SS_bg_total = None

    ratios={"iso" : h_OS , "antiiso" : h_SS}

    for selection in selection_muon :

        t_Data.Draw("pt_2:pt_1>>OS",preselection + "&&" + config.get('SELECTION','selection_electron_antiiso') + "&&" + config.get('SELECTION','selection_os') + "&&" + selection_muon[selection])
        t_Data.Draw("pt_2:pt_1>>SS",preselection + "&&" + config.get('SELECTION','selection_electron_antiiso') + "&&" + config.get('SELECTION','selection_ss') + "&&" + selection_muon[selection])

        for bg in bg_trees:
            bg_trees[bg].Draw("pt_2:pt_1>>OS_bg",weights_common + weights_processes[bg] + "(" + preselection + "&&" + config.get('SELECTION','selection_electron_antiiso') + "&&" + config.get('SELECTION','selection_os') + "&&" + selection_muon[selection]  + ")")
            bg_trees[bg].Draw("pt_2:pt_1>>SS_bg",weights_common + weights_processes[bg] + "(" + preselection + "&&" + config.get('SELECTION','selection_electron_antiiso') + "&&" + config.get('SELECTION','selection_ss') + "&&" + selection_muon[selection] + ")")
            if h_OS_bg_total is None :
               h_OS_bg_total = h_OS_bg.Clone()
               h_SS_bg_total = h_SS_bg.Clone()
            else :
                 h_OS_bg_total.Add(h_OS_bg)
                 h_SS_bg_total.Add(h_SS_bg)


        h_OS.Add(h_OS_bg_total,-1.)
        h_SS.Add(h_SS_bg_total,-1.)

        ratio = h_OS.Clone()
        ratio.Divide(h_OS, h_SS, 1., 1., "B")
        ratios.update({selection : ratio})

    iso_correction = ratios["iso"].Clone()
    iso_correction.Divide(ratios["iso"], ratios["antiiso"], 1., 1.,"B")

    c = rt.TCanvas("c","c",800,800)
    iso_correction.Draw("colz");
    fout = rt.TFile(config.get('OUTPUT','outFile'),"UPDATE")
    fout.cd()
    iso_correction.SetName("IsolationCorrection")
    iso_correction.Write()
    c.Print("figures/iso_correction.pdf")

if __name__ == '__main__':
   main()
