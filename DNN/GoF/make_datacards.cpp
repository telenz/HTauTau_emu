#include "useful_classes.h"
#include "TROOT.h"
#include "Unfold.C"
#include "HttStylesNew.cc"

void make_datacards( TString variable_1d = "predicted_prob" ,
		     int nbins = 8 ,
		     vector<float> range = {0,1} ,
		     TString directory = "../mlFramework/predictions/") {

  gROOT->SetBatch(kTRUE);
  SetStyle();

  bool verbose = true;
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
					     { em_tt.name      , em_st },
					     { em_tt.name      , em_qqh },
					     { em_tt.name      , em_ggh }};

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
  //Sample ggH(  "ggH125"   , "em-NOMINAL_ntuple_ggH.root" );
  //Sample qqH(  "qqH125"   , "em-NOMINAL_ntuple_VBFH.root" );

  map<TString,Sample> sample_map = { { "Data" , Data },
				     { "ZTT"  , ZTT } ,
				     { "ZL"   , ZL } ,
				     { "EWKZ" , EWKZ } ,
				     { "W"    , W } ,
				     { "TT"   , TT } ,
				     { "VV"   , VV } ,
				     { "QCD"  , QCD } ,
				     { "ST"   , ST } ,
				     // { "ggH125" , ggH },
				     // { "qqH125" , qqH }
  };

  cout << endl << endl << "... Sample categories ... "<< endl ;
  for(auto & smpl : sample_map ) cout << " - " << smpl.second.name << endl;

  // Define common cut strings  
  for(auto & smpl : sample_map){
    smpl.second.cutString          = "q_1*q_2<0" + em_ztt.cutstring;
    smpl.second.weightString       = weight;
    smpl.second.cutStringSS        = "q_1*q_2>0.5" + em_ztt.cutstring;
    smpl.second.weightStringSS     = weight + em_ztt.qcdweight;
    smpl.second.cutStringSSrelaxed = "q_1*q_2>0.5" + em_ztt.cutstring_ss;
    smpl.second.weightStringSSrelaxed = weight + em_ztt.qcdweight;
    smpl.second.variable_1d = em_ztt.variable_1d;
  }

  // Define sample specific cutStrings and weightStrings
  sample_map["Data"].weightString = "1*";
  sample_map["QCD"].weightString  = "1*";
  sample_map["ZTT"].cutString += "&&isZTT";
  sample_map["ZL"].cutString += "&&!isZTT";

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
  // Fill histograms
  cout << endl << endl << "... Drawing ... " << endl;

  // Define output file
  TString filename   = "htt_em.inputs-sm-13TeV-ML.root";
  TString output_dir = "output_datacards/2016/";
  TFile * file_out   = new TFile( output_dir + "/" + filename , "RECREATE" );

  // Add here also a loop over the categories
  for(auto & cat : category_map){

    cout << "**************************************" << endl;
    cout << "Category " << cat.second.name << endl;
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
      smpl.second.hist_1d -> Write( smpl.second.name );

      // Loop over systematic uncertainties
      // for(auto &sys : smpl.second.uncertainties){

      // 	full_weight_string = sys.second.weightString + sys.second.topweight + sys.second.zptmassweight + sys.second.ggscaleweight + smpl.second.norm;
      // 	full_weight_string_ss = sys.second.weightStringSS + sys.second.topweight + sys.second.zptmassweight + sys.second.ggscaleweight + smpl.second.norm;
      // 	full_weight_string_ss_relaxed = sys.second.weightStringSSrelaxed + sys.second.topweight + sys.second.zptmassweight + sys.second.ggscaleweight + smpl.second.norm;
      // 	if(verbose){
      // 	  cout << "weight string        " << " : " << full_weight_string << endl;
      // 	  cout << "weight string ss     " << " : " << full_weight_string_ss << endl;
      // 	  cout << "weight string ss rel." << " : " << full_weight_string_ss_relaxed << endl;
      // 	  cout << "cut string           " << " : " << sys.second.cutString << endl;
      // 	  cout << "cut string ss        " << " : " << sys.second.cutStringSS << endl;
      // 	  cout << "cut string ss rel.   " << " : " << sys.second.cutStringSSrelaxed << endl << endl;
      // 	}
      // 	sys.second.hist_1d          = new TH1D(sys.second.name + "_os_1d"         , "" , nbins , range[0] , range [1] );
      // 	sys.second.histSS_1d        = new TH1D(sys.second.name + "_ss_1d"         , "" , nbins , range[0] , range [1] );
      // 	sys.second.histSSrelaxed_1d = new TH1D(sys.second.name + "_ss_relaxed_1d" , "" , nbins , range[0] , range [1] );

      // 	tree -> Draw( sys.second.variable_1d + ">>" + sys.second.hist_1d->GetName() , full_weight_string + "(" + sys.second.cutString + ")" );
      // 	tree -> Draw( sys.second.variable_1d + ">>" + sys.second.histSS_1d->GetName() , full_weight_string_ss + "(" + sys.second.cutStringSS + ")" );
      // 	tree -> Draw( sys.second.variable_1d + ">>" + sys.second.histSSrelaxed_1d->GetName() , full_weight_string_ss_relaxed + "(" + sys.second.cutStringSSrelaxed + ")" );

      // 	sys.second.hist_1d -> Write( sys.second.name );  
      // }
    } 
  }
  file_out -> Close();
  //************************************************************************************************
  // Print the final sum of weights of the nominal selection
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
