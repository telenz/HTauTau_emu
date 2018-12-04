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

  if(era == "2017"){
    xsec_map    = &xsec_map_2017;
    process_map = &process_map_2017;
    luminosity  = 41900;
    trigger_filter_efficiency = 1.0;
    qcd_ss_os_iso_relaxed_ratio = 2.38;
    embedded_trigger_weight  = 1.00;
    embedded_tracking_weight = 0.99;
    samples_map["NOMINAL_ntuple_MuonEG_"    + channel] = MuonEG_Run2017;
    samples_map["NOMINAL_ntuple_DYJets_"    + channel] = DYJets_2017;
    samples_map["NOMINAL_ntuple_WJets_"     + channel] = WJets_2017;
    samples_map["NOMINAL_ntuple_TTbar_"     + channel] = TTbar_2017;
    samples_map["NOMINAL_ntuple_SingleTop_" + channel] = SingleTop_2017;
    samples_map["NOMINAL_ntuple_Diboson_"   + channel] = Diboson_2017;
    samples_map["NOMINAL_ntuple_ggH_"       + channel] = GluGluHToTauTau_2017;
    samples_map["NOMINAL_ntuple_VBFH_"      + channel] = VBFHToTauTau_2017;
    samples_map["NOMINAL_ntuple_Embedded_"  + channel] = Embedded_2017;
    input_dir="/nfs/dust/cms/user/mameyer/SM_HiggsTauTau/newMETv2/CMSSW_9_4_9/src/DesyTauAnalyses/NTupleMaker/test/HTauTau_EMu_2017/Ntuples/";

  }
  else if(era == "2016"){
    xsec_map    = &xsec_map_2016;
    process_map = &process_map_2016;
    luminosity  = 35866;
    trigger_filter_efficiency = 0.979;
    qcd_ss_os_iso_relaxed_ratio = 2.3;
    embedded_trigger_weight  = 1.03;
    embedded_tracking_weight = 0.98;
    samples_map["NOMINAL_ntuple_MuonEG_"    + channel] = MuonEG_Run2016;
    samples_map["NOMINAL_ntuple_DYJets_"    + channel] = DYJets_2016;
    samples_map["NOMINAL_ntuple_WJets_"     + channel] = WJets_2016;
    samples_map["NOMINAL_ntuple_TTbar_"     + channel] = TTbar_2016;
    samples_map["NOMINAL_ntuple_SingleTop_" + channel] = SingleTop_2016;
    samples_map["NOMINAL_ntuple_Diboson_"   + channel] = Diboson_2016;
    samples_map["NOMINAL_ntuple_ggH_"       + channel] = GluGluHToTauTau_2016;
    samples_map["NOMINAL_ntuple_VBFH_"      + channel] = VBFHToTauTau_2016;
    samples_map["NOMINAL_ntuple_Embedded_"  + channel] = Embedded_2016;
    //  input_dir="/nfs/dust/cms/user/tlenz/13TeV/2017/CMSSW/2016_legacy/CMSSW_8_0_29/src/DesyTauAnalyses/NTupleMaker/test/HTauTau_EMu_2016/NTuples_new_round_final/ntuples_v3/";
    input_dir="/nfs/dust/cms/user/tlenz/13TeV/2017/CMSSW/2016_legacy/CMSSW_8_0_29/src/DesyTauAnalyses/NTupleMaker/test/HTauTau_EMu_2016/NTuples/ntuples_v4/";
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
  double neventsWIncl   = getNEventsProcessed(input_dir+"/"+process_map->at("WJets")+".root");
  double neventsW1Jets  = getNEventsProcessed(input_dir+"/"+process_map->at("W1Jets")+".root");
  double neventsW2Jets  = getNEventsProcessed(input_dir+"/"+process_map->at("W2Jets")+".root");
  double neventsW3Jets  = getNEventsProcessed(input_dir+"/"+process_map->at("W3Jets")+".root");
  double neventsW4Jets  = getNEventsProcessed(input_dir+"/"+process_map->at("W4Jets")+".root");
  double neventsDYIncl  = getNEventsProcessed(input_dir+"/"+process_map->at("DYJets")+".root");
  double neventsDY1Jets = getNEventsProcessed(input_dir+"/"+process_map->at("DY1Jets")+".root");
  double neventsDY2Jets = getNEventsProcessed(input_dir+"/"+process_map->at("DY2Jets")+".root");
  double neventsDY3Jets = getNEventsProcessed(input_dir+"/"+process_map->at("DY3Jets")+".root");
  double neventsDY4Jets = getNEventsProcessed(input_dir+"/"+process_map->at("DY4Jets")+".root");

  TString output_dir = "NTuples_" + era + "_correct_unc_v1";
  gSystem -> Exec("mkdir " + output_dir);

  // Loop over all samples
  for (auto const& sample : samples_map){

    cout << endl << sample.first << "  :  " << endl ;

    TFile *outFile = new TFile(output_dir + "/" + sample.first + ".root","RECREATE");
    TTree *outTree = new TTree("TauCheck", "tree created as DNN input");
    bool firstTree = true;
    TList* treeList = new TList();

    for(TString const& subsample: sample.second) {

      cout << "  - " << subsample << " : ";

      TFile *inFile  = new TFile( input_dir + "/" + subsample + ".root" ,"READ");
      TTree *inTree  = (TTree*) inFile -> Get("TauCheck");
      double nevents = getNEventsProcessed( input_dir + "/" + subsample + ".root" );

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
      float jpt_1;
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
      inTree->SetBranchAddress("jpt_1",&jpt_1);

      outFile->cd();
      TTree *currentTree = new TTree(subsample,"temporary tree");

      // Create a branch for xsec_lumi_weight
      float xsec_lumi_weight;
      float qcd_correction;
      float trigger_filter_weight;
      float embedded_stitching_weight;
      float embedded_rate_weight;
      float prefiring_weight;
      int htxs_reco_flag_ggh;
      int htxs_reco_flag_qqh;
      if(firstTree){
	outTree    = inTree->CloneTree(0);
	outTree->Branch("xsec_lumi_weight", &xsec_lumi_weight, "xsec_lumi_weight/F");
	outTree->Branch("qcd_correction", &qcd_correction, "qcd_correction/F");
	outTree->Branch("trigger_filter_weight", &trigger_filter_weight, "trigger_filter_weight/F");
	outTree->Branch("embedded_stitching_weight", &embedded_stitching_weight, "embedded_stitching_weight/F");
	outTree->Branch("embedded_rate_weight", &embedded_rate_weight, "embedded_rate_weight/F");
	outTree->Branch("prefiring_weight", &prefiring_weight, "prefiring_weight/F");
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
      currentTree->Branch("prefiring_weight", &prefiring_weight, "prefiring_weight/F");
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
	  if( iso_1 > 0.15 )               continue;
	  if( iso_2 > 0.2 )                continue;
	  if( extraelec_veto > 0.5 )       continue;
	  if( extramuon_veto > 0.5 )       continue;
	  if( pt_1 < 10 )                  continue;
	  if( pt_2 < 10 )                  continue;
	  if( TMath::Max(pt_1,pt_2) < 20 ) continue;
	  if( dzeta < -50 )                continue;
	  if( mTdileptonMET > 90 )         continue;
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

	// add flags for cut categories which correspond to htxs_stage1cats
	htxs_reco_flag_ggh = 0;
	htxs_reco_flag_qqh = 0;
	if((njets>=2)&&(mjj>400)&&(jdeta>2.8)&&(pt_tt>0)&&(pt_tt<200)&&(pt_ttjj>0)&&(pt_ttjj<25)) htxs_reco_flag_ggh = 101;
	else if((njets>=2)&&(mjj>400)&&(jdeta>2.8)&&(pt_tt>0)&&(pt_tt<200)&&(pt_ttjj>25)) htxs_reco_flag_ggh = 102;
	else if(njets==0) htxs_reco_flag_ggh = 103;
	else if((njets==1)&&(pt_tt>0)&&(pt_tt<60)) htxs_reco_flag_ggh = 104;
	else if((njets==1)&&(pt_tt>60)&&(pt_tt<120)) htxs_reco_flag_ggh = 105;
	else if((njets==1)&&(pt_tt>120)&&(pt_tt<200)) htxs_reco_flag_ggh = 106;
	else if((njets==1)&&(pt_tt>200)) htxs_reco_flag_ggh = 107;
	else if((njets>=2)&&( (mjj>0&&mjj<400) || (mjj>400&&jdeta<2.8) )&&(pt_tt>0)&&(pt_tt<60)) htxs_reco_flag_ggh = 108;
	else if((njets>=2)&&( (mjj>0&&mjj<400) || (mjj>400&&jdeta<2.8) )&&(pt_tt>60)&&(pt_tt<120)) htxs_reco_flag_ggh = 109;
	else if((njets>=2)&&( (mjj>0&&mjj<400) || (mjj>400&&jdeta<2.8) )&&(pt_tt>120)&&(pt_tt<200)) htxs_reco_flag_ggh = 110;
	else if((njets>=2)&&(pt_tt>200)) htxs_reco_flag_ggh = 111;

	if((jpt_1>0)&&(jpt_1<200)&&(njets>=2)&&(mjj>400)&&(jdeta>2.8)&&(pt_ttjj>0)&&(pt_ttjj<25)) htxs_reco_flag_qqh = 201;
	else if((jpt_1>0)&&(jpt_1<200)&&(njets>=2)&&(mjj>400)&&(jdeta>2.8)&&(pt_ttjj>25)) htxs_reco_flag_qqh = 202;
	else if((jpt_1>0)&&(jpt_1<200)&&(njets>=2)&&(mjj>60)&&(mjj<120)) htxs_reco_flag_qqh = 203;
	else if(( (jpt_1>0&&jpt_1<200&&njets<2) || (jpt_1>0&&jpt_1<200&&njets>=2&&mjj>400&&jdeta<2.8) || (jpt_1>0&&jpt_1<200&&njets>=2&&mjj>0&&mjj<60) || (jpt_1>0&&jpt_1<200&&njets>=2&&mjj>120&&mjj<400))) htxs_reco_flag_qqh = 204;
	else if(jpt_1>200) htxs_reco_flag_qqh = 205;

	// prefiring weights (from AN-18-255)
	prefiring_weight=1;
	if( sample.first.Contains("TTBar") && era == "2016")      prefiring_weight = 0.989;
	else if( sample.first.Contains("TTBar") && era == "2017") prefiring_weight = 0.984;
	else if( sample.first.Contains("VBFH") && htxs_reco_flag_qqh == 201 && era == "2016") prefiring_weight = 0.972;
	else if( sample.first.Contains("VBFH") && htxs_reco_flag_qqh == 201 && era == "2017") prefiring_weight = 0.950;
	else if( sample.first.Contains("VBFH") && htxs_reco_flag_qqh == 202 && era == "2016") prefiring_weight = 0.972;
	else if( sample.first.Contains("VBFH") && htxs_reco_flag_qqh == 202 && era == "2017") prefiring_weight = 0.950;
	else if( sample.first.Contains("VBFH") && htxs_reco_flag_qqh == 203 && era == "2016") prefiring_weight = 0.972;
	else if( sample.first.Contains("VBFH") && htxs_reco_flag_qqh == 203 && era == "2017") prefiring_weight = 0.950;
	else if( sample.first.Contains("VBFH") && htxs_reco_flag_qqh == 204 && era == "2016") prefiring_weight = 0.983;
	else if( sample.first.Contains("VBFH") && htxs_reco_flag_qqh == 204 && era == "2017") prefiring_weight = 0.970;
	else if( sample.first.Contains("VBFH") && htxs_reco_flag_qqh == 205 && era == "2016") prefiring_weight = 0.920;
	else if( sample.first.Contains("VBFH") && htxs_reco_flag_qqh == 205 && era == "2017") prefiring_weight = 0.850;

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
