#include "produceGoFInput.h"


void produceGoFInput(TString directory = "../../Inputs/NTuples_2016_rasp/") {


  SetStyle();

  double lumi = 35900;
  bool verbose = true;

  bool logY = true;
  bool blindData = false;

  TString xtitle = "bins";
  TString ytitle = "Events / bin";


  // Weights
  TString Weight = "mcweight*puweight*effweight*0.978824*0.985*";
  TString qcdweight("2.30*");
  //  TString topweight("topptweight*");
  TString topweight("topptweightRun2*");
  TString zptmassweight("zptmassweight*");

  float OSSS_inclusive = 2.3;
  float OSSS_categ = 2.3;
  float OSSS_categ_err = 0.26;
  TString ggScaleWeightUp="(0.9421 - 0.00001699*pt_2)*";
  TString ggScaleWeightDown="(1.0579 + 0.00001699*pt_2)*";
  // Drell-Yan corrections (in slices of mjj/pt_sv)
  float dyCorr2D[10];
  TString labelsBins[10];
  float yeThreshold = 1;
  for (int iDY=0; iDY<10; ++iDY)
    dyCorr2D[iDY] = 1;

  // Definition of cuts
  TString btagVeto("&&nbtag==0");
  TString btagVetoUp("&&nbtag==0");
  TString btagVetoDown("&&nbtag==0");
  TString mistagVetoUp("&&nbtag==0");
  TString mistagVetoDown("&&nbtag==0");

  TString mTCut("&&mTdileptonMET<60");
  TString CutsKine= "&&pt_1>13&&pt_2>15&&TMath::Max(pt_1,pt_2)>24";
  CutsKine += mTCut;

  TString CutsIso          = "&&iso_1<0.15&&iso_2<0.2&&extraelec_veto<0.5&&extramuon_veto<0.5";
  TString CutsIsoSS        = "&&iso_1<0.50&&iso_2>0.2&&iso_2<0.5&&extraelec_veto<0.5&&extramuon_veto<0.5";
  
  TString CutsCategory             = "&&dzeta>-35";
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
  em_inclusive.binsX = new float[13]{0,50,55,60,65,70,75,80,85,90,95,100,400};
  em_inclusive.binsY = new float[7]{15,20,25,30,35,40,300};
  em_inclusive.variable = "m_vis:pt_2";

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

  vector<Sample> sample_vec= { Data , ZTT , ZLL , EWKZ , W , TT , ST , VV , QCD , ggH , VBFH };
  
  if(verbose){
    cout << endl << endl << "... Background classes ... "<< endl ;
    for(Sample & smpl : sample_vec ){
      cout << " - " << smpl.name << endl;
    }
  }

  // Define common cut strings  
  for(Sample & smpl : sample_vec){
    
    smpl.cutString          = "(os>0.5"+Cuts+")";
    smpl.weightString       = Weight;
    smpl.cutStringSS        = "(os<0.5"+Cuts+")";
    smpl.weightStringSS     = Weight+qcdweight;
    smpl.cutStringSSrelaxed = "(os<0.5"+CutsSS+")";
    smpl.weightStringSSrelaxed = Weight+qcdweight;
    smpl.variable = "m_vis : pt_2";
  }

  // Define sample specific cuts
  Data.weightString = "";
  ZTT.weightString += zptmassweight;
  ZTT.cutString += "&&isZTT";
  ZLL.weightString += zptmassweight;
  ZLL.cutString += "&&!isZTT";
  TT.weightString   += topweight;
  QCD.weightString  += qcdweight;
  QCD.cutString += "(os<0.5"+CutsSS+")";

  Data.weightStringSS = "";
  ZTT.weightStringSS += zptmassweight;
  ZTT.cutStringSS += "&&isZTT";
  ZLL.weightStringSS += zptmassweight;
  ZLL.cutStringSS += "&&!isZTT";
  TT.weightStringSS += topweight;

  Data.weightStringSSrelaxed = "";
  ZTT.weightStringSSrelaxed += zptmassweight;
  ZTT.cutStringSSrelaxed += "&&isZTT";
  ZLL.weightStringSSrelaxed += zptmassweight;
  ZLL.cutStringSSrelaxed += "&&!isZTT";
  TT.weightStringSSrelaxed += topweight;
  //************************************************************************************************

  //************************************************************************************************
  // Define systematic uncertainties

  // Uncertainties common for all samples
  for(Sample & smpl : sample_vec){

    if( smpl.name == "Data" || smpl.name == "QCD" ) continue;

    // 1.) Electron scale
    Sample eScaleUp   = smpl;
    Sample eScaleDown = smpl;
    smpl.uncertainties.insert( make_pair("eScaleUp"   , eScaleUp) );
    smpl.uncertainties.insert( make_pair("eScaleDown" , eScaleDown) );
    smpl.uncertainties["eScaleUp"].name += "_CMS_scale_e_em_13TeVUp";
    smpl.uncertainties["eScaleDown"].name += "_CMS_scale_e_em_13TeVDown";
    smpl.uncertainties["eScaleUp"].cutString.ReplaceAll("dzeta","dzeta_eUp");
    smpl.uncertainties["eScaleUp"].cutString.ReplaceAll("pt_1","pt_Up_1");
    smpl.uncertainties["eScaleUp"].cutString.ReplaceAll("mTdileptonMET","mTdileptonMET_eUp");
    smpl.uncertainties["eScaleUp"].variable.ReplaceAll("m_vis","m_vis_eUp");
    smpl.uncertainties["eScaleUp"].variable.ReplaceAll("m_sv","m_sv_eUp");
    smpl.uncertainties["eScaleDown"].cutString.ReplaceAll("dzeta","dzeta_eDown");
    smpl.uncertainties["eScaleDown"].cutString.ReplaceAll("pt_1","pt_Down_1");
    smpl.uncertainties["eScaleDown"].cutString.ReplaceAll("mTdileptonMET","mTdileptonMET_eDown");
    smpl.uncertainties["eScaleDown"].variable.ReplaceAll("m_vis","m_vis_eDown");
    smpl.uncertainties["eScaleDown"].variable.ReplaceAll("m_sv","m_sv_eDown");

    // 2.) JES
    Sample jScaleUp = smpl;
    Sample jScaleDown = smpl;
    smpl.uncertainties.insert( make_pair("jScaleUp"   , jScaleUp) );
    smpl.uncertainties.insert( make_pair("jScaleDown" , jScaleDown) );
    smpl.uncertainties["jScaleUp"].name += "_CMS_scale_j_em_13TeVUp";
    smpl.uncertainties["jScaleDown"].name += "_CMS_scale_j_em_13TeVDown";
    smpl.uncertainties["jScaleUp"].cutString.ReplaceAll("njets","njets_Up");
    smpl.uncertainties["jScaleUp"].cutString.ReplaceAll("mjj","mjj_Up");
    smpl.uncertainties["jScaleUp"].variable.ReplaceAll("mjj","mjj_Up");
    smpl.uncertainties["jScaleDown"].cutString.ReplaceAll("njets","njets_Down");
    smpl.uncertainties["jScaleDown"].cutString.ReplaceAll("mjj","mjj_Down");
    smpl.uncertainties["jScaleDown"].variable.ReplaceAll("mjj","mjj_Down");

    // 3.) MET scale
    Sample metScaleUp = smpl;
    Sample metScaleDown = smpl;
    smpl.uncertainties.insert( make_pair("metScaleUp"   , metScaleUp) );
    smpl.uncertainties.insert( make_pair("metScaleDown" , metScaleDown) );
    smpl.uncertainties["metScaleUp"].name += "_CMS_scale_met_em_13TeVUp";
    smpl.uncertainties["metScaleDown"].name += "_CMS_scale_met_em_13TeVDown";
    smpl.uncertainties["metScaleUp"].cutString.ReplaceAll("dzeta","dzeta_scaleUp");
    smpl.uncertainties["metScaleDown"].cutString.ReplaceAll("dzeta","dzeta_scaleDown");

    // 4.) MET resolution
    Sample metResoUp = smpl;
    Sample metResoDown = smpl;
    smpl.uncertainties.insert( make_pair("metResoUp"   , metResoUp) );
    smpl.uncertainties.insert( make_pair("metResoDown" , metResoDown) );
    smpl.uncertainties["metResoUp"].name += "_CMS_reso_met_em_13TeVUp";
    smpl.uncertainties["metResoDown"].name += "_CMS_reso_met_em_13TeVDown";
    smpl.uncertainties["metResoUp"].cutString.ReplaceAll("dzeta","dzeta_resoUp");
    smpl.uncertainties["metResoDown"].cutString.ReplaceAll("dzeta","dzeta_resoDown");

    // 5.) B-tag efficiency
    Sample bEffUp = smpl;
    Sample bEffDown = smpl;
    smpl.uncertainties.insert( make_pair("bEffUp"   , bEffUp) );
    smpl.uncertainties.insert( make_pair("bEffDown" , bEffDown) );
    smpl.uncertainties["bEffUp"].name += "_CMS_eff_b_13TeVUp";
    smpl.uncertainties["bEffDown"].name += "_CMS_eff_b_13TeVDown";
    smpl.uncertainties["bEffUp"].cutString+=btagVetoUp;
    smpl.uncertainties["bEffDown"].cutString+=btagVetoDown;

    // 6.) Mis-tag efficiency
    Sample bFakeUp = smpl;
    Sample bFakeDown = smpl;
    smpl.uncertainties.insert( make_pair("bFakeUp"   , bFakeUp) );
    smpl.uncertainties.insert( make_pair("bFakeDown" , bFakeDown) );
    smpl.uncertainties["bFakeUp"].name += "_CMS_fake_b_13TeVUp";
    smpl.uncertainties["bFakeDown"].name += "_CMS_fake_b_13TeVDown";
    smpl.uncertainties["bFakeUp"].cutString +=mistagVetoUp;
    smpl.uncertainties["bFakeDown"].cutString +=mistagVetoDown;

    // Sample-specific uncertainties
    // 7.) TTbar shape
    if(smpl.name == "TTbar"){
      Sample ttbarShapeUp = smpl;
      Sample ttbarShapeDown = smpl;
      smpl.uncertainties.insert( make_pair("ttbarShapeUp"   , ttbarShapeUp) );
      smpl.uncertainties.insert( make_pair("ttbarShapeDown" , ttbarShapeDown) );
      smpl.uncertainties["ttbarShapeUp"].name += "_CMS_htt_ttbarShape_13TeVUp";
      smpl.uncertainties["ttbarShapeDown"].name += "_CMS_htt_ttbarShape_13TeVDown";
      smpl.uncertainties["ttbarShapeUp"].topweight = "topptweightRun2*topptWeightRun2";
      smpl.uncertainties["ttbarShapeDown"].topweight = "";
    }

    // 8.) DY shape (EWKZ sample should be added here)
    if(smpl.name == "ZTT" || smpl.name == "ZLL" || smpl.name == "EWKZ" ){
      Sample dyShapeUp = smpl;
      Sample dyShapeDown = smpl;
      smpl.uncertainties.insert( make_pair("dyShapeUp"   , dyShapeUp) );
      smpl.uncertainties.insert( make_pair("dyShapeDown" , dyShapeDown) );
      smpl.uncertainties["dyShapeUp"].name += "_CMS_htt_dyShape_13TeVUp";
      smpl.uncertainties["dyShapeDown"].name += "_CMS_htt_dyShape_13TeVDown";
      smpl.uncertainties["dyShapeUp"].zptmassweight="(1.0+1.1*(zptmassweight-1))*";
      smpl.uncertainties["dyShapeDown"].zptmassweight="(1.0+0.9*(zptmassweight-1))*";
    }

    // // 9.) ggScale
    if(smpl.name == "ggH"){
      Sample ggScaleUp = smpl;
      Sample ggScaleDown = smpl;
      smpl.uncertainties.insert( make_pair("ggScaleUp"   , ggScaleUp) );
      smpl.uncertainties.insert( make_pair("ggScaleDown" , ggScaleDown) );
      smpl.uncertainties["ggScaleUp"].name += "_CMS_scale_gg_13TeVUp";
      smpl.uncertainties["ggScaleDown"].name += "_CMS_scale_gg_13TeVDown";
      smpl.uncertainties["ggScaleUp"].ggscaleweight=ggScaleWeightUp;
      smpl.uncertainties["ggScaleDown"].ggscaleweight=ggScaleWeightDown;
    }
  }
   
  if(verbose){
    cout << endl << endl << "... Uncertainties of samples ... " << endl << endl ;
    for(Sample & smpl : sample_vec){
      cout << smpl.name << " : " <<endl;
      for(auto& unc : smpl.uncertainties) cout<<"  - "<<unc.first<<" : "<<unc.second.name<<endl;
    }
  }


  //************************************************************************************************
  // Fill histograms (first open trees)

  // 1.) Fill nominal histograms
  cout << endl << endl << "... Drawing ... " << endl;
  for(Sample & smpl : sample_vec){

    cout << endl << "**************************************" << endl;
    cout << smpl.name << " : " << smpl.filename << endl;
    TFile *file = new TFile( directory + "/" + smpl.filename );
    TTree *tree = (TTree*) file->Get("TauCheck");
    const int nBinsX = sizeof(em_inclusive.binsX)/sizeof(float) - 1;
    const int nBinsY = sizeof(em_inclusive.binsY)/sizeof(float) - 1;
    smpl.hist = new TH2D(smpl.name + "_os" , "" , nBinsX , em_inclusive.binsX , nBinsY , em_inclusive.binsY );
    if(verbose){
      cout << smpl.name << " : " << smpl.variable << endl;
      cout << smpl.name << " : " << smpl.weightString << endl;
      cout << smpl.name << " : " << smpl.cutString << endl << endl;
    }
    tree -> Draw( smpl.variable + ">>" + smpl.hist->GetName() , smpl.weightString + "*(" + smpl.cutString + ")" );
    // do here the unfolding ???
      
    // now start the loop over the sys uncertainties
    for(auto &sys : smpl.uncertainties){
      if(verbose){
	cout << sys.first << " : " << sys.second.variable << endl;
	cout << sys.first << " : " << sys.second.weightString << endl;
	cout << sys.first << " : " << sys.second.cutString << endl << endl;
      }
      sys.second.hist = new TH2D(sys.second.name + "_os" , "" , nBinsX , em_inclusive.binsX , nBinsY , em_inclusive.binsY );
      tree -> Draw( sys.second.variable + ">>" +  sys.second.hist->GetName() , sys.second.weightString + "*(" + sys.second.cutString + ")" );
    }
  }



  // hist[1]->Add(hist[1],hist[3]); // ZTT low mass + high mass
  // hist[2]->Add(hist[2],hist[4]); // ZLL low mass + high mass
  // for (int iSys=0; iSys<nSys; ++iSys) {
  //   histSys[1][iSys]->Add(histSys[1][iSys],histSys[3][iSys]);
  //   histSys[2][iSys]->Add(histSys[2][iSys],histSys[4][iSys]);
  // }

  // //  adding up single top and VV backgrounds
  // for (int iH=8; iH<19; ++iH) {
  //   hist[7]->Add(hist[7],hist[iH]);
  //   for (int iSys=0; iSys<nSys; ++iSys)
  //     histSys[7][iSys]->Add(histSys[7][iSys],histSys[iH][iSys]);
  // }

  // // adding up W+Jets and W+gamma samples
  // for (int iH=19; iH<22; ++iH) {
  //   hist[5]->Add(hist[5],hist[iH]);
  //   for (int iSys=0; iSys<nSys; ++iSys)
  //     histSys[5][iSys]->Add(histSys[5][iSys],histSys[iH][iSys]);
  // }
  // for (int iH=23; iH<25; ++iH) {
  //   hist[5]->Add(hist[5],hist[iH]);
  //   for (int iSys=0; iSys<nSys; ++iSys)
  //     histSys[5][iSys]->Add(histSys[5][iSys],histSys[iH][iSys]);
  // }

  // std::cout << "Data in SS region = " << histSS[0]->GetSumOfWeights() << std::endl;
  // std::cout << "Data is SS relaxed region = " << histSSrelaxed[0]->GetSumOfWeights() << std::endl;
 
  // // subtracting background from SS
  // for (int iH=1; iH<25; ++iH) {
  //   histSS[0]->Add(histSS[0],histSS[iH],1,-1);
  //   histSSrelaxed[0]->Add(histSSrelaxed[0],histSSrelaxed[iH],1,-1);
  //   std::cout << sampleNames[iH] << " : " << histSSrelaxed[iH]->GetSumOfWeights() << std::endl;
  // }

  // for (int iB=1; iB<=nBins; ++iB) {
  //   float xSS = histSS[0]->GetBinContent(iB);
  //   float xSSrelaxed = histSSrelaxed[0]->GetBinContent(iB);
  //   float xSSErelaxed = histSSrelaxed[0]->GetBinError(iB);
  //   //    if (xSS<0)
  //   //      histSS[0]->SetBinContent(iB,0.);
  //   if (xSSrelaxed<0)
  //     histSSrelaxed[0]->SetBinContent(iB,0.);
  // }

  // // adding up WplusH and WminusH
  // hist[27]->Add(hist[27],hist[28]);
  // for (int iSys=0; iSys<nSys; ++iSys)
  //   histSys[27][iSys]->Add(histSys[27][iSys],histSys[28][iSys]);

  // /*
  // // applying dyExtrapolationFactor
  // for (int iB=1; iB<=nBins; ++iB) {
  //   hist[1]->SetBinContent(iB,dyExtrapolationFactor*hist[1]->GetBinContent(iB));
  //   hist[1]->SetBinError(iB,dyExtrapolationFactor*hist[2]->GetBinError(iB));
  //   hist[2]->SetBinContent(iB,dyExtrapolationFactor*hist[2]->GetBinContent(iB));
  //   hist[2]->SetBinError(iB,dyExtrapolationFactor*hist[2]->GetBinError(iB));
  //   for (int iSys=2; iSys<12; ++iSys) {
  //     histSys[1][iSys]->SetBinContent(iB,dyExtrapolationFactor*histSys[1][iSys]->GetBinContent(iB));
  //     histSys[1][iSys]->SetBinError(iB,dyExtrapolationFactor*histSys[1][iSys]->GetBinError(iB));
  //   }
  // }
  // */

  // // normalizing QCD template

  // float qcdNorm = histSS[0]->GetSumOfWeights();
  // float qcdNormRelaxed = histSSrelaxed[0]->GetSumOfWeights(); 
  // //  float ratioQCD = (qcdNorm / qcdNormRelaxed) * (OSSS_categ / OSSS_inclusive) ;
  // float ratioQCD = qcdNorm / qcdNormRelaxed;

  // std::cout << "qcdNorm = " << qcdNorm << "   QCDnormRelaxed = " << qcdNormRelaxed << "   ratio = " << ratioQCD << std::endl;

  // histSS[0]->SetLineColor(2);
  // histSSrelaxed[0]->SetLineColor(4);

  // float qcdNormE2 = 0;
  // for (int iB=1; iB<=nBins; ++iB) {
  //   float xSSrelaxed = histSSrelaxed[0]->GetBinContent(iB);
  //   float xSSErelaxed = histSSrelaxed[0]->GetBinError(iB);
  //   //    std::cout << "QCD relaxed :  " << iB << "  =  " << xSSrelaxed << " +/- " << xSSErelaxed << std::endl;
  //   float error = histSS[0]->GetBinError(iB);
  //   qcdNormE2 += error*error;
  //   float xHist = ratioQCD*histSSrelaxed[0]->GetBinContent(iB);
  //   float eHist = ratioQCD*histSSrelaxed[0]->GetBinError(iB);
  //   histSSrelaxed[0]->SetBinContent(iB,xHist);
  //   histSSrelaxed[0]->SetBinError(iB,eHist);
  // }
  // float qcdNormE = TMath::Sqrt(qcdNormE2);
  // float rOSSS    = OSSS_categ_err / OSSS_categ;
  // float rQCD     = qcdNormE / qcdNorm;
  // float qcdLN    = 1 + TMath::Sqrt(rOSSS*rOSSS+rQCD*rQCD); 
  
  // char qcdLN_Char[10];
  // sprintf(qcdLN_Char,"%4.2f",qcdLN);
  // TString qcdLN_TString(qcdLN_Char);

  // TCanvas * c1 = new TCanvas("c1","",600,600);

  // float yMax = 2*histSS[0]->GetMaximum();
  // histSS[0]->GetYaxis()->SetRangeUser(0.,yMax);

  // histSS[0]->Draw();
  // histSSrelaxed[0]->Draw("same");

  // c1->Update();
  // delete c1;

  // std::cout << "Cross check of QCD norm. : " << histSSrelaxed[0]->GetSumOfWeights()/histSS[0]->GetSumOfWeights() << std::endl;

  // //  applying corrections to DY in slices of the second 2D variable 
  // //
  // histSys[1][18] = (TH1D*)hist[1]->Clone("ZTT_2DUp");
  // histSys[1][19] = (TH1D*)hist[1]->Clone("ZTT_2DDown");
  // histSys[2][18] = (TH1D*)hist[2]->Clone("ZL_2DUp");
  // histSys[2][19] = (TH1D*)hist[2]->Clone("ZL_2DDown");
  // histSys[22][18] = (TH1D*)hist[22]->Clone("EWKZ_2DUp");
  // histSys[22][19] = (TH1D*)hist[22]->Clone("EWKZ_2DDown");
  // if (category=="em_vbf"||category=="em_boosted") {
  //   for (int iB=1; iB<=nBins; ++iB) {
  //     int binN = int((iB-1)/nBinsX);
  //     float dySF = dyCorr2D[binN];
  //     for (int iDY=1; iDY<3; ++iDY) { // ZTT and ZL
  // 	float xDY = hist[iDY]->GetBinContent(iB);
  // 	float eDY = hist[iDY]->GetBinError(iB);
  // 	hist[iDY]->SetBinContent(iB,dySF*xDY);
  // 	hist[iDY]->SetBinError(iB,dySF*eDY);
  // 	histSys[iDY][18]->SetBinContent(iB,dySF*dySF*xDY);
  // 	histSys[iDY][18]->SetBinError(iB,dySF*dySF*eDY);
  // 	histSys[iDY][19]->SetBinContent(iB,xDY);
  // 	histSys[iDY][19]->SetBinError(iB,eDY);
  // 	for (int iSys=0; iSys<nSys; ++iSys) {
  // 	  float xDYsys = histSys[iDY][iSys]->GetBinContent(iB);
  // 	  float eDYsys = histSys[iDY][iSys]->GetBinError(iB);
  // 	  histSys[iDY][iSys]->SetBinContent(iB,dySF*xDYsys);
  // 	  histSys[iDY][iSys]->SetBinError(iB,dySF*eDYsys);
  // 	}
  //     }
  //     for (int iDY=22; iDY<23; ++iDY) { // EWKZ
  //       float xDY = hist[iDY]->GetBinContent(iB);
  //       float eDY = hist[iDY]->GetBinError(iB);
  //       hist[iDY]->SetBinContent(iB,dySF*xDY);
  //       hist[iDY]->SetBinError(iB,dySF*eDY);
  //       histSys[iDY][18]->SetBinContent(iB,dySF*dySF*xDY);
  //       histSys[iDY][18]->SetBinError(iB,dySF*dySF*eDY);
  //       histSys[iDY][19]->SetBinContent(iB,xDY);
  //       histSys[iDY][19]->SetBinError(iB,eDY);
  // 	for (int iSys=0; iSys<nSys; ++iSys) {
  //         float xDYsys = histSys[iDY][iSys]->GetBinContent(iB);
  //         float eDYsys = histSys[iDY][iSys]->GetBinError(iB);
  //         histSys[iDY][iSys]->SetBinContent(iB,dySF*xDYsys);
  //         histSys[iDY][iSys]->SetBinError(iB,dySF*eDYsys);
  //       }
  //     }
  //   }
  // }

  // TH1D * histData = (TH1D*)hist[0]->Clone("data_obs");
  // TH1D * QCD = (TH1D*)histSSrelaxed[0]->Clone("QCD");
  // TH1D * ZTT = (TH1D*)hist[1]->Clone("ZTT");
  // TH1D * ZLL = (TH1D*)hist[2]->Clone("ZLL");
  // TH1D * W   = (TH1D*)hist[5]->Clone("W");
  // TH1D * TT  = (TH1D*)hist[6]->Clone("TT");
  // TH1D * VV  = (TH1D*)hist[7]->Clone("VV");
  // TH1D * EWKZ = (TH1D*)hist[22]->Clone("EWKZ");
  // TH1D * ggH125 = (TH1D*)hist[25]->Clone("ggH125");
  // TH1D * qqH125 = (TH1D*)hist[26]->Clone("qqH125");
  // TH1D * WH125 = (TH1D*)hist[27]->Clone("WH125");
  // TH1D * ZH125  = (TH1D*)hist[29]->Clone("ZH125");
  // TH1D * TTH125 = (TH1D*)hist[30]->Clone("TTH125");
  // TH1D * ggHWW125 = (TH1D*)hist[31]->Clone("HWW_gg125");
  // TH1D * qqHWW125 = (TH1D*)hist[32]->Clone("HWW_qq125");

  // TString BaseName = "htt_em.inputs-sm-13TeV_" +Suffix;
  // TString rootFileName = BaseName+".root";
  // TFile * fileInputs = new TFile(rootFileName,"recreate"); 
  // fileInputs->mkdir(category);
  // fileInputs->cd(category);
  // histData->Write("data_obs");
  // TT->Write("TT");
  // ZTT->Write("ZTT");
  // ZLL->Write("ZL");
  // W->Write("W");
  // QCD->Write("QCD");
  // VV->Write("VV");
  // EWKZ->Write("EWKZ");
  // ggH125->Write("ggH125");
  // qqH125->Write("qqH125");
  // WH125->Write("WH125");
  // ZH125->Write("ZH125");
  // TTH125->Write("TTH125");
  // ggHWW125->Write("HWW_gg125");
  // qqHWW125->Write("HWW_qq125");

  // for (int iSys=2; iSys<nSys; ++iSys) {
  //   histSys[1][iSys]->Write("ZTT"+sysName[iSys]);
  //   histSys[2][iSys]->Write("ZL"+sysName[iSys]);
  //   histSys[5][iSys]->Write("W"+sysName[iSys]);
  //   histSys[6][iSys]->Write("TT"+sysName[iSys]);
  //   histSys[7][iSys]->Write("VV"+sysName[iSys]);
  //   histSys[22][iSys]->Write("EWKZ"+sysName[iSys]);
  //   histSys[25][iSys]->Write("ggH125"+sysName[iSys]);
  //   histSys[26][iSys]->Write("qqH125"+sysName[iSys]);
  //   histSys[27][iSys]->Write("WH125"+sysName[iSys]);
  //   histSys[29][iSys]->Write("ZH125"+sysName[iSys]);
  //   histSys[30][iSys]->Write("TTH125"+sysName[iSys]);
  //   histSys[31][iSys]->Write("HWW_gg125"+sysName[iSys]);
  //   histSys[32][iSys]->Write("HWW_qq125"+sysName[iSys]);
  // }
  // for (int iSys=12; iSys<14; ++iSys) {
  //   histSys[25][iSys]->Write("ggH125"+sysName[iSys]);
  // }
  // for (int iSys=0; iSys<2; ++iSys) {
  //   histSys[6][iSys]->Write("TT"+sysName[iSys]);
  // }
  // for (int iSys=10; iSys<12; ++iSys) {
  //   histSys[1][iSys]->Write("ZTT"+sysName[iSys]);
  //   histSys[2][iSys]->Write("ZL"+sysName[iSys]);
  //   histSys[22][iSys]->Write("EWKZ"+sysName[iSys]);
	
  // }
  // for (int iSys=18; iSys<20; ++iSys) {
  //   histSys[1][iSys]->Write("ZTT"+sysName[iSys]);
  //   histSys[2][iSys]->Write("ZL"+sysName[iSys]);
  //   histSys[22][iSys]->Write("EWKZ"+sysName[iSys]);
  // }

  // fileInputs->Close();

  // ostringstream str;
  // str << BaseName << ".txt";
  // string nn = str.str();
  // const char * p = nn.c_str();

  // std::ofstream textFile(p);
  // textFile << "imax 1   number of channels" << std::endl;
  // textFile << "jmax *   number of backgrounds" << std::endl;
  // textFile << "kmax *   number of nuisance parameters" << std::endl;
  // textFile << "-----------------" << std::endl;
  // textFile << "observation " << histData->GetSumOfWeights() << std::endl;
  // textFile << "-----------------" << std::endl;
  // textFile << "shapes * * " << rootFileName << "  " << category << "/$PROCESS    " << category << "/$PROCESS_$SYSTEMATIC " << std::endl;
  // textFile << "-----------------" << std::endl;
  // textFile << "bin                              emu      emu    emu    emu    emu    emu    emu    emu    emu" << std::endl;
  // textFile << "process                       qqH125   ggH125    ZTT     TT    QCD      W     VV     ZL   EWKZ" << std::endl;
  // textFile << "process                           -1        0      1      2      3      4      5      6      7" << std::endl;
  // textFile << "rate     " 
  // 	   << qqH125->GetSumOfWeights() << "  "
  // 	   << ggH125->GetSumOfWeights() << "  " 
  // 	   << ZTT->GetSumOfWeights() << "  " 
  // 	   << TT->GetSumOfWeights() << "  "
  // 	   << QCD->GetSumOfWeights() << "  " 
  // 	   << W->GetSumOfWeights() << "  " 
  // 	   << VV->GetSumOfWeights() << "  " 
  // 	   << ZLL->GetSumOfWeights() << "  " 
  // 	   << EWKZ->GetSumOfWeights() << std::endl;
  // textFile << "-----------------------------" << std::endl;
  // textFile << "CMS_eff_m                   lnN   1.02   1.02   1.02   1.02      -   1.02   1.02      -     1.02" << std::endl;
  // textFile << "CMS_eff_e                   lnN   1.02   1.02   1.02   1.02      -   1.02   1.02      -     1.02" << std::endl;
  // textFile << "CMS_htt_ttjNorm             lnN      -      -      -   1.07      -      -      -      -     1.02" << std::endl;
  // textFile << "CMS_htt_wNorm               lnN      -      -      -      -      -   1.15      -      -        -" << std::endl;
  // textFile << "CMS_htt_vvNorm              lnN      -      -      -      -      -      -   1.05      -        -" << std::endl;
  // textFile << "CMS_htt_em_zllNorm          lnN      -      -      -      -      -      -      -   1.20        -" << std::endl;
  // textFile << "CMS_htt_qcdNorm_" << category << "  lnN      -      -      -      -   " << qcdLN_TString << "      -      -      -       -" << std::endl;
  // textFile << "lumi_13TeV                  lnN   1.06   1.06   1.06   1.06      -   1.06   1.06   1.06        -" << std::endl;
  // textFile << "CMS_scale_e_em_13TeV      shape   1.00   1.00   1.00   1.00      -   1.00   1.00      -     1.00" << std::endl;
  // textFile << "CMS_htt_ttbarShape_13TeV  shape      -      -      -   1.00      -      -      -      -        -" << std::endl;
  // textFile << "CMS_htt_dyShape_13TeV     shape      -      -   1.00      -      -      -      -      -        -" << std::endl;
  // textFile << "CMS_scale_j_13TeV         shape   1.00   1.00   1.00   1.00      -      -   1.00      -        -" << std::endl;

  // if (category=="em_vbf") {
  //   textFile << "CMS_htt_boson_scale_met     lnN   1.03   1.03   1.03      -      -   1.03      -      -     1.03" << std::endl;
  //   textFile << "CMS_htt_vvTop_scale_met     lnN      -      -      -   1.04      -      -   1.04      -        -" << std::endl;
  //   textFile << "CMS_eff_b                   lnN      -      -      -   1.04      -      -      -      -        -" << std::endl;
  //   textFile << "CMS_fake_b                  lnN  1.002  1.005  1.007      -      -  1.002  1.004      -    1.005" << std::endl;
  // }
  // if (category=="em_boosted") {
  //   textFile << "CMS_htt_boson_scale_met     lnN  1.014  1.004  1.003      -      -  1.005      -      -    1.004" << std::endl;
  //   textFile << "CMS_htt_vvTop_scale_met     lnN      -      -      -   1.03      -      -   1.02      -        -" << std::endl;
  //   textFile << "CMS_eff_b                   lnN      -      -      -   1.03      -      -      -      -        -" << std::endl;
  //   textFile << "CMS_fake_b                  lnN  1.002  1.003      -      -      -      -  1.005      -    1.005" << std::endl;
  // }
  // if (category=="em_0jet") {
  //   textFile << "CMS_htt_boson_scale_met     lnN  1.016  1.001  1.001      -      -  1.003      -      -    1.004" << std::endl;
  //   textFile << "CMS_htt_vvTop_scale_met     lnN      -      -      -  1.015      -      -  1.004      -        -" << std::endl;
  //   textFile << "CMS_eff_b                   lnN      -      -      -  1.012      -      -      -      -        -" << std::endl;
  // }

  // std::cout << "QCD  : " << QCD->GetSumOfWeights() << std::endl;
  // std::cout << "VV   : " << VV->GetSumOfWeights() << std::endl;
  // std::cout << "W    : " << W->GetSumOfWeights() << std::endl;
  // std::cout << "TT   : " << TT->GetSumOfWeights() << std::endl;
  // std::cout << "ZLL  : " << ZLL->GetSumOfWeights() << std::endl;
  // std::cout << "ZTT  : " << ZTT->GetSumOfWeights() << std::endl;
  // std::cout << "EWKZ : " << EWKZ->GetSumOfWeights() << std::endl;

  // TH1D * dummy = (TH1D*)ZTT->Clone("dummy");
  // float errLumi = 0.0;
  // float errMuon = 0.0;
  // float errElectron = 0.0;
  // float errQCD = 0.0;
  // float errVV = 0.0;
  // float errW = 0.0;
  // float errTT = 0.0;
  // for (int iB=1; iB<=nBins; ++iB) {
  //   float eQCD = errQCD*QCD->GetBinContent(iB);
  //   float eVV = errVV*VV->GetBinContent(iB);
  //   float eW = errW*W->GetBinContent(iB);
  //   float eTT = errTT*TT->GetBinContent(iB);
  //   float err2 = eQCD*eQCD + eVV*eVV + eW*eW + eTT*eTT;
  //   float errTot = TMath::Sqrt(err2);
  //   dummy->SetBinError(iB,errTot);
  // }

  // ZTT->Add(ZTT,EWKZ);
  // VV->Add(VV,QCD);
  // W->Add(W,VV);
  // TT->Add(TT,W);
  // ZLL->Add(ZLL,TT);
  // ZTT->Add(ZTT,ZLL);
  // std::cout << "BKG : " << ZTT->GetSumOfWeights() << std::endl;
  // std::cout << "DAT : " << histData->GetSumOfWeights() << std::endl;

  // TH1D * bkgdErr = (TH1D*)ZTT->Clone("bkgdErr");
  // bkgdErr->SetFillStyle(3013);
  // bkgdErr->SetFillColor(1);
  // bkgdErr->SetMarkerStyle(21);
  // bkgdErr->SetMarkerSize(0);  
  
  
  // for (int iB=1; iB<=nBins; ++iB) {
  //   QCD->SetBinError(iB,0);
  //   VV->SetBinError(iB,0);
  //   TT->SetBinError(iB,0);
  //   W->SetBinError(iB,0);
  //   ZLL->SetBinError(iB,0);
  //   ZTT->SetBinError(iB,0);
  //   float eStat =  bkgdErr->GetBinError(iB);
  //   float X = bkgdErr->GetBinContent(iB);
  //   float eLumi = errLumi * X;
  //   float eMuon = errMuon * X;
  //   float eElectron = errElectron * X;
  //   float eBkg = dummy->GetBinError(iB);
  //   float Err = TMath::Sqrt(eStat*eStat+eLumi*eLumi+eBkg*eBkg+eMuon*eMuon+eElectron*eElectron);
  //   //    bkgdErr->SetBinError(iB,Err);
  //   //    bkgdErr->SetBinContent(iB,ZTT->GetBinContent(iB));
  //   //    std::cout << iB << "  :  " << bkgdErr->GetBinContent(iB) << " " << bkgdErr->GetBinError(iB) << std::endl;
  //   ggH125->SetBinError(iB,0);
  //   qqH125->SetBinError(iB,0);
  // }
    

  // ggH125->SetLineColor(kRed);
  // qqH125->SetLineColor(kBlue);

  // ggH125->Scale(10);
  // qqH125->Scale(10);

  // InitData(histData); histData->GetXaxis()->SetRangeUser(xmin,xmax);
  // InitHist(QCD,TColor::GetColor("#FFCCFF"));
  // InitHist(W,TColor::GetColor("#DE5A6A"));
  // InitHist(TT,TColor::GetColor("#9999CC"));
  // InitHist(VV,TColor::GetColor("#6F2D35"));
  // InitHist(ZLL,TColor::GetColor("#4496C8"));
  // InitHist(ZTT,TColor::GetColor("#FFCC66"));

  // histData->GetXaxis()->SetTitle(xtitle);
  // histData->GetYaxis()->SetTitle(ytitle);
  // histData->GetYaxis()->SetTitleOffset(0.7);
  // histData->GetYaxis()->SetTitleSize(0.06);
  // float yUpper = histData->GetMaximum();
  // if (logY)
  //   histData->GetYaxis()->SetRangeUser(0.5,10*yUpper);
  // else
  //   histData->GetYaxis()->SetRangeUser(0,1.2*yUpper);

  // float yMin = 0.5;
  // yMax = 10*yUpper;

  // histData->SetMarkerSize(1.2);
  // histData->GetXaxis()->SetLabelSize(0);
  // histData->GetYaxis()->SetLabelSize(0.06);
  // histData->GetXaxis()->SetTickLength(0.07);
  // histData->GetYaxis()->SetTickLength(0.02);

  // //  nData = histData->GetSum();
  // //  float nMC   = TT->GetSum();
  // //  float eData = TMath::Sqrt(nData);

  // float chi2 = 0;
  // for (int iB=1; iB<=nBins; ++iB) {
  //   float xData = histData->GetBinContent(iB);
  //   float xMC = ZTT->GetBinContent(iB);
  //   float eMC = bkgdErr->GetBinError(iB);
  //   if (xMC>1e-1) {
  //     float diff2 = (xData-xMC)*(xData-xMC);
  //     chi2 += diff2/(xMC+eMC*eMC);
  //   }
  //   float Signal = ggH125->GetBinContent(iB) + qqH125->GetBinContent(iB);
  //   float ye = Signal/TMath::Sqrt(xMC+(0.1*xMC)*(0.1*xMC));
  //   if (ye>yeThreshold&&blindData) {
  //     histData->SetBinContent(iB,100000000);
  //     histData->SetBinError(iB,0);
  //   }

  // }
  // std::cout << std::endl;
  // std::cout << "Chi2/ndf = " << chi2 << "/" << nBins << " = " << chi2/float(nBins) << std::endl;
  // std::cout << std::endl;



  // TCanvas * canv1 = MakeCanvas("canv1", "", 1200, 600);
  // TPad *upper = new TPad("upper", "pad",0,0.31,1,1);
  // upper->Draw();
  // upper->cd();
  // upper->SetFillColor(0);
  // upper->SetBorderMode(0);
  // upper->SetBorderSize(10);
  // upper->SetTickx(1);
  // upper->SetTicky(1);
  // upper->SetLeftMargin(0.12);
  // upper->SetRightMargin(0.20);
  // upper->SetBottomMargin(0.02);
  // upper->SetFrameFillStyle(0);
  // upper->SetFrameLineStyle(0);
  // upper->SetFrameLineWidth(2);
  // upper->SetFrameBorderMode(0);
  // upper->SetFrameBorderSize(10);
  // upper->SetFrameFillStyle(0);
  // upper->SetFrameLineStyle(0);
  // upper->SetFrameLineWidth(2);
  // upper->SetFrameBorderMode(0);
  // upper->SetFrameBorderSize(10);

  // histData->Draw("e1");
  // ZTT->Draw("sameh");
  // ZLL->Draw("sameh");
  // TT->Draw("sameh");
  // W->Draw("sameh");
  // //  VV->Draw("sameh");
  // QCD->Draw("sameh");
  // histData->Draw("e1same");
  // bkgdErr->Draw("e2same");
  // ggH125->Draw("hsame");
  // qqH125->Draw("hsame");

  // TLegend * leg = new TLegend(0.85,0.0,1.0,0.9);
  // //  TLegend * leg = new TLegend(0.3,0.4,0.52,0.78);
  // SetLegendStyle(leg);
  // leg->SetTextSize(0.05);
  // leg->AddEntry(histData,"Data","lp");
  // leg->AddEntry(ZTT,"Z#rightarrow #tau#tau","f");
  // leg->AddEntry(ZLL,"Z#rightarrow ll","f");
  // leg->AddEntry(TT,"t#bar{t}","f");
  // leg->AddEntry(W,"electroweak","f");
  // //  leg->AddEntry(VV,"electroweak","f");
  // leg->AddEntry(QCD,"QCD","f");
  // leg->AddEntry(ggH125,"ggH(x10)","l");
  // leg->AddEntry(qqH125,"qqH(x10)","l");
  // leg->Draw();
  // writeExtraText = true;
  // extraText = "Preliminary";
  // CMS_lumi(upper,4,33); 
  // plotchannel("e#mu",0.3,0.92);

  // char KT[100];
  // sprintf(KT,"#chi^{2} : %5.1f",chi2);
  // TLatex * cms = new TLatex(0.25,0.85,KT);
  // cms->SetNDC();
  // cms->SetTextSize(0.05);
  // //  cms->Draw();

  // for (int iY=0; iY<nBinsY-1; ++iY) {
  //   float xLine = float(iY+1)*float(nBinsX);
  //   float xTex  = float(iY+0.2)*float(nBinsX);
  //   TLine * line = new TLine(xLine,yMin,xLine,yMax);
  //   line->SetLineStyle(3);
  //   line->Draw();
  //   TLatex * binTex = new TLatex(xTex,yMax*0.3,labelsBins[iY]);
  //   //    binTex->SetNDC();
  //   binTex->SetTextSize(0.05);
  //   binTex->Draw();
  // }

  // if (logY) upper->SetLogy(true);
    
  // upper->Draw("SAME");
  // upper->RedrawAxis();
  // upper->Modified();
  // upper->Update();
  // canv1->cd();

  // TH1D * ratioH = (TH1D*)histData->Clone("ratioH");
  // TH1D * ratioErrH = (TH1D*)bkgdErr->Clone("ratioErrH");
  // ratioH->SetMarkerColor(1);
  // ratioH->SetMarkerStyle(20);
  // ratioH->SetMarkerSize(1.2);
  // ratioH->SetLineColor(1);
  // ratioH->GetYaxis()->SetRangeUser(0.3,1.7);
  // ratioH->GetYaxis()->SetNdivisions(505);
  // ratioH->GetXaxis()->SetLabelFont(42);
  // ratioH->GetXaxis()->SetLabelOffset(0.04);
  // ratioH->GetXaxis()->SetLabelSize(0.14);
  // ratioH->GetXaxis()->SetTitleSize(0.13);
  // ratioH->GetXaxis()->SetTitleOffset(1.1);
  // ratioH->GetYaxis()->SetTitle("obs/exp");
  // ratioH->GetYaxis()->SetLabelFont(42);
  // ratioH->GetYaxis()->SetLabelOffset(0.015);
  // ratioH->GetYaxis()->SetLabelSize(0.13);
  // ratioH->GetYaxis()->SetTitleSize(0.14);
  // ratioH->GetYaxis()->SetTitleOffset(0.3);
  // ratioH->GetXaxis()->SetTickLength(0.07);
  // ratioH->GetYaxis()->SetTickLength(0.02);
  // ratioH->GetYaxis()->SetLabelOffset(0.01);

  // for (int iB=1; iB<=nBins; ++iB) {
  //   float x1 = histData->GetBinContent(iB);
  //   float x2 = ZTT->GetBinContent(iB);
  //   ratioErrH->SetBinContent(iB,1.0);
  //   ratioErrH->SetBinError(iB,0.0);
  //   float xBkg = bkgdErr->GetBinContent(iB);
  //   float errBkg = bkgdErr->GetBinError(iB);
  //   if (xBkg>0) {
  //     float relErr = errBkg/xBkg;
  //     ratioErrH->SetBinError(iB,relErr);

  //   }
  //   if (x1>0&&x2>0) {
  //     float e1 = histData->GetBinError(iB);
  //     float ratio = x1/x2;
  //     float eratio = e1/x2;
  //     ratioH->SetBinContent(iB,ratio);
  //     ratioH->SetBinError(iB,eratio);
  //   }
  //   else {
  //     ratioH->SetBinContent(iB,1000);
  //   }
  // }

  // // ------------>Primitives in pad: lower
  // TPad * lower = new TPad("lower", "pad",0,0,1,0.30);
  // lower->Draw();
  // lower->cd();
  // lower->SetFillColor(0);
  // lower->SetBorderMode(0);
  // lower->SetBorderSize(10);
  // lower->SetGridy();
  // lower->SetTickx(1);
  // lower->SetTicky(1);
  // lower->SetLeftMargin(0.12);
  // lower->SetRightMargin(0.20);
  // lower->SetTopMargin(0.026);
  // lower->SetBottomMargin(0.35);
  // lower->SetFrameFillStyle(0);
  // lower->SetFrameLineStyle(0);
  // lower->SetFrameLineWidth(2);
  // lower->SetFrameBorderMode(0);
  // lower->SetFrameBorderSize(10);
  // lower->SetFrameFillStyle(0);
  // lower->SetFrameLineStyle(0);
  // lower->SetFrameLineWidth(2);
  // lower->SetFrameBorderMode(0);
  // lower->SetFrameBorderSize(10);

  // ratioH->Draw("e1");
  // ratioErrH->Draw("e2same");

  // lower->Modified();
  // lower->RedrawAxis();
  // canv1->cd();
  // canv1->Modified();
  // canv1->cd();
  // canv1->SetSelected(canv1);

  // canv1->Print("figures/"+Suffix+".png");
  // //  canv1->Print("figures/"+Suffix+".pdf","Portrait pdf");

}
