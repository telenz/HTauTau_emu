#include <iostream>
#include <map>
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TList.h"
#include "settings_for_eras.h"

bool applyPreselection = true;

void create_dnn_ntuples( TString era = "2017" ,
			 TString inputDir="/nfs/dust/cms/user/mameyer/SM_HiggsTauTau/newMETv2/CMSSW_9_4_9/src/DesyTauAnalyses/NTupleMaker/test/HTauTau_EMu_2017/Ntuples/"){

  TString channel = "em";
  double luminosity = 0;
  float qcd_ss_os_iso_relaxed_ratio = 0.49;
  float trigger_filter_efficiency = 0.979;

  // Mapping of subsamples to output root-file
  map< TString , vector<TString> > samples_map;
  const map<TString, double>  *xsec_map    = 0;
  const map<TString, TString> *process_map = 0;

  if(era == "2017"){
    xsec_map    = &xsec_map_2017;
    process_map = &process_map_2017;
    luminosity  = 41900;
    samples_map["NOMINAL_ntuple_MuonEG_"    + channel] = MuonEG_Run2017;
    samples_map["NOMINAL_ntuple_DYJets_"    + channel] = DYJets_2017;
    samples_map["NOMINAL_ntuple_WJets_"     + channel] = WJets_2017;
    samples_map["NOMINAL_ntuple_TTbar_"     + channel] = TTbar_2017;
    samples_map["NOMINAL_ntuple_SingleTop_" + channel] = SingleTop_2017;
    samples_map["NOMINAL_ntuple_Diboson_"   + channel] = Diboson_2017;
    // samples_map["NOMINAL_ntuple_ggH_"       + channel] = GluGluHToTauTau_2017;
    samples_map["NOMINAL_ntuple_VBFH_"      + channel] = VBFHToTauTau_2017;
    //samples_map["NOMINAL_ntuple_EWKZ_"      + channel] = EWKZ_2017 ;
    inputDir="/nfs/dust/cms/user/mameyer/SM_HiggsTauTau/newMETv2/CMSSW_9_4_9/src/DesyTauAnalyses/NTupleMaker/test/HTauTau_EMu_2017/Ntuples/";
  }
  else if(era == "2016"){
    xsec_map    = &xsec_map_2016;
    process_map = &process_map_2016;
    luminosity  = 35866;
    samples_map["NOMINAL_ntuple_MuonEG_"    + channel] = MuonEG_Run2016;
    samples_map["NOMINAL_ntuple_DYJets_"    + channel] = DYJets_2016;
    samples_map["NOMINAL_ntuple_WJets_"     + channel] = WJets_2016;
    samples_map["NOMINAL_ntuple_TTbar_"     + channel] = TTbar_2016;
    samples_map["NOMINAL_ntuple_SingleTop_" + channel] = SingleTop_2016;
    samples_map["NOMINAL_ntuple_Diboson_"   + channel] = Diboson_2016;
    samples_map["NOMINAL_ntuple_ggH_"       + channel] = GluGluHToTauTau_2016;
    samples_map["NOMINAL_ntuple_VBFH_"      + channel] = VBFHToTauTau_2016;
    samples_map["NOMINAL_ntuple_EWKZ_"      + channel] = EWKZ_2016 ;
    inputDir="/nfs/dust/cms/user/mameyer/SM_HiggsTauTau/CMSSW_8_0_29/src/DesyTauAnalyses/NTupleMaker/test/HTauTau_EMu_2016/NTuples/";
  }

  // Needed for stitching
  double xsecWIncl      = xsec_map->at(process_map->at("WJets"));
  double xsecW1Jets     = xsec_map->at(process_map->at("W1Jets"));
  double xsecW2Jets     = xsec_map->at(process_map->at("W2Jets"));
  double xsecW3Jets     = xsec_map->at(process_map->at("W3Jets"));
  double xsecW4Jets     = xsec_map->at(process_map->at("W4Jets"));
  double xsecDYIncl     = xsec_map->at(process_map->at("DYJets"));
  double xsecDY1Jets    = xsec_map->at(process_map->at("DY1Jets"));
  double xsecDY2Jets    = xsec_map->at(process_map->at("DY2Jets"));
  double xsecDY3Jets    = xsec_map->at(process_map->at("DY3Jets"));
  double xsecDY4Jets    = xsec_map->at(process_map->at("DY4Jets"));
  double neventsWIncl   = getNEventsProcessed(inputDir+"/"+process_map->at("WJets")+".root");
  double neventsW1Jets  = getNEventsProcessed(inputDir+"/"+process_map->at("W1Jets")+".root");
  double neventsW2Jets  = getNEventsProcessed(inputDir+"/"+process_map->at("W2Jets")+".root");
  double neventsW3Jets  = getNEventsProcessed(inputDir+"/"+process_map->at("W3Jets")+".root");
  double neventsW4Jets  = getNEventsProcessed(inputDir+"/"+process_map->at("W4Jets")+".root");
  double neventsDYIncl  = getNEventsProcessed(inputDir+"/"+process_map->at("DYJets")+".root");
  double neventsDY1Jets = getNEventsProcessed(inputDir+"/"+process_map->at("DY1Jets")+".root");
  double neventsDY2Jets = getNEventsProcessed(inputDir+"/"+process_map->at("DY2Jets")+".root");
  double neventsDY3Jets = getNEventsProcessed(inputDir+"/"+process_map->at("DY3Jets")+".root");
  double neventsDY4Jets = getNEventsProcessed(inputDir+"/"+process_map->at("DY4Jets")+".root");

  // Loop over all samples
  for (auto const& sample : samples_map){

    cout << endl << sample.first << "  :  " << endl ;

    TFile *outFile = new TFile("NTuples_" + era + "_test_new_script/" + sample.first + ".root","RECREATE");
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
      if( xsec_map->find(subsample) == xsec_map->end() && !sample.first.Contains("MuonEG")){
	cout << endl << endl << "Sample " << subsample << " is missing in xsec_map. Exit code." << endl << endl ;
	exit(-1);
      }
      float xsec = 1;
      if(!sample.first.Contains("MuonEG")) xsec_map->at(subsample);

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
