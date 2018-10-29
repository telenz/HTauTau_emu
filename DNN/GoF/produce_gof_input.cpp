#include "produce_gof_input.h"
#include "TROOT.h"
#include "Unfold.C"
#include "HttStylesNew.cc"

void produce_gof_input(TString variable_1d = "pt_1" , TString directory = "../../Inputs/NTuples_2016/") {

  gROOT->SetBatch(kTRUE);
  SetStyle();

  bool verbose = false;
  bool plot_2d = false;
  bool apply_btag_veto = false;

  TString variable_2d = "pt_2 : m_vis"; // convention for TH2D is "var_y : var_x"

  //************************************************************************************************
  // Define some common weights and cuts

  TString btag_weight = "btag0weight*";
  TString Weight      = "xsec_lumi_weight*mcweight*puweight*effweight*0.978824*0.985*"; // two numbers are filter efficiencies? FIXME -> alexei 0.979 from Daniel Winterbottom -> which one to use
  if(apply_btag_veto) Weight += btag_weight;
  TString qcdweight = "2.30*";

  // Definition of cuts
  TString mTCut    = "&& mTdileptonMET<60 ";
  TString CutsKine = "&& pt_1>13 && pt_2>15 && TMath::Max(pt_1,pt_2)>24 ";
  CutsKine += mTCut;

  TString CutsIso   = "&& iso_1<0.15 && iso_2<0.2 && extraelec_veto<0.5 && extramuon_veto<0.5 ";
  TString CutsIsoSS = "&& iso_1<0.50 && iso_2>0.2 && iso_2<0.5 && extraelec_veto<0.5 && extramuon_veto<0.5 ";

  TString btag_veto     = "&& nbtag==0 ";
  TString CutsCategory = "&& dzeta>-35 && metFilters && trg_muonelectron";
  if(apply_btag_veto) CutsCategory += btag_veto;


  TString Cuts   = CutsKine + CutsIso   + CutsCategory;
  TString CutsSS = CutsKine + CutsIsoSS + CutsCategory;

  //************************************************************************************************
  // Define different categories (use of category "em_cat_in_use" which is set below the definition)
  // For now the used attributes of the category class are only the binning and the name definitions

  class Category em_incl("em_inclusive");
  class Category em_0jet("em_0jet");
  class Category em_boosted("em_boosted");
  class Category em_vbf("em_vbf");

  // Inclusive category
  em_incl.nbins_x_2d = 12;
  em_incl.nbins_y_2d = 6;
  em_incl.bins_x_2d  = new float[em_incl.nbins_x_2d+1]{0,50,55,60,65,70,75,80,85,90,95,100,400};
  em_incl.bins_y_2d  = new float[em_incl.nbins_y_2d+1]{15,20,25,30,35,40,300};
  em_incl.nbins_1d = 12;
  em_incl.bins_1d  = new float[em_incl.nbins_1d+1]{0,50,55,60,65,70,75,80,85,90,95,100,400};
  em_incl.suffix = "inclusive";

  // 0jet category
  em_0jet.nbins_x_2d = 12;
  em_0jet.nbins_y_2d = 6;
  em_0jet.bins_x_2d = new float[em_0jet.nbins_x_2d+1]{0,50,55,60,65,70,75,80,85,90,95,100,400};
  em_0jet.bins_y_2d = new float[em_0jet.nbins_y_2d+1]{15,20,25,30,35,40,300};

  // Boosted category
  em_boosted.nbins_x_2d = 10;
  em_boosted.nbins_y_2d = 6;
  em_boosted.bins_x_2d = new float[em_boosted.nbins_x_2d+1]{0,80,90,100,110,120,130,140,150,160,300};
  em_boosted.bins_y_2d = new float[em_boosted.nbins_y_2d+1]{0,100,150,200,250,300,5000};

  // VBF category
  em_vbf.nbins_x_2d = 5;
  em_vbf.nbins_y_2d = 4;
  em_vbf.bins_x_2d = new float[em_vbf.nbins_x_2d+1]{0,95,115,135,155,400};
  em_vbf.bins_y_2d = new float[em_vbf.nbins_y_2d+1]{300,700,1100,1500,10000};

  // Make a vector from these categories
  vector<class Category> category_vec = { em_incl };

  class Category em_cat_in_use = em_incl;

  //************************************************************************************************
  // Define samples

  Sample Data( "data_obs" , "MuonEG_Run2016_dnn_em_v1.root" );
  Sample ZTT(  "ZTT"      , "DYJets_dnn_em_v1.root" );
  Sample ZLL(  "ZL"       , "DYJets_dnn_em_v1.root" );
  Sample EWKZ( "EWKZ"     , "EWKZ_dnn_em_v1.root" );
  Sample W(    "W"        , "WJets_dnn_em_v1.root" );
  Sample TT(   "TT"       , "TTbar_dnn_em_v1.root" );
  Sample VV(   "VV"       , "Diboson_dnn_em_v1.root" );
  Sample ST(   "ST"       , "SingleTop_dnn_em_v1.root" );
  Sample QCD(  "QCD"      , "MuonEG_Run2016_dnn_em_v1.root" );
  Sample ggH(  "ggH125"   , "ggH_dnn_em_v1.root" );
  Sample qqH(  "qqH125"   , "VBFH_dnn_em_v1.root" );

  // Define pre-defined norms
  ZTT.norm = "1.02*";
  ZLL.norm = "1.02*";

  map<TString,Sample> sample_map = { { "Data" , Data },
				     { "ZTT"  , ZTT } ,
				     { "ZLL"  , ZLL } ,
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
    smpl.second.cutString          = "os>0.5"+Cuts;
    smpl.second.weightString       = Weight;
    smpl.second.cutStringSS        = "os<0.5"+Cuts;
    smpl.second.weightStringSS     = Weight+qcdweight;
    smpl.second.cutStringSSrelaxed = "os<0.5"+CutsSS;
    smpl.second.weightStringSSrelaxed = Weight+qcdweight;
    smpl.second.variable_1d = variable_1d;
    smpl.second.variable_2d = variable_2d;
  }

  // Define sample specific cutStrings and weightStrings
  sample_map["Data"].weightString = "1*";
  sample_map["QCD"].weightString  = "1*";
  sample_map["ZTT"].cutString += "&&isZTT";
  sample_map["ZLL"].cutString += "&&!isZTT";

  sample_map["Data"].weightStringSS = qcdweight;
  sample_map["QCD"].weightStringSS  = qcdweight;
  sample_map["ZTT"].cutStringSS += "&&isZTT";
  sample_map["ZLL"].cutStringSS += "&&!isZTT";

  sample_map["Data"].weightStringSSrelaxed = qcdweight;
  sample_map["QCD"].weightStringSSrelaxed  = qcdweight;
  sample_map["ZTT"].cutStringSSrelaxed += "&&isZTT";
  sample_map["ZLL"].cutStringSSrelaxed += "&&!isZTT";

  // Define sample specific weights
  sample_map["TT"].topweight = "topptweightRun2*";
  sample_map["ZTT"].zptmassweight = "zptmassweight*";
  sample_map["ZLL"].zptmassweight = "zptmassweight*";

  //************************************************************************************************
  // Define systematic uncertainties

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
    smpl.second.uncertainties["eScaleUp"].cutString.ReplaceAll("dzeta","dzeta_eUp");
    smpl.second.uncertainties["eScaleUp"].cutString.ReplaceAll("pt_1","pt_Up_1");
    smpl.second.uncertainties["eScaleUp"].cutString.ReplaceAll("mTdileptonMET","mTdileptonMET_eUp");
    smpl.second.uncertainties["eScaleUp"].variable_2d.ReplaceAll("m_vis","m_vis_eUp");
    smpl.second.uncertainties["eScaleUp"].variable_2d.ReplaceAll("m_sv","m_sv_eUp");
    smpl.second.uncertainties["eScaleDown"].cutString.ReplaceAll("dzeta","dzeta_eDown");
    smpl.second.uncertainties["eScaleDown"].cutString.ReplaceAll("pt_1","pt_Down_1");
    smpl.second.uncertainties["eScaleDown"].cutString.ReplaceAll("mTdileptonMET","mTdileptonMET_eDown");
    smpl.second.uncertainties["eScaleDown"].variable_2d.ReplaceAll("m_vis","m_vis_eDown");
    smpl.second.uncertainties["eScaleDown"].variable_2d.ReplaceAll("m_sv","m_sv_eDown");

    // 2.) JES
    Sample jScaleUp = smpl.second;
    Sample jScaleDown = smpl.second;
    smpl.second.uncertainties.insert( make_pair("jScaleUp"   , jScaleUp) );
    smpl.second.uncertainties.insert( make_pair("jScaleDown" , jScaleDown) );
    smpl.second.uncertainties["jScaleUp"].name   += "_CMS_scale_j_13TeVUp";
    smpl.second.uncertainties["jScaleDown"].name += "_CMS_scale_j_13TeVDown";
    smpl.second.uncertainties["jScaleUp"].cutString.ReplaceAll("njets","njets_Up");
    smpl.second.uncertainties["jScaleUp"].cutString.ReplaceAll("mjj","mjj_Up");
    smpl.second.uncertainties["jScaleUp"].variable_1d.ReplaceAll("mjj","mjj_Up");
    smpl.second.uncertainties["jScaleUp"].variable_2d.ReplaceAll("mjj","mjj_Up");
    smpl.second.uncertainties["jScaleDown"].cutString.ReplaceAll("njets","njets_Down");
    smpl.second.uncertainties["jScaleDown"].cutString.ReplaceAll("mjj","mjj_Down");
    smpl.second.uncertainties["jScaleDown"].variable_1d.ReplaceAll("mjj","mjj_Down");
    smpl.second.uncertainties["jScaleDown"].variable_2d.ReplaceAll("mjj","mjj_Down");

    // 3.) MET scale
    Sample metScaleUp = smpl.second;
    Sample metScaleDown = smpl.second;
    smpl.second.uncertainties.insert( make_pair("metScaleUp"   , metScaleUp) );
    smpl.second.uncertainties.insert( make_pair("metScaleDown" , metScaleDown) );
    smpl.second.uncertainties["metScaleUp"].name   += "_CMS_scale_met_em_13TeVUp";
    smpl.second.uncertainties["metScaleDown"].name += "_CMS_scale_met_em_13TeVDown";
    smpl.second.uncertainties["metScaleUp"].cutString.ReplaceAll("dzeta","dzeta_scaleUp");
    smpl.second.uncertainties["metScaleDown"].cutString.ReplaceAll("dzeta","dzeta_scaleDown");

    // 4.) MET resolution
    Sample metResoUp = smpl.second;
    Sample metResoDown = smpl.second;
    smpl.second.uncertainties.insert( make_pair("metResoUp"   , metResoUp) );
    smpl.second.uncertainties.insert( make_pair("metResoDown" , metResoDown) );
    smpl.second.uncertainties["metResoUp"].name   += "_CMS_reso_met_em_13TeVUp";
    smpl.second.uncertainties["metResoDown"].name += "_CMS_reso_met_em_13TeVDown";
    smpl.second.uncertainties["metResoUp"].cutString.ReplaceAll("dzeta","dzeta_resoUp");
    smpl.second.uncertainties["metResoDown"].cutString.ReplaceAll("dzeta","dzeta_resoDown");

    // Sample-specific uncertainties
    // 5.) TTbar shape
    if(smpl.second.name == "TTbar"){
      Sample ttbarShapeUp = smpl.second;
      Sample ttbarShapeDown = smpl.second;
      smpl.second.uncertainties.insert( make_pair("ttbarShapeUp"   , ttbarShapeUp) );
      smpl.second.uncertainties.insert( make_pair("ttbarShapeDown" , ttbarShapeDown) );
      smpl.second.uncertainties["ttbarShapeUp"].name   += "_CMS_htt_ttbarShape_13TeVUp";
      smpl.second.uncertainties["ttbarShapeDown"].name += "_CMS_htt_ttbarShape_13TeVDown";
      smpl.second.uncertainties["ttbarShapeUp"].topweight = "topptweightRun2*topptweightRun2*";
      smpl.second.uncertainties["ttbarShapeDown"].topweight = "";
    }

    // 6.) DY shape (EWKZ sample should be added here)
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
    if(smpl.second.name == "ggH"){
      TString ggScaleWeightUp="(0.9421 - 0.00001699*pt_2)*";
      TString ggScaleWeightDown="(1.0579 + 0.00001699*pt_2)*";

      Sample ggScaleUp = smpl.second;
      Sample ggScaleDown = smpl.second;
      smpl.second.uncertainties.insert( make_pair("ggScaleUp"   , ggScaleUp) );
      smpl.second.uncertainties.insert( make_pair("ggScaleDown" , ggScaleDown) );
      smpl.second.uncertainties["ggScaleUp"].name   += "_CMS_scale_gg_13TeVUp";
      smpl.second.uncertainties["ggScaleDown"].name += "_CMS_scale_gg_13TeVDown";
      smpl.second.uncertainties["ggScaleUp"].ggscaleweight=ggScaleWeightUp;
      smpl.second.uncertainties["ggScaleDown"].ggscaleweight=ggScaleWeightDown;
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
      if( !plot_2d ) cout << endl << "Variable = " << smpl.second.variable_1d << endl;
      else           cout << endl << "Variable = " << smpl.second.variable_2d << endl;
    }

    cout << "**************************************" << endl;
    cout << smpl.second.name << " : " << smpl.second.filename << endl;
    TFile *file = new TFile( directory + "/" + smpl.second.filename );
    TTree *tree = (TTree*) file->Get("TauCheck");

    smpl.second.hist_1d          = new TH1D(smpl.second.name + "_os_1d"         , "" , em_cat_in_use.nbins_1d , em_cat_in_use.bins_1d );
    smpl.second.histSS_1d        = new TH1D(smpl.second.name + "_ss_1d"         , "" , em_cat_in_use.nbins_1d , em_cat_in_use.bins_1d );
    smpl.second.histSSrelaxed_1d = new TH1D(smpl.second.name + "_ss_relaxed_1d" , "" , em_cat_in_use.nbins_1d , em_cat_in_use.bins_1d );

    smpl.second.hist_2d          = new TH2D(smpl.second.name + "_os_2d"         , "" , em_cat_in_use.nbins_x_2d , em_cat_in_use.bins_x_2d , em_cat_in_use.nbins_y_2d , em_cat_in_use.bins_y_2d );
    smpl.second.histSS_2d        = new TH2D(smpl.second.name + "_ss_2d"         , "" , em_cat_in_use.nbins_x_2d , em_cat_in_use.bins_x_2d , em_cat_in_use.nbins_y_2d , em_cat_in_use.bins_y_2d );
    smpl.second.histSSrelaxed_2d = new TH2D(smpl.second.name + "_ss_relaxed_2d" , "" , em_cat_in_use.nbins_x_2d , em_cat_in_use.bins_x_2d , em_cat_in_use.nbins_y_2d , em_cat_in_use.bins_y_2d );

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
      sys.second.hist_1d          = new TH1D(sys.second.name + "_os_1d"         , "" , em_cat_in_use.nbins_1d , em_cat_in_use.bins_1d );
      sys.second.histSS_1d        = new TH1D(sys.second.name + "_ss_1d"         , "" , em_cat_in_use.nbins_1d , em_cat_in_use.bins_1d );
      sys.second.histSSrelaxed_1d = new TH1D(sys.second.name + "_ss_relaxed_1d" , "" , em_cat_in_use.nbins_1d , em_cat_in_use.bins_1d );

      sys.second.hist_2d          = new TH2D(sys.second.name + "_os_2d"         , "" , em_cat_in_use.nbins_x_2d , em_cat_in_use.bins_x_2d , em_cat_in_use.nbins_y_2d , em_cat_in_use.bins_y_2d );
      sys.second.histSS_2d        = new TH2D(sys.second.name + "_ss_2d"         , "" , em_cat_in_use.nbins_x_2d , em_cat_in_use.bins_x_2d , em_cat_in_use.nbins_y_2d , em_cat_in_use.bins_y_2d );
      sys.second.histSSrelaxed_2d = new TH2D(sys.second.name + "_ss_relaxed_2d" , "" , em_cat_in_use.nbins_x_2d , em_cat_in_use.bins_x_2d , em_cat_in_use.nbins_y_2d , em_cat_in_use.bins_y_2d );

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
    if( smpl.first == "ggH"  ||
	smpl.first == "qqH"  ||
	smpl.first == "Data" ||
	smpl.first == "QCD"    ) continue;
    sample_map["QCD"].histSS_1d        -> Add( smpl.second.histSS_1d , -1 );
    sample_map["QCD"].histSSrelaxed_1d -> Add( smpl.second.histSSrelaxed_1d , -1 );
  }
  sample_map["QCD"].hist_1d = (TH1D*) sample_map["QCD"].histSSrelaxed_1d -> Clone();

  // 2.) Calculate normalization via ss/ss_relaxed
  double qcd_norm = sample_map["QCD"].histSS_1d->GetSumOfWeights()/sample_map["QCD"].histSSrelaxed_1d->GetSumOfWeights();
  cout << endl << "qcd_norm = " << qcd_norm << endl << endl;
  sample_map["QCD"].hist_1d -> Scale(qcd_norm);

  //************************************************************************************************
  // Write all histograms to output file
  cout << endl << endl << "... Writing histograms to output file ... " << endl;

  TString rootFileName = "htt_em.inputs-sm-13TeV-";
  variable_1d.ReplaceAll(" ","");
  variable_2d.ReplaceAll(" ","");
  if(!plot_2d) rootFileName += variable_1d + ".root";
  else         rootFileName += variable_2d( 0 , variable_2d.First(":") ) + "-" + variable_2d( variable_2d.First(":")+1 , variable_2d.Length() ) + ".root";
  TFile * fileOut      = new TFile( "output/" + rootFileName , "RECREATE" );
  fileOut             -> mkdir(em_cat_in_use.name);
  fileOut             -> cd(em_cat_in_use.name);

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
  cout << "output file : " << rootFileName << endl << endl;
}
