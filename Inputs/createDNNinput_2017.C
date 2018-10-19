#include <iostream>
#include <map>

#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TList.h"

double luminosity = 35866;
bool applyPreselection = true;

double getNEventsProcessed(TString filename)
{
  TFile * file = new TFile(filename);
  TH1D * histWeightsH = (TH1D*)file->Get("histWeightsH");
  double nevents = histWeightsH->GetSumOfWeights();
  file -> Close();
  delete file;
  return nevents;
}

void createDNNinput_2017(TString inputDir="/nfs/dust/cms/user/mameyer/SM_HiggsTauTau/newMETv2/CMSSW_9_4_9/src/DesyTauAnalyses/NTupleMaker/test/HTauTau_EMu_2017/Ntuples/"){

  // Define the subsamples that belong to a certain proccess
  vector<TString> MuonEG_Run2017  = { "MuonEG_Run2017B" ,
				      "MuonEG_Run2017C" ,
				      "MuonEG_Run2017D" ,
				      "MuonEG_Run2017E" ,
				      "MuonEG_Run2017F" };
  vector<TString> DYJets          = { "DY1JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8" ,
				      "DY2JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8" ,
				      "DY3JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8" ,
				      "DY4JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8" ,
				      "DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8" };
  vector<TString> WJets           = { "W1JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8" ,
				      "W2JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8" ,
				      "W3JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8" ,
				      "W4JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8" ,
				      "WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8" };
  vector<TString> TTbar           = { "TTTo2L2Nu_TuneCP5_PSweights_13TeV-powheg-pythia8" ,
				      "TTToHadronic_TuneCP5_PSweights_13TeV-powheg-pythia8" ,
				      "TTToSemiLeptonic_TuneCP5_PSweights_13TeV-powheg-pythia8" };
  vector<TString> SingleTop       = { "ST_t-channel_antitop_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8" ,
				      "ST_t-channel_top_4f_inclusiveDecays_TuneCP5_13TeV-powhegV2-madspin-pythia8" ,
				      "ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8" ,
				      "ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8" };
  vector<TString> Diboson         = { "WW_TuneCP5_13TeV-pythia8" ,
				      "WZ_TuneCP5_13TeV-pythia8" ,
				      "ZZ_TuneCP5_13TeV-pythia8" };
  // vector<TString> GluGluHToTauTau = { "GluGluHToTauTau_M125" };
  // vector<TString> VBFHToTauTau    = { "VBFHToTauTau_M125" };

  // Mapping of subsamples to output root-file
  TString version = "v1";
  TString channel = "em";
  map< TString , vector<TString> > samples_map = {
    { "MuonEG_Run2017_dnn_" + channel + "_" + version , MuonEG_Run2017 },
    { "DYJets_dnn_"         + channel + "_" + version , DYJets },
    { "WJets_dnn_"          + channel + "_" + version , WJets },
    { "TTbar_dnn_"          + channel + "_" + version , TTbar },
    { "SingleTop_dnn_"      + channel + "_" + version , SingleTop },
    { "Diboson_dnn_"        + channel + "_" + version , Diboson },
    // { "ggH_dnn_"            + channel + "_" + version , GluGluHToTauTau },
    // { "VBFH_dnn_"           + channel + "_" + version , VBFHToTauTau }
  };

  // Cross-section map ( needs to be checked again )
  map<TString, double> xsec_map = {
    // { "DYJetsToLL_M-10to50"      , 15820*1.079 },
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
    // { "GluGluHToTauTau_M125"     , 48.58*0.0627 },
    // { "VBFHToTauTau_M125"        , 3.782*0.0627 }
  };

  // Needed for stitching
  double xsecWIncl      = xsec_map["WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8"];
  double xsecW1Jets     = xsec_map["W1JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8"];
  double xsecW2Jets     = xsec_map["W2JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8"];
  double xsecW3Jets     = xsec_map["W3JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8"];
  double xsecW4Jets     = xsec_map["W4JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8"];
  double xsecDYIncl     = xsec_map["DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8"];
  double xsecDY1Jets    = xsec_map["DY1JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8"];
  double xsecDY2Jets    = xsec_map["DY2JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8"];
  double xsecDY3Jets    = xsec_map["DY3JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8"];
  double xsecDY4Jets    = xsec_map["DY4JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8"];
  double neventsWIncl   = getNEventsProcessed(inputDir+"/WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8.root");
  double neventsW1Jets  = getNEventsProcessed(inputDir+"/W1JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8.root");
  double neventsW2Jets  = getNEventsProcessed(inputDir+"/W2JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8.root");
  double neventsW3Jets  = getNEventsProcessed(inputDir+"/W3JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8.root");
  double neventsW4Jets  = getNEventsProcessed(inputDir+"/W4JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8.root");
  double neventsDYIncl  = getNEventsProcessed(inputDir+"/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8.root");
  double neventsDY1Jets = getNEventsProcessed(inputDir+"/DY1JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8.root");
  double neventsDY2Jets = getNEventsProcessed(inputDir+"/DY2JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8.root");
  double neventsDY3Jets = getNEventsProcessed(inputDir+"/DY3JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8.root");
  double neventsDY4Jets = getNEventsProcessed(inputDir+"/DY4JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8.root");
  
  // Loop over all samples
  for (auto const& sample : samples_map){

    cout << endl << sample.first << "  :  " << endl ;

    TFile *outFile = new TFile("NTuples_2017/" + sample.first + ".root","RECREATE");
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
	  if( iso_1 > 0.15 )              continue;
	  if( iso_2 > 0.2)                continue;
	  if( extraelec_veto > 0.5)       continue;
	  if( extramuon_veto > 0.5)       continue;
	  if( TMath::Max(pt_1,pt_2) < 24) continue;
	  if( metFilters < 0.5 )          continue;
	  if( trg_muonelectron < 0.5 )    continue;
	}
	xsec_lumi_weight = xsec*luminosity/nevents;
	
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
