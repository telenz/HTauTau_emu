#include <iostream>
#include <map>

#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TList.h"

double luminosity = 35866;
bool applyPreselection = true;
float qcd_ss_os_iso_relaxed_ratio = 0.49;
float trigger_filter_efficiency = 0.979;

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

void create_dnn_input_2016(TString inputDir="/nfs/dust/cms/user/mameyer/SM_HiggsTauTau/CMSSW_8_0_29/src/DesyTauAnalyses/NTupleMaker/test/HTauTau_EMu_2016/NTuples/"){

  // Define the subsamples that belong to a certain proccess
  vector<TString> MuonEG_Run2016  = { "MuonEG_Run2016B" , "MuonEG_Run2016C" , "MuonEG_Run2016D" , "MuonEG_Run2016E" , "MuonEG_Run2016F" , "MuonEG_Run2016G" , "MuonEG_Run2016H" };
  vector<TString> DYJets          = { "DY1JetsToLL_M-50" , "DY2JetsToLL_M-50" , "DY3JetsToLL_M-50" , "DY4JetsToLL_M-50" , "DYJetsToLL_M-50" , "DYJetsToLL_M-10to50" };
  vector<TString> EWKZ            = { "EWKZ2Jets" };
  vector<TString> WJets           = { "W1JetsToLNu" , "W2JetsToLNu" , "W3JetsToLNu" , "W4JetsToLNu" , "WJetsToLNu" , "WGToLNuG" , "WGstarToLNuEE" , "WGstarToLNuMuMu" , "EWKWPlus2Jet" , "EWKWMinus2Jet" };
  vector<TString> TTbar           = { "TTbar" };
  vector<TString> SingleTop       = { "ST_t-channel_antitop" , "ST_t-channel_top" , "ST_tW_antitop" , "ST_tW_top" };
  vector<TString> Diboson         = { "VVTo2L2Nu" , "WZJToLLLNu" , "WZTo1L1Nu2Q" , "WZTo1L3Nu" , "WZTo2L2Q" , "ZZTo2L2Q" , "ZZTo4L" , "WWToLNuQQ" };
  vector<TString> GluGluHToTauTau = { "GluGluHToTauTau_M125" };
  vector<TString> VBFHToTauTau    = { "VBFHToTauTau_M125" };

  // Mapping of subsamples to output root-file
  TString version = "v1";
  TString channel = "em";
  map< TString , vector<TString> > samples_map = {
    { "NOMINAL_ntuple_MuonEG_Run2016_" + channel , MuonEG_Run2016 },
    { "NOMINAL_ntuple_DYJets_"         + channel , DYJets },
    { "NOMINAL_ntuple_WJets_"          + channel , WJets },
    { "NOMINAL_ntuple_EWKZ_"           + channel , EWKZ },
    { "NOMINAL_ntuple_TTbar_"          + channel , TTbar },
    { "NOMINAL_ntuple_SingleTop_"      + channel , SingleTop },
    { "NOMINAL_ntuple_Diboson_"        + channel , Diboson },
    { "NOMINAL_ntuple_ggH_"            + channel , GluGluHToTauTau },
    { "NOMINAL_ntuple_VBFH_"           + channel , VBFHToTauTau }
  };

  // Cross-section map (taken from AN2016_355_v10 with minor unrelevant deviations - everything was checked)
  map<TString, double> xsec_map = {
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

  // Needed for stitching
  double xsecWIncl      = xsec_map["WJetsToLNu"];
  double xsecW1Jets     = xsec_map["W1JetsToLNu"];
  double xsecW2Jets     = xsec_map["W2JetsToLNu"];
  double xsecW3Jets     = xsec_map["W3JetsToLNu"];
  double xsecW4Jets     = xsec_map["W4JetsToLNu"];
  double xsecDYIncl     = xsec_map["DYJetsToLL_M-50"];
  double xsecDY1Jets    = xsec_map["DY1JetsToLL_M-50"];
  double xsecDY2Jets    = xsec_map["DY2JetsToLL_M-50"];
  double xsecDY3Jets    = xsec_map["DY3JetsToLL_M-50"];
  double xsecDY4Jets    = xsec_map["DY4JetsToLL_M-50"];
  double neventsWIncl   = getNEventsProcessed(inputDir+"/WJetsToLNu.root");
  double neventsW1Jets  = getNEventsProcessed(inputDir+"/W1JetsToLNu.root");
  double neventsW2Jets  = getNEventsProcessed(inputDir+"/W2JetsToLNu.root");
  double neventsW3Jets  = getNEventsProcessed(inputDir+"/W3JetsToLNu.root");
  double neventsW4Jets  = getNEventsProcessed(inputDir+"/W4JetsToLNu.root");
  double neventsDYIncl  = getNEventsProcessed(inputDir+"/DYJetsToLL_M-50.root");
  double neventsDY1Jets = getNEventsProcessed(inputDir+"/DY1JetsToLL_M-50.root");
  double neventsDY2Jets = getNEventsProcessed(inputDir+"/DY2JetsToLL_M-50.root");
  double neventsDY3Jets = getNEventsProcessed(inputDir+"/DY3JetsToLL_M-50.root");
  double neventsDY4Jets = getNEventsProcessed(inputDir+"/DY4JetsToLL_M-50.root");

  // Loop over all samples
  for (auto const& sample : samples_map){

    cout << endl << sample.first << "  :  " << endl ;

    TFile *outFile = new TFile("NTuples_2016_NewSysNaming/" + sample.first + ".root","RECREATE");
    TTree *outTree = new TTree("TauCheck", "tree created as DNN input");
    bool firstTree = true;
    TList* treeList = new TList();

    for(TString const& subsample: sample.second) {

      cout << "  - " << subsample << " : ";

      TFile *inFile  = new TFile( inputDir + "/" + subsample + ".root" ,"READ");
      TTree *inTree  = (TTree*) inFile -> Get("TauCheck");
      double nevents = getNEventsProcessed( inputDir + "/" + subsample + ".root" );

      // SetBranchAddress for variables that need are needed for preselection or stitching
      unsigned int npartons;
      float iso_1;
      float iso_2;
      bool extraelec_veto;
      bool extramuon_veto;
      float pt_1;
      float pt_2;
      bool metFilters;
      bool trg_muonelectron;
      inTree->SetBranchAddress("npartons",&npartons);
      inTree->SetBranchAddress("iso_1",&iso_1);
      inTree->SetBranchAddress("iso_2",&iso_2);
      inTree->SetBranchAddress("extraelec_veto",&extraelec_veto);
      inTree->SetBranchAddress("extramuon_veto",&extramuon_veto);
      inTree->SetBranchAddress("pt_1",&pt_1);
      inTree->SetBranchAddress("pt_2",&pt_2);
      inTree->SetBranchAddress("metFilters",&metFilters);
      inTree->SetBranchAddress("trg_muonelectron",&trg_muonelectron);

      outFile->cd();
      TTree *currentTree = new TTree(subsample,"temporary tree");

      // Create a branch for xsec_lumi_weight
      float xsec_lumi_weight;
      float qcd_correction;
      float trigger_filter_weight;
      if(firstTree){
	outTree    = inTree->CloneTree(0);
	outTree->Branch("xsec_lumi_weight", &xsec_lumi_weight, "xsec_lumi_weight/F");
	outTree->Branch("qcd_correction", &qcd_correction, "qcd_correction/F");
	outTree->Branch("trigger_filter_weight", &trigger_filter_weight, "trigger_filter_weight/F");
	firstTree  = false;
      }
      currentTree = inTree->CloneTree(0);
      currentTree->Branch("xsec_lumi_weight", &xsec_lumi_weight, "xsec_lumi_weight/F");
      currentTree->Branch("qcd_correction", &qcd_correction, "qcd_correction/F");
      currentTree->Branch("trigger_filter_weight", &trigger_filter_weight, "trigger_filter_weight/F");

      // lumi-xsec-weight added
      if( xsec_map.find(subsample) == xsec_map.end() && !sample.first.Contains("MuonEG")){
	cout << endl << endl << "Sample " << subsample << " is missing in xsec_map. Exit code." << endl << endl ;
	exit(-1);
      }
      float xsec = xsec_map[subsample];

      for (int i=0; i<inTree->GetEntries(); i++) {
	inTree->GetEntry(i);

	// Add here preselection if necessary
	if(applyPreselection){
	  if( iso_1 > 0.5 )                continue;
	  if( iso_2 > 0.5 )                continue;
	  if( extraelec_veto > 0.5 )       continue;
	  if( extramuon_veto > 0.5 )       continue;
	  if( TMath::Max(pt_1,pt_2) < 24 ) continue;
	  if( metFilters < 0.5 )           continue;
	  if( trg_muonelectron < 0.5 )     continue;
	}
	xsec_lumi_weight = xsec*luminosity/nevents;
	qcd_correction = qcd_ss_os_iso_relaxed_ratio;
	trigger_filter_weight = trigger_filter_efficiency;
	
	// Stitching only for wjets MC in n-jet binned samples in npartons
	if( subsample.Contains("W") && subsample.Contains("JetsToLNu") ){
	  if(npartons == 1)      xsec_lumi_weight = luminosity / ( neventsW1Jets/xsecW1Jets + neventsWIncl/xsecWIncl );
	  else if(npartons == 2) xsec_lumi_weight = luminosity / ( neventsW2Jets/xsecW2Jets + neventsWIncl/xsecWIncl );
	  else if(npartons == 3) xsec_lumi_weight = luminosity / ( neventsW3Jets/xsecW3Jets + neventsWIncl/xsecWIncl );
	  else if(npartons == 4) xsec_lumi_weight = luminosity / ( neventsW4Jets/xsecW4Jets + neventsWIncl/xsecWIncl );
	  else                   xsec_lumi_weight = luminosity / ( neventsWIncl/xsecWIncl );
	}
	else if( subsample.Contains("DY") && subsample.Contains("JetsToLL_M-50") ){
	  if(npartons == 1)      xsec_lumi_weight = luminosity / ( neventsDY1Jets/xsecDY1Jets + neventsDYIncl/xsecDYIncl );
	  else if(npartons == 2) xsec_lumi_weight = luminosity / ( neventsDY2Jets/xsecDY2Jets + neventsDYIncl/xsecDYIncl );
	  else if(npartons == 3) xsec_lumi_weight = luminosity / ( neventsDY3Jets/xsecDY3Jets + neventsDYIncl/xsecDYIncl );
	  else if(npartons == 4) xsec_lumi_weight = luminosity / ( neventsDY4Jets/xsecDY4Jets + neventsDYIncl/xsecDYIncl );
	  else                   xsec_lumi_weight = luminosity / ( neventsDYIncl/xsecDYIncl );
	}

	if( sample.first.Contains("MuonEG")){
	  xsec_lumi_weight = 1.;
	  trigger_filter_weight = 1.;
	}

	currentTree->Fill();
      }
      cout<<currentTree->GetEntries()<<endl;
      treeList->Add(currentTree);
      inFile->Delete();
    }
    outTree = TTree::MergeTrees(treeList);
    cout<<"Together : "<<outTree->GetEntries()<<endl;
    outTree  -> Write( "" , TObject::kOverwrite );
    treeList -> Delete();
    outFile  -> Close();
  }
  cout << endl; 
}
