#ifndef SETTINGS_FOR_ERAS_H
#define SETTINGS_FOR_ERAS_H

// **************************************************************************************************
// Define the subsamples that belong to a certain process
// 2018
const vector<TString> MuonEG_Run2018       = { "MuonEG_Run2018A" ,
                                               "MuonEG_Run2018B", 
                                               "MuonEG_Run2018C" ,
                                               "MuonEG_Run2018D" };
const vector<TString> Embedded_2018        = { "EmbeddingRun2018A" ,
                                               "EmbeddingRun2018B" ,
                                               "EmbeddingRun2018C" , 
                                               "EmbeddingRun2018D" };
const vector<TString> DYJets_2018          = { "DY1JetsToLL_M-50_TuneCP5_13TeV_madgraphMLM_pythia8" ,
                                               "DY2JetsToLL_M-50_TuneCP5_13TeV_madgraphMLM_pythia8" ,
                                               "DY3JetsToLL_M-50_TuneCP5_13TeV_madgraphMLM_pythia8" ,
                                               "DY4JetsToLL_M-50_TuneCP5_13TeV_madgraphMLM_pythia8" ,
                                               "DYJetsToLL_M-50_TuneCP5_13TeV_madgraphMLM_pythia8",
                                               "EWKZ2Jets_ZToNuNu_TuneCP5_PSweights_13TeV-madgraph-pythia8" ,
                                               "EWKZ2Jets_ZToLL_M-50_TuneCP5_PSweights_13TeV-madgraph-pythia8" , 
                                               "DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8" };
const vector<TString> WJets_2018           = { "W1JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8" ,
                                               "W2JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8" ,
                                               "W3JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8" ,
                                               "W4JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8" ,
                                               "WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8" ,
                                               "EWKWMinus2Jets_WToLNu_M-50_TuneCP5_13TeV-madgraph-pythia8" ,
                                               "EWKWPlus2Jets_WToLNu_M-50_TuneCP5_13TeV-madgraph-pythia8" ,
                                               "WGToLNuG_TuneCP5_13TeV-madgraphMLM-pythia8" };
const vector<TString> TTbar_2018           = { "TTTo2L2Nu_TuneCP5_13TeV_powheg_pythia8" ,
                                               "TTToHadronic_TuneCP5_13TeV_powheg_pythia8" ,
                                               "TTToSemiLeptonic_TuneCP5_13TeV_powheg_pythia8" };
const vector<TString> SingleTop_2018       = { "ST_t-channel_antitop_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8" ,
                                               "ST_t-channel_top_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8" ,
                                               "ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8" ,
                                               "ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8" };
const vector<TString> Diboson_2018         = { "WW_TuneCP5_13TeV-pythia8" ,
                                               "WZ_TuneCP5_13TeV-pythia8" ,
                                               "ZZ_TuneCP5_13TeV-pythia8" };
const vector<TString> GluGluHToTauTau_2018 = { "GluGluHToTauTau_M125_13TeV_powheg_pythia8" };
const vector<TString> VBFHToTauTau_2018    = { "VBFHToTauTau_M125_13TeV_powheg_pythia8",
                                               "ZHToTauTau_M125_13TeV_powheg_pythia8",
                                               "WplusHToTauTau_M125_13TeV_powheg_pythia8",
                                               "WminusHToTauTau_M125_13TeV_powheg_pythia8" };
const vector<TString> ZHToTauTau_2018      = { "ZHToTauTau_M125_13TeV_powheg_pythia8" };
const vector<TString> WHToTauTau_2018      = { "WplusHToTauTau_M125_13TeV_powheg_pythia8" , "WminusHToTauTau_M125_13TeV_powheg_pythia8" };
const vector<TString> ggHToWW_2018         = { "GluGluHToWWTo2L2Nu_M125_13TeV_powheg2_JHUGenV714_pythia8" };
const vector<TString> VBFHToWW_2018        = { "VBFHToWWTo2L2Nu_M125_13TeV_powheg2_JHUGenV714_pythia8" };
const vector<TString> ttH_2018             = { "ttHToTauTau_M125_TuneCP5_13TeV-powheg-pythia8" };

// 2017
const vector<TString> MuonEG_Run2017       = { "MuonEG_Run2017B" ,
					       "MuonEG_Run2017CtoF"};
const vector<TString> Embedded_2017        = { "Embedding_Run2017" };
const vector<TString> DYJets_2017          = { "DY1JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8" ,
					       "DY2JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8" ,
					       "DY3JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8" ,
                      "DY4JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8" ,
                      "DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8",
                      "EWKZ2Jets_ZToLL_M-50_TuneCP5_13TeV-madgraph-pythia8",
                      "EWKZ2Jets_ZToNuNu_TuneCP5_13TeV-madgraph-pythia8"
                      };
const vector<TString> WJets_2017           = { "W1JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8" ,
					       "W2JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8" ,
					       "W3JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8" ,
					       "W4JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8" ,
					       "WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8" ,
					       "WGToLNuG_TuneCP5_13TeV-madgraphMLM-pythia8" ,
					       "EWKWMinus2Jets_WToLNu_M-50_TuneCP5_13TeV-madgraph-pythia8" ,
					       "EWKWPlus2Jets_WToLNu_M-50_TuneCP5_13TeV-madgraph-pythia8"};
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
const vector<TString> VBFHToTauTau_2017    = { "VBFHToTauTau_M125_13TeV_powheg_pythia8",
					       "ZHToTauTau_M125_13TeV_powheg_pythia8",
					       "WplusHToTauTau_M125_13TeV_powheg_pythia8",
					       "WminusHToTauTau_M125_13TeV_powheg_pythia8" };
const vector<TString> ZHToTauTau_2017      = { "ZHToTauTau_M125_13TeV_powheg_pythia8" };
const vector<TString> WHToTauTau_2017      = { "WplusHToTauTau_M125_13TeV_powheg_pythia8" , "WminusHToTauTau_M125_13TeV_powheg_pythia8" };
const vector<TString> ggHToWW_2017         = { "GluGluHToWWTo2L2Nu" };
const vector<TString> VBFHToWW_2017        = { "VBFHToWWTo2L2Nu_M125_13TeV_powheg2_JHUGenV714_pythia8" };
const vector<TString> ttH_2017             = { "ttHToTauTau_M125_TuneCP5_13TeV-powheg-pythia8" };

// 2016
const vector<TString> MuonEG_Run2016       = { "MuonEG_Run2016B" , "MuonEG_Run2016C" , "MuonEG_Run2016D" , "MuonEG_Run2016E" , "MuonEG_Run2016F" , "MuonEG_Run2016G" , "MuonEG_Run2016H" };
const vector<TString> Embedded_2016        = { "Embedding_Run2016" };
const vector<TString> DYJets_2016          = { "DY1JetsToLL_M-50" , "DY2JetsToLL_M-50" , "DY3JetsToLL_M-50" , "DY4JetsToLL_M-50" , "DYJetsToLL_M-50" , "DYJetsToLL_M-10to50" , "EWKZ2Jets" };
const vector<TString> WJets_2016           = { "W1JetsToLNu" , "W2JetsToLNu" , "W3JetsToLNu" , "W4JetsToLNu" , "WJetsToLNu" , "WGToLNuG" , "WGstarToLNuEE" , "WGstarToLNuMuMu" , "EWKWPlus2Jets" , "EWKWMinus2Jet" };
const vector<TString> TTbar_2016           = { "TTbar" };
const vector<TString> SingleTop_2016       = { "ST_t-channel_antitop" , "ST_t-channel_top" , "ST_tW_antitop" , "ST_tW_top" };
const vector<TString> Diboson_2016         = { "VVTo2L2Nu" , "WZJToLLLNu" , "WZTo1L1Nu2Q" , "WZTo1L3Nu" , "WZTo2L2Q" , "ZZTo2L2Q" , "ZZTo4L" , "WWToLNuQQ" };
const vector<TString> GluGluHToTauTau_2016 = { "GluGluHToTauTau_M125" };
const vector<TString> VBFHToTauTau_2016    = { "VBFHToTauTau_M125" , "ZHToTauTau_M125" , "WplusHToTauTau_M125" , "WminusHToTauTau_M125" };
const vector<TString> ZHToTauTau_2016      = { "ZHToTauTau_M125" };
const vector<TString> WHToTauTau_2016      = { "WplusHToTauTau_M125" , "WminusHToTauTau_M125" };
const vector<TString> ggHToWW_2016         = { "GluGluHToWWTo2L2Nu_M125" };
const vector<TString> VBFHToWW_2016        = { "VBFHToWWTo2L2Nu_M125" };
const vector<TString> ttH_2016             = { "ttHJetToTT_M125" };

// **************************************************************************************************
// **************************************************************************************************
// Cross-section map
// 2018 (taken from 2017 - 2017 was checked)
const map<TString, double> xsec_map_2018 = {
   { "DYJetsToLL_M-50_TuneCP5_13TeV_madgraphMLM_pythia8"  , 6077.22 }, // NNLO xsec -> k-factor=1.1137 (see mail from Izaak on tauid hn 14.10.19)
   { "DY1JetsToLL_M-50_TuneCP5_13TeV_madgraphMLM_pythia8" , 877.8*1.137 },
   { "DY2JetsToLL_M-50_TuneCP5_13TeV_madgraphMLM_pythia8" , 304.4*1.137 },
   { "DY3JetsToLL_M-50_TuneCP5_13TeV_madgraphMLM_pythia8" , 111.5*1.137 },
   { "DY4JetsToLL_M-50_TuneCP5_13TeV_madgraphMLM_pythia8" , 44.04*1.137 },
   { "EWKZ2Jets_ZToNuNu_TuneCP5_PSweights_13TeV-madgraph-pythia8" , 10.66 },
   { "EWKZ2Jets_ZToLL_M-50_TuneCP5_PSweights_13TeV-madgraph-pythia8" , 4.321 },
   { "DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8" , 21658.0 },
   { "WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8"  , 61526.7 },   // NNLO -> k-factor=1.162
   { "W1JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8" , 8104.0*1.162 },
   { "W2JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8" , 2793.0*1.162 },
   { "W3JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8" , 992.5*1.162 },
   { "W4JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8" , 544.3*1.162 },
   { "EWKWMinus2Jets_WToLNu_M-50_TuneCP5_13TeV-madgraph-pythia8" , 23.24},
   { "EWKWPlus2Jets_WToLNu_M-50_TuneCP5_13TeV-madgraph-pythia8" , 29.59},
   { "WGToLNuG_TuneCP5_13TeV-madgraphMLM-pythia8" , 464.4},
   { "TTTo2L2Nu_TuneCP5_13TeV_powheg_pythia8"        , 88.29 },
   { "TTToHadronic_TuneCP5_13TeV_powheg_pythia8"     , 377.96 },
   { "TTToSemiLeptonic_TuneCP5_13TeV_powheg_pythia8" , 365.35 },
   { "ST_t-channel_antitop_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8" , 80.95 },
   { "ST_t-channel_top_4f_InclusiveDecays_TuneCP5_13TeV-powheg-madspin-pythia8"     , 136.02 },
   { "ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8"                  , 35.85 },
   { "ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8"                      , 35.85 },
   { "WW_TuneCP5_13TeV-pythia8" , 118.7 },  // NNLO
   { "WZ_TuneCP5_13TeV-pythia8" , 27.57 },
   { "ZZ_TuneCP5_13TeV-pythia8" , 12.14 },
   { "GluGluHToTauTau_M125_13TeV_powheg_pythia8" , 48.58*0.0627 },
   { "VBFHToTauTau_M125_13TeV_powheg_pythia8" , 3.782*0.0627 },
   { "ZHToTauTau_M125_13TeV_powheg_pythia8" , 0.0594 },
   { "WplusHToTauTau_M125_13TeV_powheg_pythia8" , 0.0527 },
   { "WminusHToTauTau_M125_13TeV_powheg_pythia8" , 0.0358 },
   { "GluGluHToWWTo2L2Nu_M125_13TeV_powheg2_JHUGenV714_pythia8" , 1.091343 }, // 48.58 (Production) * 0.214 (H->WW) * 0.104976 (WW->2l2Nu), from Dennis Roy (Heavy H->WW analysis), SM H->WW analysis still uses old ggh cross section (according to their AN), other values are the same
   { "VBFHToWWTo2L2Nu_M125_13TeV_powheg2_JHUGenV714_pythia8" , 0.084962 },// 3.782 * 0.214 * 0.104976, from Dennis Roy (Heavy H->WW analysis)
   { "ttHToTauTau_M125_TuneCP5_13TeV-powheg-pythia8" , 0.5071*0.0627 },
};
// 2017 (checked again - reference is http://cms.cern.ch/iCMS/jsp/db_notes/noteInfo.jsp?cmsnoteid=CMS%20AN-2018/255 -> DY xsec updated )
const map<TString, double> xsec_map_2017 = {
   // { "DYJetsToLL_M-10to50"      , 15820*1.079 },
  { "DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8"  , 6077.22 }, // NNLO xsec -> k-factor=1.137 (see mail from Izaak on tauid hn 14.10.19)
  { "DY1JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8" , 877.8*1.137 },
  { "DY2JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8" , 304.4*1.137 },
  { "DY3JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8" , 111.5*1.137 },
  { "DY4JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8" , 44.04*1.137 },
  { "EWKZ2Jets_ZToLL_M-50_TuneCP5_13TeV-madgraph-pythia8" , 4.321 },
  { "EWKZ2Jets_ZToNuNu_TuneCP5_13TeV-madgraph-pythia8" , 10.66 },
  { "WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8"  , 61526.7 },   // NNLO -> k-factor=1.162
  { "W1JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8" , 8104.0*1.162 },
  { "W2JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8" , 2793.0*1.162 },
  { "W3JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8" , 992.5*1.162 },
  { "W4JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8" , 544.3*1.162 },
  { "WGToLNuG_TuneCP5_13TeV-madgraphMLM-pythia8"    , 464.4 },
  { "EWKWMinus2Jets_WToLNu_M-50_TuneCP5_13TeV-madgraph-pythia8" , 23.24 },
  { "EWKWPlus2Jets_WToLNu_M-50_TuneCP5_13TeV-madgraph-pythia8" , 29.59 },
  { "TTTo2L2Nu_TuneCP5_PSweights_13TeV-powheg-pythia8"        , 88.29 },
  { "TTToHadronic_TuneCP5_PSweights_13TeV-powheg-pythia8"     , 377.96 },
  { "TTToSemiLeptonic_TuneCP5_PSweights_13TeV-powheg-pythia8" , 365.35 },
  { "ST_t-channel_antitop_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8" , 80.95 },
  { "ST_t-channel_top_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8"     , 136.02 },
  { "ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8"                  , 35.85 },
  { "ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8"                      , 35.85 },
  { "WW_TuneCP5_13TeV-pythia8" , 118.7 },  // NNLO
  { "WZ_TuneCP5_13TeV-pythia8" , 27.57 },
  { "ZZ_TuneCP5_13TeV-pythia8" , 12.14 },
  { "GluGluHToTauTau_M125_13TeV_powheg_pythia8" , 48.58*0.0627 },
  { "VBFHToTauTau_M125_13TeV_powheg_pythia8"    , 3.782*0.0627 },
  { "ZHToTauTau_M125_13TeV_powheg_pythia8"      , 0.0594 },
  { "WplusHToTauTau_M125_13TeV_powheg_pythia8"  , 0.0527 },
  { "WminusHToTauTau_M125_13TeV_powheg_pythia8" , 0.0358 },
  { "GluGluHToWWTo2L2Nu"                        , 1.091343 },  // see 2018
  { "VBFHToWWTo2L2Nu_M125_13TeV_powheg2_JHUGenV714_pythia8" , 0.084962 }, // see 2018
  { "ttHToTauTau_M125_TuneCP5_13TeV-powheg-pythia8"         , 0.5071*0.0627 }  // from: see links above
};

// 2016 (taken from AN2016_355_v10 with minor unrelevant deviations - everything was checked)
const map<TString, double> xsec_map_2016 = {
    { "DYJetsToLL_M-10to50"      , 18610 },
    { "DYJetsToLL_M-50"          , 6077.22 }, // NNLO xsec -> k-factor=1.227 (see mail from Izaak on tauid hn 14.10.19)
    { "DY1JetsToLL_M-50"         , 1.227*1012.5 },
    { "DY2JetsToLL_M-50"         , 1.227*332.8 },
    { "DY3JetsToLL_M-50"         , 1.227*101.8 },
    { "DY4JetsToLL_M-50"         , 1.227*54.8 },
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
    { "WGToLNuG"                 , 178.4 }, // xsdb
    { "WGstarToLNuMuMu"          , 2.793 },
    { "WGstarToLNuEE"            , 3.526 },
    { "EWKWPlus2Jets"            , 25.62 },
    { "EWKWMinus2Jet"            , 20.20 },
    { "EWKZ2Jets"                , 3.987 },
    { "GluGluHToTauTau_M125"     , 48.58*0.0627 },
    { "VBFHToTauTau_M125"        , 3.782*0.0627 },
    { "ZHToTauTau_M125"          , 0.0594 },
    { "WplusHToTauTau_M125"      , 0.0527 },
    { "WminusHToTauTau_M125"     , 0.0358 },
    { "GluGluHToWWTo2L2Nu_M125"  , 1.091343 }, // see 2018
    { "VBFHToWWTo2L2Nu_M125"     , 0.084962 }, // see 2018
    { "ttHJetToTT_M125"          , 0.5071*0.0627 } // see 2017
  };
// **************************************************************************************************
// **************************************************************************************************
// maps short name to filenames - needed for stitching
const map<TString , TString> process_map_2018 = {
  { "WJets"   , "WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8"},
  { "W1Jets"  , "W1JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8"},
  { "W2Jets"  , "W2JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8"},
  { "W3Jets"  , "W3JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8"},
  { "W4Jets"  , "W4JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8"},
  { "DYJets"  , "DYJetsToLL_M-50_TuneCP5_13TeV_madgraphMLM_pythia8"},
  { "DY1Jets" , "DY1JetsToLL_M-50_TuneCP5_13TeV_madgraphMLM_pythia8"},
  { "DY2Jets" , "DY2JetsToLL_M-50_TuneCP5_13TeV_madgraphMLM_pythia8"},
  { "DY3Jets" , "DY3JetsToLL_M-50_TuneCP5_13TeV_madgraphMLM_pythia8"},
  { "DY4Jets" , "DY4JetsToLL_M-50_TuneCP5_13TeV_madgraphMLM_pythia8"},
};

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

// **************************************************************************************************
// **************************************************************************************************
// maps for number of events in sample, needed for proper normalization in 2016 & 2017
map<TString, int> n_events_per_sample_2017 = {
{"WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8"  , 74635450},
{"W1JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8" , 54988117},
{"W2JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8" , 32368249},
{"W3JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8" , 19700377},
{"W4JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8" , 11333705},
{"ZZ_TuneCP5_13TeV-pythia8" , 1949768},
{"WW_TuneCP5_13TeV-pythia8" , 7791498},
{"WZ_TuneCP5_13TeV-pythia8" , 3928630},
{"DYJetsToLL_M-10to50" , 39521230},
{"DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8"     , 97800939},
{"DY1JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8"    , 34859434},
{"DY2JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8"    , 9790490},
{"DY3JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8"    , 6897933},
{"DY4JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8"    , 4346952},
{"EWKZ2Jets_ZToNuNu_TuneCP5_13TeV-madgraph-pythia8" , 3000000},
{"EWKZ2Jets_ZToLL_M-50_TuneCP5_13TeV-madgraph-pythia8" , 954336},
{"EWKWMinus2Jets_WToLNu_M-50_TuneCP5_13TeV-madgraph-pythia8" , 4449714},
{"EWKWPlus2Jets_WToLNu_M-50_TuneCP5_13TeV-madgraph-pythia8" , 4453013},
{"WGToLNuG_TuneCP5_13TeV-madgraphMLM-pythia8" , 6283083},
{"VBFHToTauTau_M125_13TeV_powheg_pythia8" , 2977152},
{"ZHToTauTau_M125_13TeV_powheg_pythia8" , 4940230},
{"WplusHToTauTau_M125_13TeV_powheg_pythia8" , 4000000},
{"WminusHToTauTau_M125_13TeV_powheg_pythia8" , 3860872},
{"GluGluHToWWTo2L2Nu" , 953600}, 
{"VBFHToWWTo2L2Nu_M125_13TeV_powheg2_JHUGenV714_pythia8" , 500000},
{"ttHToTauTau_M125_TuneCP5_13TeV-powheg-pythia8" , 21713806},
};
//TTTo2L2Nu_TuneCP5_PSweights_13TeV-powheg-pythia8, TTToHadronic_TuneCP5_PSweights_13TeV-powheg-pythia8, TTToSemiLeptonic_TuneCP5_PSweights_13TeV-powheg-pythia8 , ST_t-channel_top_4f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8, ST_t-channel_antitop_4f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8, ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8, ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8, GluGluHToTauTau_M125_13TeV_powheg_pythia8 can be normalized using GetSumOfWeights()

double getNEventsProcessed(TString filename)
{
  TFile * file = new TFile(filename);
  TH1D * histWeightsH = (TH1D*)file->Get("histWeightsH");
  if(!histWeightsH){
    cout << endl << endl << "Histogram histWeightsH doesn't exist in the file "<< filename <<". Quit program." << endl << endl;
    exit(-1);
  }
  double nevents = histWeightsH->GetSumOfWeights();
  std::cout<<"--------------------------------Normalization from GetSumOfWeights()-----------------------------------"<<std::endl;
  file -> Close();
  delete file;
  return nevents;
}


#endif
