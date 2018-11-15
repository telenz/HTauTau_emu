#include "useful_classes.h"
#include "TROOT.h"
#include "Unfold.C"
#include "HttStylesNew.cc"

void produce_gof_input( TString category_name = "em_inclusive" ,
			bool plot_2d = false,
		        TString variable_1d = "pt_1" ,
			int nbins = 8 ,
			vector<float> range = {0,400} ,
			TString variable_2d = "pt_2:m_vis" ,  // convention for TH2D is "var_y : var_x"
			TString directory = "../../Inputs/NTuples_2016_No_EWKZ/") {

  gROOT->SetBatch(kTRUE);
  SetStyle();

  bool verbose = false;
  bool apply_btag_veto = false;
  //************************************************************************************************
  // Define some common weights and cuts

  TString btag_weight = "btag0weight*";
  TString weight      = "xsec_lumi_weight*mcweight*puweight*effweight*trigger_filter_weight*";
  if(apply_btag_veto) weight += btag_weight;

  TString mt_cut    = "&& mTdileptonMET<60 ";
  TString cuts_kine = "&& pt_1>13 && pt_2>10 && TMath::Max(pt_1,pt_2)>24 && metFilters && trg_muonelectron";
  cuts_kine += mt_cut;

  TString cuts_iso_general    = "&& iso_1<0.15 && iso_2<0.2 && extraelec_veto<0.5 && extramuon_veto<0.5 ";
  TString cuts_iso_ss_general = "&& extraelec_veto<0.5 && extramuon_veto<0.5 ";

  TString btag_veto    = "&& nbtag==0 ";
  //************************************************************************************************
  // Define different categories (use of category "em_cat_in_use" which is set below the definition)
  // For now the used attributes of the category class are only the binning and the name definitions

  class Category em_incl("em_inclusive");
  class Category em_0jet("em_0jet");
  class Category em_boosted("em_boosted");
  class Category em_vbf("em_vbf");

  // Inclusive category
  TString cuts_category_specific = "&& dzeta>-35";
  em_incl.suffix       = "inclusive";
  em_incl.cutstring    = cuts_kine + cuts_iso_general + cuts_category_specific;
  em_incl.cutstring_ss = cuts_kine + cuts_iso_ss_general + "&& iso_1<0.50 && iso_2>0.2 && iso_2<0.5" + cuts_category_specific;
  em_incl.variable_2d  = variable_2d;  // first variable corresponds to y-variable
  em_incl.variable_1d  = variable_1d;
  em_incl.qcdweight    = "2.3*";
  em_incl.bins_x_2d    = {0,50,55,60,65,70,75,80,85,90,95,100,400};
  em_incl.bins_y_2d    = {15,20,25,30,35,40,300};
  float bound = range[0];
  while(bound <= range[1]){
    em_incl.bins_1d.push_back(bound);
    bound += (range[1]-range[0])/nbins;
  }
  em_incl.gg_scale_weight_up   = "(0.9421 - 0.00001699*pt_2)*";
  em_incl.gg_scale_weight_down = "(1.0579 + 0.00001699*pt_2)*";

  // 0jet category
  cuts_category_specific = "&& jets==0 && dzeta>-35";
  em_0jet.cutstring    = cuts_kine + cuts_iso_general + cuts_category_specific;
  em_0jet.cutstring_ss = cuts_kine + cuts_iso_ss_general + "&& iso_1<0.3 && iso_2>0.1 && iso_2<0.3" + cuts_category_specific;
  em_0jet.variable_2d = "pt_2:m_vis";  // first variable corresponds to y-variable
  em_0jet.qcdweight   = "2.26*";
  em_0jet.gg_scale_weight_up   = "(0.9421 - 0.00001699*pt_2)*";
  em_0jet.gg_scale_weight_down = "(1.0579 + 0.00001699*pt_2)*";
  em_0jet.bins_x_2d = {0,50,55,60,65,70,75,80,85,90,95,100,400};
  em_0jet.bins_y_2d = {15,20,25,30,35,40,300};

  // Boosted category
  cuts_category_specific = "&&(njets==1 || (njets==2 && mjj<300) || njets>2)&&dzeta>-35";
  em_boosted.cutstring    = cuts_kine + cuts_iso_general + cuts_category_specific;
  em_boosted.cutstring_ss = cuts_kine + cuts_iso_ss_general + "&& iso_1<0.3 && iso_2>0.1 && iso_2<0.3" + cuts_category_specific;
  em_boosted.variable_2d = "pt_sv:m_sv";
  em_boosted.qcdweight   = "2.25*";
  em_boosted.gg_scale_weight_up   = "(0.9358 + 0.00088712 * pt_sv)*";
  em_boosted.gg_scale_weight_down = "(1.0642 - 0.00088712 * pt_sv)*";
  em_boosted.bins_x_2d = {0,80,90,100,110,120,130,140,150,160,300};
  em_boosted.bins_y_2d = {0,100,150,200,250,300,5000};

  // VBF category
  cuts_category_specific = "&& njets==2 && mjj>=300 && dzeta>-10 ";
  em_vbf.cutstring    = cuts_kine + cuts_iso_general + cuts_category_specific;
  em_vbf.cutstring_ss = cuts_kine + cuts_iso_ss_general + "&& iso_1<0.5 && iso_2>0.2 && iso_2<0.5" + cuts_category_specific;
  em_vbf.variable_2d = "mjj:m_sv";
  em_vbf.qcdweight   = "2.84*";
  em_vbf.gg_scale_weight_up   = "(1.032 + 0.00010196 * mjj)*";
  em_vbf.gg_scale_weight_down = "(0.968 - 0.00010196 * mjj)*";
  em_vbf.bins_x_2d = {0,95,115,135,155,400};
  em_vbf.bins_y_2d = {300,700,1100,1500,10000};

  // Make a map from these categories
  map< TString , Category > category_map = { { em_incl.name    , em_incl },
					     { em_0jet.name    , em_0jet },
					     { em_boosted.name , em_boosted },
					     { em_vbf.name     , em_vbf }};

  Category category_in_use = category_map[category_name];
  if(apply_btag_veto) category_in_use.cutstring    += btag_veto;
  if(apply_btag_veto) category_in_use.cutstring_ss += btag_veto;

  //************************************************************************************************
  // Define samples

  Sample Data( "data_obs" , "NOMINAL_ntuple_MuonEG_em.root" );
  Sample ZTT(  "ZTT"      , "NOMINAL_ntuple_DYJets_em.root" );
  Sample ZL(   "ZL"       , "NOMINAL_ntuple_DYJets_em.root" );
  Sample W(    "W"        , "NOMINAL_ntuple_WJets_em.root" );
  Sample TT(   "TT"       , "NOMINAL_ntuple_TTbar_em.root" );
  Sample VV(   "VV"       , "NOMINAL_ntuple_Diboson_em.root" );
  Sample ST(   "ST"       , "NOMINAL_ntuple_SingleTop_em.root" );
  Sample QCD(  "QCD"      , "NOMINAL_ntuple_MuonEG_em.root" );
  Sample ggH(  "ggH125"   , "NOMINAL_ntuple_ggH_em.root" );
  Sample qqH(  "qqH125"   , "NOMINAL_ntuple_VBFH_em.root" );

  // Define pre-defined norms
  // ZTT.norm = "1.02*";
  // ZL.norm  = "1.02*";

  map<TString,Sample> sample_map = { { "Data" , Data },
				     { "ZTT"  , ZTT } ,
				     { "ZL"   , ZL } ,
				     { "W"    , W } ,
				     { "TT"   , TT } ,
				     { "VV"   , VV } ,
				     { "QCD"  , QCD } ,
				     { "ST"   , ST } ,
				     { "ggH125" , ggH },
				     { "qqH125" , qqH }
  };

  cout << endl << endl << "... Sample categories ... "<< endl ;
  for(auto & smpl : sample_map ) cout << " - " << smpl.second.name << endl;

  // Define common cut strings  
  for(auto & smpl : sample_map){
    smpl.second.cutString          = "os>0.5" + category_in_use.cutstring;
    smpl.second.weightString       = weight;
    smpl.second.cutStringSS        = "os<0.5" + category_in_use.cutstring;
    smpl.second.weightStringSS     = weight + category_in_use.qcdweight;
    smpl.second.cutStringSSrelaxed = "os<0.5" + category_in_use.cutstring_ss;
    smpl.second.weightStringSSrelaxed = weight + category_in_use.qcdweight;
    smpl.second.variable_1d = category_in_use.variable_1d;
    smpl.second.variable_2d = category_in_use.variable_2d;
  }

  // Define sample specific cutStrings and weightStrings
  sample_map["Data"].weightString = "1*";
  sample_map["QCD"].weightString  = "1*";
  sample_map["ZTT"].cutString += "&&isZTT";
  sample_map["ZL"].cutString += "&&!isZTT";

  sample_map["Data"].weightStringSS = category_in_use.qcdweight;
  sample_map["QCD"].weightStringSS  = category_in_use.qcdweight;
  sample_map["ZTT"].cutStringSS += "&&isZTT";
  sample_map["ZL"].cutStringSS += "&&!isZTT";

  sample_map["Data"].weightStringSSrelaxed = category_in_use.qcdweight;
  sample_map["QCD"].weightStringSSrelaxed  = category_in_use.qcdweight;
  sample_map["ZTT"].cutStringSSrelaxed += "&&isZTT";
  sample_map["ZL"].cutStringSSrelaxed += "&&!isZTT";

  // Define sample specific weights
  sample_map["TT"].topweight = "topptweight*";
  sample_map["ZTT"].zptmassweight = "zptmassweight*";
  sample_map["ZL"].zptmassweight = "zptmassweight*";

  //************************************************************************************************
  // Define systematic uncertainties

  // Open one tree to test existence of uncertainty shifts
  TFile *file_ = new TFile( directory + "/" + Data.filename );
  TTree *tree_ = (TTree*) file_->Get("TauCheck");

  for(auto & smpl : sample_map){

    if( smpl.first == "Data" || smpl.first == "QCD" ) continue;

    TString var1 , var1Up , var1Down , var2 , var2Up , var2Down;
    if(plot_2d){
      var1     = smpl.second.variable_2d( 0 , smpl.second.variable_2d.First(":") );
      var2     = smpl.second.variable_2d( smpl.second.variable_2d.First(":")+1 , smpl.second.variable_2d.Length() );
      var1Up   = var1;
      var1Down = var1;
      var2Up   = var2;
      var2Down = var2;
    }

    // Uncertainties common for all samples
    // 1.) Electron scale
    Sample eScaleUp   = smpl.second;
    Sample eScaleDown = smpl.second;
    smpl.second.uncertainties.insert( make_pair("eScaleUp"   , eScaleUp) );
    smpl.second.uncertainties.insert( make_pair("eScaleDown" , eScaleDown) );
    smpl.second.uncertainties["eScaleUp"].name += "_CMS_scale_e_13TeVUp";
    smpl.second.uncertainties["eScaleDown"].name += "_CMS_scale_e_13TeVDown";
    smpl.second.uncertainties["eScaleUp"].cutString.ReplaceAll("dzeta","dzeta_escaleUp");
    smpl.second.uncertainties["eScaleDown"].cutString.ReplaceAll("dzeta","dzeta_escaleDown");
    smpl.second.uncertainties["eScaleUp"].cutString.ReplaceAll("pt_1","pt_1_escaleUp");
    smpl.second.uncertainties["eScaleDown"].cutString.ReplaceAll("pt_1","pt_1_escaleDown");
    smpl.second.uncertainties["eScaleUp"].cutString.ReplaceAll("mTdileptonMET","mTdileptonMET_escaleUp");
    smpl.second.uncertainties["eScaleDown"].cutString.ReplaceAll("mTdileptonMET","mTdileptonMET_escaleDown");
    if(!plot_2d){
      if(tree_ -> GetBranch(variable_1d+"_escaleUp")){
	smpl.second.uncertainties["eScaleUp"].variable_1d = variable_1d + "_escaleUp";
	smpl.second.uncertainties["eScaleDown"].variable_1d = variable_1d + "_escaleDown";
      }
      else cout << "No systematic shift for electron scale uncertainty for variable " << variable_1d << " available in tree." << endl;
    }
    else{
      if(tree_ -> GetBranch(var1Up+"_escaleUp")){
	var1Up   = var1 + "_escaleUp";
	var1Down = var1 + "_escaleDown";
      }
      if(tree_ -> GetBranch(var2Up+"_escaleUp")){
	var2Up   = var2 + "_escaleUp";
	var2Down = var2 + "_escaleDown";
      }
      smpl.second.uncertainties["eScaleUp"].variable_2d   = var1Up   + ":" + var2Up;
      smpl.second.uncertainties["eScaleDown"].variable_2d = var1Down + ":" + var2Down;
    }

    // 2.) JES
    Sample jesUp = smpl.second;
    Sample jesDown = smpl.second;
    smpl.second.uncertainties.insert( make_pair("jesUp"   , jesUp) );
    smpl.second.uncertainties.insert( make_pair("jesDown" , jesDown) );
    smpl.second.uncertainties["jesUp"].name   += "_CMS_scale_j_13TeVUp";
    smpl.second.uncertainties["jesDown"].name += "_CMS_scale_j_13TeVDown";
    smpl.second.uncertainties["jesUp"].cutString.ReplaceAll("dzeta","dzeta_jesUp");
    smpl.second.uncertainties["jesDown"].cutString.ReplaceAll("dzeta","dzeta_jesDown");
    smpl.second.uncertainties["jesUp"].cutString.ReplaceAll("mTdileptonMET","mTdileptonMET_jesUp");
    smpl.second.uncertainties["jesDown"].cutString.ReplaceAll("mTdileptonMET","mTdileptonMET_jesDown");
    if(!plot_2d){
      if(tree_ -> GetBranch(variable_1d+"_jesUp")){
	smpl.second.uncertainties["jesUp"].variable_1d = variable_1d + "_jesUp";
	smpl.second.uncertainties["jesDown"].variable_1d = variable_1d + "_jesDown";
      }
      else cout << "No systematic shift for JES uncertainty for variable " << variable_1d << " available in tree." << endl;
    }
    else{
      if(tree_ -> GetBranch(var1Up+"_jesUp")){
	var1Up   = var1 + "_jesUp";
	var1Down = var1 + "_jesDown";
      }
      if(tree_ -> GetBranch(var2Up+"_jesUp")){
	var2Up   = var2 + "_jesUp";
	var2Down = var2 + "_jesDown";
      }
      smpl.second.uncertainties["jesUp"].variable_2d   = var1Up   + ":" + var2Up;
      smpl.second.uncertainties["jesDown"].variable_2d = var1Down + ":" + var2Down;
    }

    // 4.) Unclustered MET scale
    Sample unclMetUp = smpl.second;
    Sample unclMetDown = smpl.second;
    smpl.second.uncertainties.insert( make_pair("unclMetUp"   , unclMetUp) );
    smpl.second.uncertainties.insert( make_pair("unclMetDown" , unclMetDown) );
    smpl.second.uncertainties["unclMetUp"].name   += "_CMS_scale_met_unclustered_13TeVUp";
    smpl.second.uncertainties["unclMetDown"].name += "_CMS_scale_met_unclustered_13TeVDown";
    smpl.second.uncertainties["unclMetUp"].cutString.ReplaceAll("dzeta","dzeta_unclMetUp");
    smpl.second.uncertainties["unclMetDown"].cutString.ReplaceAll("dzeta","dzeta_unclMetDown");
    smpl.second.uncertainties["unclMetUp"].cutString.ReplaceAll("mTdileptonMET","mTdileptonMET_unclMetUp");
    smpl.second.uncertainties["unclMetDown"].cutString.ReplaceAll("mTdileptonMET","mTdileptonMET_unclMetDown");
    if(!plot_2d){
      if(tree_ -> GetBranch(variable_1d+"_unclMetUp")){
	smpl.second.uncertainties["unclMetUp"].variable_1d = variable_1d + "_unclMetUp";
	smpl.second.uncertainties["unclMetDown"].variable_1d = variable_1d + "_unclMetDown";
      }
      else cout << "No systematic shift for unclustered MET uncertainty for variable " << variable_1d << " available in tree." << endl;
    }
    else{
      if(tree_ -> GetBranch(var1Up+"_unclMetUp")){
	var1Up   = var1 + "_unclMetUp";
	var1Down = var1 + "_unclMetDown";
      }
      if(tree_ -> GetBranch(var2Up+"_unclMetUp")){
	var2Up   = var2 + "_unclMetUp";
	var2Down = var2 + "_unclMetDown";
      }
      smpl.second.uncertainties["unclMetUp"].variable_2d   = var1Up   + ":" + var2Up;
      smpl.second.uncertainties["unclMetDown"].variable_2d = var1Down + ":" + var2Down;
    }

    // Sample-specific uncertainties
    // 5.) TTbar shape
    if(smpl.second.name == "TT"){
      Sample ttbarShapeUp = smpl.second;
      Sample ttbarShapeDown = smpl.second;
      smpl.second.uncertainties.insert( make_pair("ttbarShapeUp"   , ttbarShapeUp) );
      smpl.second.uncertainties.insert( make_pair("ttbarShapeDown" , ttbarShapeDown) );
      smpl.second.uncertainties["ttbarShapeUp"].name   += "_CMS_htt_ttbarShape_13TeVUp";
      smpl.second.uncertainties["ttbarShapeDown"].name += "_CMS_htt_ttbarShape_13TeVDown";
      smpl.second.uncertainties["ttbarShapeUp"].topweight = "topptweight*topptweight*";
      smpl.second.uncertainties["ttbarShapeDown"].topweight = "";
    }

    // 6.) DY shape
    if(smpl.second.name == "ZTT" || smpl.second.name == "ZL" ){
      Sample dyShapeUp = smpl.second;
      Sample dyShapeDown = smpl.second;
      smpl.second.uncertainties.insert( make_pair("dyShapeUp"   , dyShapeUp) );
      smpl.second.uncertainties.insert( make_pair("dyShapeDown" , dyShapeDown) );
      smpl.second.uncertainties["dyShapeUp"].name   += "_CMS_htt_dyShape_13TeVUp";
      smpl.second.uncertainties["dyShapeDown"].name += "_CMS_htt_dyShape_13TeVDown";
      smpl.second.uncertainties["dyShapeUp"].zptmassweight="(1.0+1.1*(zptmassweight-1))*";
      smpl.second.uncertainties["dyShapeDown"].zptmassweight="(1.0+0.9*(zptmassweight-1))*";
    }

    // 7.) ggScale
    if(smpl.second.name == "ggH125"){
      Sample ggScaleUp = smpl.second;
      Sample ggScaleDown = smpl.second;
      smpl.second.uncertainties.insert( make_pair("ggScaleUp"   , ggScaleUp) );
      smpl.second.uncertainties.insert( make_pair("ggScaleDown" , ggScaleDown) );
      smpl.second.uncertainties["ggScaleUp"].name   += "_CMS_scale_gg_13TeVUp";
      smpl.second.uncertainties["ggScaleDown"].name += "_CMS_scale_gg_13TeVDown";
      smpl.second.uncertainties["ggScaleUp"].ggscaleweight=category_in_use.gg_scale_weight_up;
      smpl.second.uncertainties["ggScaleDown"].ggscaleweight=category_in_use.gg_scale_weight_down;
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

  // 1.) Fill nominal histograms
  cout << endl << endl << "... Drawing ... " << endl;
  for(auto & smpl : sample_map){

    if( smpl.first == "Data" ){
      if( !plot_2d ) cout << endl << "Variable 1d = " << smpl.second.variable_1d << endl;
      else           cout << endl << "Variable 2d = " << smpl.second.variable_2d << endl;
    }

    cout << "**************************************" << endl;
    cout << smpl.second.name << " : " << smpl.second.filename << endl;
    TFile *file = new TFile( directory + "/" + smpl.second.filename );
    TTree *tree = (TTree*) file->Get("TauCheck");

    smpl.second.hist_1d          = new TH1D(smpl.second.name + "_os_1d"         , "" , nbins , range[0] , range [1] );
    smpl.second.histSS_1d        = new TH1D(smpl.second.name + "_ss_1d"         , "" , nbins , range[0] , range [1] );
    smpl.second.histSSrelaxed_1d = new TH1D(smpl.second.name + "_ss_relaxed_1d" , "" , nbins , range[0] , range [1] );

    const int nbins_x_2d = category_in_use.bins_x_2d.size() - 1;
    const int nbins_y_2d = category_in_use.bins_y_2d.size() - 1;

    smpl.second.hist_2d          = new TH2D(smpl.second.name + "_os_2d"         , "" , nbins_x_2d , &category_in_use.bins_x_2d[0] , nbins_y_2d , &category_in_use.bins_y_2d[0] );
    smpl.second.histSS_2d        = new TH2D(smpl.second.name + "_ss_2d"         , "" , nbins_x_2d , &category_in_use.bins_x_2d[0] , nbins_y_2d , &category_in_use.bins_y_2d[0] );
    smpl.second.histSSrelaxed_2d = new TH2D(smpl.second.name + "_ss_relaxed_2d" , "" , nbins_x_2d , &category_in_use.bins_x_2d[0] , nbins_y_2d , &category_in_use.bins_y_2d[0] );

    TString full_weight_string            = smpl.second.weightString + smpl.second.topweight + smpl.second.zptmassweight + smpl.second.ggscaleweight + smpl.second.norm;
    TString full_weight_string_ss         = smpl.second.weightStringSS + smpl.second.topweight + smpl.second.zptmassweight + smpl.second.ggscaleweight + smpl.second.norm;
    TString full_weight_string_ss_relaxed = smpl.second.weightStringSSrelaxed + smpl.second.topweight + smpl.second.zptmassweight + smpl.second.ggscaleweight + smpl.second.norm;

    if(verbose){
      cout << "Variable_2d          " << " : " << smpl.second.variable_2d << endl;
      cout << "weight string        " << " : " << full_weight_string << endl;
      cout << "weight string ss     " << " : " << full_weight_string_ss << endl;
      cout << "weight string ss rel." << " : " << full_weight_string_ss_relaxed << endl;
      cout << "cut string           " << " : " << smpl.second.cutString << endl;
      cout << "cut string ss        " << " : " << smpl.second.cutStringSS << endl;
      cout << "cut string ss rel.   " << " : " << smpl.second.cutStringSSrelaxed << endl << endl;
    }

    if(!plot_2d){
      tree -> Draw( smpl.second.variable_1d + ">>" + smpl.second.hist_1d->GetName() , full_weight_string + "(" + smpl.second.cutString + ")" );
      tree -> Draw( smpl.second.variable_1d + ">>" + smpl.second.histSS_1d->GetName() , full_weight_string_ss + "(" + smpl.second.cutStringSS + ")" );
      tree -> Draw( smpl.second.variable_1d + ">>" + smpl.second.histSSrelaxed_1d->GetName() , full_weight_string_ss_relaxed + "(" + smpl.second.cutStringSSrelaxed + ")" );
    }
    else{
      tree -> Draw( smpl.second.variable_2d + ">>" + smpl.second.hist_2d->GetName() , full_weight_string + "(" + smpl.second.cutString + ")" );
      tree -> Draw( smpl.second.variable_2d + ">>" + smpl.second.histSS_2d->GetName() , full_weight_string_ss + "(" + smpl.second.cutStringSS + ")" );
      tree -> Draw( smpl.second.variable_2d + ">>" + smpl.second.histSSrelaxed_2d->GetName() , full_weight_string_ss_relaxed + "(" + smpl.second.cutStringSSrelaxed + ")" );

      smpl.second.hist_1d          = (TH1D*) Unfold(smpl.second.hist_2d);
      smpl.second.histSS_1d        = (TH1D*) Unfold(smpl.second.histSS_2d);
      smpl.second.histSSrelaxed_1d = (TH1D*) Unfold(smpl.second.histSSrelaxed_2d);
    }

    // Loop over systematic uncertainties
    for(auto &sys : smpl.second.uncertainties){

      full_weight_string = sys.second.weightString + sys.second.topweight + sys.second.zptmassweight + sys.second.ggscaleweight + smpl.second.norm;
      full_weight_string_ss = sys.second.weightStringSS + sys.second.topweight + sys.second.zptmassweight + sys.second.ggscaleweight + smpl.second.norm;
      full_weight_string_ss_relaxed = sys.second.weightStringSSrelaxed + sys.second.topweight + sys.second.zptmassweight + sys.second.ggscaleweight + smpl.second.norm;
      if(verbose){
	cout << "Variable_2d          " << " : " << sys.second.variable_2d << endl;
	cout << "weight string        " << " : " << full_weight_string << endl;
	cout << "weight string ss     " << " : " << full_weight_string_ss << endl;
	cout << "weight string ss rel." << " : " << full_weight_string_ss_relaxed << endl;
	cout << "cut string           " << " : " << sys.second.cutString << endl;
	cout << "cut string ss        " << " : " << sys.second.cutStringSS << endl;
	cout << "cut string ss rel.   " << " : " << sys.second.cutStringSSrelaxed << endl << endl;
      }
      sys.second.hist_1d          = new TH1D(sys.second.name + "_os_1d"         , "" , nbins , range[0] , range [1] );
      sys.second.histSS_1d        = new TH1D(sys.second.name + "_ss_1d"         , "" , nbins , range[0] , range [1] );
      sys.second.histSSrelaxed_1d = new TH1D(sys.second.name + "_ss_relaxed_1d" , "" , nbins , range[0] , range [1] );

      sys.second.hist_2d          = new TH2D(sys.second.name + "_os_2d"         , "" , nbins_x_2d , &category_in_use.bins_x_2d[0] , nbins_y_2d , &category_in_use.bins_y_2d[0] );
      sys.second.histSS_2d        = new TH2D(sys.second.name + "_ss_2d"         , "" , nbins_x_2d , &category_in_use.bins_x_2d[0] , nbins_y_2d , &category_in_use.bins_y_2d[0] );
      sys.second.histSSrelaxed_2d = new TH2D(sys.second.name + "_ss_relaxed_2d" , "" , nbins_x_2d , &category_in_use.bins_x_2d[0] , nbins_y_2d , &category_in_use.bins_y_2d[0] );

      if(!plot_2d){
	tree -> Draw( sys.second.variable_1d + ">>" + sys.second.hist_1d->GetName() , full_weight_string + "(" + sys.second.cutString + ")" );
	tree -> Draw( sys.second.variable_1d + ">>" + sys.second.histSS_1d->GetName() , full_weight_string_ss + "(" + sys.second.cutStringSS + ")" );
	tree -> Draw( sys.second.variable_1d + ">>" + sys.second.histSSrelaxed_1d->GetName() , full_weight_string_ss_relaxed + "(" + sys.second.cutStringSSrelaxed + ")" );
      }
      else{
	tree -> Draw( sys.second.variable_2d + ">>" + sys.second.hist_2d->GetName() , full_weight_string + "(" + sys.second.cutString + ")" );
	tree -> Draw( sys.second.variable_2d + ">>" + sys.second.histSS_2d->GetName() , full_weight_string_ss + "(" + sys.second.cutStringSS + ")" );
	tree -> Draw( sys.second.variable_2d + ">>" + sys.second.histSSrelaxed_2d->GetName() , full_weight_string_ss_relaxed + "(" + sys.second.cutStringSSrelaxed + ")" );

	sys.second.hist_1d          = (TH1D*) Unfold(sys.second.hist_2d);
	sys.second.histSS_1d        = (TH1D*) Unfold(sys.second.histSS_2d);
	sys.second.histSSrelaxed_1d = (TH1D*) Unfold(sys.second.histSSrelaxed_2d);
      }
    }
  }
  //************************************************************************************************
  // Determine QCD background

  // 1.) Take the shape from ss relaxed region
  for(auto & smpl : sample_map){
    if( smpl.first == "ggH125"  ||
	smpl.first == "qqH125"  ||
	smpl.first == "Data"    ||
	smpl.first == "QCD"      ) continue;
    sample_map["QCD"].histSS_1d        -> Add( smpl.second.histSS_1d , -1 );
    //sample_map["QCD"].histSSrelaxed_1d -> Add( smpl.second.histSSrelaxed_1d , -1 );
  }
  sample_map["QCD"].hist_1d = (TH1D*) sample_map["QCD"].histSS_1d -> Clone();

  // 2.) Calculate normalization via ss/ss_relaxed
  //double qcd_norm = sample_map["QCD"].histSS_1d->Integral(0,sample_map["QCD"].histSS_1d->GetNbinsX()+1)/sample_map["QCD"].histSSrelaxed_1d->Integral(0,sample_map["QCD"].histSSrelaxed_1d->GetNbinsX()+1);
  //cout << endl << "qcd_norm = " << qcd_norm << endl << endl;
  //sample_map["QCD"].hist_1d -> Scale(qcd_norm);

  //************************************************************************************************
  // Write all histograms to output file
  cout << endl << endl << "... Writing histograms to output file ... " << endl;

  TString filename = "htt_em.inputs-sm-13TeV";
  if(category_in_use.name != "em_inclusive") filename += category_in_use.name;
  variable_1d.ReplaceAll(" ","");
  category_in_use.variable_2d.ReplaceAll(" ","");
  if(!plot_2d)                                               filename += "-" + variable_1d + ".root";
  else if(plot_2d && category_in_use.name == "em_inclusive") filename += "-" + category_in_use.variable_2d( 0 , category_in_use.variable_2d.First(":") ) + "-" + category_in_use.variable_2d( category_in_use.variable_2d.First(":")+1 , category_in_use.variable_2d.Length() ) + ".root";
  else filename += ".root";
  TString output_directory = "output/";
  if(!plot_2d) output_directory += "2016/var_1d/";
  else         output_directory += "2016/var_2d/";

  TFile * fileOut      = new TFile( output_directory + "/" + filename , "RECREATE" );
  fileOut             -> mkdir(category_in_use.name);
  fileOut             -> cd(category_in_use.name);

  for(auto & smpl : sample_map){
    smpl.second.hist_1d -> Write( smpl.second.name );
    for(auto & sys : smpl.second.uncertainties)  sys.second.hist_1d -> Write( sys.second.name );
  }
  fileOut -> Close();

  // Write the final sum of weights of the nominal selection
  cout << endl << "... Final histogram content : "<< endl;
  TH1D * allBkg = (TH1D*) sample_map["QCD"].hist_1d -> Clone();
  for(auto & smpl : sample_map) {
    cout << smpl.second.name << " : " << smpl.second.hist_1d -> GetSumOfWeights() << endl;
    // Add all background
    if( smpl.first == "Data" || smpl.first == "QCD" ) continue;
    allBkg->Add(smpl.second.hist_1d);
  }
  cout << "Bkg together : " << allBkg -> GetSumOfWeights() << endl;
  cout << endl;
  cout << "output file : " << filename << endl << endl;
}
