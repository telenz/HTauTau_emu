#include "../GoF/bin/useful_classes.h"
#include "../GoF/bin/systematic_uncertainties.h"
#include "../GoF/bin/Config.cc"
#include "../GoF/bin/Unfold.C"
#include "../GoF/bin/HttStylesNew.cc"
#include <algorithm>
#include <typeinfo>
#include "TSystem.h"
#include "TROOT.h"

using namespace std;

void make_histograms(TString config_name="config_for_gof_2016.cfg") {

  TH1::SetDefaultSumw2();
  TH2::SetDefaultSumw2();
  gROOT->SetBatch(kTRUE);
  SetStyle();

  //************************************************************************************************
  // Config is read and parameters are set

  Config cfg(config_name);

  const TString directory = cfg.get<string>("directory");
  const string era        = cfg.get<string>("era");
  const bool use_embedded = cfg.get<bool>("use_embedded");
  const bool plot_2d      = cfg.get<bool>("plot_2d");
  const bool verbose      = cfg.get<bool>("verbose");
  const bool take_percentile_subrange  = cfg.get<bool>("take_percentile_subrange");
  const bool apply_equidistant_binning = cfg.get<bool>("apply_equidistant_binning");
  const TString output_file_suffix     = cfg.get<string>("output_file_suffix");
  const bool is_dnn_prediction         = cfg.get<bool>("is_dnn_prediction");
  const bool stage1                    = cfg.get<bool>("stage1");
  const bool no_uncertainties          = cfg.get<bool>("no_uncertainties");

  vector<string> category_names_vector = cfg.get<vector<string>>("categories");

  map< TString , Category > category_map;

  int i=1;
  int nbins = 0;
  for( auto cat_name : category_names_vector){
    class Category category_(cat_name);
    category_map[cat_name] = category_;
    category_map.at(cat_name).plot_2d = cfg.get<bool>(cat_name+"_plot_2d");
    category_map.at(cat_name).variable = cfg.get<string>(cat_name+"_variable");
    category_map.at(cat_name).nbins_1d = cfg.get<int>(cat_name+"_nbins_1d");
    category_map.at(cat_name).binning_1d = cfg.get<vector<float>>(cat_name+"_binning_1d");
    category_map.at(cat_name).binning_2d_x = cfg.get<vector<float>>(cat_name+"_binning_2d_x");
    category_map.at(cat_name).binning_2d_y = cfg.get<vector<float>>(cat_name+"_binning_2d_y");
    if(is_dnn_prediction) category_map.at(cat_name).class_nr = cfg.get<int>(cat_name+"_class_nr");
  }

  //************************************************************************************************
  // Define some common weights and cuts

  TString weight      = "xsec_lumi_weight*mcweight*puweight*effweight*trigger_filter_weight*prefiringweight*";
  TString mt_cut    = "&& mTdileptonMET<60 ";
  TString cuts_kine = "&& pt_1>13 && pt_2>10 && TMath::Max(pt_1,pt_2)>24 && metFilters && trg_muonelectron && nbtag == 0";
  cuts_kine += mt_cut;

  TString cuts_iso_general    = "&& iso_1<0.15 && iso_2<0.2 && extraelec_veto<0.5 && extramuon_veto<0.5 ";
  TString cuts_iso_ss_general = "&& extraelec_veto<0.5 && extramuon_veto<0.5 ";

  //************************************************************************************************
  // Set cuts, weights and binning on category level (cuts and weights will be later changed on individual basis)

  TString cuts_category_specific = "";
  for(auto &cat : category_map){
    cat.second.cutstring    = cuts_kine + cuts_iso_general + cuts_category_specific;
    if(is_dnn_prediction)   cat.second.cutstring += Form("&& predicted_class == %d",cat.second.class_nr);
    cat.second.bins_x_2d    = {0,50,55,60,65,70,75,80,85,90,95,100,400};
    cat.second.bins_y_2d    = {15,20,25,30,35,40,300};
  }

  //************************************************************************************************
  // Define the set of samples (used in each of the categories)

  Sample Data("data_obs");
  Sample ZTT("ZTT");
  Sample ZL("ZL");
  Sample W("W");
  Sample TTL("TTL");
  Sample TTT("TTT");
  Sample VVL("VVL");
  Sample VVT("VVT");
  Sample ST("ST");
  Sample QCD("QCD");
  Sample EMB("EMB");
  Sample ggH("ggH125");
  Sample qqH("qqH125");
  Sample ZH("ZH125");
  Sample WH("WH125");
  Sample ggHWW("ggHWW125");
  Sample qqHWW("qqHWW125");
  Sample ttH("ttH125");

  ggH.is_signal   = true;
  qqH.is_signal   = true;
  ZH.is_signal    = true;
  WH.is_signal    = true;
  ggHWW.is_signal = true;
  qqHWW.is_signal = true;
  ttH.is_signal   = true;

  if(!is_dnn_prediction){
    Data.filename   = "em-NOMINAL_ntuple_MuonEG.root";
    ZTT.filename    = "em-NOMINAL_ntuple_DYJets.root" ;
    ZL.filename     = "em-NOMINAL_ntuple_DYJets.root" ;
    W.filename      = "em-NOMINAL_ntuple_WJets.root" ;
    TTL.filename    = "em-NOMINAL_ntuple_TTbar.root" ;
    TTT.filename    = "em-NOMINAL_ntuple_TTbar.root" ;
    VVL.filename    = "em-NOMINAL_ntuple_Diboson.root" ;
    VVT.filename    = "em-NOMINAL_ntuple_Diboson.root" ;
    ST.filename     = "em-NOMINAL_ntuple_SingleTop.root" ;
    QCD.filename    = "em-NOMINAL_ntuple_MuonEG.root" ;
    EMB.filename    = "em-NOMINAL_ntuple_Embedded.root" ;
    ggH.filename    = "em-NOMINAL_ntuple_ggH.root" ;
    qqH.filename    = "em-NOMINAL_ntuple_VBFH.root" ;
    ZH.filename     = "em-NOMINAL_ntuple_ZH.root" ;
    WH.filename     = "em-NOMINAL_ntuple_WH.root" ;
    ggHWW.filename  = "em-NOMINAL_ntuple_ggHWW.root" ;
    qqHWW.filename  = "em-NOMINAL_ntuple_VBFHWW.root" ;
    ttH.filename    = "em-NOMINAL_ntuple_ttH.root" ;
  }
  else{
    Data.filename   = "em-NOMINAL_ntuple_Data.root" ;
    ZTT.filename    = "em-NOMINAL_ntuple_ZTT.root" ;
    ZL.filename     = "em-NOMINAL_ntuple_ZL.root" ;
    W.filename      = "em-NOMINAL_ntuple_W.root" ;
    TTL.filename    = "em-NOMINAL_ntuple_TT.root" ;
    TTT.filename    = "em-NOMINAL_ntuple_TT.root" ;
    VVL.filename    = "em-NOMINAL_ntuple_Diboson.root" ;
    VVT.filename    = "em-NOMINAL_ntuple_Diboson.root" ;
    ST.filename     = "em-NOMINAL_ntuple_SingleTop.root" ;
    QCD.filename    = "em-NOMINAL_ntuple_Data.root" ;
    EMB.filename    = "em-NOMINAL_ntuple_ZTT.root" ;
    ggH.filename    = "em-NOMINAL_ntuple_ggH125.root" ;
    qqH.filename    = "em-NOMINAL_ntuple_qqH125.root" ;
  }

  map<TString,Sample> sample_map = { { "0_Data" , Data },
				     { "1_QCD"  , QCD } ,
				     { "2_ZL"   , ZL } ,
				     { "3_W"    , W } ,
				     { "4_TTL"  , TTL } ,
				     { "5_TTT"  , TTT } ,
				     { "6_VVL"  , VVL } ,
				     { "7_VVT"  , VVT } ,
				     { "8_ST"   , ST } ,
				     { "9_ZTT"  , ZTT } ,
				     { "10_ggH125" , ggH },
				     { "11_qqH125" , qqH },
				     { "12_ZH125"  , ZH },
				     { "13_WH125"  , WH },
				     { "14_ggHWW"  , ggHWW },
				     { "15_qqHWW"  , qqHWW },
				     { "16_ttH"    , ttH }
  };

  if(use_embedded){
    sample_map.erase("9_ZTT");
    sample_map["9_EMB"] = EMB;
  }

  cout << endl << endl << "... Sample categories ... "<< endl ;
  for(auto & smpl : sample_map ) cout << " - " << smpl.second.name << endl;
  cout << endl;

  // Now add to every category this set of samples and add the cuts
  for(auto &cat : category_map){
    for(auto smpl : sample_map){
      cat.second.sample_list[smpl.first] = smpl.second;
      cat.second.sample_list[smpl.first].cutString      = "q_1*q_2<0" + cat.second.cutstring;
      cat.second.sample_list[smpl.first].weightString   = weight;
      cat.second.sample_list[smpl.first].cutStringSS    = "q_1*q_2>0" + cat.second.cutstring;
      cat.second.sample_list[smpl.first].weightStringSS = weight + "qcdweight*";
      cat.second.sample_list[smpl.first].variable       = cat.second.variable;
    }
  }

  // Define sample specific cutStrings and weightStrings but common for all categories
  for(auto &cat : category_map){
    cat.second.sample_list["0_Data"].weightString   = "1*";
    cat.second.sample_list["0_Data"].weightStringSS = "1*";
    cat.second.sample_list["1_QCD"].weightString    = "1*";
    cat.second.sample_list["1_QCD"].weightStringSS  = "qcdweight*";
    cat.second.sample_list["1_QCD"].cutString       = "1==2";  // don't fill anything in this histogram should remain empty

    cat.second.sample_list["2_ZL"].cutString       += "&&!isZTTEM";
    cat.second.sample_list["2_ZL"].cutStringSS     += "&&!isZTTEM";
    cat.second.sample_list["2_ZL"].weightString    += "zptmassweight*";
    cat.second.sample_list["2_ZL"].weightStringSS  += "zptmassweight*";

    cat.second.sample_list["4_TTL"].cutString      += "&& isZTTEM<0.5";
    cat.second.sample_list["4_TTL"].cutStringSS    += "&& isZTTEM<0.5";
    cat.second.sample_list["5_TTT"].cutString      += "&& isZTTEM>0.5";
    cat.second.sample_list["5_TTT"].cutStringSS    += "&& isZTTEM>0.5";
    cat.second.sample_list["4_TTL"].weightString   += "topptweight*";
    cat.second.sample_list["4_TTL"].weightStringSS += "topptweight*";
    cat.second.sample_list["5_TTT"].weightString   += "topptweight*";
    cat.second.sample_list["5_TTT"].weightStringSS += "topptweight*";

    cat.second.sample_list["6_VVL"].cutString      += "&& isZTTEM<0.5";
    cat.second.sample_list["6_VVL"].cutStringSS    += "&& isZTTEM<0.5";
    cat.second.sample_list["7_VVT"].cutString      += "&& isZTTEM>0.5";
    cat.second.sample_list["7_VVT"].cutStringSS    += "&& isZTTEM>0.5";

    cat.second.sample_list["10_ggH125"].weightString    += "weight_ggh_NNLOPS*";
    cat.second.sample_list["10_ggH125"].weightStringSS  += "weight_ggh_NNLOPS*";

    if(use_embedded){
      cat.second.sample_list["9_EMB"].weightString   = "mcweight*effweight*embeddedWeight*embedded_stitching_weight*embedded_rate_weight*";
      cat.second.sample_list["9_EMB"].weightStringSS = "mcweight*effweight*embeddedWeight*embedded_stitching_weight*embedded_rate_weight*qcdweight*";
      cat.second.sample_list["9_EMB"].cutString   += "&& mcweight<1";
      cat.second.sample_list["9_EMB"].cutStringSS += "&& mcweight<1";
    }
    else{
      cat.second.sample_list["9_ZTT"].cutString      += "&&isZTTEM";
      cat.second.sample_list["9_ZTT"].cutStringSS    += "&&isZTTEM";
      cat.second.sample_list["9_ZTT"].weightString   += "zptmassweight*";
      cat.second.sample_list["9_ZTT"].weightStringSS += "zptmassweight*";
    }

    // These samples are not supposed to be subtracted from the ss region for QCD estimation
    cat.second.sample_list["0_Data"].subtract_from_ss    = false;
    cat.second.sample_list["1_QCD"].subtract_from_ss     = false;
    cat.second.sample_list["10_ggH125"].subtract_from_ss = false;
    cat.second.sample_list["11_qqH125"].subtract_from_ss = false;
    cat.second.sample_list["12_ZH125"].subtract_from_ss  = false;
    cat.second.sample_list["13_WH125"].subtract_from_ss  = false;
    cat.second.sample_list["14_ggHWW"].subtract_from_ss  = false;
    cat.second.sample_list["15_qqHWW"].subtract_from_ss  = false;
    cat.second.sample_list["16_ttH"].subtract_from_ss    = false;
    if(use_embedded){ // if embedded is used, the following two processes are included in the embedded sample
      cat.second.sample_list["5_TTT"].subtract_from_ss   = false;
      cat.second.sample_list["7_VVT"].subtract_from_ss   = false;
    }

  } // end of loop over categories

  //************************************************************************************************
  // Define systematic uncertainties
    
  // Open one tree to test existence of uncertainty shifts
  TFile *file_ = new TFile( directory + "/" + Data.filename );
  TTree *tree_ = (TTree*) file_->Get("TauCheck");

  for(auto& cat : category_map){
    for(auto & smpl : cat.second.sample_list){

      if( no_uncertainties ) continue;

      if( smpl.second.name == "data_obs" ) continue;

      // 1.) QCD uncertainties (10 nuisances)
      smpl.second = create_systematic_uncertainty("qcd0jetRateUp"  , "_CMS_htt_qcd_0jet_rate_Run" +era+ "Up"  , cat.second.plot_2d, smpl.second, tree_, false, "", true, "qcdweight*","qcdweight_0jet_rate_up*");
      smpl.second = create_systematic_uncertainty("qcd0jetRateDown", "_CMS_htt_qcd_0jet_rate_Run" +era+ "Down", cat.second.plot_2d, smpl.second, tree_, false, "", true, "qcdweight*","qcdweight_0jet_rate_down*");
      smpl.second = create_systematic_uncertainty("qcd1jetRateUp"  , "_CMS_htt_qcd_1jet_rate_Run" +era+ "Up"  , cat.second.plot_2d, smpl.second, tree_, false, "", true, "qcdweight*","qcdweight_1jet_rate_up*");
      smpl.second = create_systematic_uncertainty("qcd1jetRateDown", "_CMS_htt_qcd_1jet_rate_Run" +era+ "Down", cat.second.plot_2d, smpl.second, tree_, false, "", true, "qcdweight*","qcdweight_1jet_rate_down*");
      smpl.second = create_systematic_uncertainty("qcd2jetRateUp"  , "_CMS_htt_qcd_2jet_rate_Run" +era+ "Up"  , cat.second.plot_2d, smpl.second, tree_, false, "", true, "qcdweight*","qcdweight_2jet_rate_up*");
      smpl.second = create_systematic_uncertainty("qcd2jetRateDown", "_CMS_htt_qcd_2jet_rate_Run" +era+ "Down", cat.second.plot_2d, smpl.second, tree_, false, "", true, "qcdweight*","qcdweight_2jet_rate_down*");
      smpl.second = create_systematic_uncertainty("qcd0jetShapeUp"  , "_CMS_htt_qcd_0jet_shape_Run" +era+ "Up"  , cat.second.plot_2d, smpl.second, tree_, false, "", true, "qcdweight*","qcdweight_0jet_shape_up*");
      smpl.second = create_systematic_uncertainty("qcd0jetShapeDown", "_CMS_htt_qcd_0jet_shape_Run" +era+ "Down", cat.second.plot_2d, smpl.second, tree_, false, "", true, "qcdweight*","qcdweight_0jet_shape_down*");
      smpl.second = create_systematic_uncertainty("qcd1jetShapeUp"  , "_CMS_htt_qcd_1jet_shape_Run" +era+ "Up"  , cat.second.plot_2d, smpl.second, tree_, false, "", true, "qcdweight*","qcdweight_1jet_shape_up*");
      smpl.second = create_systematic_uncertainty("qcd1jetShapeDown", "_CMS_htt_qcd_1jet_shape_Run" +era+ "Down", cat.second.plot_2d, smpl.second, tree_, false, "", true, "qcdweight*","qcdweight_1jet_shape_down*");
      smpl.second = create_systematic_uncertainty("qcdIsoUp"  , "_CMS_htt_qcd_isoUp", cat.second.plot_2d, smpl.second, tree_, false, "", true, "qcdweight*","qcdweight_iso_up*");
      smpl.second = create_systematic_uncertainty("qcdIsoDown", "_CMS_htt_qcd_isoDown", cat.second.plot_2d, smpl.second, tree_, false, "", true, "qcdweight*","qcdweight_iso_down*");
      smpl.second = create_systematic_uncertainty("qcd2jetShapeUp"  , "_CMS_htt_qcd_2jet_shape_Run" +era+ "Up"  , cat.second.plot_2d, smpl.second, tree_, false, "", true, "qcdweight*","qcdweight_2jet_shape_up*");
      smpl.second = create_systematic_uncertainty("qcd2jetShapeDown", "_CMS_htt_qcd_2jet_shape_Run" +era+ "Down", cat.second.plot_2d, smpl.second, tree_, false, "", true, "qcdweight*","qcdweight_2jet_shape_down*");
      
      if( smpl.second.name == "QCD"  ) continue;

      // 2.) Electron scale (applied on both embedded and mc) // not used at the moment
      smpl.second = create_systematic_uncertainty("escale_all_Up"  , "_CMS_scale_eUp"  , cat.second.plot_2d, smpl.second, tree_, true, "escaleUp");
      smpl.second = create_systematic_uncertainty("escale_all_Down", "_CMS_scale_eDown", cat.second.plot_2d, smpl.second, tree_, true, "escaleDown");

      // 3.) Electron scale (applied only on embedded)
      if(smpl.second.name == "EMB"){
	smpl.second = create_systematic_uncertainty("escale_emb_Up"  , "_CMS_scale_emb_eUp"  , cat.second.plot_2d, smpl.second, tree_, true, "escaleUp");
	smpl.second = create_systematic_uncertainty("escale_emb_Down", "_CMS_scale_emb_eDown", cat.second.plot_2d, smpl.second, tree_, true, "escaleDown");
      }

      // 4.) Electron scale and resolution (applied only on MC)
      if(smpl.second.name != "EMB"){
	smpl.second = create_systematic_uncertainty("escale_mc_Up"  , "_CMS_scale_mc_eUp"  , cat.second.plot_2d, smpl.second, tree_, true, "escaleUp");
	smpl.second = create_systematic_uncertainty("escale_mc_Down", "_CMS_scale_mc_eDown", cat.second.plot_2d, smpl.second, tree_, true, "escaleDown");
   smpl.second = create_systematic_uncertainty("ereso_mc_Up"  , "_CMS_reso_mc_eUp"  , cat.second.plot_2d, smpl.second, tree_, true, "eresoUp");
	smpl.second = create_systematic_uncertainty("ereso_mc_Down", "_CMS_reso_mc_eDown", cat.second.plot_2d, smpl.second, tree_, true, "eresoDown");
      }

      // 5.) TT contamination in embedded sample  (only initialize this uncertainty -> calculate it later)
      if(smpl.second.name == "EMB"){
	smpl.second = create_systematic_uncertainty("ttContEmbUp"  , "_CMS_htt_emb_ttbar_Run" + era + "Up"  , cat.second.plot_2d, smpl.second, tree_, false, "", false, "","");
	smpl.second = create_systematic_uncertainty("ttContEmbDown", "_CMS_htt_emb_ttbar_Run" + era + "Down", cat.second.plot_2d, smpl.second, tree_, false, "", false, "","");
      }

      if(smpl.second.name == "EMB") continue;
      
      // 6.) (b-)mistag uncertainty
      smpl.second = create_systematic_uncertainty("mistagUp"  , "_CMS_htt_mistag_b_Run" +era+ "Up"  , cat.second.plot_2d, smpl.second, tree_, true, "mistagUp");
      smpl.second = create_systematic_uncertainty("mistagDown", "_CMS_htt_mistag_b_Run" +era+ "Down", cat.second.plot_2d, smpl.second, tree_, true, "mistagDown");
      smpl.second = create_systematic_uncertainty("btagUp"  , "_CMS_htt_eff_b_Run" +era+ "Up"  , cat.second.plot_2d, smpl.second, tree_, true, "btagUp");
      smpl.second = create_systematic_uncertainty("btagDown", "_CMS_htt_eff_b_Run" +era+ "Down", cat.second.plot_2d, smpl.second, tree_, true, "btagDown");

      // 7.) JES
      // smpl.second = create_systematic_uncertainty("jesUp"  , "_CMS_scale_j_Run" +era+ "Up"  , cat.second.plot_2d, smpl.second, tree_, true, "jesUp");
      // smpl.second = create_systematic_uncertainty("jesDown", "_CMS_scale_j_Run" +era+ "Down", cat.second.plot_2d, smpl.second, tree_, true, "jesDown");
      smpl.second = create_systematic_uncertainty("jecUncEta0To5Up"  , "_CMS_scale_j_eta0to5_Run" +era+ "Up"  , cat.second.plot_2d, smpl.second, tree_, true, "jecUncEta0To5Up");
      smpl.second = create_systematic_uncertainty("jecUncEta0To5Down", "_CMS_scale_j_eta0to5_Run" +era+ "Down", cat.second.plot_2d, smpl.second, tree_, true, "jecUncEta0To5Down");
      smpl.second = create_systematic_uncertainty("jecUncEta0To3Up"  , "_CMS_scale_j_eta0to3_Run" +era+ "Up"  , cat.second.plot_2d, smpl.second, tree_, true, "jecUncEta0To3Up");
      smpl.second = create_systematic_uncertainty("jecUncEta0To3Down", "_CMS_scale_j_eta0to3_Run" +era+ "Down", cat.second.plot_2d, smpl.second, tree_, true, "jecUncEta0To3Down");
      smpl.second = create_systematic_uncertainty("jecUncEta3To5Up"  , "_CMS_scale_j_eta3to5_Run" +era+ "Up"  , cat.second.plot_2d, smpl.second, tree_, true, "jecUncEta3To5Up");
      smpl.second = create_systematic_uncertainty("jecUncEta3To5Down", "_CMS_scale_j_eta3to5_Run" +era+ "Down", cat.second.plot_2d, smpl.second, tree_, true, "jecUncEta3To5Down");
      smpl.second = create_systematic_uncertainty("jecUncRelativeBalUp"  , "_CMS_scale_j_RelativeBal_Run" +era+ "Up"  , cat.second.plot_2d, smpl.second, tree_, true, "jecUncRelativeBalUp");
      smpl.second = create_systematic_uncertainty("jecUncRelativeBalDown", "_CMS_scale_j_RelativeBal_Run" +era+ "Down", cat.second.plot_2d, smpl.second, tree_, true, "jecUncRelativeBalDown");
      if(era=="2017" || era=="2018"){
	smpl.second = create_systematic_uncertainty("jecUncRelativeSampleUp"  , "_CMS_scale_j_RelativeSample_Run" +era+ "Up"  , cat.second.plot_2d, smpl.second, tree_, true, "jecUncRelativeSampleUp");
	smpl.second = create_systematic_uncertainty("jecUncRelativeSampleDown", "_CMS_scale_j_RelativeSample_Run" +era+ "Down", cat.second.plot_2d, smpl.second, tree_, true, "jecUncRelativeSampleDown");
      }

      // 8.) JES correlated
      // smpl.second = create_systematic_uncertainty("jesUp"  , "_CMS_scale_j_RunUp"  , cat.second.plot_2d, smpl.second, tree_, true, "jesUp");
      // smpl.second = create_systematic_uncertainty("jesDown", "_CMS_scale_j_RunDown", cat.second.plot_2d, smpl.second, tree_, true, "jesDown");
      smpl.second = create_systematic_uncertainty("jecUncEta0To5Up_corr"  , "_CMS_scale_j_eta0to5Up"  , cat.second.plot_2d, smpl.second, tree_, true, "jecUncEta0To5Up");
      smpl.second = create_systematic_uncertainty("jecUncEta0To5Down_corr", "_CMS_scale_j_eta0to5Down", cat.second.plot_2d, smpl.second, tree_, true, "jecUncEta0To5Down");
      smpl.second = create_systematic_uncertainty("jecUncEta0To3Up_corr"  , "_CMS_scale_j_eta0to3Up"  , cat.second.plot_2d, smpl.second, tree_, true, "jecUncEta0To3Up");
      smpl.second = create_systematic_uncertainty("jecUncEta0To3Down_corr", "_CMS_scale_j_eta0to3Down", cat.second.plot_2d, smpl.second, tree_, true, "jecUncEta0To3Down");
      smpl.second = create_systematic_uncertainty("jecUncEta3To5Up_corr"  , "_CMS_scale_j_eta3to5Up"  , cat.second.plot_2d, smpl.second, tree_, true, "jecUncEta3To5Up");
      smpl.second = create_systematic_uncertainty("jecUncEta3To5Down_corr", "_CMS_scale_j_eta3to5Down", cat.second.plot_2d, smpl.second, tree_, true, "jecUncEta3To5Down");
      smpl.second = create_systematic_uncertainty("jecUncRelativeBalUp_corr"  , "_CMS_scale_j_RelativeBalUp"  , cat.second.plot_2d, smpl.second, tree_, true, "jecUncRelativeBalUp");
      smpl.second = create_systematic_uncertainty("jecUncRelativeBalDown_corr", "_CMS_scale_j_RelativeBalDown", cat.second.plot_2d, smpl.second, tree_, true, "jecUncRelativeBalDown");
      if(era=="2017"){
	smpl.second = create_systematic_uncertainty("jecUncRelativeSampleUp_corr"  , "_CMS_scale_j_RelativeSampleUp"  , cat.second.plot_2d, smpl.second, tree_, true, "jecUncRelativeSampleUp");
	smpl.second = create_systematic_uncertainty("jecUncRelativeSampleDown_corr", "_CMS_scale_j_RelativeSampleDown", cat.second.plot_2d, smpl.second, tree_, true, "jecUncRelativeSampleDown");
      }

      // 8.) Recoil scale/resolution uncertainties
      if(smpl.second.name == "ZTT" || smpl.second.name == "ZL" || smpl.second.name == "W" || smpl.second.name.Contains("125")){
	smpl.second = create_systematic_uncertainty("recoilscaleUp"  , "_CMS_htt_boson_scale_met_Run" + era + "Up"  , cat.second.plot_2d, smpl.second, tree_, true, "recoilscaleUp");
	smpl.second = create_systematic_uncertainty("recoilscaleDown", "_CMS_htt_boson_scale_met_Run" + era + "Down", cat.second.plot_2d, smpl.second, tree_, true, "recoilscaleDown");
	smpl.second = create_systematic_uncertainty("recoilresoUp"  , "_CMS_htt_boson_reso_met_Run" + era + "Up"  , cat.second.plot_2d, smpl.second, tree_, true, "recoilresoUp");
	smpl.second = create_systematic_uncertainty("recoilresoDown", "_CMS_htt_boson_reso_met_Run" + era + "Down", cat.second.plot_2d, smpl.second, tree_, true, "recoilresoDown");
      }

      // 9.) Unclustered MET scale
      smpl.second = create_systematic_uncertainty("unclMetUp"  , "_CMS_scale_met_unclusteredUp"  , cat.second.plot_2d, smpl.second, tree_, true, "unclMetUp");
      smpl.second = create_systematic_uncertainty("unclMetDown", "_CMS_scale_met_unclusteredDown", cat.second.plot_2d, smpl.second, tree_, true, "unclMetDown");

      // 10.) TTbar shape
      if(smpl.second.name == "TTT" || smpl.second.name == "TTL"){
	smpl.second = create_systematic_uncertainty("ttbarShapeUp"  , "_CMS_htt_ttbarShapeUp"  , cat.second.plot_2d, smpl.second, tree_, false, "", true, "topptweight*","topptweight*topptweight*");
	smpl.second = create_systematic_uncertainty("ttbarShapeDown", "_CMS_htt_ttbarShapeDown", cat.second.plot_2d, smpl.second, tree_, false, "", true, "topptweight*","");
      }

      // 11.) DY shape
      if(smpl.second.name == "ZTT" || smpl.second.name == "ZL" ){
         if (era == "2016"){
            smpl.second = create_systematic_uncertainty("dyShapeUp"  , "_CMS_htt_dyShape_Run"+era+"Up"  , cat.second.plot_2d, smpl.second, tree_, false, "", true, "zptmassweight*","(1.0+1.1*(zptmassweight-1))*");
            smpl.second = create_systematic_uncertainty("dyShapeDown", "_CMS_htt_dyShape_Run"+era+"Down", cat.second.plot_2d, smpl.second, tree_, false, "", true, "zptmassweight*","(1.0+0.9*(zptmassweight-1))*");
         }
         else{
            smpl.second = create_systematic_uncertainty("dyShapeUp"  , "_CMS_htt_dyShape"+era+"Up"  , cat.second.plot_2d, smpl.second, tree_, false, "", true, "zptmassweight*","(1.0+1.1*(zptmassweight-1))*");
            smpl.second = create_systematic_uncertainty("dyShapeDown", "_CMS_htt_dyShape"+era+"Down", cat.second.plot_2d, smpl.second, tree_, false, "", true, "zptmassweight*","(1.0+0.9*(zptmassweight-1))*");
         }
      }

      // 12.) ggh reweightings
      if( smpl.second.name.Contains("ggH") ){
	smpl.second = create_systematic_uncertainty("gghShapeResUp"  , "_THU_ggH_ResUp"  , cat.second.plot_2d, smpl.second, tree_, false, "", true, "weight_ggh_NNLOPS*", "weight_ggh_NNLOPS*THU_ggH_Res*");
	smpl.second = create_systematic_uncertainty("gghShapeResDown", "_THU_ggH_ResDown", cat.second.plot_2d, smpl.second, tree_, false, "", true, "weight_ggh_NNLOPS*", "weight_ggh_NNLOPS/THU_ggH_Res*");
	smpl.second = create_systematic_uncertainty("gghShapeMig01Up"  , "_THU_ggH_Mig01Up"  , cat.second.plot_2d, smpl.second, tree_, false, "", true, "weight_ggh_NNLOPS*", "weight_ggh_NNLOPS*THU_ggH_Mig01*");
	smpl.second = create_systematic_uncertainty("gghShapeMig01Down", "_THU_ggH_Mig01Down", cat.second.plot_2d, smpl.second, tree_, false, "", true, "weight_ggh_NNLOPS*", "weight_ggh_NNLOPS/THU_ggH_Mig01*");
	smpl.second = create_systematic_uncertainty("gghShapeMig12Up"  , "_THU_ggH_Mig12Up"  , cat.second.plot_2d, smpl.second, tree_, false, "", true, "weight_ggh_NNLOPS*", "weight_ggh_NNLOPS*THU_ggH_Mig12*");
	smpl.second = create_systematic_uncertainty("gghShapeMig12Down", "_THU_ggH_Mig12Down", cat.second.plot_2d, smpl.second, tree_, false, "", true, "weight_ggh_NNLOPS*", "weight_ggh_NNLOPS/THU_ggH_Mig12*");
	smpl.second = create_systematic_uncertainty("gghShapeVBF2jUp"  , "_THU_ggH_VBF2jUp"  , cat.second.plot_2d, smpl.second, tree_, false, "", true, "weight_ggh_NNLOPS*", "weight_ggh_NNLOPS*THU_ggH_VBF2j*");
	smpl.second = create_systematic_uncertainty("gghShapeVBF2jDown", "_THU_ggH_VBF2jDown", cat.second.plot_2d, smpl.second, tree_, false, "", true, "weight_ggh_NNLOPS*", "weight_ggh_NNLOPS/THU_ggH_VBF2j*");
	smpl.second = create_systematic_uncertainty("gghShapeVBF3jUp"  , "_THU_ggH_VBF3jUp"  , cat.second.plot_2d, smpl.second, tree_, false, "", true, "weight_ggh_NNLOPS*", "weight_ggh_NNLOPS*THU_ggH_VBF3j*");
	smpl.second = create_systematic_uncertainty("gghShapeVBF3jDown", "_THU_ggH_VBF3jDown", cat.second.plot_2d, smpl.second, tree_, false, "", true, "weight_ggh_NNLOPS*", "weight_ggh_NNLOPS/THU_ggH_VBF3j*");
	smpl.second = create_systematic_uncertainty("gghShapePT60Up"  , "_THU_ggH_PT60Up"  , cat.second.plot_2d, smpl.second, tree_, false, "", true, "weight_ggh_NNLOPS*", "weight_ggh_NNLOPS*THU_ggH_PT60*");
	smpl.second = create_systematic_uncertainty("gghShapePT60Down", "_THU_ggH_PT60Down", cat.second.plot_2d, smpl.second, tree_, false, "", true, "weight_ggh_NNLOPS*", "weight_ggh_NNLOPS/THU_ggH_PT60*");
	smpl.second = create_systematic_uncertainty("gghShapePT120Up"  , "_THU_ggH_PT120Up"  , cat.second.plot_2d, smpl.second, tree_, false, "", true, "weight_ggh_NNLOPS*", "weight_ggh_NNLOPS*THU_ggH_PT120*");
	smpl.second = create_systematic_uncertainty("gghShapePT120Down", "_THU_ggH_PT120Down", cat.second.plot_2d, smpl.second, tree_, false, "", true, "weight_ggh_NNLOPS*", "weight_ggh_NNLOPS/THU_ggH_PT120*");
	smpl.second = create_systematic_uncertainty("gghShapeqmtopUp"  , "_THU_ggH_qmtopUp"  , cat.second.plot_2d, smpl.second, tree_, false, "", true, "weight_ggh_NNLOPS*", "weight_ggh_NNLOPS*THU_ggH_qmtop*");
	smpl.second = create_systematic_uncertainty("gghShapeqmtopDown", "_THU_ggH_qmtopDown", cat.second.plot_2d, smpl.second, tree_, false, "", true, "weight_ggh_NNLOPS*", "weight_ggh_NNLOPS/THU_ggH_qmtop*");
	smpl.second = create_systematic_uncertainty("gghShapeMuUp"  , "_THU_ggH_MuUp"  , cat.second.plot_2d, smpl.second, tree_, false, "", true, "weight_ggh_NNLOPS*", "weight_ggh_NNLOPS*THU_ggH_Mu*");
	smpl.second = create_systematic_uncertainty("gghShapeMuDown", "_THU_ggH_MuDown", cat.second.plot_2d, smpl.second, tree_, false, "", true, "weight_ggh_NNLOPS*", "weight_ggh_NNLOPS/THU_ggH_Mu*");
      }

      // 13.) prefire uncertainty
      if (era !="2018"){
         smpl.second = create_systematic_uncertainty("prefireWeightUp"  , "_CMS_prefiringUp"  , cat.second.plot_2d, smpl.second, tree_, false, "", true, "prefiringweight*","prefiringweightup*");
         smpl.second = create_systematic_uncertainty("prefireWeightDown", "_CMS_prefiringDown", cat.second.plot_2d, smpl.second, tree_, false, "", true, "prefiringweight*","prefiringweightdown*");
      }
    } // end of loop over samples
    if(verbose){
      cout << endl << endl << "... Uncertainties of samples ... " << endl << endl ;
      for(auto & smpl : cat.second.sample_list){
	cout << smpl.second.name << " : " <<endl;
	for(auto& unc : smpl.second.uncertainties) cout<<"  - "<<unc.first<<" : "<<unc.second.name<<endl;
      }
    }
  } // end of loop over categories
  //************************************************************************************************
  // Fill histograms for each of the categories and each of the samples
  cout << endl << endl << "... Drawing ... " << endl;

  // Define output file
  TString filename = era+"em-synced-"+output_file_suffix+".root";
  TString output_dir = "output/"+era+"/";
  if(!is_dnn_prediction){
    if(!plot_2d) output_dir += "/var_1d/";
    else         output_dir += "/var_2d/";
  }
  gSystem -> Exec("mkdir " + output_dir);
  TFile * file_out   = new TFile( output_dir + "/" + filename , "RECREATE" );

  // Add here also a loop over the categories
  for(auto & cat : category_map){

    cout << "**************************************" << endl;
    cout << "**************************************" << endl;
    cout << endl << "Category : " << cat.second.name << endl;
    cout << endl << "Variable 1d = " << cat.second.variable << endl;

    for(auto & smpl : cat.second.sample_list){
      cout << "**************************************" << endl;
      cout << smpl.second.name << " : " << smpl.second.filename << endl;
      TFile *file = new TFile( directory + "/" + smpl.second.filename );
      TTree *tree = (TTree*) file->Get("TauCheck");

      //*******************************************************************************************
      // Calculate the binning
      if( smpl.second.name == "data_obs" ){
	if(!cat.second.plot_2d){
	  cat.second.binning_1d = calc_binning_1d(take_percentile_subrange, apply_equidistant_binning, cat.second, directory);
	}
	else{
	  std::pair<vector<float>, vector<float>> binning = calc_binning_2d(take_percentile_subrange, apply_equidistant_binning, cat.second, directory);
	  cat.second.binning_2d_x = binning.first;
	  cat.second.binning_2d_y = binning.second;
	}
      }

      nbins = cat.second.binning_1d.size() -1;
      smpl.second.hist_1d   = new TH1D(smpl.second.name + "_os_1d", "", nbins, &cat.second.binning_1d[0]);
      smpl.second.histSS_1d = new TH1D(smpl.second.name + "_ss_1d", "", nbins, &cat.second.binning_1d[0]);

      const int nbins_2d_x = cat.second.binning_2d_x.size()-1;
      const int nbins_2d_y = cat.second.binning_2d_y.size()-1;
      smpl.second.hist_2d = new TH2D(smpl.second.name + "_os_2d", "", nbins_2d_x, &cat.second.binning_2d_x[0], nbins_2d_y, &cat.second.binning_2d_y[0] );
      smpl.second.histSS_2d = new TH2D(smpl.second.name + "_ss_2d", "", nbins_2d_x,  &cat.second.binning_2d_x[0], nbins_2d_y,  &cat.second.binning_2d_y[0] );

      TString full_weight_string    = smpl.second.weightString;
      TString full_weight_string_ss = smpl.second.weightStringSS;

      if(verbose){
	cout << "variable          " << " : " << smpl.second.variable << endl;
	cout << "weight string        " << " : " << full_weight_string << endl;
	cout << "weight string ss     " << " : " << full_weight_string_ss << endl;
	cout << "cut string           " << " : " << smpl.second.cutString << endl;
	cout << "cut string ss        " << " : " << smpl.second.cutStringSS << endl;
      }
      if(!cat.second.plot_2d){
	tree -> Draw( smpl.second.variable + ">>" + smpl.second.hist_1d->GetName() , full_weight_string + "(" + smpl.second.cutString + ")" );
	tree -> Draw( smpl.second.variable + ">>" + smpl.second.histSS_1d->GetName() , full_weight_string_ss + "(" + smpl.second.cutStringSS + ")" );
      }
      else{
	tree -> Draw( smpl.second.variable + ">>" + smpl.second.hist_2d->GetName() , full_weight_string + "(" + smpl.second.cutString + ")" );
	tree -> Draw( smpl.second.variable + ">>" + smpl.second.histSS_2d->GetName() , full_weight_string_ss + "(" + smpl.second.cutStringSS + ")" );

	smpl.second.hist_1d          = (TH1D*) Unfold(smpl.second.hist_2d);
	smpl.second.histSS_1d        = (TH1D*) Unfold(smpl.second.histSS_2d);
      }

      // Make QCD estimation
      if( smpl.second.name == "QCD" )         cat.second.sample_list["1_QCD"].hist_1d -> Add(smpl.second.histSS_1d , +1);
      else if( smpl.second.subtract_from_ss ) cat.second.sample_list["1_QCD"].hist_1d -> Add(smpl.second.histSS_1d , -1);

      // Loop over systematic uncertainties
      for(auto &sys : smpl.second.uncertainties){

	full_weight_string    = sys.second.weightString;
	full_weight_string_ss = sys.second.weightStringSS;
	if(verbose){
	  cout << "variable             " << " : " << sys.second.variable << endl;
	  cout << "weight string        " << " : " << full_weight_string << endl;
	  cout << "weight string ss     " << " : " << full_weight_string_ss << endl;
	  cout << "cut string           " << " : " << sys.second.cutString << endl;
	  cout << "cut string ss        " << " : " << sys.second.cutStringSS << endl;
	}

	sys.second.hist_1d   = new TH1D(sys.second.name + "_os_1d", "", nbins, &cat.second.binning_1d[0]);
	sys.second.histSS_1d = new TH1D(sys.second.name + "_ss_1d", "", nbins, &cat.second.binning_1d[0]);

	sys.second.hist_2d   = new TH2D(sys.second.name + "_os_2d"         , "" , nbins_2d_x ,  &cat.second.binning_2d_x[0] , nbins_2d_y ,  &cat.second.binning_2d_y[0] );
	sys.second.histSS_2d   = new TH2D(sys.second.name + "_ss_2d"         , "" , nbins_2d_x ,  &cat.second.binning_2d_x[0] , nbins_2d_y ,  &cat.second.binning_2d_y[0]);

	if(!cat.second.plot_2d){
	  tree -> Draw( sys.second.variable + ">>" + sys.second.hist_1d->GetName() , full_weight_string + "(" + sys.second.cutString + ")" );
	  tree -> Draw( sys.second.variable + ">>" + sys.second.histSS_1d->GetName() , full_weight_string_ss + "(" + sys.second.cutStringSS + ")" );
	}
	else{
	  tree -> Draw( sys.second.variable + ">>" + sys.second.hist_2d->GetName() , full_weight_string + "(" + sys.second.cutString + ")" );
	  tree -> Draw( sys.second.variable + ">>" + sys.second.histSS_2d->GetName() , full_weight_string_ss + "(" + sys.second.cutStringSS + ")" );

	  sys.second.hist_1d          = (TH1D*) Unfold(sys.second.hist_2d);
	  sys.second.histSS_1d        = (TH1D*) Unfold(sys.second.histSS_2d);
	}

	// Make QCD estimation for up-downward variation for qcdweight
	if( !sys.first.Contains("qcd") ) continue;

	if( smpl.second.name == "QCD" )         cat.second.sample_list["1_QCD"].uncertainties[sys.first].hist_1d -> Add(sys.second.histSS_1d , +1);
	else if( smpl.second.subtract_from_ss ) cat.second.sample_list["1_QCD"].uncertainties[sys.first].hist_1d -> Add(sys.second.histSS_1d , -1);;

      } // end of loop over sys uncertainties
    } // end of loop over samples

    // Calculate embedded uncertainty
    if(use_embedded && cat.second.sample_list.at("9_EMB").uncertainties.find("ttContEmbUp") != cat.second.sample_list.at("9_EMB").uncertainties.end()){
      cat.second.sample_list.at("9_EMB").uncertainties.at("ttContEmbUp").hist_1d   -> Add(cat.second.sample_list.at("5_TTT").hist_1d , 0.1);
      cat.second.sample_list.at("9_EMB").uncertainties.at("ttContEmbDown").hist_1d -> Add(cat.second.sample_list.at("5_TTT").hist_1d , -0.1);
      cat.second.sample_list.at("9_EMB").uncertainties.at("ttContEmbUp").hist_1d   -> Add(cat.second.sample_list.at("7_VVT").hist_1d , 0.1);
      cat.second.sample_list.at("9_EMB").uncertainties.at("ttContEmbDown").hist_1d -> Add(cat.second.sample_list.at("7_VVT").hist_1d , -0.1);
    }
    //***********************************************************************************************
    // Blinding
    double threshold = 0.5;
    double err_sys = 0.09;
    TH1D * allBkg = (TH1D*) cat.second.sample_list["1_QCD"].hist_1d -> Clone();
    allBkg -> Reset();
    TH1D * allSig = (TH1D*) cat.second.sample_list["1_QCD"].hist_1d -> Clone();
    allSig -> Reset();

    for(auto & smpl : cat.second.sample_list) {
      if( smpl.second.is_signal )  allSig->Add(smpl.second.hist_1d);
      else if( smpl.second.name != "data_obs" && ( !use_embedded || (smpl.first != "TTT" && smpl.first != "VVT") )) allBkg->Add(smpl.second.hist_1d);
    }

    for(int i_bin=1; i_bin <= allSig->GetNbinsX(); i_bin++ ){
      double s = allSig->GetBinContent(i_bin);
      double b = allBkg->GetBinContent(i_bin);
      if( s/sqrt(s + b + pow(err_sys*b,2) ) > threshold){
	cout<<"BLINDING applied!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
	for(auto & smpl : cat.second.sample_list) smpl.second.hist_1d->SetBinContent(i_bin,0);
      }
    }
    //***********************************************************************************************
    // Write all histograms to output file
    cout << endl << endl << "... Writing histograms to output file ... " << endl;

    file_out -> mkdir(cat.second.name);
    file_out -> cd(cat.second.name);
    for(auto & smpl : cat.second.sample_list){
      if( use_embedded && (smpl.first.Contains("TTT") || smpl.first.Contains("VVT")) ) continue;
      smpl.second.hist_1d -> Write( smpl.second.name );
      for(auto & sys : smpl.second.uncertainties){
	if(sys.second.name.Contains("qcd") && smpl.second.name != "QCD") continue;
	sys.second.hist_1d -> Write( sys.second.name );
      }
    }

    // Write the final sum of weights of the nominal selection
    cout << endl << "... Final histogram content : "<< endl;
    for(auto & smpl : cat.second.sample_list) {
      if( use_embedded && (smpl.first.Contains("TTT") || smpl.first.Contains("VVT")) ) continue;
      cout << smpl.second.name << " : " << smpl.second.hist_1d -> GetSumOfWeights() << endl;
    }
    cout << "Bkg together : " << allBkg -> GetSumOfWeights() << endl << endl;
  } // end of loop over categories
  cout << "output file : " << output_dir + "/" + filename << endl << endl;
  file_out -> Close();
}
