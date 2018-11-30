#include "useful_classes.h"
#include "systematic_uncertainties.h"
#include "TROOT.h"
#include "Unfold.C"
#include "HttStylesNew.cc"
#include <algorithm>
#include <typeinfo>
#include "Config.cc"

using namespace std;

void make_histograms(TString era_for_config="2016") {

  Config cfg("config_"+era_for_config+".cfg");

  const TString directory = cfg.get<string>("directory");
  const string era        = cfg.get<string>("era");
  const bool use_embedded = cfg.get<bool>("use_embedded");
  const bool plot_2d      = cfg.get<bool>("plot_2d");
  const bool verbose      = cfg.get<bool>("verbose");
  const bool take_percentile_subrange = cfg.get<bool>("take_percentile_subrange");

  vector<string> category_names_vector = cfg.get<vector<string>>("categories");

  map< TString , Category > category_map;

  int i=1;
  int nbins=0;
  for( auto cat_name : category_names_vector){
    class Category category_(cat_name);
    category_map[cat_name] = category_;
    category_map.at(cat_name).plot_2d = cfg.get<bool>(cat_name+"_plot_2d");
    category_map.at(cat_name).variable = cfg.get<string>(cat_name+"_variable");
    nbins  = cfg.get<int>(cat_name+"_nbins");
  }

  gROOT->SetBatch(kTRUE);
  SetStyle();

  double min_percentile = 0.01;
  double max_percentile = 0.99;
  vector<double> percentile_ranges = { 0.01 , 0.10 , 0.20 , 0.30 , 0.40 , 0.50 , 0.60 , 0.70 , 0.80 , 0.90 , 0.99 };

  //************************************************************************************************
  // Define some common weights and cuts

  TString weight      = "xsec_lumi_weight*mcweight*puweight*effweight*trigger_filter_weight*";
  TString mt_cut    = "&& mTdileptonMET<60 ";
  TString cuts_kine = "&& pt_1>13 && pt_2>10 && TMath::Max(pt_1,pt_2)>24 && metFilters && trg_muonelectron && nbtag == 0";
  cuts_kine += mt_cut;

  TString cuts_iso_general    = "&& iso_1<0.15 && iso_2<0.2 && extraelec_veto<0.5 && extramuon_veto<0.5 ";
  TString cuts_iso_ss_general = "&& extraelec_veto<0.5 && extramuon_veto<0.5 ";

  //************************************************************************************************
  // Define different categories (use of category "em_cat_in_use" which is set below the definition)
  // For now the used attributes of the category class are only the binning and the name definitions

  // Inclusive category
  TString cuts_category_specific = "&& dzeta>-35";
  for(auto &cat : category_map){
    cat.second.cutstring    = cuts_kine + cuts_iso_general + cuts_category_specific;
    cat.second.qcdweight    = "qcdweight*";
    cat.second.bins_x_2d    = {0,50,55,60,65,70,75,80,85,90,95,100,400};
    cat.second.bins_y_2d    = {15,20,25,30,35,40,300};
  }

  //************************************************************************************************
  // Define samples

  Sample Data( "data_obs" , "NOMINAL_ntuple_MuonEG_em.root" );
  Sample ZTT(  "ZTT"      , "NOMINAL_ntuple_DYJets_em.root" );
  Sample ZL(   "ZL"       , "NOMINAL_ntuple_DYJets_em.root" );
  Sample W(    "W"        , "NOMINAL_ntuple_WJets_em.root" );
  Sample TT(   "TT"       , "NOMINAL_ntuple_TTbar_em.root" );
  Sample TTcont( "TTcont"  , "NOMINAL_ntuple_TTbar_em.root" );
  Sample VV(   "VV"       , "NOMINAL_ntuple_Diboson_em.root" );
  Sample VVcont( "VVcont"  , "NOMINAL_ntuple_Diboson_em.root" );
  Sample ST(   "ST"       , "NOMINAL_ntuple_SingleTop_em.root" );
  Sample QCD(  "QCD"      , "NOMINAL_ntuple_MuonEG_em.root" );
  Sample EMB(  "EMB"      , "NOMINAL_ntuple_Embedded_em.root" );
  Sample ggH(  "ggH125"   , "NOMINAL_ntuple_ggH_em.root" );
  Sample qqH(  "qqH125"   , "NOMINAL_ntuple_VBFH_em.root" );

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

  // Define common cut strings  
  for(auto & smpl : sample_map){
    smpl.second.qcdweight          = "qcdweight*";
    smpl.second.cutString          = "os>0.5" + category_map.begin()->second.cutstring;
    smpl.second.weightString       = weight;
    smpl.second.cutStringSS        = "os<0.5" + category_map.begin()->second.cutstring;
    smpl.second.weightStringSS     = weight;
    smpl.second.variable_1d = category_map.begin()->second.variable;
    smpl.second.variable_2d = category_map.begin()->second.variable;
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

    if( smpl.second.name == "data_obs" || smpl.first.Contains("cont") ) continue;

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
    // 3.) Electron scale (applied only on embedded)
    if(smpl.second.name == "EMB"){
      smpl.second = create_systematic_uncertainty("escale_emb_Up"  , "_CMS_scale_emb_e_Run" +era+ "Up"  , plot_2d, smpl.second, tree_, true, "escaleUp");
      smpl.second = create_systematic_uncertainty("escale_emb_Down", "_CMS_scale_emb_e_Run" +era+ "Down", plot_2d, smpl.second, tree_, true, "escaleDown");
    }
    // 4.) Electron scale (applied only on MC)
    if(smpl.second.name != "EMB"){
      smpl.second = create_systematic_uncertainty("escale_mc_Up"  , "_CMS_scale_mc_e_Run" +era+ "Up"  , plot_2d, smpl.second, tree_, true, "escaleUp");
      smpl.second = create_systematic_uncertainty("escale_mc_Down", "_CMS_scale_mc_e_Run" +era+ "Down", plot_2d, smpl.second, tree_, true, "escaleDown");
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

    // 8.) Recoil scale/resolution uncertainties
    smpl.second = create_systematic_uncertainty("recoilscaleUp"  , "_CMS_htt_boson_scale_met_Run" +era+ "Up"  , plot_2d, smpl.second, tree_, true, "recoilscaleUp");
    smpl.second = create_systematic_uncertainty("recoilscaleDown", "_CMS_htt_boson_scale_met_Run" +era+ "Down", plot_2d, smpl.second, tree_, true, "recoilscaleDown");
    smpl.second = create_systematic_uncertainty("recoilresoUp"  , "_CMS_htt_boson_reso_met_Run" +era+ "Up"  , plot_2d, smpl.second, tree_, true, "recoilresoUp");
    smpl.second = create_systematic_uncertainty("recoilresoDown", "_CMS_htt_boson_reso_met_Run" +era+ "Down", plot_2d, smpl.second, tree_, true, "recoilresoDown");

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
  TString filename   = "htt_em.inputs-sm-Run"+era+"-"+category_map.begin()->second.variable_1d+".root";
  if(!plot_2d)    filename += "-" + category_map.begin()->second.variable_1d + ".root";
  else if(plot_2d && category_map.begin()->second.name == "em_inclusive") filename += "-" + category_map.begin()->second.variable_2d( 0 , category_map.begin()->second.variable_2d.First(":") ) + "-" + category_map.begin()->second.variable_2d( category_map.begin()->second.variable_2d.First(":")+1 , category_map.begin()->second.variable_2d.Length() ) + ".root";
  else filename += ".root";
  TString output_dir = "output/"+era+"/";
  if(!plot_2d) output_dir += "/var_1d/";
  else         output_dir += "/var_2d/";
  TFile * file_out   = new TFile( output_dir + "/" + filename , "RECREATE" );

  // Add here also a loop over the categories
  for(auto & cat : category_map){

    vector<double> binning_1d;
    for(auto & smpl : sample_map){
      cout << "**************************************" << endl;
      cout << smpl.second.name << " : " << smpl.second.filename << endl;
      TFile *file = new TFile( directory + "/" + smpl.second.filename );
      TTree *tree = (TTree*) file->Get("TauCheck");
      
      if( smpl.second.name == "data_obs" ){
	if( !plot_2d ) cout << endl << "Variable 1d = " << smpl.second.variable_1d << endl;
	else           cout << endl << "Variable 2d = " << smpl.second.variable_2d << endl;
	//*******************************************************************************************
	// Get the 1st and 99th percentiles
	if( take_percentile_subrange ){
	  float var;
	  int valid_type = tree->SetBranchAddress(cat.second.variable,&var);
	  cout<<"valid_type = "<<valid_type<<endl;
	  vector<float> values;
	  if(valid_type==0){
	    for(int evt=0; evt<tree->GetEntries(); evt++){
	      tree->GetEntry(evt);
	      if(var <= -10 ) continue;
	      values.push_back(var);
	    }
	    int min_element = (int) (min_percentile*values.size());
	    int max_element = (int) (max_percentile*values.size());
	    std::nth_element(values.begin(), values.begin() + (min_percentile*values.size()), values.end());
	    std::nth_element(values.begin(), values.begin() + (max_percentile*values.size()), values.end());
	    cout<<"range start = "<<values[min_element]<<endl;
	    cout<<"range ends  = "<<values[max_element]<<endl<<endl;
	    float range[2];
	    range[0] = values[min_element];
	    range[1] = values[max_element];
	    for( auto bound : percentile_ranges){
	      int element = (int) (bound*values.size());
	      std::nth_element(values.begin(), values.begin() + (bound*values.size()), values.end());
	      cout<<"range start = "<<values[element]<<endl;
	      binning_1d.push_back(values[element]);
	    }
	    nbins = binning_1d.size() -1;
	    cout<<endl;
	  }
	}
      }

      smpl.second.hist_1d   = new TH1D(smpl.second.name + "_os_1d", "", nbins, &binning_1d[0]);
      smpl.second.histSS_1d = new TH1D(smpl.second.name + "_ss_1d", "", nbins, &binning_1d[0]);

      const int nbins_x_2d = category_map.begin()->second.bins_x_2d.size() - 1;
      const int nbins_y_2d = category_map.begin()->second.bins_y_2d.size() - 1;

      smpl.second.hist_2d = new TH2D(smpl.second.name + "_os_2d", "", nbins_x_2d, &category_map.begin()->second.bins_x_2d[0], nbins_y_2d, &category_map.begin()->second.bins_y_2d[0] );
      smpl.second.histSS_2d = new TH2D(smpl.second.name + "_ss_2d", "", nbins_x_2d, &category_map.begin()->second.bins_x_2d[0], nbins_y_2d, &category_map.begin()->second.bins_y_2d[0] );

      TString full_weight_string    = smpl.second.weightString;
      TString full_weight_string_ss = smpl.second.weightStringSS + smpl.second.qcdweight;

      if(verbose){
	cout << "Variable_2d          " << " : " << smpl.second.variable_2d << endl;
	cout << "weight string        " << " : " << full_weight_string << endl;
	cout << "weight string ss     " << " : " << full_weight_string_ss << endl;
	cout << "cut string           " << " : " << smpl.second.cutString << endl;
	cout << "cut string ss        " << " : " << smpl.second.cutStringSS << endl;
      }

      if(!plot_2d){
	tree -> Draw( smpl.second.variable_1d + ">>" + smpl.second.hist_1d->GetName() , full_weight_string + "(" + smpl.second.cutString + ")" );
	tree -> Draw( smpl.second.variable_1d + ">>" + smpl.second.histSS_1d->GetName() , full_weight_string_ss + "(" + smpl.second.cutStringSS + ")" );
      }
      else{
	tree -> Draw( smpl.second.variable_2d + ">>" + smpl.second.hist_2d->GetName() , full_weight_string + "(" + smpl.second.cutString + ")" );
	tree -> Draw( smpl.second.variable_2d + ">>" + smpl.second.histSS_2d->GetName() , full_weight_string_ss + "(" + smpl.second.cutStringSS + ")" );

	smpl.second.hist_1d          = (TH1D*) Unfold(smpl.second.hist_2d);
	smpl.second.histSS_1d        = (TH1D*) Unfold(smpl.second.histSS_2d);
      }

      // Make QCD estimation
      if( smpl.second.name == "QCD" ) sample_map["1_QCD"].hist_1d -> Add(smpl.second.histSS_1d , +1);
      else if( smpl.second.name != "ggH125" && smpl.second.name != "qqH125" && smpl.second.name != "data_obs" && smpl.second.name != "TTcont" && smpl.second.name != "VVcont" ) sample_map["1_QCD"].hist_1d -> Add(smpl.second.histSS_1d , -1);

      // Loop over systematic uncertainties
      for(auto &sys : smpl.second.uncertainties){

	full_weight_string    = sys.second.weightString;
	full_weight_string_ss = sys.second.weightStringSS + sys.second.qcdweight;
	if(verbose){
	  cout << "Variable_2d          " << " : " << sys.second.variable_2d << endl;
	  cout << "weight string        " << " : " << full_weight_string << endl;
	  cout << "weight string ss     " << " : " << full_weight_string_ss << endl;
	  cout << "cut string           " << " : " << sys.second.cutString << endl;
	  cout << "cut string ss        " << " : " << sys.second.cutStringSS << endl;
	}
	sys.second.hist_1d   = new TH1D(sys.second.name + "_os_1d", "", nbins, &binning_1d[0]);
	sys.second.histSS_1d = new TH1D(sys.second.name + "_ss_1d", "", nbins, &binning_1d[0]);

	sys.second.hist_2d   = new TH2D(sys.second.name + "_os_2d"         , "" , nbins_x_2d , &category_map.begin()->second.bins_x_2d[0] , nbins_y_2d , &category_map.begin()->second.bins_y_2d[0] );
	sys.second.histSS_2d   = new TH2D(sys.second.name + "_ss_2d"         , "" , nbins_x_2d , &category_map.begin()->second.bins_x_2d[0] , nbins_y_2d , &category_map.begin()->second.bins_y_2d[0] );

	if(!plot_2d){
	  tree -> Draw( sys.second.variable_1d + ">>" + sys.second.hist_1d->GetName() , full_weight_string + "(" + sys.second.cutString + ")" );
	  tree -> Draw( sys.second.variable_1d + ">>" + sys.second.histSS_1d->GetName() , full_weight_string_ss + "(" + sys.second.cutStringSS + ")" );
	}
	else{
	  tree -> Draw( sys.second.variable_2d + ">>" + sys.second.hist_2d->GetName() , full_weight_string + "(" + sys.second.cutString + ")" );
	  tree -> Draw( sys.second.variable_2d + ">>" + sys.second.histSS_2d->GetName() , full_weight_string_ss + "(" + sys.second.cutStringSS + ")" );

	  sys.second.hist_1d          = (TH1D*) Unfold(sys.second.hist_2d);
	  sys.second.histSS_1d        = (TH1D*) Unfold(sys.second.histSS_2d);
	}
      } // end of loop over sys uncertainties
    } // end of loop over samples

    // Calculate embedded uncertainty
    if(use_embedded){
      sample_map["9_EMB"].uncertainties["ttContEmbUp"].hist_1d   -> Add(sample_map["5_TTcont"].hist_1d , 0.1);
      sample_map["9_EMB"].uncertainties["ttContEmbDown"].hist_1d -> Add(sample_map["5_TTcont"].hist_1d , -0.1);
      sample_map["9_EMB"].uncertainties["ttContEmbUp"].hist_1d   -> Add(sample_map["7_VVcont"].hist_1d , 0.1);
      sample_map["9_EMB"].uncertainties["ttContEmbDown"].hist_1d -> Add(sample_map["7_VVcont"].hist_1d , -0.1);
    }

    //***********************************************************************************************
    // Write all histograms to output file
    cout << endl << endl << "... Writing histograms to output file ... " << endl;

    file_out -> mkdir(cat.second.name);
    file_out -> cd(cat.second.name);
    for(auto & smpl : sample_map){
      if( smpl.first.Contains("cont") ) continue;
      smpl.second.hist_1d -> Write( smpl.second.name );
      for(auto & sys : smpl.second.uncertainties){
	if(sys.second.name.Contains("qcd") && smpl.second.name != "QCD") continue;
	sys.second.hist_1d -> Write( sys.second.name );
      }
    }

    // Write the final sum of weights of the nominal selection
    cout << endl << "... Final histogram content : "<< endl;
    TH1D * allBkg = (TH1D*) sample_map["1_QCD"].hist_1d -> Clone();
    for(auto & smpl : sample_map) {
      cout << smpl.second.name << " : " << smpl.second.hist_1d -> GetSumOfWeights() << endl;
      if( smpl.second.name == "data_obs" || smpl.second.name == "QCD" ) continue;
      allBkg->Add(smpl.second.hist_1d);
    }
    cout << "Bkg together : " << allBkg -> GetSumOfWeights() << endl << endl;
    cout << "output file : " << output_dir + "/" + filename << endl << endl;

  } // end of loop over categories
  file_out -> Close();
}
