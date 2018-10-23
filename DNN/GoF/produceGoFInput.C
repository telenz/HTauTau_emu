#include "produceGoFInput.h"
#include "TROOT.h"

void produceGoFInput(TString directory = "../../Inputs/NTuples_2016_rasp/") {

  gROOT->SetBatch(kTRUE);
  SetStyle();

  bool verbose = true;

  //************************************************************************************************
  // Define some common weights and cuts
  TString Weight    = "mcweight*puweight*effweight*0.978824*0.985*";
  TString qcdweight = "2.30*";

  // Definition of cuts
  TString mTCut    = "&&mTdileptonMET<60";
  TString CutsKine = "&&pt_1>13&&pt_2>15&&TMath::Max(pt_1,pt_2)>24";
  CutsKine += mTCut;

  TString CutsIso   = "&&iso_1<0.15&&iso_2<0.2&&extraelec_veto<0.5&&extramuon_veto<0.5";
  TString CutsIsoSS = "&&iso_1<0.50&&iso_2>0.2&&iso_2<0.5&&extraelec_veto<0.5&&extramuon_veto<0.5";

  TString btagVeto     = "&&nbtag==0";
  TString CutsCategory = "&&dzeta>-35";
  CutsCategory += btagVeto;

  TString Cuts   = CutsKine + CutsIso   + CutsCategory;
  TString CutsSS = CutsKine + CutsIsoSS + CutsCategory;

  //************************************************************************************************
  // Define different category cuts
  // still needs to set ggscaleweight, labelBins, yeThreshols, change variale for eUp and jesUp, OSSS_categ +err

  class Category em_inclusive("em_inclusive");
  class Category em_0jet("em_0jet");
  class Category em_boosted("em_boosted");
  class Category em_vbf("em_vbf");

  // Inclusive category
  em_inclusive.cutString   = CutsKine + CutsIso + CutsCategory;
  em_inclusive.cutStringSS = CutsKine + CutsIsoSS + CutsCategory;
  em_inclusive.nBinsX = 12;
  em_inclusive.nBinsY = 6;
  em_inclusive.binsX = new float[em_inclusive.nBinsX+1]{0,50,55,60,65,70,75,80,85,90,95,100,400};
  em_inclusive.binsY = new float[em_inclusive.nBinsY+1]{15,20,25,30,35,40,300};
  em_inclusive.variable = "m_vis:pt_2";
  em_inclusive.suffix = "inclusive";

  // 0jet category
  CutsCategory   = "&&njets==0&&dzeta>-35";
  CutsIsoSS = "&&iso_1<0.3&&iso_2>0.1&&iso_2<0.3&&extraelec_veto<0.5&&extramuon_veto<0.5";
  em_0jet.cutString   = CutsKine + CutsIso + CutsCategory;
  em_0jet.cutStringSS = CutsKine + CutsIsoSS + CutsCategory;
  em_0jet.binsX = new float[13]{0,50,55,60,65,70,75,80,85,90,95,100,400};
  em_0jet.binsY = new float[7]{15,20,25,30,35,40,300};
  em_0jet.variable   = "m_vis:pt_2";

  // Boosted category
  CutsCategory   = "&&(njets==1 || (njets==2 && mjj<300) || njets>2)&&dzeta>-35";
  CutsIsoSS = "&&iso_1<0.3&&iso_2>0.1&&iso_2<0.3&&extraelec_veto<0.5&&extramuon_veto<0.5";
  em_boosted.cutString   = CutsKine + CutsIso + CutsCategory;
  em_boosted.cutStringSS = CutsKine + CutsIsoSS + CutsCategory;
  em_boosted.binsX = new float[11]{0,80,90,100,110,120,130,140,150,160,300};
  em_boosted.binsY = new float[7]{0,100,150,200,250,300,5000};
  em_boosted.variable   = "m_sv:pt_sv";

  // VBF category
  CutsCategory = "&& njets==2 && mjj>=300 && dzeta>-10";
  CutsIsoSS = "&&iso_1<0.5&&iso_2>0.2&&iso_2<0.5&&extraelec_veto<0.5&&extramuon_veto<0.5";
  em_vbf.cutString   = CutsKine + CutsIso + CutsCategory;
  em_vbf.cutStringSS = CutsKine + CutsIsoSS + CutsCategory;
  em_vbf.binsX = new float[6]{0,95,115,135,155,400};
  em_vbf.binsY = new float[5]{300,700,1100,1500,10000};
  em_vbf.variable   = "m_sv:mjj";

  // Make a vector from these categories
  vector<class Category> category_vec = { em_inclusive };

  //************************************************************************************************
  // Define samples
  Sample Data( "Data"      , "MuonEG_Run2016_dnn_em_v1.root" );
  Sample ZTT(  "ZTT"       , "DYJets_dnn_em_v1.root" );
  Sample ZLL(  "ZLL"       , "DYJets_dnn_em_v1.root" );
  Sample EWKZ( "EWKZ"      , "EWKZ_em_v1.root" );
  Sample W(    "WJets"     , "WJets_dnn_em_v1.root" );
  Sample TT(   "TTbar"     , "TTbar_dnn_em_v1.root" );
  Sample ST(   "SingleTop" , "SingleTop_dnn_em_v1.root" );
  Sample VV(   "Diboson"   , "Diboson_dnn_em_v1.root" );
  Sample QCD(  "QCD"       , "MuonEG_Run2016_dnn_em_v1.root" );
  Sample ggH(  "ggH"       , "ggH_dnn_em_v1.root" );
  Sample VBFH( "VBFH"      , "VBFH_dnn_em_v1.root" );
  
  // Define pre-defined norms
  ZTT.norm = 1.02;
  ZLL.norm = 1.02;
  QCD.norm = 0.503821;

  map<TString,Sample> sample_map = { { "Data" , Data },
				     { "ZTT"  , ZTT } ,
				     { "ZLL"  , ZLL } ,
				     { "EWKZ" , EWKZ } ,
				     { "W"    , W } ,
				     { "TT"   , TT } ,
				     { "ST"   , ST } ,
				     { "VV"   , VV } ,
				     { "QCD"  , QCD },
				     { "ggH"  , ggH },
				     { "VBFH" , VBFH }};

  if(verbose){
    cout << endl << endl << "... Background classes ... "<< endl ;
    for(auto & smpl : sample_map ) cout << " - " << smpl.second.name << endl;
  }

  // Define common cut strings  
  for(auto & smpl : sample_map){
    smpl.second.cutString          = "os>0.5"+Cuts;
    smpl.second.weightString       = Weight;
    smpl.second.cutStringSS        = "os<0.5"+Cuts;
    smpl.second.weightStringSS     = Weight+qcdweight;
    smpl.second.cutStringSSrelaxed = "os<0.5"+CutsSS;
    smpl.second.weightStringSSrelaxed = Weight+qcdweight;
    smpl.second.variable = "m_vis : pt_2";
  }

  // Define sample specific cuts
  sample_map["Data"].weightString = "1*";
  sample_map["ZTT"].cutString += "&&isZTT";
  sample_map["ZLL"].cutString += "&&!isZTT";

  sample_map["Data"].weightStringSS = "1*";
  sample_map["ZTT"].cutStringSS += "&&isZTT";
  sample_map["ZLL"].cutStringSS += "&&!isZTT";

  sample_map["Data"].weightStringSSrelaxed = "1*";
  sample_map["ZTT"].cutStringSSrelaxed += "&&isZTT";
  sample_map["ZLL"].cutStringSSrelaxed += "&&!isZTT";

  // Define sample specific weights
  sample_map["TT"].topweight = "topptweightRun2*";
  sample_map["ZTT"].zptmassweight = "zptmassweight*";
  sample_map["ZLL"].zptmassweight = "zptmassweight*";
  sample_map["EWKZ"].zptmassweight = "zptmassweight*";

  //************************************************************************************************
  // Define systematic uncertainties

  for(auto & smpl : sample_map){

    if( smpl.second.name == "Data" || smpl.second.name == "QCD" ) continue;

    // Uncertainties common for all samples
    // 1.) Electron scale
    Sample eScaleUp   = smpl.second;
    Sample eScaleDown = smpl.second;
    smpl.second.uncertainties.insert( make_pair("eScaleUp"   , eScaleUp) );
    smpl.second.uncertainties.insert( make_pair("eScaleDown" , eScaleDown) );
    smpl.second.uncertainties["eScaleUp"].name += "_CMS_scale_e_em_13TeVUp";
    smpl.second.uncertainties["eScaleDown"].name += "_CMS_scale_e_em_13TeVDown";
    smpl.second.uncertainties["eScaleUp"].cutString.ReplaceAll("dzeta","dzeta_eUp");
    smpl.second.uncertainties["eScaleUp"].cutString.ReplaceAll("pt_1","pt_Up_1");
    smpl.second.uncertainties["eScaleUp"].cutString.ReplaceAll("mTdileptonMET","mTdileptonMET_eUp");
    smpl.second.uncertainties["eScaleUp"].variable.ReplaceAll("m_vis","m_vis_eUp");
    smpl.second.uncertainties["eScaleUp"].variable.ReplaceAll("m_sv","m_sv_eUp");
    smpl.second.uncertainties["eScaleDown"].cutString.ReplaceAll("dzeta","dzeta_eDown");
    smpl.second.uncertainties["eScaleDown"].cutString.ReplaceAll("pt_1","pt_Down_1");
    smpl.second.uncertainties["eScaleDown"].cutString.ReplaceAll("mTdileptonMET","mTdileptonMET_eDown");
    smpl.second.uncertainties["eScaleDown"].variable.ReplaceAll("m_vis","m_vis_eDown");
    smpl.second.uncertainties["eScaleDown"].variable.ReplaceAll("m_sv","m_sv_eDown");

    // 2.) JES
    Sample jScaleUp = smpl.second;
    Sample jScaleDown = smpl.second;
    smpl.second.uncertainties.insert( make_pair("jScaleUp"   , jScaleUp) );
    smpl.second.uncertainties.insert( make_pair("jScaleDown" , jScaleDown) );
    smpl.second.uncertainties["jScaleUp"].name += "_CMS_scale_j_em_13TeVUp";
    smpl.second.uncertainties["jScaleDown"].name += "_CMS_scale_j_em_13TeVDown";
    smpl.second.uncertainties["jScaleUp"].cutString.ReplaceAll("njets","njets_Up");
    smpl.second.uncertainties["jScaleUp"].cutString.ReplaceAll("mjj","mjj_Up");
    smpl.second.uncertainties["jScaleUp"].variable.ReplaceAll("mjj","mjj_Up");
    smpl.second.uncertainties["jScaleDown"].cutString.ReplaceAll("njets","njets_Down");
    smpl.second.uncertainties["jScaleDown"].cutString.ReplaceAll("mjj","mjj_Down");
    smpl.second.uncertainties["jScaleDown"].variable.ReplaceAll("mjj","mjj_Down");

    // 3.) MET scale
    Sample metScaleUp = smpl.second;
    Sample metScaleDown = smpl.second;
    smpl.second.uncertainties.insert( make_pair("metScaleUp"   , metScaleUp) );
    smpl.second.uncertainties.insert( make_pair("metScaleDown" , metScaleDown) );
    smpl.second.uncertainties["metScaleUp"].name += "_CMS_scale_met_em_13TeVUp";
    smpl.second.uncertainties["metScaleDown"].name += "_CMS_scale_met_em_13TeVDown";
    smpl.second.uncertainties["metScaleUp"].cutString.ReplaceAll("dzeta","dzeta_scaleUp");
    smpl.second.uncertainties["metScaleDown"].cutString.ReplaceAll("dzeta","dzeta_scaleDown");

    // 4.) MET resolution
    Sample metResoUp = smpl.second;
    Sample metResoDown = smpl.second;
    smpl.second.uncertainties.insert( make_pair("metResoUp"   , metResoUp) );
    smpl.second.uncertainties.insert( make_pair("metResoDown" , metResoDown) );
    smpl.second.uncertainties["metResoUp"].name += "_CMS_reso_met_em_13TeVUp";
    smpl.second.uncertainties["metResoDown"].name += "_CMS_reso_met_em_13TeVDown";
    smpl.second.uncertainties["metResoUp"].cutString.ReplaceAll("dzeta","dzeta_resoUp");
    smpl.second.uncertainties["metResoDown"].cutString.ReplaceAll("dzeta","dzeta_resoDown");

    // 5.) B-tag efficiency
    TString btagVetoUp     = "&&nbtag==0";
    TString btagVetoDown   = "&&nbtag==0";
    Sample bEffUp = smpl.second;
    Sample bEffDown = smpl.second;
    smpl.second.uncertainties.insert( make_pair("bEffUp"   , bEffUp) );
    smpl.second.uncertainties.insert( make_pair("bEffDown" , bEffDown) );
    smpl.second.uncertainties["bEffUp"].name += "_CMS_eff_b_13TeVUp";
    smpl.second.uncertainties["bEffDown"].name += "_CMS_eff_b_13TeVDown";
    smpl.second.uncertainties["bEffUp"].cutString+=btagVetoUp;
    smpl.second.uncertainties["bEffDown"].cutString+=btagVetoDown;

    // 6.) Mis-tag efficiency
    TString mistagVetoUp   = "&&nbtag==0";
    TString mistagVetoDown = "&&nbtag==0";
    Sample bFakeUp = smpl.second;
    Sample bFakeDown = smpl.second;
    smpl.second.uncertainties.insert( make_pair("bFakeUp"   , bFakeUp) );
    smpl.second.uncertainties.insert( make_pair("bFakeDown" , bFakeDown) );
    smpl.second.uncertainties["bFakeUp"].name += "_CMS_fake_b_13TeVUp";
    smpl.second.uncertainties["bFakeDown"].name += "_CMS_fake_b_13TeVDown";
    smpl.second.uncertainties["bFakeUp"].cutString +=mistagVetoUp;
    smpl.second.uncertainties["bFakeDown"].cutString +=mistagVetoDown;

    // Sample-specific uncertainties
    // 7.) TTbar shape
    if(smpl.second.name == "TTbar"){
      Sample ttbarShapeUp = smpl.second;
      Sample ttbarShapeDown = smpl.second;
      smpl.second.uncertainties.insert( make_pair("ttbarShapeUp"   , ttbarShapeUp) );
      smpl.second.uncertainties.insert( make_pair("ttbarShapeDown" , ttbarShapeDown) );
      smpl.second.uncertainties["ttbarShapeUp"].name += "_CMS_htt_ttbarShape_13TeVUp";
      smpl.second.uncertainties["ttbarShapeDown"].name += "_CMS_htt_ttbarShape_13TeVDown";
      smpl.second.uncertainties["ttbarShapeUp"].topweight = "topptweightRun2*topptweightRun2*";
      smpl.second.uncertainties["ttbarShapeDown"].topweight = "";
    }

    // 8.) DY shape (EWKZ sample should be added here)
    if(smpl.second.name == "ZTT" || smpl.second.name == "ZLL" || smpl.second.name == "EWKZ" ){
      Sample dyShapeUp = smpl.second;
      Sample dyShapeDown = smpl.second;
      smpl.second.uncertainties.insert( make_pair("dyShapeUp"   , dyShapeUp) );
      smpl.second.uncertainties.insert( make_pair("dyShapeDown" , dyShapeDown) );
      smpl.second.uncertainties["dyShapeUp"].name += "_CMS_htt_dyShape_13TeVUp";
      smpl.second.uncertainties["dyShapeDown"].name += "_CMS_htt_dyShape_13TeVDown";
      smpl.second.uncertainties["dyShapeUp"].zptmassweight="(1.0+1.1*(zptmassweight-1))*";
      smpl.second.uncertainties["dyShapeDown"].zptmassweight="(1.0+0.9*(zptmassweight-1))*";
    }

    // // 9.) ggScale
    if(smpl.second.name == "ggH"){
      TString ggScaleWeightUp="(0.9421 - 0.00001699*pt_2)*";
      TString ggScaleWeightDown="(1.0579 + 0.00001699*pt_2)*";

      Sample ggScaleUp = smpl.second;
      Sample ggScaleDown = smpl.second;
      smpl.second.uncertainties.insert( make_pair("ggScaleUp"   , ggScaleUp) );
      smpl.second.uncertainties.insert( make_pair("ggScaleDown" , ggScaleDown) );
      smpl.second.uncertainties["ggScaleUp"].name += "_CMS_scale_gg_13TeVUp";
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
  // Fill histograms (first open trees)

  // 1.) Fill nominal histograms
  cout << endl << endl << "... Drawing ... " << endl;
  for(auto & smpl : sample_map){

    if(smpl.second.name != "Data") continue;
    cout << endl << "**************************************" << endl;
    cout << smpl.second.name << " : " << smpl.second.filename << endl;
    TFile *file = new TFile( directory + "/" + smpl.second.filename );
    TTree *tree = (TTree*) file->Get("TauCheck");

    Float_t binsX[]={0,50,55,60,65,70,75,80,85,90,95,100,400};
    Float_t binsY[]={15,20,25,30,35,40,300};
    const int nBinsX = sizeof(binsX)/sizeof(Float_t)-1;
    const int nBinsY = sizeof(binsY)/sizeof(Float_t)-1;

    smpl.second.hist   = new TH2D(smpl.second.name + "_os" , "" , nBinsX , binsX , nBinsY , binsY );
    smpl.second.histSS = new TH2D(smpl.second.name + "_ss" , "" , nBinsX , binsX , nBinsY , binsY );

    TString full_weight_string    = smpl.second.weightString + smpl.second.topweight + smpl.second.zptmassweight + smpl.second.ggscaleweight;
    TString full_weight_string_ss = smpl.second.weightStringSS + smpl.second.topweight + smpl.second.zptmassweight + smpl.second.ggscaleweight;

    if(verbose){
      cout << "Variable        " << " : " << smpl.second.variable << endl;
      cout << "weight string   " << " : " << full_weight_string << endl;
      cout << "weight string ss" << " : " << full_weight_string_ss << endl;
      cout << "cut string      " << " : " << smpl.second.cutString << endl;
      cout << "cut string ss   " << " : " << smpl.second.cutStringSS << endl << endl;
    }

    tree -> Draw( smpl.second.variable + ">>" + smpl.second.hist->GetName() , full_weight_string + "(" + smpl.second.cutString + ")" );
    tree -> Draw( smpl.second.variable + ">>" + smpl.second.histSS->GetName() , full_weight_string_ss + "(" + smpl.second.cutStringSS + ")" );
    // do here the unfolding ???

    // now start the loop over the sys uncertainties
    for(auto &sys : smpl.second.uncertainties){

      full_weight_string = sys.second.weightString + sys.second.topweight + sys.second.zptmassweight + sys.second.ggscaleweight;
      full_weight_string_ss = sys.second.weightStringSS + sys.second.topweight + sys.second.zptmassweight + sys.second.ggscaleweight;
      if(verbose){
	cout << sys.second.name    << " : " << endl;
	cout << "Variable        " << " : " << sys.second.variable << endl;
	cout << "weight string   " << " : " << full_weight_string << endl;
	cout << "weight string ss" << " : " << full_weight_string_ss << endl;
	cout << "cut string      " << " : " << sys.second.cutString << endl;
	cout << "cut string ss   " << " : " << sys.second.cutStringSS << endl << endl;
      }
      sys.second.hist   = new TH2D(sys.second.name + "_os" , "" , nBinsX , binsX , nBinsY , binsY );
      sys.second.histSS = new TH2D(sys.second.name + "_ss" , "" , nBinsX , binsX , nBinsY , binsY );

      tree -> Draw( sys.second.variable + ">>" +  sys.second.hist->GetName() , full_weight_string + "(" + sys.second.cutString + ")" );
      tree -> Draw( sys.second.variable + ">>" +  sys.second.histSS->GetName() , full_weight_string_ss + "(" + sys.second.cutStringSS + ")" );
    }
  }
  exit(-1);
  //************************************************************************************************
  // Determine QCD background (subtracting other background from the SS region
  cout << endl << endl << "... Determining QCD background ... " << endl;

  sample_map["QCD"].hist = (TH2D*) sample_map["QCD"].histSS -> Clone();

  for(auto & smpl : sample_map){
    if( smpl.second.name == "ggH"  ||
	smpl.second.name == "VBFH" ||
	smpl.second.name == "Data" ||
	smpl.second.name == "QCD"    ) continue;
    sample_map["QCD"].hist -> Add( smpl.second.histSS , -1 );
  }

  // Normalize QCD (FIXME)

  //************************************************************************************************
  // Write all histograms to output file
  cout << endl << endl << "... Writing histograms to output file ... " << endl;

  TString rootFileName = "htt_em.inputs-sm-13TeV_" + em_inclusive.suffix + "_NEW.root";
  TFile * fileOut      = new TFile(rootFileName,"recreate");
  fileOut             -> mkdir(em_inclusive.name);
  fileOut             -> cd(em_inclusive.name);

  for(auto & smpl : sample_map){
    smpl.second.hist -> Write();
    for(auto & sys : smpl.second.uncertainties){
      sys.second.hist->Write();
    }
  }
  fileOut -> Close();

  // Write the final sum of weights of the nominal selection
  cout << endl << " Final histogram content : "<< endl;
  for(auto & smpl : sample_map){
    cout << smpl.second.name << " : " << smpl.second.hist -> GetSumOfWeights() << endl;
  }
  cout << endl;
}
