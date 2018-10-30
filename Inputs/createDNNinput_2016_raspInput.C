#include <iostream>
#include <map>

#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TList.h"

double luminosity = 35900;
bool applyPreselection = false;

double getNEventsProcessed(TString filename)
{
  TFile * file = new TFile(filename);
  TH1D * histWeightsH = (TH1D*)file->Get("histWeightsH");
  double nevents = histWeightsH->GetSumOfWeights();
  file -> Close();
  delete file;
  return nevents;
}

void createDNNinput_2016_raspInput(TString inputDir="/nfs/dust/cms/user/rasp/Run/Run2016/EMu_2016/msv_v2/"){

  // Define the subsamples that belong to a certain proccess
  vector<TString> MuonEG_Run2016  = { "MuonEG_Run2016" };
  vector<TString> DYJets          = { "DY1JetsToLL_M-50_13TeV-madgraphMLM" , "DY2JetsToLL_M-50_13TeV-madgraphMLM" , "DY3JetsToLL_M-50_13TeV-madgraphMLM" , "DY4JetsToLL_M-50_13TeV-madgraphMLM" , "DYJetsToLL_M-50_13TeV-madgraphMLM" , "DYJetsToLL_M-10to50_13TeV-madgraphMLM" };
  vector<TString> EWKZ           = { "EWKZ2Jets_ZToLL_M-50_13TeV-madgraph" };
  vector<TString> WJets           = { "W1JetsToLNu_13TeV-madgraphMLM" , "W2JetsToLNu_13TeV-madgraphMLM" , "W3JetsToLNu_13TeV-madgraphMLM" , "W4JetsToLNu_13TeV-madgraphMLM" , "WJetsToLNu_13TeV-madgraphMLM" , "WGToLNuG_13TeV-madgraphMLM-pythia8" , "WGstarToLNuEE_012Jets_13TeV-madgraph" , "WGstarToLNuMuMu_012Jets_13TeV-madgraph" , "EWKWPlus2Jets_WToLNu_13TeV-madgraph" , "EWKWMinus2Jets_WToLNu_13TeV-madgraph" };
  vector<TString> TTbar           = { "TTJets_13TeV-powheg" };
  // vector<TString> SingleTop       = { "ST_t-channel_antitop_4f_inclusiveDecays_13TeV-powheg" , "ST_t-channel_top_4f_inclusiveDecays_13TeV-powheg" , "ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg" , "ST_tW_top_5f_inclusiveDecays_13TeV-powheg" };
  vector<TString> Diboson         = { "ST_t-channel_antitop_4f_inclusiveDecays_13TeV-powheg" , "ST_t-channel_top_4f_inclusiveDecays_13TeV-powheg" , "ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg" , "ST_tW_top_5f_inclusiveDecays_13TeV-powheg" , "VVTo2L2Nu_13TeV_amcatnloFXFX" , "WZJToLLLNu_13TeV_amcatnloFXFX" , "WZTo1L1Nu2Q_13TeV_amcatnloFXFX" , "WZTo1L3Nu_13TeV_amcatnloFXFX" , "WZTo2L2Q_13TeV_amcatnloFXFX" , "ZZTo2L2Q_13TeV_amcatnloFXFX" , "ZZTo4L_13TeV_powheg" , "WWToLNuQQ_13TeV_powheg" };
  vector<TString> GluGluHToTauTau = { "GluGluHToTauTau_M125_13TeV_powheg" };
  vector<TString> VBFHToTauTau    = { "VBFHToTauTau_M125_13TeV_powheg" };

  // Mapping of subsamples to output root-file
  TString version = "v1";
  TString channel = "em";
  map< TString , vector<TString> > samples_map = {
    { "MuonEG_Run2016_dnn_" + channel + "_" + version , MuonEG_Run2016 },
    { "DYJets_dnn_"         + channel + "_" + version , DYJets },
    { "EWKZ_"               + channel + "_" + version , EWKZ },
    { "WJets_dnn_"          + channel + "_" + version , WJets },
    { "TTbar_dnn_"          + channel + "_" + version , TTbar },
    // { "SingleTop_dnn_"      + channel + "_" + version , SingleTop },
    { "Diboson_dnn_"        + channel + "_" + version , Diboson },
    { "ggH_dnn_"            + channel + "_" + version , GluGluHToTauTau },
    { "VBFH_dnn_"           + channel + "_" + version , VBFHToTauTau }
  };

  // Cross-section map (taken from AN2016_355_v10 with minor unrelevant deviations - everything was checked)
  map<TString, double> xsec_map = {
    { "DYJetsToLL_M-10to50_13TeV-madgraphMLM"      , 18610 },
    { "DYJetsToLL_M-50_13TeV-madgraphMLM"          , 5765 },
    { "DY1JetsToLL_M-50_13TeV-madgraphMLM"         , 1.164*1012.5 },
    { "DY2JetsToLL_M-50_13TeV-madgraphMLM"         , 1.164*332.8 },
    { "DY3JetsToLL_M-50_13TeV-madgraphMLM"         , 1.164*101.8 },
    { "DY4JetsToLL_M-50_13TeV-madgraphMLM"         , 1.164*54.8 },
    { "WJetsToLNu_13TeV-madgraphMLM"               , 61526.7 },
    { "W1JetsToLNu_13TeV-madgraphMLM"              , 1.221*9644.5 },
    { "W2JetsToLNu_13TeV-madgraphMLM"              , 1.221*3144.5 },
    { "W3JetsToLNu_13TeV-madgraphMLM"              , 1.221*954.8 },
    { "W4JetsToLNu_13TeV-madgraphMLM"              , 1.221*485.6 },
    { "TTJets_13TeV-powheg"                    , 831.76 },
    { "ST_t-channel_antitop_4f_inclusiveDecays_13TeV-powheg"     , 80.95 },
    { "ST_t-channel_top_4f_inclusiveDecays_13TeV-powheg"         , 136.95 },
    { "ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg"            , 35.6 },
    { "ST_tW_top_5f_inclusiveDecays_13TeV-powheg"                , 35.6 },
    { "VVTo2L2Nu_13TeV_amcatnloFXFX"                , 11.95 },
    { "WWToLNuQQ_13TeV_powheg"                , 49.997 },
    { "WZTo2L2Q_13TeV_amcatnloFXFX"                 , 5.595 },
    { "WZTo1L1Nu2Q_13TeV_amcatnloFXFX"              , 10.71 },
    { "WZTo1L3Nu_13TeV_amcatnloFXFX"                , 3.05 },
    { "WZJToLLLNu_13TeV_amcatnloFXFX"               , 4.708 },
    { "ZZTo4L_13TeV_powheg"                   , 1.212 },
    { "ZZTo2L2Q_13TeV_amcatnloFXFX"                 , 3.22 },
    { "WGToLNuG_13TeV-madgraphMLM-pythia8"                 , 489.0 },
    { "WGstarToLNuMuMu_012Jets_13TeV-madgraph"          , 2.793 },
    { "WGstarToLNuEE_012Jets_13TeV-madgraph"            , 3.526 },
    { "EWKWPlus2Jets_WToLNu_13TeV-madgraph"             , 25.62 },
    { "EWKWMinus2Jets_WToLNu_13TeV-madgraph"            , 20.20 },
    { "EWKZ2Jets_ZToLL_M-50_13TeV-madgraph"             , 3.987 },
    { "GluGluHToTauTau_M125_13TeV_powheg"     , 48.58*0.0627 },
    { "VBFHToTauTau_M125_13TeV_powheg"        , 3.782*0.0627 }
  };

  // Needed for stitching
  double xsecWIncl      = xsec_map["WJetsToLNu_13TeV-madgraphMLM"];
  double xsecW1Jets     = xsec_map["W1JetsToLNu_13TeV-madgraphMLM"];
  double xsecW2Jets     = xsec_map["W2JetsToLNu_13TeV-madgraphMLM"];
  double xsecW3Jets     = xsec_map["W3JetsToLNu_13TeV-madgraphMLM"];
  double xsecW4Jets     = xsec_map["W4JetsToLNu_13TeV-madgraphMLM"];
  double xsecDYIncl     = xsec_map["DYJetsToLL_M-50_13TeV-madgraphMLM"];
  double xsecDY1Jets    = xsec_map["DY1JetsToLL_M-50_13TeV-madgraphMLM"];
  double xsecDY2Jets    = xsec_map["DY2JetsToLL_M-50_13TeV-madgraphMLM"];
  double xsecDY3Jets    = xsec_map["DY3JetsToLL_M-50_13TeV-madgraphMLM"];
  double xsecDY4Jets    = xsec_map["DY4JetsToLL_M-50_13TeV-madgraphMLM"];
  double neventsWIncl   = getNEventsProcessed(inputDir+"/WJetsToLNu_13TeV-madgraphMLM.root");
  double neventsW1Jets  = getNEventsProcessed(inputDir+"/W1JetsToLNu_13TeV-madgraphMLM.root");
  double neventsW2Jets  = getNEventsProcessed(inputDir+"/W2JetsToLNu_13TeV-madgraphMLM.root");
  double neventsW3Jets  = getNEventsProcessed(inputDir+"/W3JetsToLNu_13TeV-madgraphMLM.root");
  double neventsW4Jets  = getNEventsProcessed(inputDir+"/W4JetsToLNu_13TeV-madgraphMLM.root");
  double neventsDYIncl  = getNEventsProcessed(inputDir+"/DYJetsToLL_M-50_13TeV-madgraphMLM.root");
  double neventsDY1Jets = getNEventsProcessed(inputDir+"/DY1JetsToLL_M-50_13TeV-madgraphMLM.root");
  double neventsDY2Jets = getNEventsProcessed(inputDir+"/DY2JetsToLL_M-50_13TeV-madgraphMLM.root");
  double neventsDY3Jets = getNEventsProcessed(inputDir+"/DY3JetsToLL_M-50_13TeV-madgraphMLM.root");
  double neventsDY4Jets = getNEventsProcessed(inputDir+"/DY4JetsToLL_M-50_13TeV-madgraphMLM.root");
  
  // Loop over all samples
  for (auto const& sample : samples_map){

    cout << endl << sample.first << "  :  " << endl ;

    TFile *outFile = new TFile("NTuples_2016_rasp/" + sample.first + ".root","RECREATE");
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
      // float iso_1;
      // float iso_2;
      // bool extraelec_veto;
      // bool extramuon_veto;
      // float pt_1;
      // float pt_2;
      // bool metFilters;
      // bool trg_muonelectron;
      inTree->SetBranchAddress("npartons",&npartons);
      // inTree->SetBranchAddress("iso_1",&iso_1);
      // inTree->SetBranchAddress("iso_2",&iso_2);
      // inTree->SetBranchAddress("extraelec_veto",&extraelec_veto);
      // inTree->SetBranchAddress("extramuon_veto",&extramuon_veto);
      // inTree->SetBranchAddress("pt_1",&pt_1);
      // inTree->SetBranchAddress("pt_2",&pt_2);
      // inTree->SetBranchAddress("metFilters",&metFilters);
      // inTree->SetBranchAddress("trg_muonelectron",&trg_muonelectron);

      outFile->cd();
      TTree *currentTree = new TTree(subsample,"temporary tree");

      // Create a branch for xsec_lumi_weight
      float xsec_lumi_weight;
      if(firstTree){
	outTree    = inTree->CloneTree(0);
	TBranch *w = outTree->Branch("xsec_lumi_weight", &xsec_lumi_weight, "xsec_lumi_weight/F");
	firstTree  = false;
      }
      currentTree = inTree->CloneTree(0);
      TBranch *w  = currentTree->Branch("xsec_lumi_weight", &xsec_lumi_weight, "xsec_lumi_weight/F");

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
	  // if( iso_1 > 0.15 )              continue;
	  // if( iso_2 > 0.2)                continue;
	  // if( extraelec_veto > 0.5)       continue;
	  // if( extramuon_veto > 0.5)       continue;
	  // if( TMath::Max(pt_1,pt_2) < 24) continue;
	  // if( metFilters < 0.5 )          continue;
	  // if( trg_muonelectron < 0.5 )    continue;
	}
	xsec_lumi_weight = xsec*luminosity/nevents;
	
	// Stitching only for wjets MC in n-jet binned samples in npartons
	if( subsample.Contains("W") && subsample.Contains("JetsToLNu_13TeV-madgraphMLM") ){
	  if(npartons == 1)      xsec_lumi_weight = luminosity / ( neventsW1Jets/xsecW1Jets + neventsWIncl/xsecWIncl );
	  else if(npartons == 2) xsec_lumi_weight = luminosity / ( neventsW2Jets/xsecW2Jets + neventsWIncl/xsecWIncl );
	  else if(npartons == 3) xsec_lumi_weight = luminosity / ( neventsW3Jets/xsecW3Jets + neventsWIncl/xsecWIncl );
	  else if(npartons == 4) xsec_lumi_weight = luminosity / ( neventsW4Jets/xsecW4Jets + neventsWIncl/xsecWIncl );
	  else                   xsec_lumi_weight = luminosity / ( neventsWIncl/xsecWIncl );
	}
	else if( subsample.Contains("DY") && subsample.Contains("JetsToLL_M-50_13TeV-madgraphMLM") ){
	  if(npartons == 1)      xsec_lumi_weight = luminosity / ( neventsDY1Jets/xsecDY1Jets + neventsDYIncl/xsecDYIncl );
	  else if(npartons == 2) xsec_lumi_weight = luminosity / ( neventsDY2Jets/xsecDY2Jets + neventsDYIncl/xsecDYIncl );
	  else if(npartons == 3) xsec_lumi_weight = luminosity / ( neventsDY3Jets/xsecDY3Jets + neventsDYIncl/xsecDYIncl );
	  else if(npartons == 4) xsec_lumi_weight = luminosity / ( neventsDY4Jets/xsecDY4Jets + neventsDYIncl/xsecDYIncl );
	  else                   xsec_lumi_weight = luminosity / ( neventsDYIncl/xsecDYIncl );
	}

	if( sample.first.Contains("MuonEG")) xsec_lumi_weight = 1.;

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
