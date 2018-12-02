#include "../GoF/useful_classes.h"
#include "../GoF/systematic_uncertainties.h"
#include "../GoF/Config.cc"
#include "../GoF/Unfold.C"
#include "../GoF/HttStylesNew.cc"
#include "TROOT.h"
#include <algorithm>
#include <typeinfo>

using namespace std;

void make_histograms(TString config_name="config_for_gof_2016.cfg") {

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
  const bool take_percentile_subrange = cfg.get<bool>("take_percentile_subrange");
  const TString output_file_suffix = cfg.get<string>("output_file_suffix");
  const bool is_dnn_prediction = cfg.get<bool>("is_dnn_prediction");

  vector<string> category_names_vector = cfg.get<vector<string>>("categories");

  map< TString , Category > category_map;

  int i=1;
  int nbins = 0;
  for( auto cat_name : category_names_vector){
    class Category category_(cat_name);
    category_map[cat_name] = category_;
    category_map.at(cat_name).plot_2d = cfg.get<bool>(cat_name+"_plot_2d");
    category_map.at(cat_name).variable = cfg.get<string>(cat_name+"_variable");
    category_map.at(cat_name).binning_1d = cfg.get<vector<float>>(cat_name+"_binning_1d");
    category_map.at(cat_name).binning_2d_x = cfg.get<vector<float>>(cat_name+"_binning_2d_x");
    category_map.at(cat_name).binning_2d_y = cfg.get<vector<float>>(cat_name+"_binning_2d_y");
  }

  double min_percentile = 0.01;
  double max_percentile = 0.99;
  vector<double> percentile_ranges; // = { 0.01 , 0.10 , 0.20 , 0.30 , 0.40 , 0.50 , 0.60 , 0.70 , 0.80 , 0.90 , 0.99 };
  for(int i=0; i<=10; i++) percentile_ranges.push_back(0.01+i*0.098);
  //************************************************************************************************
  // Define some common weights and cuts

  TString weight      = "xsec_lumi_weight*mcweight*puweight*effweight*trigger_filter_weight*";
  TString mt_cut    = "&& mTdileptonMET<60 ";
  TString cuts_kine = "&& pt_1>13 && pt_2>10 && TMath::Max(pt_1,pt_2)>24 && metFilters && trg_muonelectron && nbtag == 0";
  cuts_kine += mt_cut;

  TString cuts_iso_general    = "&& iso_1<0.15 && iso_2<0.2 && extraelec_veto<0.5 && extramuon_veto<0.5 ";
  TString cuts_iso_ss_general = "&& extraelec_veto<0.5 && extramuon_veto<0.5 ";

  //************************************************************************************************
  // Set cuts, weights and binning on category level (cuts and weights will be later changed on individual basis)

  TString cuts_category_specific = "&& dzeta>-35";
  for(auto &cat : category_map){
    cat.second.cutstring    = cuts_kine + cuts_iso_general + cuts_category_specific;
    cat.second.bins_x_2d    = {0,50,55,60,65,70,75,80,85,90,95,100,400};
    cat.second.bins_y_2d    = {15,20,25,30,35,40,300};
  }

  //************************************************************************************************
  // Define the set of samples (used in each of the categories)

  Sample Data("data_obs");
  Sample ZTT("ZTT");
  Sample ZL("ZL");
  Sample W("W");
  Sample TT("TT");
  Sample TTcont("TTcont");
  Sample VV("VV");
  Sample VVcont("VVcont");
  Sample ST("ST");
  Sample QCD("QCD");
  Sample EMB("EMB");
  Sample ggH("ggH125");
  Sample qqH("qqH125");

  if(!is_dnn_prediction){
    
    Data.filename   = "NOMINAL_ntuple_MuonEG_em.root";
    ZTT.filename    = "NOMINAL_ntuple_DYJets_em.root" ;
    ZL.filename     = "NOMINAL_ntuple_DYJets_em.root" ;
    W.filename      = "NOMINAL_ntuple_WJets_em.root" ;
    TT.filename     = "NOMINAL_ntuple_TTbar_em.root" ;
    TTcont.filename = "NOMINAL_ntuple_TTbar_em.root" ;
    VV.filename     = "NOMINAL_ntuple_Diboson_em.root" ;
    VVcont.filename = "NOMINAL_ntuple_Diboson_em.root" ;
    ST.filename     = "NOMINAL_ntuple_SingleTop_em.root" ;
    QCD.filename    = "NOMINAL_ntuple_MuonEG_em.root" ;
    EMB.filename    = "NOMINAL_ntuple_Embedded_em.root" ;
    ggH.filename    = "NOMINAL_ntuple_ggH_em.root" ;
    qqH.filename    = "NOMINAL_ntuple_VBFH_em.root" ;
  }
  else{
    Data.filename   = "em-NOMINAL_ntuple_Data.root" ;
    ZTT.filename    = "em-NOMINAL_ntuple_ZTT.root" ;
    ZL.filename     = "em-NOMINAL_ntuple_ZL.root" ;
    W.filename      = "em-NOMINAL_ntuple_W.root" ;
    TT.filename     = "em-NOMINAL_ntuple_TT.root" ;
    TTcont.filename = "em-NOMINAL_ntuple_TT.root" ;
    VV.filename     = "em-NOMINAL_ntuple_Diboson.root" ;
    VVcont.filename = "em-NOMINAL_ntuple_Diboson.root" ;
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
				     { "4_TT"   , TT } ,
				     { "5_TTcont", TTcont } ,
				     { "6_VV"   , VV } ,
				     { "7_VVcont", VVcont } ,
				     { "8_ST"   , ST } ,
				     { "9_ZTT"  , ZTT } ,
				     { "10_ggH125" , ggH },
				     { "11_qqH125" , qqH }
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
    cat.second.sample_list["2_ZL"].cutString       += "&&!isZTT";
    cat.second.sample_list["2_ZL"].cutStringSS     += "&&!isZTT";
    cat.second.sample_list["2_ZL"].weightString    += "zptmassweight*";
    cat.second.sample_list["2_ZL"].weightStringSS  += "zptmassweight*";
    cat.second.sample_list["4_TT"].weightString    += "topptweight*prefiring_weight*";
    cat.second.sample_list["4_TT"].weightStringSS  += "topptweight*prefiring_weight*";
    cat.second.sample_list["5_TTcont"].weightString    += "topptweight*prefiring_weight*";
    cat.second.sample_list["5_TTcont"].weightStringSS  += "topptweight*prefiring_weight*";
    cat.second.sample_list["10_ggH125"].weightString    += "weight_ggh_NNLOPS*";
    cat.second.sample_list["10_ggH125"].weightStringSS  += "weight_ggh_NNLOPS*";
    cat.second.sample_list["11_qqH125"].weightString    += "prefiring_weight*";
    cat.second.sample_list["11_qqH125"].weightStringSS  += "prefiring_weight*";

    if(use_embedded){
      cat.second.sample_list["9_EMB"].weightString   = "mcweight*effweight*embeddedWeight*embedded_stitching_weight*embedded_rate_weight*";
      cat.second.sample_list["9_EMB"].weightStringSS = "mcweight*effweight*embeddedWeight*embedded_stitching_weight*embedded_rate_weight*qcdweight*";
      cat.second.sample_list["9_EMB"].cutString   += "&& mcweight<1";
      cat.second.sample_list["9_EMB"].cutStringSS += "&& mcweight<1";
      cat.second.sample_list["4_TT"].cutString    += "&& veto_embedded<0.5";
      cat.second.sample_list["4_TT"].cutStringSS  += "&& veto_embedded<0.5";
      cat.second.sample_list["6_VV"].cutString    += "&& veto_embedded<0.5";
      cat.second.sample_list["6_VV"].cutStringSS  += "&& veto_embedded<0.5";
      cat.second.sample_list["5_TTcont"].cutString    += "&& veto_embedded>0.5";
      cat.second.sample_list["5_TTcont"].cutStringSS  += "&& veto_embedded>0.5";
      cat.second.sample_list["7_VVcont"].cutString    += "&& veto_embedded>0.5";
      cat.second.sample_list["7_VVcont"].cutStringSS  += "&& veto_embedded>0.5";
    }
    else{
      cat.second.sample_list["9_ZTT"].cutString      += "&&isZTT";
      cat.second.sample_list["9_ZTT"].cutStringSS    += "&&isZTT";
      cat.second.sample_list["9_ZTT"].weightString   += "zptmassweight*";
      cat.second.sample_list["9_ZTT"].weightStringSS += "zptmassweight*";
    }
  }

  //************************************************************************************************
  // Define systematic uncertainties
    
  // Open one tree to test existence of uncertainty shifts
  TFile *file_ = new TFile( directory + "/" + Data.filename );
  TTree *tree_ = (TTree*) file_->Get("TauCheck");

  for(auto& cat : category_map){
    for(auto & smpl : cat.second.sample_list){
      if( smpl.second.name == "data_obs" || smpl.first.Contains("cont") ) continue;

      // 1.) QCD uncertainties (10 nuisances)
      smpl.second = create_systematic_uncertainty("qcd0jetRateUp"  , "_CMS_htt_qcd_0jet_rate_Run" +era+ "Up"  , cat.second.plot_2d, smpl.second, tree_, false, "", true, "qcdweight*","qcdweight_0jet_rate_up*");
      smpl.second = create_systematic_uncertainty("qcd0jetRateDown", "_CMS_htt_qcd_0jet_rate_Run" +era+ "Down", cat.second.plot_2d, smpl.second, tree_, false, "", true, "qcdweight*","qcdweight_0jet_rate_down*");
      // smpl.second = create_systematic_uncertainty("qcd1jetRateUp"  , "_CMS_htt_qcd_1jet_rate_Run" +era+ "Up"  , cat.second.plot_2d, smpl.second, tree_, false, "", true, "qcdweight*","qcdweight_1jet_rate_up*");
      // smpl.second = create_systematic_uncertainty("qcd1jetRateDown", "_CMS_htt_qcd_1jet_rate_Run" +era+ "Down", cat.second.plot_2d, smpl.second, tree_, false, "", true, "qcdweight*","qcdweight_1jet_rate_down*");
      smpl.second = create_systematic_uncertainty("qcd0jetShapeUp"  , "_CMS_htt_qcd_0jet_shape_Run" +era+ "Up"  , cat.second.plot_2d, smpl.second, tree_, false, "", true, "qcdweight*","qcdweight_0jet_shape_up*");
      smpl.second = create_systematic_uncertainty("qcd0jetShapeDown", "_CMS_htt_qcd_0jet_shape_Run" +era+ "Down", cat.second.plot_2d, smpl.second, tree_, false, "", true, "qcdweight*","qcdweight_0jet_shape_down*");
      smpl.second = create_systematic_uncertainty("qcd1jetShapeUp"  , "_CMS_htt_qcd_1jet_shape_Run" +era+ "Up"  , cat.second.plot_2d, smpl.second, tree_, false, "", true, "qcdweight*","qcdweight_1jet_shape_up*");
      smpl.second = create_systematic_uncertainty("qcd1jetShapeDown", "_CMS_htt_qcd_1jet_shape_Run" +era+ "Down", cat.second.plot_2d, smpl.second, tree_, false, "", true, "qcdweight*","qcdweight_1jet_shape_down*");
      smpl.second = create_systematic_uncertainty("qcdIsoUp"  , "_CMS_htt_qcd_iso_Run" +era+ "Up"  , cat.second.plot_2d, smpl.second, tree_, false, "", true, "qcdweight*","qcdweight_iso_up*");
      smpl.second = create_systematic_uncertainty("qcdIsoDown", "_CMS_htt_qcd_iso_Run" +era+ "Down"  , cat.second.plot_2d, smpl.second, tree_, false, "", true, "qcdweight*","qcdweight_iso_down*");

      if( smpl.second.name == "QCD"  ) continue;

      // 2.) Electron scale (applied on both embedded and mc)
      smpl.second = create_systematic_uncertainty("escale_all_Up"  , "_CMS_scale_e_Run" +era+ "Up"  , cat.second.plot_2d, smpl.second, tree_, true, "escaleUp");
      smpl.second = create_systematic_uncertainty("escale_all_Down", "_CMS_scale_e_Run" +era+ "Down", cat.second.plot_2d, smpl.second, tree_, true, "escaleDown");
      // 3.) Electron scale (applied only on embedded)
      if(smpl.second.name == "EMB"){
	smpl.second = create_systematic_uncertainty("escale_emb_Up"  , "_CMS_scale_emb_e_Run" +era+ "Up"  , cat.second.plot_2d, smpl.second, tree_, true, "escaleUp");
	smpl.second = create_systematic_uncertainty("escale_emb_Down", "_CMS_scale_emb_e_Run" +era+ "Down", cat.second.plot_2d, smpl.second, tree_, true, "escaleDown");
      }
      // 4.) Electron scale (applied only on MC)
      if(smpl.second.name != "EMB"){
	smpl.second = create_systematic_uncertainty("escale_mc_Up"  , "_CMS_scale_mc_e_Run" +era+ "Up"  , cat.second.plot_2d, smpl.second, tree_, true, "escaleUp");
	smpl.second = create_systematic_uncertainty("escale_mc_Down", "_CMS_scale_mc_e_Run" +era+ "Down", cat.second.plot_2d, smpl.second, tree_, true, "escaleDown");
      }

      // 5.) TT contamination in embedded sample  (only initialize this uncertainty -> calculate it later)
      if(smpl.second.name == "EMB"){
	smpl.second = create_systematic_uncertainty("ttContEmbUp"  , "_CMS_htt_emb_ttbar_Run" +era+ "Up"  , cat.second.plot_2d, smpl.second, tree_, false, "", false, "","");
	smpl.second = create_systematic_uncertainty("ttContEmbDown", "_CMS_htt_emb_ttbar_Run" +era+ "Down", cat.second.plot_2d, smpl.second, tree_, false, "", false, "","");
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

      if(smpl.second.name == "ZTT" || smpl.second.name == "ZL" || smpl.second.name == "W" || smpl.second.name == "ggH125" || smpl.second.name == "qqH125"){
      // 9.) Recoil scale/resolution uncertainties
	smpl.second = create_systematic_uncertainty("recoilscaleUp"  , "_CMS_htt_boson_scale_met_Run" +era+ "Up"  , cat.second.plot_2d, smpl.second, tree_, true, "recoilscaleUp");
	smpl.second = create_systematic_uncertainty("recoilscaleDown", "_CMS_htt_boson_scale_met_Run" +era+ "Down", cat.second.plot_2d, smpl.second, tree_, true, "recoilscaleDown");
	smpl.second = create_systematic_uncertainty("recoilresoUp"  , "_CMS_htt_boson_reso_met_Run" +era+ "Up"  , cat.second.plot_2d, smpl.second, tree_, true, "recoilresoUp");
	smpl.second = create_systematic_uncertainty("recoilresoDown", "_CMS_htt_boson_reso_met_Run" +era+ "Down", cat.second.plot_2d, smpl.second, tree_, true, "recoilresoDown");
	// 8.) Unclustered MET scale
	smpl.second = create_systematic_uncertainty("unclMetUp"  , "_CMS_scale_met_unclustered_Run" +era+ "Up"  , cat.second.plot_2d, smpl.second, tree_, true, "");
	smpl.second = create_systematic_uncertainty("unclMetDown", "_CMS_scale_met_unclustered_Run" +era+ "Down", cat.second.plot_2d, smpl.second, tree_, true, "");
      }
      else{
      // 8.) Unclustered MET scale
      smpl.second = create_systematic_uncertainty("unclMetUp"  , "_CMS_scale_met_unclustered_Run" +era+ "Up"  , cat.second.plot_2d, smpl.second, tree_, true, "unclMetUp");
      smpl.second = create_systematic_uncertainty("unclMetDown", "_CMS_scale_met_unclustered_Run" +era+ "Down", cat.second.plot_2d, smpl.second, tree_, true, "unclMetDown");
      }

      // 10.) TTbar shape
      if(smpl.second.name == "TT"){
	smpl.second = create_systematic_uncertainty("ttbarShapeUp"  , "_CMS_htt_ttbarShape_Run" +era+ "Up"  , cat.second.plot_2d, smpl.second, tree_, false, "", true, "topptweight*","topptweight*topptweight*");
	smpl.second = create_systematic_uncertainty("ttbarShapeDown", "_CMS_htt_ttbarShape_Run" +era+ "Down", cat.second.plot_2d, smpl.second, tree_, false, "", true, "topptweight*","");
      }

      // 11.) DY shape
      if(smpl.second.name == "ZTT" || smpl.second.name == "ZL" ){
	smpl.second = create_systematic_uncertainty("dyShapeUp"  , "_CMS_htt_dyShape_Run" +era+ "Up"  , cat.second.plot_2d, smpl.second, tree_, false, "", true, "zptmassweight*","(1.0+1.1*(zptmassweight-1))*");
	smpl.second = create_systematic_uncertainty("dyShapeDown", "_CMS_htt_dyShape_Run" +era+ "Down", cat.second.plot_2d, smpl.second, tree_, false, "", true, "zptmassweight*","(1.0+0.9*(zptmassweight-1))*");
      }

      // 12.) ggh reweighting
      if(smpl.second.name == "ggH125" ){
	smpl.second = create_systematic_uncertainty("gghShapeResUp"  , "_THU_ggH_Res_Run" +era+ "Up"  , cat.second.plot_2d, smpl.second, tree_, false, "", true, "weight_ggh_NNLOPS*", "weight_ggh_NNLOPS*THU_ggH_Res*");
	smpl.second = create_systematic_uncertainty("gghShapeResDown", "_THU_ggH_Res_Run" +era+ "Down", cat.second.plot_2d, smpl.second, tree_, false, "", true, "weight_ggh_NNLOPS*", "weight_ggh_NNLOPS/THU_ggH_Res*");
	smpl.second = create_systematic_uncertainty("gghShapeMig01Up"  , "_THU_ggH_Mig01_Run" +era+ "Up"  , cat.second.plot_2d, smpl.second, tree_, false, "", true, "weight_ggh_NNLOPS*", "weight_ggh_NNLOPS*THU_ggH_Mig01*");
	smpl.second = create_systematic_uncertainty("gghShapeMig01Down", "_THU_ggH_Mig01_Run" +era+ "Down", cat.second.plot_2d, smpl.second, tree_, false, "", true, "weight_ggh_NNLOPS*", "weight_ggh_NNLOPS/THU_ggH_Mig01*");
	smpl.second = create_systematic_uncertainty("gghShapeMig12Up"  , "_THU_ggH_Mig12_Run" +era+ "Up"  , cat.second.plot_2d, smpl.second, tree_, false, "", true, "weight_ggh_NNLOPS*", "weight_ggh_NNLOPS*THU_ggH_Mig12*");
	smpl.second = create_systematic_uncertainty("gghShapeMig12Down", "_THU_ggH_Mig12_Run" +era+ "Down", cat.second.plot_2d, smpl.second, tree_, false, "", true, "weight_ggh_NNLOPS*", "weight_ggh_NNLOPS/THU_ggH_Mig12*");
	smpl.second = create_systematic_uncertainty("gghShapeVBF2jUp"  , "_THU_ggH_VBF2j_Run" +era+ "Up"  , cat.second.plot_2d, smpl.second, tree_, false, "", true, "weight_ggh_NNLOPS*", "weight_ggh_NNLOPS*THU_ggH_VBF2j*");
	smpl.second = create_systematic_uncertainty("gghShapeVBF2jDown", "_THU_ggH_VBF2j_Run" +era+ "Down", cat.second.plot_2d, smpl.second, tree_, false, "", true, "weight_ggh_NNLOPS*", "weight_ggh_NNLOPS/THU_ggH_VBF2j*");
	smpl.second = create_systematic_uncertainty("gghShapeVBF3jUp"  , "_THU_ggH_VBF3j_Run" +era+ "Up"  , cat.second.plot_2d, smpl.second, tree_, false, "", true, "weight_ggh_NNLOPS*", "weight_ggh_NNLOPS*THU_ggH_VBF3j*");
	smpl.second = create_systematic_uncertainty("gghShapeVBF3jDown", "_THU_ggH_VBF3j_Run" +era+ "Down", cat.second.plot_2d, smpl.second, tree_, false, "", true, "weight_ggh_NNLOPS*", "weight_ggh_NNLOPS/THU_ggH_VBF3j*");
	smpl.second = create_systematic_uncertainty("gghShapePT60Up"  , "_THU_ggH_PT60_Run" +era+ "Up"  , cat.second.plot_2d, smpl.second, tree_, false, "", true, "weight_ggh_NNLOPS*", "weight_ggh_NNLOPS*THU_ggH_PT60*");
	smpl.second = create_systematic_uncertainty("gghShapePT60Down", "_THU_ggH_PT60_Run" +era+ "Down", cat.second.plot_2d, smpl.second, tree_, false, "", true, "weight_ggh_NNLOPS*", "weight_ggh_NNLOPS/THU_ggH_PT60*");
	smpl.second = create_systematic_uncertainty("gghShapePT120Up"  , "_THU_ggH_PT120_Run" +era+ "Up"  , cat.second.plot_2d, smpl.second, tree_, false, "", true, "weight_ggh_NNLOPS*", "weight_ggh_NNLOPS*THU_ggH_PT120*");
	smpl.second = create_systematic_uncertainty("gghShapePT120Down", "_THU_ggH_PT120_Run" +era+ "Down", cat.second.plot_2d, smpl.second, tree_, false, "", true, "weight_ggh_NNLOPS*", "weight_ggh_NNLOPS/THU_ggH_PT120*");
	smpl.second = create_systematic_uncertainty("gghShapeqmtopUp"  , "_THU_ggH_qmtop_Run" +era+ "Up"  , cat.second.plot_2d, smpl.second, tree_, false, "", true, "weight_ggh_NNLOPS*", "weight_ggh_NNLOPS*THU_ggH_qmtop*");
	smpl.second = create_systematic_uncertainty("gghShapeqmtopDown", "_THU_ggH_qmtop_Run" +era+ "Down", cat.second.plot_2d, smpl.second, tree_, false, "", true, "weight_ggh_NNLOPS*", "weight_ggh_NNLOPS/THU_ggH_qmtop*");
	smpl.second = create_systematic_uncertainty("gghShapeMuUp"  , "_THU_ggH_Mu_Run" +era+ "Up"  , cat.second.plot_2d, smpl.second, tree_, false, "", true, "weight_ggh_NNLOPS*", "weight_ggh_NNLOPS*THU_ggH_Mu*");
	smpl.second = create_systematic_uncertainty("gghShapeMuDown", "_THU_ggH_Mu_Run" +era+ "Down", cat.second.plot_2d, smpl.second, tree_, false, "", true, "weight_ggh_NNLOPS*", "weight_ggh_NNLOPS/THU_ggH_Mu*");
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
  TString filename = "htt_em.inputs-sm-Run"+era+"-"+output_file_suffix+".root";
  TString output_dir = "output/"+era+"/";
  if(!is_dnn_prediction){
    if(!plot_2d) output_dir += "/var_1d/";
    else         output_dir += "/var_2d/";
  }
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
      // Get the 1st and 99th percentiles
      if( smpl.second.name == "data_obs" && take_percentile_subrange ){

	if(!cat.second.plot_2d){
	  cat.second.binning_1d.clear();
	  cout<<endl;
	  float min_val = tree->GetMinimum(cat.second.variable);
	  float max_val = tree->GetMaximum(cat.second.variable);
	  if(min_val == -10) min_val=0;
	  cout<<endl<<"Minimum value in tree = "<<min_val<<endl;
	  cout<<"Maximum value in tree = "<<max_val<<endl<<endl;

	  TH1D* hist_aux = new TH1D("hist_aux", "", 1000000, min_val, max_val);
	  tree -> Draw( cat.second.variable + ">> hist_aux" , "1*("+cat.second.variable+Form(">%f",min_val)+ cat.second.cutstring + ")" );
	  unsigned int i_aux=0;
	  int entries   = hist_aux->GetSumOfWeights();
	  int nbins_aux = hist_aux->GetNbinsX()+1;
	  int count =0;
	  for(int ibin=1; ibin<nbins_aux+1; ibin++){
	    count += hist_aux->GetBinContent(ibin);
	    if(count>=percentile_ranges[i_aux]*entries){
	      cat.second.binning_1d.push_back(hist_aux->GetBinCenter(ibin));
	      cout<<"bin range starts at = "<<hist_aux->GetBinCenter(ibin)<<endl;
	      if(i_aux+1 == percentile_ranges.size()) break;
	      i_aux +=1;
	    }
	  }
	}
	else{
	  cat.second.binning_2d_x.clear();
	  cat.second.binning_2d_y.clear();
	  TString var_y = cat.second.variable( 0 , cat.second.variable.First(":") );
	  TString var_x = cat.second.variable( cat.second.variable.First(":")+1 , cat.second.variable.Length());
	  float min_val_x = tree->GetMinimum(var_x);
	  float max_val_x = tree->GetMaximum(var_x);
	  float min_val_y = tree->GetMinimum(var_y);
	  float max_val_y = tree->GetMaximum(var_y);
	  if(min_val_x == -10) min_val_x=0;
	  if(min_val_y == -10) min_val_y=0;
	  cout<<endl<<"Minimum value of "<<var_x<<" in tree = "<<min_val_x<<endl;
	  cout<<"Maximum value of "<<var_x<<" in tree = "<<max_val_x<<endl;
	  cout<<"Minimum value of "<<var_y<<" in tree = "<<min_val_y<<endl;
	  cout<<"Maximum value of "<<var_y<<" in tree = "<<max_val_y<<endl<<endl;

	  TH1D* hist_aux_x = new TH1D("hist_aux_x", "", 1000000, min_val_x, max_val_x);
	  TH1D* hist_aux_y = new TH1D("hist_aux_y", "", 1000000, min_val_y, max_val_y);
	  tree -> Draw( var_x + ">> hist_aux_x" , "1*("+cat.second.variable+Form(">%f",min_val_x)+ cat.second.cutstring + ")" );
	  tree -> Draw( var_y + ">> hist_aux_y" , "1*("+cat.second.variable+Form(">%f",min_val_y)+ cat.second.cutstring + ")" );

	  unsigned int idx_bins = 0;
	  int count_events =0;
	  for(int ibin=1; ibin<hist_aux_x->GetNbinsX()+1; ibin++){
	    count_events += hist_aux_x->GetBinContent(ibin);
	    if(count_events >= percentile_ranges[idx_bins]*hist_aux_x->GetEntries()){
	      cat.second.binning_2d_x.push_back(hist_aux_x->GetBinCenter(ibin));
	      cout<<"bin range starts at = "<<hist_aux_x->GetBinCenter(ibin)<<endl;
	      if(idx_bins+1 == percentile_ranges.size()) break;
	      idx_bins +=1;
	    }
	  }
	  idx_bins = 0;
	  count_events = 0;
	  for(int ibin=1; ibin<hist_aux_y->GetNbinsX()+1; ibin++){
	    count_events += hist_aux_y->GetBinContent(ibin);
	    if(count_events >= percentile_ranges[idx_bins]*hist_aux_y->GetEntries()){
	      cat.second.binning_2d_y.push_back(hist_aux_y->GetBinCenter(ibin));
	      cout<<"bin range starts at = "<<hist_aux_y->GetBinCenter(ibin)<<endl;
	      if(idx_bins+1 == percentile_ranges.size()) break;
	      idx_bins +=1;
	    }
	  }


	}
      } // end of take_percentile_subrange

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
      if( smpl.second.name == "QCD" ) cat.second.sample_list["1_QCD"].hist_1d -> Add(smpl.second.histSS_1d , +1);
      else if( smpl.second.name != "ggH125" && smpl.second.name != "qqH125" && smpl.second.name != "data_obs" && smpl.second.name != "TTcont" && smpl.second.name != "VVcont" ) cat.second.sample_list["1_QCD"].hist_1d -> Add(smpl.second.histSS_1d , -1);

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

	if( smpl.second.name == "QCD" ) cat.second.sample_list["1_QCD"].uncertainties[sys.first].hist_1d -> Add(sys.second.histSS_1d , +1);
	else if( smpl.second.name != "ggH125" && smpl.second.name != "qqH125" && smpl.second.name != "data_obs" && smpl.second.name != "TTcont" && sys.second.name != "VVcont" ) cat.second.sample_list["1_QCD"].uncertainties[sys.first].hist_1d -> Add(sys.second.histSS_1d , -1);;

      } // end of loop over sys uncertainties
    } // end of loop over samples

    // Calculate embedded uncertainty
    if(use_embedded){
      cat.second.sample_list["9_EMB"].uncertainties["ttContEmbUp"].hist_1d   -> Add(cat.second.sample_list["5_TTcont"].hist_1d , 0.1);
      cat.second.sample_list["9_EMB"].uncertainties["ttContEmbDown"].hist_1d -> Add(cat.second.sample_list["5_TTcont"].hist_1d , -0.1);
      cat.second.sample_list["9_EMB"].uncertainties["ttContEmbUp"].hist_1d   -> Add(cat.second.sample_list["7_VVcont"].hist_1d , 0.1);
      cat.second.sample_list["9_EMB"].uncertainties["ttContEmbDown"].hist_1d -> Add(cat.second.sample_list["7_VVcont"].hist_1d , -0.1);
    }

    //***********************************************************************************************
    // Write all histograms to output file
    cout << endl << endl << "... Writing histograms to output file ... " << endl;

    file_out -> mkdir(cat.second.name);
    file_out -> cd(cat.second.name);
    for(auto & smpl : cat.second.sample_list){
      if( smpl.first.Contains("cont") ) continue;
      smpl.second.hist_1d -> Write( smpl.second.name );
      for(auto & sys : smpl.second.uncertainties){
	if(sys.second.name.Contains("qcd") && smpl.second.name != "QCD") continue;
	sys.second.hist_1d -> Write( sys.second.name );
      }
    }

    // Write the final sum of weights of the nominal selection
    cout << endl << "... Final histogram content : "<< endl;
    TH1D * allBkg = (TH1D*) cat.second.sample_list["1_QCD"].hist_1d -> Clone();
    for(auto & smpl : cat.second.sample_list) {
      cout << smpl.second.name << " : " << smpl.second.hist_1d -> GetSumOfWeights() << endl;
      if( smpl.second.name == "data_obs" || smpl.second.name == "QCD" ) continue;
      allBkg->Add(smpl.second.hist_1d);
    }
    cout << "Bkg together : " << allBkg -> GetSumOfWeights() << endl << endl;
  } // end of loop over categories
  cout << "output file : " << output_dir + "/" + filename << endl << endl;
  file_out -> Close();
}
