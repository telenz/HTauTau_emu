import ROOT as rt
import QCDWeights as qcdweights
import ConfigParser
import json
from array import array
from ROOT import TF1

def main() :

    rt.gStyle.SetOptStat(0);
    rt.gROOT.SetBatch(rt.kTRUE)
    config = ConfigParser.ConfigParser()
    config.read('settings.cfg')

    inDir = config.get('INPUT','input_directory')

    f_Data,t_Data = qcdweights.openTree(inDir+"em-NOMINAL_ntuple_MuonEG.root","TauCheck")
    f_DY,t_DY = qcdweights.openTree(inDir+"em-NOMINAL_ntuple_DYJets.root","TauCheck")
    f_SingleTop,t_SingleTop = qcdweights.openTree(inDir+"em-NOMINAL_ntuple_SingleTop.root","TauCheck")
    f_TTbar,t_TTbar = qcdweights.openTree(inDir+"em-NOMINAL_ntuple_TTbar.root","TauCheck")
    f_WJets,t_WJets = qcdweights.openTree(inDir+"em-NOMINAL_ntuple_WJets.root","TauCheck")
    f_Diboson,t_Diboson = qcdweights.openTree(inDir+"em-NOMINAL_ntuple_Diboson.root","TauCheck")
    trees = {"data" : t_Data, "dy" : t_DY, "singletop": t_SingleTop, "ttbar" : t_TTbar, "wjets": t_WJets, "diboson": t_Diboson}

    f_weights = rt.TFile(config.get('OUTPUT','outFile'), "UPDATE")
    f_weights.cd()
    transfer_functions_0jet = f_weights.Get("OS_SS_transfer_factors_njet0")
    transfer_functions_Gt0jet = f_weights.Get("OS_SS_transfer_factors_njetgt0")

    binsx = json.loads(config.get('OUTPUT','binsx'))
    binsy = json.loads(config.get('OUTPUT','binsy'))
    h_OS = rt.TH2F("OS","OS",len(binsx)-1,array('d',binsx),len(binsx)-1,array('d',binsy))
    h_SS = rt.TH2F("SS","SS",len(binsx)-1,array('d',binsx),len(binsx)-1,array('d',binsy))

    h_OS_bg = rt.TH2F("OS_bg","OS_bg",len(binsx)-1,array('d',binsx),len(binsx)-1,array('d',binsy))
    h_SS_bg = rt.TH2F("SS_bg","SS_bg",len(binsx)-1,array('d',binsx),len(binsx)-1,array('d',binsy))

    c = rt.TCanvas("c","c",800,800)

    for process in trees :
        for event in trees[process] :
            iso_1 = getattr(event, "iso_1")
            iso_2 = getattr(event, "iso_2")
            pt_1 = getattr(event, "pt_1")
            pt_2 = getattr(event, "pt_2")
            os = getattr(event, "os")
            njets = getattr(event, "njets")
            dr_tt = getattr(event, "dr_tt")
            dzeta = getattr(event, "dzeta")
            mTdileptonMET = getattr(event, "mTdileptonMET")
            xsec_lumi_weight = getattr(event, "xsec_lumi_weight")
            mcweight = getattr(event, "mcweight")
            puweight = getattr(event, "puweight")
            effweight = getattr(event, "effweight")
            trigger_filter_weight = getattr(event, "trigger_filter_weight")
            zptmassweight = getattr(event, "zptmassweight")
            topptweight = getattr(event, "topptweight")
            prefiring_weight = getattr(event, "prefiring_weight")
            if process == "data" :
               weight = 1.0
            else :
               weight = config.get('COMMON_WEIGHTS','weights_common')
            if process == "dy" :
               weight = weight * config.get('WEIGHTS_PER_PROCESS','dy')
            if process == "ttbar" :
               weight = weight * config.get('WEIGHTS_PER_PROCESS','ttbar')
            if pt_1 < config.get('SELECTION','pt_ele_cut') :
                continue
            if pt_2 < config.get('SELECTION','pt_muon_cut') :
                continue
            if rt.TMath.Max(pt_1,pt_2) < config.get('SELECTION','maxpt_cut') :
                continue
            if dzeta < config.get('SELECTION','dzeta_cut') :
                continue
            if mTdileptonMET > config.get('SELECTION','mtdileptonmet_cut') :
                continue
            if iso_1 > config.get('ISO_SELECTION_ELECTRON','iso_ele') :
               continue
            if (iso_2 < config.get('ISO_SELECTION_MUON','iso') ) or (iso_2 > config.get('SELECTION','iso_max')) :
               continue
            if njets==0 :
               qcdweight = transfer_functions_0jet.Eval(dr_tt)
            else :
               qcdweight = transfer_functions_Gt0jet.Eval(dr_tt)
            if os > 0.5 :
               if process == "data" :
                  h_OS.Fill(pt_1,pt_2,weight)
               else :
                  h_OS_bg.Fill(pt_1,pt_2,weight)
            else :
               if process == "data" :
                  h_SS.Fill(pt_1,pt_2,weight*qcdweight)
               else :
                  h_SS_bg.Fill(pt_1,pt_2,weight*qcdweight)

    h_OS.Add(h_OS_bg,-1.)
    h_SS.Add(h_SS_bg,-1.)
    ratio = h_OS.Clone()
    ratio.Divide(h_OS, h_SS , 1., 1., "B")
    ratio.Draw("colz")
    c.Update()
    c.Print("figures/non_closure_correction_pt.pdf")

    f_weights.cd()
    ratio.SetName("NonClosureCorrection")
    ratio.Write()


if __name__ == '__main__' :
   main()
