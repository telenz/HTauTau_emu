#include <iostream>
#include <map>
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TList.h"
#include "TSystem.h"
#include "settings_for_eras.h"

bool applyPreselection = true;

void create_dnn_ntuples( TString era = "2017" ){

  // Some definitions
  TString channel = "em";
  double luminosity = 0;
  float qcd_ss_os_iso_relaxed_ratio = 0;
  float trigger_filter_efficiency = 1;
  float embedded_trigger_weight = 1.0;
  float embedded_tracking_weight = 1.0;
  TString input_dir;

  // Mapping of subsamples to output root-file
  map< TString , vector<TString> > samples_map;
  const map<TString, double>  *xsec_map    = 0;
  const map<TString, TString> *process_map = 0;
  map<TString , int> n_events_per_sample;
  
  if (era == "2018"){
     xsec_map    = &xsec_map_2018;
     process_map = &process_map_2018;
     luminosity  = 59740;
     trigger_filter_efficiency = 1.0;
     qcd_ss_os_iso_relaxed_ratio = 1.89; //number from Janek's talk in TauPOG meeting (10.04.19)
     embedded_trigger_weight  = 1.00;
     embedded_tracking_weight = 0.99; //2017 scale factors for embedded currently used
     samples_map[channel + "-NOMINAL_ntuple_MuonEG"   ] = MuonEG_Run2018;
     samples_map[channel + "-NOMINAL_ntuple_Embedded" ] = Embedded_2018;
     samples_map[channel + "-NOMINAL_ntuple_DYJets"   ] = DYJets_2018;
     samples_map[channel + "-NOMINAL_ntuple_WJets"    ] = WJets_2018;
     samples_map[channel + "-NOMINAL_ntuple_TTbar"    ] = TTbar_2018;
     samples_map[channel + "-NOMINAL_ntuple_SingleTop"] = SingleTop_2018;
     samples_map[channel + "-NOMINAL_ntuple_Diboson"  ] = Diboson_2018;
     samples_map[channel + "-NOMINAL_ntuple_ggH"      ] = GluGluHToTauTau_2018;
     samples_map[channel + "-NOMINAL_ntuple_VBFH"     ] = VBFHToTauTau_2018;
     samples_map[channel + "-NOMINAL_ntuple_ZH"       ] = ZHToTauTau_2018;
     samples_map[channel + "-NOMINAL_ntuple_WH"       ] = WHToTauTau_2018;
     samples_map[channel + "-NOMINAL_ntuple_ggHWW"    ] = ggHToWW_2018;
     samples_map[channel + "-NOMINAL_ntuple_VBFHWW"   ] = VBFHToWW_2018;
     samples_map[channel + "-NOMINAL_ntuple_ttH"      ] = ttH_2018;
     input_dir="/nfs/dust/cms/user/mameyer/SM_HiggsTauTau/master/CMSSW_10_2_15_patch2/src/DesyTauAnalyses/NTupleMaker/test/HTauTau_EMu_2018_all_eras/";  
  }
  else if(era == "2017"){
    xsec_map    = &xsec_map_2017;
    process_map = &process_map_2017;
    n_events_per_sample = n_events_per_sample_2017;
    luminosity  = 41530;                 // Take number from LUMI twiki : https://twiki.cern.ch/twiki/bin/view/CMS/TWikiLUM#SummaryTable
    trigger_filter_efficiency = 1.0;
    qcd_ss_os_iso_relaxed_ratio = 2.38;
    embedded_trigger_weight  = 1.00;
    embedded_tracking_weight = 0.99;
    samples_map[channel + "-NOMINAL_ntuple_MuonEG"   ] = MuonEG_Run2017;
    samples_map[channel + "-NOMINAL_ntuple_Embedded" ] = Embedded_2017;
    samples_map[channel + "-NOMINAL_ntuple_DYJets"   ] = DYJets_2017;
    samples_map[channel + "-NOMINAL_ntuple_WJets"    ] = WJets_2017;
    samples_map[channel + "-NOMINAL_ntuple_TTbar"    ] = TTbar_2017;
    samples_map[channel + "-NOMINAL_ntuple_SingleTop"] = SingleTop_2017;
    samples_map[channel + "-NOMINAL_ntuple_Diboson"  ] = Diboson_2017;
    samples_map[channel + "-NOMINAL_ntuple_ggH"      ] = GluGluHToTauTau_2017;
    samples_map[channel + "-NOMINAL_ntuple_VBFH"     ] = VBFHToTauTau_2017;
    samples_map[channel + "-NOMINAL_ntuple_ZH"       ] = ZHToTauTau_2017;
    samples_map[channel + "-NOMINAL_ntuple_WH"       ] = WHToTauTau_2017;
    samples_map[channel + "-NOMINAL_ntuple_ggHWW"    ] = ggHToWW_2017;
    samples_map[channel + "-NOMINAL_ntuple_VBFHWW"   ] = VBFHToWW_2017;
    samples_map[channel + "-NOMINAL_ntuple_ttH"      ] = ttH_2017;
    //input_dir="/nfs/dust/cms/user/mameyer/SM_HiggsTauTau/newMETv2/CMSSW_9_4_9/src/DesyTauAnalyses/NTupleMaker/test/HTauTau_EMu_2017_all_eras/";
    input_dir="/nfs/dust/cms/user/mameyer/SM_HiggsTauTau/master/CMSSW_10_2_15_patch2/src/DesyTauAnalyses/NTupleMaker/test/HTauTau_EMu_2017_all_eras/";
  }
  else if(era == "2016"){
    xsec_map    = &xsec_map_2016;
    process_map = &process_map_2016;
    //n_events_per_sample = n_events_per_sample_2016;
    luminosity  = 35920;               // Take number from LUMI twiki : https://twiki.cern.ch/twiki/bin/view/CMS/TWikiLUM#SummaryTable
    trigger_filter_efficiency = 0.979;
    qcd_ss_os_iso_relaxed_ratio = 2.3;
    embedded_trigger_weight  = 1.03;
    embedded_tracking_weight = 0.98;
    samples_map[channel + "-NOMINAL_ntuple_MuonEG"   ] = MuonEG_Run2016;
    samples_map[channel + "-NOMINAL_ntuple_Embedded" ] = Embedded_2016;
    samples_map[channel + "-NOMINAL_ntuple_DYJets"   ] = DYJets_2016;
    samples_map[channel + "-NOMINAL_ntuple_WJets"    ] = WJets_2016;
    samples_map[channel + "-NOMINAL_ntuple_TTbar"    ] = TTbar_2016;
    samples_map[channel + "-NOMINAL_ntuple_SingleTop"] = SingleTop_2016;
    samples_map[channel + "-NOMINAL_ntuple_Diboson"  ] = Diboson_2016;
    samples_map[channel + "-NOMINAL_ntuple_ggH"      ] = GluGluHToTauTau_2016;
    samples_map[channel + "-NOMINAL_ntuple_VBFH"     ] = VBFHToTauTau_2016;
    samples_map[channel + "-NOMINAL_ntuple_ZH"       ] = ZHToTauTau_2016;
    samples_map[channel + "-NOMINAL_ntuple_WH"       ] = WHToTauTau_2016;
    samples_map[channel + "-NOMINAL_ntuple_ggHWW"    ] = ggHToWW_2016;
    samples_map[channel + "-NOMINAL_ntuple_VBFHWW"   ] = VBFHToWW_2016;
    samples_map[channel + "-NOMINAL_ntuple_ttH"      ] = ttH_2016;
    input_dir="/nfs/dust/cms/user/tlenz/13TeV/2017/CMSSW/2016_legacy/CMSSW_8_0_29/src/DesyTauAnalyses/NTupleMaker/test/HTauTau_EMu_2016/NTuples/ntuples_v5/";
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
  double neventsWIncl = 0;
  double neventsW1Jets  = 0;
  double neventsW2Jets  = 0;
  double neventsW3Jets  = 0;
  double neventsW4Jets  = 0;
  double neventsDYIncl  = 0;
  double neventsDY1Jets = 0;
  double neventsDY2Jets = 0;
  double neventsDY3Jets = 0;
  double neventsDY4Jets = 0;
  if (n_events_per_sample.size()==0){
     neventsWIncl   = getNEventsProcessed(input_dir+"/"+process_map->at("WJets")+".root");
     neventsW1Jets  = getNEventsProcessed(input_dir+"/"+process_map->at("W1Jets")+".root");
     neventsW2Jets  = getNEventsProcessed(input_dir+"/"+process_map->at("W2Jets")+".root");
     neventsW3Jets  = getNEventsProcessed(input_dir+"/"+process_map->at("W3Jets")+".root");
     neventsW4Jets  = getNEventsProcessed(input_dir+"/"+process_map->at("W4Jets")+".root");
     neventsDYIncl  = getNEventsProcessed(input_dir+"/"+process_map->at("DYJets")+".root");
     neventsDY1Jets = getNEventsProcessed(input_dir+"/"+process_map->at("DY1Jets")+".root");
     neventsDY2Jets = getNEventsProcessed(input_dir+"/"+process_map->at("DY2Jets")+".root");
     neventsDY3Jets = getNEventsProcessed(input_dir+"/"+process_map->at("DY3Jets")+".root");
     neventsDY4Jets = getNEventsProcessed(input_dir+"/"+process_map->at("DY4Jets")+".root");
  }
  else{
     neventsWIncl   = n_events_per_sample.at("WJetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8");
     neventsW1Jets  = n_events_per_sample.at("W1JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8");
     neventsW2Jets  = n_events_per_sample.at("W2JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8");
     neventsW3Jets  = n_events_per_sample.at("W3JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8");
     neventsW4Jets  = n_events_per_sample.at("W4JetsToLNu_TuneCP5_13TeV-madgraphMLM-pythia8");
     neventsDYIncl  = n_events_per_sample.at("DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8");
     neventsDY1Jets = n_events_per_sample.at("DY1JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8");
     neventsDY2Jets = n_events_per_sample.at("DY2JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8");
     neventsDY3Jets = n_events_per_sample.at("DY3JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8");
     neventsDY4Jets = n_events_per_sample.at("DY4JetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8");
  }
  TString output_dir = "NTuples_" + era;
  gSystem -> Exec("mkdir " + output_dir);

  // Loop over all samples
  for (auto const& sample : samples_map){

    cout << endl << sample.first << "  :  " << endl ;

    TFile *outFile = new TFile(output_dir + "/" + sample.first + ".root","RECREATE");
    TTree *outTree = new TTree("TauCheck", "tree created as DNN input");
    bool firstTree = true;
    TList* treeList = new TList();

    
    for(TString const& subsample: sample.second) {
       bool FoundSampleInMap=false;
      cout << "  - " << subsample << " : ";

      TFile *inFile  = new TFile( input_dir + "/" + subsample + ".root" ,"READ");
      TTree *inTree  = (TTree*) inFile -> Get("TauCheck");
      double nevents=0.;
      for (auto const& sample_evt : n_events_per_sample){
         if (subsample==sample_evt.first){
            nevents=n_events_per_sample.at(subsample);
            FoundSampleInMap=true;
         }
      }
      if (!FoundSampleInMap){
         nevents = getNEventsProcessed( input_dir + "/" + subsample + ".root" );
      }
      
      // SetBranchAddress for variables that need are needed for preselection or stitching
      unsigned int npartons;
      float iso_1;
      float iso_2;
      bool extraelec_veto;
      bool extramuon_veto;
      float pt_1;
      float pt_2;
      float dzeta;
      float mTdileptonMET;
      bool metFilters;
      bool trg_muonelectron;
      int run;
      int njets;
      float mjj;
      float jdeta;
      float pt_tt;
      float pt_ttjj;
      float dijetpt;
      float jpt_1;
      float jpt_2;
      float jeta_1;
      float jeta_2;
      int htxs_stage1p1cat;
      inTree->SetBranchAddress("npartons",&npartons);
      inTree->SetBranchAddress("iso_1",&iso_1);
      inTree->SetBranchAddress("iso_2",&iso_2);
      inTree->SetBranchAddress("extraelec_veto",&extraelec_veto);
      inTree->SetBranchAddress("extramuon_veto",&extramuon_veto);
      inTree->SetBranchAddress("pt_1",&pt_1);
      inTree->SetBranchAddress("pt_2",&pt_2);
      inTree->SetBranchAddress("dzeta",&dzeta);
      inTree->SetBranchAddress("mTdileptonMET",&mTdileptonMET);
      inTree->SetBranchAddress("metFilters",&metFilters);
      inTree->SetBranchAddress("trg_muonelectron",&trg_muonelectron);
      inTree->SetBranchAddress("run",&run);
      inTree->SetBranchAddress("njets",&njets);
      inTree->SetBranchAddress("mjj",&mjj);
      inTree->SetBranchAddress("jdeta",&jdeta);
      inTree->SetBranchAddress("pt_tt",&pt_tt);
      inTree->SetBranchAddress("pt_ttjj",&pt_ttjj);
      inTree->SetBranchAddress("dijetpt",&dijetpt);
      inTree->SetBranchAddress("jpt_1",&jpt_1);
      inTree->SetBranchAddress("jpt_2",&jpt_2);
      inTree->SetBranchAddress("jeta_1",&jeta_1);
      inTree->SetBranchAddress("jeta_2",&jeta_2);
      inTree->SetBranchAddress("htxs_stage1p1cat",&htxs_stage1p1cat);

      outFile->cd();
      TTree *currentTree = new TTree(subsample,"temporary tree");

      // Create a branch for xsec_lumi_weight
      float xsec_lumi_weight;
      float qcd_correction;
      float trigger_filter_weight;
      float embedded_stitching_weight;
      float embedded_rate_weight;
      int htxs_reco_flag_ggh;
      int htxs_reco_flag_qqh;
      if(firstTree){
	outTree    = inTree->CloneTree(0);
	outTree->Branch("xsec_lumi_weight", &xsec_lumi_weight, "xsec_lumi_weight/F");
	outTree->Branch("qcd_correction", &qcd_correction, "qcd_correction/F");
	outTree->Branch("trigger_filter_weight", &trigger_filter_weight, "trigger_filter_weight/F");
	outTree->Branch("embedded_stitching_weight", &embedded_stitching_weight, "embedded_stitching_weight/F");
	outTree->Branch("embedded_rate_weight", &embedded_rate_weight, "embedded_rate_weight/F");
	outTree->Branch("htxs_reco_flag_ggh", &htxs_reco_flag_ggh, "htxs_reco_flag_ggh/I");
	outTree->Branch("htxs_reco_flag_qqh", &htxs_reco_flag_qqh, "htxs_reco_flag_qqh/I");
	firstTree  = false;
      }
      currentTree = inTree->CloneTree(0);
      currentTree->Branch("xsec_lumi_weight", &xsec_lumi_weight, "xsec_lumi_weight/F");
      currentTree->Branch("qcd_correction", &qcd_correction, "qcd_correction/F");
      currentTree->Branch("trigger_filter_weight", &trigger_filter_weight, "trigger_filter_weight/F");
      currentTree->Branch("embedded_stitching_weight", &embedded_stitching_weight, "embedded_stitching_weight/F");
      currentTree->Branch("embedded_rate_weight", &embedded_rate_weight, "embedded_rate_weight/F");
      currentTree->Branch("htxs_reco_flag_ggh", &htxs_reco_flag_ggh, "htxs_reco_flag_ggh/I");
      currentTree->Branch("htxs_reco_flag_qqh", &htxs_reco_flag_qqh, "htxs_reco_flag_qqh/I");

      // lumi-xsec-weight added
      if( xsec_map->find(subsample) == xsec_map->end() && !sample.first.Contains("MuonEG")  && !sample.first.Contains("Embedded")){
	cout << endl << endl << "Sample " << subsample << " is missing in xsec_map. Exit code." << endl << endl ;
	exit(-1);
      }
      float xsec = 1;
      if(!sample.first.Contains("MuonEG") && !sample.first.Contains("Embedded")) xsec = xsec_map->at(subsample);

      for (int i=0; i<inTree->GetEntries(); i++) {
         inTree->GetEntry(i);

	// Add here preselection if necessary
	if(applyPreselection){
	  if( iso_1 > 0.5 )                continue;
	  if( iso_2 > 0.5 )                continue;
	  if( extraelec_veto > 0.5 )       continue;
	  if( extramuon_veto > 0.5 )       continue;
	  if( pt_1 < 10 )                  continue;
	  if( pt_2 < 10 )                  continue;
	  if( TMath::Max(pt_1,pt_2) < 20 ) continue;
	  //if( dzeta < -50 )                continue;
	  if( mTdileptonMET > 90 )         continue;
	  if( metFilters < 0.5 )           continue;
	  if( trg_muonelectron < 0.5 )     continue;
	}

	xsec_lumi_weight = xsec*luminosity/nevents;
	qcd_correction = qcd_ss_os_iso_relaxed_ratio;
	trigger_filter_weight = trigger_filter_efficiency;

	// Replace jet variables to have an effectie cut of jetpt > 30 GeV
	if(njets < 2){
	  jdeta   = -10;
	  mjj     = -10;
	  dijetpt = -10;
	  pt_ttjj = -10;
	  jpt_2   = -10;
	  jeta_2  = -10;
	  if(njets < 1){
	    jpt_1 = -10;
	    jeta_1= -10;
	  }
	}
	
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

	if( sample.first.Contains("MuonEG") || sample.first.Contains("Embedded")){
	  xsec_lumi_weight = 1.;
	  trigger_filter_weight = 1.;
	}
	if( sample.first.Contains("Embedded") && era == "2016"){
	  embedded_stitching_weight = 
	    ((run >= 272007) && (run < 275657))*(1.0/0.891)
	    +((run >= 275657) && (run < 276315))*(1.0/0.910)
	    +((run >= 276315) && (run < 276831))*(1.0/0.953)
	    +((run >= 276831) && (run < 277772))*(1.0/0.947)
	    +((run >= 277772) && (run < 278820))*(1.0/0.942)
	    +((run >= 278820) && (run < 280919))*(1.0/0.906)
	    +((run >= 280919) && (run < 284045))*(1.0/0.950);
	}
	else embedded_stitching_weight = 1.;

	embedded_rate_weight = embedded_trigger_weight * embedded_tracking_weight;

	// add flags for cut categories which correspond to htxs_stage1p1cats
	htxs_reco_flag_ggh = 0;
	htxs_reco_flag_qqh = 0;
	// reco bins corresponding to stage 1.1 categories (see: https://indico.cern.ch/event/788748/contributions/3492755/attachments/1882178/3101573/SMHTT_ML_2017_status.pdf)
	if(njets==0)                    htxs_reco_flag_ggh = 100;
	else if(njets==1 && pt_tt<120)  htxs_reco_flag_ggh = 101;
	else if(njets==1 && pt_tt>=120) htxs_reco_flag_ggh = 102;
	else if(njets>=2)               htxs_reco_flag_ggh = 103;

	if(njets<=1)                                htxs_reco_flag_qqh = 200;
	else if(njets>=2 && mjj<350)                htxs_reco_flag_qqh = 201;
	else if(njets>=2 && mjj>=350 && pt_tt<200)  htxs_reco_flag_qqh = 202;
	else if(njets>=2 && mjj>=350 && pt_tt>=200) htxs_reco_flag_qqh = 203;

	// Select hadronic and leptonic part of VH sample
	if( subsample.Contains("VH") || subsample.Contains("WplusH") || subsample.Contains("WminusH") ){
	  if( sample.first.Contains("VBFH") && (htxs_stage1p1cat>210 || htxs_stage1p1cat<200) ) continue;
	  if( (sample.first.Contains("WH") || sample.first.Contains("ZH")) && htxs_stage1p1cat<=210 && htxs_stage1p1cat>=200 ) continue;
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
