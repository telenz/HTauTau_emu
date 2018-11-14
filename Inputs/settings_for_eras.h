#ifndef SETTINGS_FOR_ERAS_H
#define SETTINGS_FOR_ERAS_H

// **************************************************************************************************
// Define the subsamples that belong to a certain process
// 2017
const vector<TString> MuonEG_Run2017       = { "MuonEG_Run2017B" ,
					       "MuonEG_Run2017CtoF"};
const vector<TString> DYJets_2017          = { "DY1JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8" ,
					       "DY2JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8" ,
					       "DY3JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8" ,
					       "DY4JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8" ,
					       "DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8" };
const vector<TString> WJets_2017           = { "W1JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8" ,
					       "W2JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8" ,
					       "W3JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8" ,
					       "W4JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8" ,
					       "WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8" };
const vector<TString> TTbar_2017           = { "TTTo2L2Nu_TuneCP5_PSweights_13TeV-powheg-pythia8" ,
					       "TTToHadronic_TuneCP5_PSweights_13TeV-powheg-pythia8" ,
					       "TTToSemiLeptonic_TuneCP5_PSweights_13TeV-powheg-pythia8" };
const vector<TString> SingleTop_2017       = { "ST_t-channel_antitop_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8" ,
					       "ST_t-channel_top_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8" ,
					       "ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8" ,
					       "ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8" };
const vector<TString> Diboson_2017         = { "WW_TuneCP5_13TeV-pythia8" ,
					       "WZ_TuneCP5_13TeV-pythia8" ,
					       "ZZ_TuneCP5_13TeV-pythia8" };
const vector<TString> GluGluHToTauTau_2017 = { "GluGluHToTauTau_M125_13TeV_powheg_pythia8" };
const vector<TString> VBFHToTauTau_2017    = { "VBFHToTauTau_M125_13TeV_powheg_pythia8" };
const vector<TString> Embedded             = { "Embeddind_Run2017" };

// 2016
const vector<TString> MuonEG_Run2016  = { "MuonEG_Run2016B" , "MuonEG_Run2016C" , "MuonEG_Run2016D" , "MuonEG_Run2016E" , "MuonEG_Run2016F" , "MuonEG_Run2016G" , "MuonEG_Run2016H" };
const vector<TString> DYJets_2016     = { "DY1JetsToLL_M-50" , "DY2JetsToLL_M-50" , "DY3JetsToLL_M-50" , "DY4JetsToLL_M-50" , "DYJetsToLL_M-50" , "DYJetsToLL_M-10to50" , "EWKZ2Jets" };
const vector<TString> WJets_2016      = { "W1JetsToLNu" , "W2JetsToLNu" , "W3JetsToLNu" , "W4JetsToLNu" , "WJetsToLNu" , "WGToLNuG" , "WGstarToLNuEE" , "WGstarToLNuMuMu" , "EWKWPlus2Jet" , "EWKWMinus2Jet" };
const vector<TString> TTbar_2016      = { "TTbar" };
const vector<TString> SingleTop_2016  = { "ST_t-channel_antitop" , "ST_t-channel_top" , "ST_tW_antitop" , "ST_tW_top" };
const vector<TString> Diboson_2016    = { "VVTo2L2Nu" , "WZJToLLLNu" , "WZTo1L1Nu2Q" , "WZTo1L3Nu" , "WZTo2L2Q" , "ZZTo2L2Q" , "ZZTo4L" , "WWToLNuQQ" };
const vector<TString> GluGluHToTauTau_2016 = { "GluGluHToTauTau_M125" };
const vector<TString> VBFHToTauTau_2016    = { "VBFHToTauTau_M125" };
// **************************************************************************************************
// **************************************************************************************************
// Cross-section map ( needs to be checked again )
// 2017 (needs to be checked again)
const map<TString, double> xsec_map_2017 = {
  { "DYJetsToLL_M-10to50"      , 15820*1.079 },
  { "DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8"  , 5345*1.079 },
  { "DY1JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8" , 875.7*1.079 },
  { "DY2JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8" , 306.9*1.079 },
  { "DY3JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8" , 111.9*1.079 },
  { "DY4JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8" , 43.97*1.079 },
  { "WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8"  , 52760*1.166 },
  { "W1JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8" , 8104.*1.166 },
  { "W2JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8" , 2796.*1.166 },
  { "W3JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8" , 993.5*1.166 },
  { "W4JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8" , 544.4*1.166 },
  { "TTTo2L2Nu_TuneCP5_PSweights_13TeV-powheg-pythia8"        , 88.29 },
  { "TTToHadronic_TuneCP5_PSweights_13TeV-powheg-pythia8"     , 377.96 },
  { "TTToSemiLeptonic_TuneCP5_PSweights_13TeV-powheg-pythia8" , 365.34 },
  { "ST_t-channel_antitop_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8" , 26.38 },
  { "ST_t-channel_top_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8"     , 44.33 },
  { "ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8"                  , 35.85 },
  { "ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8"                      , 35.85 },
  { "WW_TuneCP5_13TeV-pythia8" , 49.997 },
  { "WZ_TuneCP5_13TeV-pythia8" , 5.595 },
  { "ZZ_TuneCP5_13TeV-pythia8" , 10.71 },
  { "GluGluHToTauTau_M125_13TeV_powheg_pythia8" , 48.58*0.0627 },
  { "VBFHToTauTau_M125_13TeV_powheg_pythia8"    , 3.782*0.0627 }
};

// 2016 (taken from AN2016_355_v10 with minor unrelevant deviations - everything was checked)
const map<TString, double> xsec_map_2016 = {
    { "DYJetsToLL_M-10to50"      , 18610 },
    { "DYJetsToLL_M-50"          , 5765 },
    { "DY1JetsToLL_M-50"         , 1.164*1012.5 },
    { "DY2JetsToLL_M-50"         , 1.164*332.8 },
    { "DY3JetsToLL_M-50"         , 1.164*101.8 },
    { "DY4JetsToLL_M-50"         , 1.164*54.8 },
    { "WJetsToLNu"               , 61526.7 },
    { "W1JetsToLNu"              , 1.221*9644.5 },
    { "W2JetsToLNu"              , 1.221*3144.5 },
    { "W3JetsToLNu"              , 1.221*954.8 },
    { "W4JetsToLNu"              , 1.221*485.6 },
    { "TTbar"                    , 831.76 },
    { "ST_t-channel_antitop"     , 80.95 },
    { "ST_t-channel_top"         , 136.02 },
    { "ST_tW_antitop"            , 35.6 },
    { "ST_tW_top"                , 35.6 },
    { "VVTo2L2Nu"                , 11.95 },
    { "WWToLNuQQ"                , 49.997 },
    { "WZTo2L2Q"                 , 5.595 },
    { "WZTo1L1Nu2Q"              , 10.71 },
    { "WZTo1L3Nu"                , 3.05 },
    { "WZJToLLLNu"               , 5.26 },
    { "ZZTo4L"                   , 1.212 },
    { "ZZTo2L2Q"                 , 3.22 },
    { "WGToLNuG"                 , 489.0 },
    { "WGstarToLNuMuMu"          , 2.793 },
    { "WGstarToLNuEE"            , 3.526 },
    { "EWKWPlus2Jet"             , 25.62 },
    { "EWKWMinus2Jet"            , 20.20 },
    { "EWKZ2Jets"                , 3.987 },
    { "GluGluHToTauTau_M125"     , 48.58*0.0627 },
    { "VBFHToTauTau_M125"        , 3.782*0.0627 }
  };
// **************************************************************************************************
// **************************************************************************************************
// maps short name to filenames - needed for stitching

const map<TString , TString> process_map_2017 = {
  { "WJets"   , "WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8"},
  { "W1Jets"  , "W1JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8"},
  { "W2Jets"  , "W2JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8"},
  { "W3Jets"  , "W3JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8"},
  { "W4Jets"  , "W4JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8"},
  { "DYJets"  , "DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8"},
  { "DY1Jets" , "DY1JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8"},
  { "DY2Jets" , "DY2JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8"},
  { "DY3Jets" , "DY3JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8"},
  { "DY4Jets" , "DY4JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8"},
};

const map<TString , TString> process_map_2016 = {
  { "WJets"   , "WJetsToLNu"},
  { "W1Jets"  , "W1JetsToLNu"},
  { "W2Jets"  , "W2JetsToLNu"},
  { "W3Jets"  , "W3JetsToLNu"},
  { "W4Jets"  , "W4JetsToLNu"},
  { "DYJets"  , "DYJetsToLL_M-50"},
  { "DY1Jets" , "DY1JetsToLL_M-50"},
  { "DY2Jets" , "DY2JetsToLL_M-50"},
  { "DY3Jets" , "DY3JetsToLL_M-50"},
  { "DY4Jets" , "DY4JetsToLL_M-50"},
};

double getNEventsProcessed(TString filename)
{
  TFile * file = new TFile(filename);
  TH1D * histWeightsH = (TH1D*)file->Get("histWeightsH");
  if(!histWeightsH){
    cout << endl << endl << "Histogram histWeightsH doesn't exist in the file "<< filename <<". Quit program." << endl << endl;
    exit(-1);
  }
  double nevents = histWeightsH->GetSumOfWeights();
  file -> Close();
  delete file;
  return nevents;
}


#endif
