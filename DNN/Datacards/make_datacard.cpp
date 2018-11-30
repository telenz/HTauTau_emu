#include "../GoF/useful_classes.h"
#include "../GoF/systematic_uncertainties.h"
#include "../GoF/HttStylesNew.cc"
#include "TROOT.h"

void make_datacard( TString variable_1d = "predicted_prob" ,
		    int nbins = 40,
		    vector<float> range = {0.2,1} ,
		    TString directory = "/nfs/dust/cms/user/tlenz/13TeV/2017/SM_HTauTau/HTauTau_emu/DNN/mareikesdnnsetup/mlFramework/predictions/",
		    TString era = "2016",
		    bool use_embedded = false) {

  gROOT->SetBatch(kTRUE);
  SetStyle();

  bool verbose = false;
  bool plot_2d = false; // this should be set always on false for this script!
  //************************************************************************************************
  // Define some common weights and cuts

  TString weight      = "xsec_lumi_weight*mcweight*puweight*effweight*trigger_filter_weight*";

  TString mt_cut    = "&& mTdileptonMET<60 ";
  TString cuts_kine = "&& pt_1>13 && pt_2>10 && TMath::Max(pt_1,pt_2)>24 && metFilters && trg_muonelectron && nbtag==0";
  cuts_kine += mt_cut;

  TString cuts_iso_general    = "&& iso_1<0.15 && iso_2<0.2 && extraelec_veto<0.5 && extramuon_veto<0.5 ";
  TString cuts_iso_ss_general = "&& extraelec_veto<0.5 && extramuon_veto<0.5 ";

  //************************************************************************************************
  // Define different categories (use of category "em_cat_in_use" which is set below the definition)
  // For now the used attributes of the category class are only the binning and the name definitions

  // The categories are filled with the predicted_classes
  class Category em_ztt("em_ztt");
  class Category em_vv("em_vv");
  class Category em_ss("em_ss");
  class Category em_tt("em_tt");
  class Category em_misc("em_misc");
  class Category em_st("em_st");
  class Category em_qqh("em_qqh");
  class Category em_ggh("em_ggh");

  em_ztt.class_nr   = 5;
  em_vv.class_nr    = 0;
  em_ss.class_nr    = 1;
  em_tt.class_nr    = 2;
  em_misc.class_nr  = 3;
  em_st.class_nr    = 6;
  em_qqh.class_nr   = 4;
  em_ggh.class_nr   = 7;

  // ZTT category
  TString cuts_category_specific = "&& dzeta>-35";
  em_ztt.cutstring    = cuts_kine + cuts_iso_general + cuts_category_specific;
  em_ztt.variable_1d  = variable_1d;
  em_ztt.qcdweight    = "qcdweight*";

  // Make a map from these categories
  map< TString , Category > category_map = { { em_ztt.name     , em_ztt },
					     { em_vv.name      , em_vv },
					     { em_ss.name      , em_ss },
					     { em_tt.name      , em_tt },
					     { em_misc.name    , em_misc },
					     { em_st.name      , em_st },
					     { em_qqh.name     , em_qqh },
					     { em_ggh.name     , em_ggh }};

  //************************************************************************************************
  // Define samples

  Sample Data( "data_obs" , "em-NOMINAL_ntuple_Data.root" );
  Sample ZTT(  "ZTT"      , "em-NOMINAL_ntuple_ZTT.root" );
  Sample ZL(   "ZL"       , "em-NOMINAL_ntuple_ZL.root" );
  Sample W(    "W"        , "em-NOMINAL_ntuple_W.root" );
  Sample TT(   "TT"       , "em-NOMINAL_ntuple_TT.root" );
  Sample TTcont( "TTcont" , "em-NOMINAL_ntuple_TT.root" );
  Sample VV(   "VV"       , "em-NOMINAL_ntuple_Diboson.root" );
  Sample VVcont( "VVcont" , "em-NOMINAL_ntuple_Diboson_em.root" );
  Sample ST(   "ST"       , "em-NOMINAL_ntuple_SingleTop.root" );
  Sample QCD(  "QCD"      , "em-NOMINAL_ntuple_Data.root" );
  Sample EMB(  "EMB"      , "em-NOMINAL_ntuple_ZTT.root" );
  Sample ggH(  "ggH125"   , "em-NOMINAL_ntuple_ggH125.root" );
  Sample qqH(  "qqH125"   , "em-NOMINAL_ntuple_qqH125.root" );

  map<TString,Sample> sample_map = { { "0_Data" , Data },
				     { "1_QCD"  , QCD } ,
				     { "2_ZL"   , ZL } ,
				     { "3_W"    , W } ,
				     { "4_TT"   , TT } ,
				     { "5_TTcont", TTcont } ,
				     { "6_VV"   , VV } ,
				     { "7_VVcont"   , VVcont } ,
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

  // Define common cut strings  
  for(auto & smpl : sample_map){
    smpl.second.qcdweight          = "qcdweight*";
    smpl.second.cutString          = "q_1*q_2<0" + em_ztt.cutstring;
    smpl.second.weightString       = weight;
    smpl.second.cutStringSS        = "q_1*q_2>0" + em_ztt.cutstring;
    smpl.second.weightStringSS     = weight;
    smpl.second.variable_1d = em_ztt.variable_1d;
    smpl.second.variable_2d = em_ztt.variable_2d;
  }

  // Define sample specific cutStrings and weightStrings
  sample_map["0_Data"].weightString   = "1*";
  sample_map["0_Data"].weightStringSS = "1*";
  sample_map["1_QCD"].weightString    = "1*";
  sample_map["1_QCD"].weightStringSS  = "1*";
  sample_map["1_QCD"].cutString       = "1==2";  // don't fill anything in this histogram should remain empty
  sample_map["2_ZL"].cutString       += "&&!isZTT";
  sample_map["2_ZL"].cutStringSS     += "&&!isZTT";
  sample_map["2_ZL"].weightString    += "zptmassweight*";
  sample_map["2_ZL"].weightStringSS  += "zptmassweight*";
  sample_map["4_TT"].weightString    += "topptweight*";
  sample_map["4_TT"].weightStringSS  += "topptweight*";
  sample_map["5_TTcont"].weightString    += "topptweight*";
  sample_map["5_TTcont"].weightStringSS  += "topptweight*";

  if(use_embedded){
    sample_map["9_EMB"].weightString   = "mcweight*effweight*embeddedWeight*embedded_stitching_weight*embedded_rate_weight*";
    sample_map["9_EMB"].weightStringSS = "mcweight*effweight*embeddedWeight*embedded_stitching_weight*embedded_rate_weight*";
    sample_map["9_EMB"].cutString   += "&& mcweight<1";
    sample_map["9_EMB"].cutStringSS += "&& mcweight<1";
    sample_map["4_TT"].cutString    += "&& veto_embedded<0.5";
    sample_map["4_TT"].cutStringSS  += "&& veto_embedded<0.5";
    sample_map["6_VV"].cutString    += "&& veto_embedded<0.5";
    sample_map["6_VV"].cutStringSS  += "&& veto_embedded<0.5";
    sample_map["5_TTcont"].cutString    += "&& veto_embedded>0.5";
    sample_map["5_TTcont"].cutStringSS  += "&& veto_embedded>0.5";
    sample_map["7_VVcont"].cutString    += "&& veto_embedded>0.5";
    sample_map["7_VVcont"].cutStringSS  += "&& veto_embedded>0.5";

  }
  else{
    sample_map["9_ZTT"].cutString      += "&&isZTT";
    sample_map["9_ZTT"].cutStringSS    += "&&isZTT";
    sample_map["9_ZTT"].weightString   += "zptmassweight*";
    sample_map["9_ZTT"].weightStringSS += "zptmassweight*";
  }

  //************************************************************************************************
  // Define systematic uncertainties

  // Open one tree to test existence of uncertainty shifts
  TFile *file_ = new TFile( directory + "/" + Data.filename );
  TTree *tree_ = (TTree*) file_->Get("TauCheck");

  for(auto & smpl : sample_map){

    if( smpl.second.name == "data_obs" || smpl.first.Contains("cont")) continue;

    // 1.) QCD uncertainties (10 nuisances)
    // smpl.second = create_systematic_uncertainty("qcd0jetRateUp"  , "_CMS_htt_qcd_0jet_rate_Run" +era+ "Up"  , plot_2d, smpl.second, tree_, false, "", true, "qcdweight*","qcdweight_0jet_rate_up*");
    // smpl.second = create_systematic_uncertainty("qcd0jetRateDown", "_CMS_htt_qcd_0jet_rate_Run" +era+ "Down", plot_2d, smpl.second, tree_, false, "", true, "qcdweight*","qcdweight_0jet_rate_down*");
    // smpl.second = create_systematic_uncertainty("qcd1jetRateUp"  , "_CMS_htt_qcd_1jet_rate_Run" +era+ "Up"  , plot_2d, smpl.second, tree_, false, "", true, "qcdweight*","qcdweight_1jet_rate_up*");
    // smpl.second = create_systematic_uncertainty("qcd1jetRateDown", "_CMS_htt_qcd_1jet_rate_Run" +era+ "Down", plot_2d, smpl.second, tree_, false, "", true, "qcdweight*","qcdweight_1jet_rate_down*");
    // smpl.second = create_systematic_uncertainty("qcd0jetShapeUp"  , "_CMS_htt_qcd_0jet_shape_Run" +era+ "Up"  , plot_2d, smpl.second, tree_, false, "", true, "qcdweight*","qcdweight_0jet_shape_up*");
    // smpl.second = create_systematic_uncertainty("qcd0jetShapeDown", "_CMS_htt_qcd_0jet_shape_Run" +era+ "Down", plot_2d, smpl.second, tree_, false, "", true, "qcdweight*","qcdweight_0jet_shape_down*");
    // smpl.second = create_systematic_uncertainty("qcd1jetShapeUp"  , "_CMS_htt_qcd_1jet_shape_Run" +era+ "Up"  , plot_2d, smpl.second, tree_, false, "", true, "qcdweight*","qcdweight_1jet_shape_up*");
    // smpl.second = create_systematic_uncertainty("qcd1jetShapeDown", "_CMS_htt_qcd_1jet_shape_Run" +era+ "Down", plot_2d, smpl.second, tree_, false, "", true, "qcdweight*","qcdweight_1jet_shape_down*");
    // smpl.second = create_systematic_uncertainty("qcdIsoUp"  , "_CMS_htt_qcd_iso_Run" +era+ "Up"  , plot_2d, smpl.second, tree_, false, "", true, "qcdweight*","qcdweight_iso_up*");
    // smpl.second = create_systematic_uncertainty("qcdIsoDown", "_CMS_htt_qcd_iso_Run" +era+ "Down"  , plot_2d, smpl.second, tree_, false, "", true, "qcdweight*","qcdweight_iso_down*");

    if( smpl.second.name == "QCD"  ) continue;

    // 2.) Electron scale (applied on both embedded and mc)
    smpl.second = create_systematic_uncertainty("escale_all_Up"  , "_CMS_scale_e_Run" +era+ "Up"  , plot_2d, smpl.second, tree_, true, "escaleUp");
    smpl.second = create_systematic_uncertainty("escale_all_Down", "_CMS_scale_e_Run" +era+ "Down", plot_2d, smpl.second, tree_, true, "escaleDown");
    smpl.second.uncertainties["escale_all_Up"].filename.ReplaceAll("NOMINAL","_escaleUp");
    smpl.second.uncertainties["escale_all_Down"].filename.ReplaceAll("NOMINAL","_escaleDown");
    // 3.) Electron scale (applied only on embedded)
    if(smpl.second.name == "EMB"){
      smpl.second = create_systematic_uncertainty("escale_emb_Up"  , "_CMS_scale_emb_e_Run" +era+ "Up"  , plot_2d, smpl.second, tree_, true, "escaleUp");
      smpl.second = create_systematic_uncertainty("escale_emb_Down", "_CMS_scale_emb_e_Run" +era+ "Down", plot_2d, smpl.second, tree_, true, "escaleDown");
      smpl.second.uncertainties["escale_all_Up"].filename.ReplaceAll("NOMINAL","_escaleUp");
      smpl.second.uncertainties["escale_all_Down"].filename.ReplaceAll("NOMINAL","_escaleDown");
    }
    // 4.) Electron scale (applied only on MC)
    if(smpl.second.name != "EMB"){
      smpl.second = create_systematic_uncertainty("escale_mc_Up"  , "_CMS_scale_mc_e_Run" +era+ "Up"  , plot_2d, smpl.second, tree_, true, "escaleUp");
      smpl.second = create_systematic_uncertainty("escale_mc_Down", "_CMS_scale_mc_e_Run" +era+ "Down", plot_2d, smpl.second, tree_, true, "escaleDown");
      smpl.second.uncertainties["escale_all_Up"].filename.ReplaceAll("NOMINAL","_escaleUp");
      smpl.second.uncertainties["escale_all_Down"].filename.ReplaceAll("NOMINAL","_escaleDown");
    }

    // 11.) TT contamination in embedded sample  (only initialize this uncertainty -> calculate it later)
    if(smpl.second.name == "EMB"){
      smpl.second = create_systematic_uncertainty("ttContEmbUp"  , "_CMS_htt_emb_ttbar_Run" +era+ "Up"  , plot_2d, smpl.second, tree_, false, "", false, "","");
      smpl.second = create_systematic_uncertainty("ttContEmbDown", "_CMS_htt_emb_ttbar_Run" +era+ "Down", plot_2d, smpl.second, tree_, false, "", false, "","");
    }

    if(smpl.second.name == "EMB") continue;

    // 5.) (b-)mistag uncertainty
    // smpl.second = create_systematic_uncertainty("mistagUp"  , "_CMS_htt_mistag_b_Run" +era+ "Up"  , plot_2d, smpl.second, tree_, true, "mistagUp");
    // smpl.second = create_systematic_uncertainty("mistagDown", "_CMS_htt_mistag_b_Run" +era+ "Down", plot_2d, smpl.second, tree_, true, "mistagDown");
    // smpl.second = create_systematic_uncertainty("btagUp"  , "_CMS_htt_eff_b_Run" +era+ "Up"  , plot_2d, smpl.second, tree_, true, "btagUp");
    // smpl.second = create_systematic_uncertainty("btagDown", "_CMS_htt_eff_b_Run" +era+ "Down", plot_2d, smpl.second, tree_, true, "btagDown");

    // 6.) JES
    //old
    smpl.second = create_systematic_uncertainty("jesUp"  , "_CMS_scale_j_Run" +era+ "Up"  , plot_2d, smpl.second, tree_, true, "jesUp");
    smpl.second = create_systematic_uncertainty("jesDown", "_CMS_scale_j_Run" +era+ "Down", plot_2d, smpl.second, tree_, true, "jesDown");
    smpl.second.uncertainties["jesUp"].filename.ReplaceAll("NOMINAL","_jesUp");
    smpl.second.uncertainties["jesUp"].filename.ReplaceAll("NOMINAL","_jesDown");

    //new (to come)
    // smpl.second = create_systematic_uncertainty("jecUncEta0To5Up"  , "_CMS_scale_j_eta0to5_Run" +era+ "Up"  , plot_2d, smpl.second, tree_, true, "jecUncEta0To5Up");
    // smpl.second = create_systematic_uncertainty("jecUncEta0To5Down", "_CMS_scale_j_eta0to5_Run" +era+ "Down", plot_2d, smpl.second, tree_, true, "jecUncEta0To5Down");
    // smpl.second = create_systematic_uncertainty("jecUncEta0To3Up"  , "_CMS_scale_j_eta0to3_Run" +era+ "Up"  , plot_2d, smpl.second, tree_, true, "jecUncEta0To3Up");
    // smpl.second = create_systematic_uncertainty("jecUncEta0To3Down", "_CMS_scale_j_eta0to3_Run" +era+ "Down", plot_2d, smpl.second, tree_, true, "jecUncEta0To3Down");
    // smpl.second = create_systematic_uncertainty("jecUncEta3To5Up"  , "_CMS_scale_j_eta3to5_Run" +era+ "Up"  , plot_2d, smpl.second, tree_, true, "jecUncEta3To5Up");
    // smpl.second = create_systematic_uncertainty("jecUncEta3To5Down", "_CMS_scale_j_eta3to5_Run" +era+ "Down", plot_2d, smpl.second, tree_, true, "jecUncEta3To5Down");
    // smpl.second = create_systematic_uncertainty("jecUncRelativeBalUp"  , "_CMS_scale_j_RelativeBal_Run" +era+ "Up"  , plot_2d, smpl.second, tree_, true, "jecUncRelativeBalUp");
    // smpl.second = create_systematic_uncertainty("jecUncRelativeBalDown", "_CMS_scale_j_RelativeBal_Run" +era+ "Down", plot_2d, smpl.second, tree_, true, "jecUncRelativeBalDown");

    // 7.) Unclustered MET scale
    smpl.second = create_systematic_uncertainty("unclMetUp"  , "_CMS_scale_met_unclustered_Run" +era+ "Up"  , plot_2d, smpl.second, tree_, true, "unclMetUp");
    smpl.second = create_systematic_uncertainty("unclMetDown", "_CMS_scale_met_unclustered_Run" +era+ "Down", plot_2d, smpl.second, tree_, true, "unclMetDown");
    smpl.second.uncertainties["unclMetUp"].filename.ReplaceAll("NOMINAL","_unclMetUp");
    smpl.second.uncertainties["unclMetUp"].filename.ReplaceAll("NOMINAL","_unclMetDown");

    // 8.) Recoil scale/resolution uncertainties
    smpl.second = create_systematic_uncertainty("recoilscaleUp"  , "_CMS_htt_boson_scale_met_Run" +era+ "Up"  , plot_2d, smpl.second, tree_, false, "recoilscaleUp");
    smpl.second = create_systematic_uncertainty("recoilscaleDown", "_CMS_htt_boson_scale_met_Run" +era+ "Down", plot_2d, smpl.second, tree_, false, "recoilscaleDown");
    smpl.second = create_systematic_uncertainty("recoilresoUp"  , "_CMS_htt_boson_reso_met_Run" +era+ "Up"  , plot_2d, smpl.second, tree_, false, "recoilresoUp");
    smpl.second = create_systematic_uncertainty("recoilresoDown", "_CMS_htt_boson_reso_met_Run" +era+ "Down", plot_2d, smpl.second, tree_, false, "recoilresoDown");
    // smpl.second.uncertainties["recoilscaleUp"].filename.ReplaceAll("NOMINAL","_recoilscaleUp");
    // smpl.second.uncertainties["recoilscaleUp"].filename.ReplaceAll("NOMINAL","_recoilscaleDown");
    // smpl.second.uncertainties["recoilresoUp"].filename.ReplaceAll("NOMINAL","_recoilresoUp");
    // smpl.second.uncertainties["recoilresoUp"].filename.ReplaceAll("NOMINAL","_recoilresoDown");

    // 9.) TTbar shape
    if(smpl.second.name == "TT"){
      smpl.second = create_systematic_uncertainty("ttbarShapeUp"  , "_CMS_htt_ttbarShape_Run" +era+ "Up"  , plot_2d, smpl.second, tree_, false, "", true, "topptweight*","topptweight*topptweight*");
      smpl.second = create_systematic_uncertainty("ttbarShapeDown", "_CMS_htt_ttbarShape_Run" +era+ "Down", plot_2d, smpl.second, tree_, false, "", true, "topptweight*","");
    }

    // 10.) DY shape
    if(smpl.second.name == "ZTT" || smpl.second.name == "ZL" ){
      smpl.second = create_systematic_uncertainty("dyShapeUp"  , "_CMS_htt_dyShape_Run" +era+ "Up"  , plot_2d, smpl.second, tree_, false, "", true, "zptmassweight*","(1.0+1.1*(zptmassweight-1))*");
      smpl.second = create_systematic_uncertainty("dyShapeDown", "_CMS_htt_dyShape_Run" +era+ "Down", plot_2d, smpl.second, tree_, false, "", true, "zptmassweight*","(1.0+0.9*(zptmassweight-1))*");
    }
  }

  if(verbose){
    cout << endl << endl << "... Uncertainties of samples ... " << endl << endl ;
    for(auto & smpl : sample_map){
      cout << smpl.second.name << " : " <<endl;
      for(auto& unc : smpl.second.uncertainties) cout<<"  - "<<unc.first<<" : "<<unc.second.name<<endl;
    }
  }
  //************************************************************************************************
  // Fill histograms
  cout << endl << endl << "... Drawing ... " << endl;

  // Define output file
  TString filename   = "htt_em.inputs-sm-Run"+era+"-ML.root";
  TString output_dir = "output/"+era+"/";
  TFile * file_out   = new TFile( output_dir + "/" + filename , "RECREATE" );

  // Add here also a loop over the categories
  for(auto & cat : category_map){

    cout << "**************************************" << endl;
    cout << "Category " << cat.second.name << " : " << cat.second.class_nr << endl << endl;
    file_out -> mkdir(cat.second.name);

    for(auto & smpl : sample_map){

      cout << "-" << smpl.second.name << " : " << smpl.second.filename << endl;
      TFile *file = new TFile( directory + "/" + smpl.second.filename , "READ");
      TTree *tree = (TTree*) file->Get("TauCheck");

      smpl.second.hist_1d          = new TH1D(smpl.second.name + "_os_1d"         , "" , nbins , range[0] , range [1] );
      smpl.second.histSS_1d        = new TH1D(smpl.second.name + "_ss_1d"         , "" , nbins , range[0] , range [1] );

      TString full_weight_string            = smpl.second.weightString + smpl.second.topweight + smpl.second.zptmassweight + smpl.second.ggscaleweight + smpl.second.norm;
      TString full_weight_string_ss         = smpl.second.weightStringSS + smpl.second.topweight + smpl.second.zptmassweight + smpl.second.ggscaleweight + smpl.second.norm + smpl.second.qcdweight;

      if(verbose){
	cout << "weight string        " << " : " << full_weight_string << endl;
	cout << "weight string ss     " << " : " << full_weight_string_ss << endl;
	cout << "cut string           " << " : " << smpl.second.cutString << endl;
	cout << "cut string ss        " << " : " << smpl.second.cutStringSS << endl;
      }

      tree -> Draw(smpl.second.variable_1d+">>"+smpl.second.hist_1d->GetName()   , full_weight_string    +"("+smpl.second.cutString   + " && predicted_class ==" + Form("%d",cat.second.class_nr) + ")" );
      tree -> Draw(smpl.second.variable_1d+">>"+smpl.second.histSS_1d->GetName() , full_weight_string_ss +"("+smpl.second.cutStringSS + " && predicted_class ==" + Form("%d",cat.second.class_nr) + ")" );

      // Make QCD estimation
      if( smpl.second.name == "QCD" ) sample_map["1_QCD"].hist_1d -> Add(smpl.second.histSS_1d , +1);
      else if( smpl.second.name != "ggH125" && smpl.second.name != "qqH125" && smpl.second.name != "data_obs" && smpl.second.name != "TTcont" && smpl.second.name != "VVcont" ) sample_map["1_QCD"].hist_1d -> Add(smpl.second.histSS_1d , -1);

      // Loop over systematic uncertainties
      for(auto &sys : smpl.second.uncertainties){

	file = new TFile( directory + "/" + sys.second.filename , "READ");
	tree = (TTree*) file->Get("TauCheck");

	full_weight_string = sys.second.weightString + sys.second.topweight + sys.second.zptmassweight + sys.second.ggscaleweight + smpl.second.norm;
	full_weight_string_ss = sys.second.weightStringSS + sys.second.topweight + sys.second.zptmassweight + sys.second.ggscaleweight + smpl.second.norm + sys.second.qcdweight;
	if(verbose){
	  cout << "weight string        " << " : " << full_weight_string << endl;
	  cout << "weight string ss     " << " : " << full_weight_string_ss << endl;
	  cout << "cut string           " << " : " << sys.second.cutString << endl;
	  cout << "cut string ss        " << " : " << sys.second.cutStringSS << endl;
	}
	sys.second.hist_1d          = new TH1D(sys.second.name + "_os_1d"         , "" , nbins , range[0] , range [1] );
	sys.second.histSS_1d        = new TH1D(sys.second.name + "_ss_1d"         , "" , nbins , range[0] , range [1] );

	tree -> Draw( sys.second.variable_1d + ">>" + sys.second.hist_1d->GetName() , full_weight_string + "(" + sys.second.cutString + ")" );
	tree -> Draw( sys.second.variable_1d + ">>" + sys.second.histSS_1d->GetName() , full_weight_string_ss + "(" + sys.second.cutStringSS + ")" );

      }
    }// end of loop over samples

    // Calculate embedded uncertainty
    if(use_embedded){
      sample_map["9_EMB"].uncertainties["ttContEmbUp"].hist_1d   -> Add(sample_map["5_TTcont"].hist_1d , 0.1);
      sample_map["9_EMB"].uncertainties["ttContEmbDown"].hist_1d -> Add(sample_map["5_TTcont"].hist_1d , -0.1);
      sample_map["9_EMB"].uncertainties["ttContEmbUp"].hist_1d   -> Add(sample_map["5_VVcont"].hist_1d , 0.1);
      sample_map["9_EMB"].uncertainties["ttContEmbDown"].hist_1d -> Add(sample_map["5_VVcont"].hist_1d , -0.1);
    }

    //***********************************************************************************************
    // Print the final sum of weights of the nominal selection
    file_out -> cd(cat.second.name);
    for(auto & smpl : sample_map){
      if(smpl.first.Contains("cont")) continue;
      smpl.second.hist_1d -> Write( smpl.second.name );
      for(auto & sys : smpl.second.uncertainties){
	if(sys.second.name.Contains("qcd") && smpl.second.name != "QCD") continue;
	sys.second.hist_1d -> Write( sys.second.name );
      }
    }

    cout << endl << "... Final histogram content : "<< endl;
    TH1D * allBkg = (TH1D*) sample_map["4_TT"].hist_1d -> Clone();
    for(auto & smpl : sample_map) {
      cout << smpl.second.name << " : " << smpl.second.hist_1d -> GetSumOfWeights() << endl;
      // Add all background
      if( smpl.first == "0_Data" || smpl.first == "4_TT" ) continue;
      allBkg->Add(smpl.second.hist_1d);
    }
    cout << "Bkg together : " << allBkg -> GetSumOfWeights() << endl;
    cout << endl;
  } // end of loop over categories
  file_out -> Close();
  //************************************************************************************************
}
