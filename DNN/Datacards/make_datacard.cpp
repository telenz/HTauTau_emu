#include "../GoF/useful_classes.h"
#include "TROOT.h"
#include "../GoF/HttStylesNew.cc"

void make_datacard( TString variable_1d = "predicted_prob" ,
		     int nbins = 8 ,
		     vector<float> range = {0.2,1} ,
		     TString directory = "/nfs/dust/cms/user/mameyer/SM_HiggsTauTau/HTauTau_emu/DNN/FreshCheckout/mlFramework/predictions/") {

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
  TString cuts_kine = "&& pt_1>13 && pt_2>10 && TMath::Max(pt_1,pt_2)>24 && trg_muonelectron";
  cuts_kine += mt_cut;

  TString cuts_iso_general    = "&& iso_1<0.15 && iso_2<0.2 && extraelec_veto<0.5 && extramuon_veto<0.5 ";
  TString cuts_iso_ss_general = "&& extraelec_veto<0.5 && extramuon_veto<0.5 ";

  TString btag_veto    = "&& nbtag==0 ";
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

  // Inclusive category
  TString cuts_category_specific = "&& dzeta>-35";
  em_ztt.suffix       = "ztt";
  em_ztt.cutstring    = cuts_kine + cuts_iso_general + cuts_category_specific;
  em_ztt.cutstring_ss = cuts_kine + cuts_iso_ss_general + "&& iso_1<0.50 && iso_2>0.2 && iso_2<0.5" + cuts_category_specific;
  em_ztt.variable_1d  = variable_1d;
  em_ztt.qcdweight    = "2.30*";
  em_ztt.bins_x_2d    = {0,50,55,60,65,70,75,80,85,90,95,100,400};
  em_ztt.bins_y_2d    = {15,20,25,30,35,40,300};
  float bound = range[0];
  while(bound <= range[1]){
    em_ztt.bins_1d.push_back(bound);
    bound += (range[1]-range[0])/nbins;
  }
  em_ztt.gg_scale_weight_up   = "(0.9421 - 0.00001699*pt_2)*";
  em_ztt.gg_scale_weight_down = "(1.0579 + 0.00001699*pt_2)*";

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
  Sample EWKZ( "EWKZ"     , "em-NOMINAL_ntuple_EWKZ.root" );
  Sample W(    "W"        , "em-NOMINAL_ntuple_W.root" );
  Sample TT(   "TT"       , "em-NOMINAL_ntuple_TT.root" );
  Sample VV(   "VV"       , "em-NOMINAL_ntuple_Diboson.root" );
  Sample ST(   "ST"       , "em-NOMINAL_ntuple_SingleTop.root" );
  Sample QCD(  "QCD"      , "em-NOMINAL_ntuple_Data.root" );
  Sample ggH(  "ggH125"   , "em-NOMINAL_ntuple_ggH125.root" );
  Sample qqH(  "qqH125"   , "em-NOMINAL_ntuple_qqH125.root" );

  map<TString,Sample> sample_map = { { "Data" , Data },
				     { "ZTT"  , ZTT } ,
				     { "ZL"   , ZL } ,
				     { "EWKZ" , EWKZ } ,
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
    smpl.second.cutString          = "q_1*q_2<0" + em_ztt.cutstring;
    smpl.second.weightString       = weight;
    smpl.second.cutStringSS        = "q_1*q_2>0" + em_ztt.cutstring;
    smpl.second.weightStringSS     = weight + em_ztt.qcdweight;
    smpl.second.cutStringSSrelaxed = "q_1*q_2>0" + em_ztt.cutstring_ss;
    smpl.second.weightStringSSrelaxed = weight + em_ztt.qcdweight;
    smpl.second.variable_1d = em_ztt.variable_1d;
  }

  // Define sample specific cutStrings and weightStrings
  sample_map["Data"].weightString = "1*";
  sample_map["QCD"].weightString  = "1*";
  sample_map["ZTT"].cutString += "&&isZTT";
  sample_map["ZL"].cutString += "&&!isZTT";
  //sample_map["QCD"].cutString  = "q_1*q_2>0" + em_ztt.cutstring;

  sample_map["Data"].weightStringSS = em_ztt.qcdweight;
  sample_map["QCD"].weightStringSS  = em_ztt.qcdweight;
  sample_map["ZTT"].cutStringSS += "&&isZTT";
  sample_map["ZL"].cutStringSS += "&&!isZTT";

  sample_map["Data"].weightStringSSrelaxed = em_ztt.qcdweight;
  sample_map["QCD"].weightStringSSrelaxed  = em_ztt.qcdweight;
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
    smpl.second.uncertainties["eScaleUp"].filename.ReplaceAll("NOMINAL","_escaleUp");
    smpl.second.uncertainties["eScaleDown"].filename.ReplaceAll("NOMINAL","_escaleDown");

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
    smpl.second.uncertainties["jesUp"].filename.ReplaceAll("NOMINAL","_jesUp");
    smpl.second.uncertainties["jesDown"].filename.ReplaceAll("NOMINAL","_jesDown");

    // 3.) Unclustered MET scale
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
    smpl.second.uncertainties["unclMetUp"].filename.ReplaceAll("NOMINAL","_unclMetUp");
    smpl.second.uncertainties["unclMetDown"].filename.ReplaceAll("NOMINAL","_unclMetDown");

    // Sample-specific uncertainties
    // 4.) TTbar shape
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

    // 5.) DY shape (EWKZ sample should be added here)
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

  //   // 6.) ggScale
  //   if(smpl.second.name == "ggH125"){
  //     Sample ggScaleUp = smpl.second;
  //     Sample ggScaleDown = smpl.second;
  //     smpl.second.uncertainties.insert( make_pair("ggScaleUp"   , ggScaleUp) );
  //     smpl.second.uncertainties.insert( make_pair("ggScaleDown" , ggScaleDown) );
  //     smpl.second.uncertainties["ggScaleUp"].name   += "_CMS_scale_gg_13TeVUp";
  //     smpl.second.uncertainties["ggScaleDown"].name += "_CMS_scale_gg_13TeVDown";
  //     smpl.second.uncertainties["ggScaleUp"].ggscaleweight=category_in_use.gg_scale_weight_up;
  //     smpl.second.uncertainties["ggScaleDown"].ggscaleweight=category_in_use.gg_scale_weight_down;
  //   }
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
  TString filename   = "htt_em.inputs-sm-13TeV-ML.root";
  TString output_dir = "output/2016/";
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
      smpl.second.histSSrelaxed_1d = new TH1D(smpl.second.name + "_ss_relaxed_1d" , "" , nbins , range[0] , range [1] );

      TString full_weight_string            = smpl.second.weightString + smpl.second.topweight + smpl.second.zptmassweight + smpl.second.ggscaleweight + smpl.second.norm;
      TString full_weight_string_ss         = smpl.second.weightStringSS + smpl.second.topweight + smpl.second.zptmassweight + smpl.second.ggscaleweight + smpl.second.norm;
      TString full_weight_string_ss_relaxed = smpl.second.weightStringSSrelaxed + smpl.second.topweight + smpl.second.zptmassweight + smpl.second.ggscaleweight + smpl.second.norm;

      if(verbose){
	cout << "weight string        " << " : " << full_weight_string << endl;
	cout << "weight string ss     " << " : " << full_weight_string_ss << endl;
	cout << "weight string ss rel." << " : " << full_weight_string_ss_relaxed << endl;
	cout << "cut string           " << " : " << smpl.second.cutString << endl;
	cout << "cut string ss        " << " : " << smpl.second.cutStringSS << endl;
	cout << "cut string ss rel.   " << " : " << smpl.second.cutStringSSrelaxed << endl << endl;
      }

      tree -> Draw(smpl.second.variable_1d+">>"+smpl.second.hist_1d->GetName()   , full_weight_string    +"("+smpl.second.cutString   + " && predicted_class ==" + Form("%d",cat.second.class_nr) + ")" );
      tree -> Draw(smpl.second.variable_1d+">>"+smpl.second.histSS_1d->GetName() , full_weight_string_ss +"("+smpl.second.cutStringSS + " && predicted_class ==" + Form("%d",cat.second.class_nr) + ")" );
      tree -> Draw(smpl.second.variable_1d+">>"+smpl.second.histSSrelaxed_1d->GetName(),full_weight_string_ss_relaxed+"("+smpl.second.cutStringSSrelaxed+"&&predicted_class =="+Form("%d",cat.second.class_nr)+")");

      // Write to file
      file_out -> cd(cat.second.name);
      if( smpl.second.name != "QCD") smpl.second.hist_1d -> Write( smpl.second.name );

      // Loop over systematic uncertainties
      for(auto &sys : smpl.second.uncertainties){

	file = new TFile( directory + "/" + sys.second.filename , "READ");
	tree = (TTree*) file->Get("TauCheck");

	full_weight_string = sys.second.weightString + sys.second.topweight + sys.second.zptmassweight + sys.second.ggscaleweight + smpl.second.norm;
	full_weight_string_ss = sys.second.weightStringSS + sys.second.topweight + sys.second.zptmassweight + sys.second.ggscaleweight + smpl.second.norm;
	full_weight_string_ss_relaxed = sys.second.weightStringSSrelaxed + sys.second.topweight + sys.second.zptmassweight + sys.second.ggscaleweight + smpl.second.norm;
	if(verbose){
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

	tree -> Draw( sys.second.variable_1d + ">>" + sys.second.hist_1d->GetName() , full_weight_string + "(" + sys.second.cutString + ")" );
	tree -> Draw( sys.second.variable_1d + ">>" + sys.second.histSS_1d->GetName() , full_weight_string_ss + "(" + sys.second.cutStringSS + ")" );
	tree -> Draw( sys.second.variable_1d + ">>" + sys.second.histSSrelaxed_1d->GetName() , full_weight_string_ss_relaxed + "(" + sys.second.cutStringSSrelaxed + ")" );

	// Write to file
	file_out -> cd(cat.second.name);
	sys.second.hist_1d -> Write( sys.second.name );
      }
    }// end of loop over samples
    //***********************************************************************************************
    // Determine QCD background

    // 1.) Take the shape from ss relaxed region
    for(auto & smpl : sample_map){
      if( smpl.first == "ggH125"  ||
    	  smpl.first == "qqH125"  ||
    	  smpl.first == "Data"    ||
    	  smpl.first == "QCD"      ) continue;
      sample_map["QCD"].histSS_1d        -> Add( smpl.second.histSS_1d , -1 );
      sample_map["QCD"].histSSrelaxed_1d -> Add( smpl.second.histSSrelaxed_1d , -1 );
    }
    sample_map["QCD"].hist_1d = (TH1D*) sample_map["QCD"].histSSrelaxed_1d -> Clone();

    // 2.) Calculate normalization via ss/ss_relaxed
    double qcd_norm = sample_map["QCD"].histSS_1d->GetSumOfWeights()/sample_map["QCD"].histSSrelaxed_1d->GetSumOfWeights();
    cout << endl << "qcd_norm = " << qcd_norm << endl << endl;
    sample_map["QCD"].hist_1d -> Scale(qcd_norm);
    file_out -> cd(cat.second.name);
    sample_map["QCD"].hist_1d -> Write( sample_map["QCD"].name );
    //***********************************************************************************************
    // Print the final sum of weights of the nominal selection
    cout << endl << "... Final histogram content : "<< endl;
    TH1D * allBkg = (TH1D*) sample_map["TT"].hist_1d -> Clone();
    for(auto & smpl : sample_map) {
      cout << smpl.second.name << " : " << smpl.second.hist_1d -> GetSumOfWeights() << endl;
      // Add all background
      if( smpl.first == "Data" || smpl.first == "TT" ) continue;
      allBkg->Add(smpl.second.hist_1d);
    }
    cout << "Bkg together : " << allBkg -> GetSumOfWeights() << endl;
    cout << endl;
  } // end of loop over categories
  file_out -> Close();
  //************************************************************************************************
}
