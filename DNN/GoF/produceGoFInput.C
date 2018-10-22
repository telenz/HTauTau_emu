#include "produceGoFInput.h"


void produceGoFInput(TString directory = "../../Inputs/NTuples_2016_rasp/") {


  SetStyle();

  double lumi = 35900;

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

  class category em_inclusive("em_inclusive");
  class category em_0jet("em_0jet");
  class category em_boosted("em_boosted");
  class category em_vbf("em_vbf");

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
  vector<class category> categoryVec = { em_inclusive };
  //************************************************************************************************

  //************************************************************************************************
  // Define samples
  sample Data( "Data"      , "MuonEG_Run2016.root" );
  sample ZTT(  "ZTT"       , "DYJets_dnn_em_v1.root" );
  sample ZLL(  "ZLL"       , "DYJets_dnn_em_v1.root" );
  sample EWKZ( "EWKZ"      , "EWKZ_dnn_em_v1.root" );
  sample W(    "WJets"     , "WJets_dnn_em_v1.root" );
  sample TT(   "TTbar"     , "TTbar_dnn_em_v1.root" );
  sample ST(   "SingleTop" , "SingleTop_dnn_em_v1.root" );
  sample VV(   "Diboson"   , "Diboson_dnn_em_v1.root" );
  sample QCD(  "QCD"       , "MuonEG_Run2016.root" );
  sample ggH(  "ggH"       , "ggH_dnn_em_v1.root" );
  sample VBFH( "VBFH"      , "VBFH_dnn_em_v1.root" );
  
  // Define pre-defined norms
  ZTT.norm = 1.02;
  ZLL.norm = 1.02;
  QCD.norm = 0.503821;

  vector<sample> sampleVec= { Data , ZTT , ZLL , EWKZ , W , TT , ST , VV , QCD , ggH , VBFH };

  // Define common cut strings  
  for(sample & sampleName : sampleVec){
    sampleName.cutString          = "(os>0.5"+Cuts+")";
    sampleName.weightString       = Weight;
    sampleName.cutStringSS        = "(os<0.5"+Cuts+")";
    sampleName.weightStringSS     = Weight+qcdweight;
    sampleName.cutStringSSrelaxed = "(os<0.5"+CutsSS+")";
    sampleName.weightStringSSrelaxed = Weight+qcdweight;
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

  // for(auto &cat : categoryVec){
  //   cat.sampleList = &sampleVec;
  // }
  //************************************************************************************************

  //************************************************************************************************
  // Define systematic uncertainties

  // Common uncertainties
  // 1.) Electron scale
  sysUncertainty eScaleUp("_CMS_scale_e_em_13TeVUp");
  eScaleUp.cutString.ReplaceAll("dzeta","dzeta_eUp");
  eScaleUp.cutString.ReplaceAll("pt_1","pt_Up_1");
  eScaleUp.cutString.ReplaceAll("mTdileptonMET","mTdileptonMET_eUp");
  sysUncertainty eScaleDown("_CMS_scale_e_em_13TeVDown");
  eScaleDown.cutString.ReplaceAll("dzeta","dzeta_eDown");
  eScaleDown.cutString.ReplaceAll("pt_1","pt_Down_1");
  eScaleDown.cutString.ReplaceAll("mTdileptonMET","mTdileptonMET_eDown");

  // 2.) JES
  sysUncertainty jScaleUp("_CMS_scale_j_em_13TeVUp");
  jScaleUp.cutString.ReplaceAll("njets","njets_Up");
  jScaleUp.cutString.ReplaceAll("mjj","mjj_Up");
  sysUncertainty jScaleDown("_CMS_scale_j_em_13TeVDown");
  jScaleDown.cutString.ReplaceAll("njets","njets_Down");
  jScaleDown.cutString.ReplaceAll("mjj","mjj_Down");

  // 3.) MET scale
  sysUncertainty metScaleUp("_CMS_scale_met_em_13TeVUp");
  metScaleUp.cutString.ReplaceAll("dzeta","dzeta_scaleUp");
  sysUncertainty metScaleDown("_CMS_scale_met_em_13TeVDown");
  metScaleDown.cutString.ReplaceAll("dzeta","dzeta_scaleDown");

  // 4.) MET resolution
  sysUncertainty metResoUp("_CMS_reso_met_em_13TeVUp");
  metResoUp.cutString.ReplaceAll("dzeta","dzeta_resoUp");
  sysUncertainty metResoDown("_CMS_reso_met_em_13TeVDown");
  metResoDown.cutString.ReplaceAll("dzeta","dzeta_resoDown");

  // 5.) B-tag efficiency
  sysUncertainty bEffUp("_CMS_eff_b_13TeVUp");
  bEffUp.cutString+=btagVetoUp;
  sysUncertainty bEffDown("_CMS_eff_b_13TeVDown");
  bEffDown.cutString+=btagVetoDown;

  // 6.) Mis-tag efficiency
  sysUncertainty bFakeUp("_CMS_fake_b_13TeVUp");
  bFakeUp.cutString +=mistagVetoUp;
  sysUncertainty bFakeDown("_CMS_fake_b_13TeVDown");
  bFakeDown.cutString +=mistagVetoDown;

  vector<sysUncertainty> uncVec = { eScaleUp , eScaleUp ,
				    jScaleUp , jScaleDown ,
				    metScaleUp , metScaleDown ,
				    metResoUp , metResoDown ,
				    bEffUp , bEffDown ,
				    bFakeUp , bFakeDown
  };

  for(sample & sampleName : sampleVec) sampleName.uncertainties = uncVec;

  // Sample-specific uncertainties
  // 7.) TTbar shape
  sysUncertainty ttbarshapeUp("_CMS_htt_ttbarShape_13TeVUp");
  ttbarshapeUp.topweight = "topptweightRun2*topptWeightRun2";
  sysUncertainty ttbarshapeDown("_CMS_htt_ttbarShape_13TeVDown");
  ttbarshapeDown.topweight = "";
  TT.uncertainties.push_back(ttbarshapeUp);
  TT.uncertainties.push_back(ttbarshapeDown);

  // 8.) DY shape (EWKZ sample should be added here)
  sysUncertainty dyShapeUp("_CMS_htt_dyShape_13TeVUp");
  dyShapeUp.zptmassweight="(1.0+1.1*(zptmassweight-1))*";
  sysUncertainty dyShapeDown("_CMS_htt_dyShape_13TeVUp");
  dyShapeDown.zptmassweight="(1.0+0.9*(zptmassweight-1))*";
  ZTT.uncertainties.push_back(dyShapeUp);
  ZTT.uncertainties.push_back(dyShapeDown);
  ZLL.uncertainties.push_back(dyShapeUp);
  ZLL.uncertainties.push_back(dyShapeDown);
  
  // 9.) ggScale
  sysUncertainty ggScaleUp("_CMS_scale_gg_13TeVUp");
  ggScaleUp.ggscaleweight=ggScaleWeightUp;
  sysUncertainty ggScaleDown("_CMS_scale_gg_13TeVDown");
  ggScaleDown.ggscaleweight=ggScaleWeightDown;
  ggH.uncertainties.push_back(ggScaleUp);
  ggH.uncertainties.push_back(ggScaleDown);

  //************************************************************************************************
  // Fill histograms (first open trees)

  // 1.) Fill nominal histograms
  for(auto &sample : sampleVec){

    TFile *file = new TFile( directory + sample.filename );
    TTree *tree = (TTree*) file->Get("TauCheck");

    tree -> Draw( sample.variable + ">>" + sample.hist->GetName() , sample.weightString + "*(" + sample.cutString + ")" );
    // do here the unfolding ???
      
    // now start the loop over the sys uncertainties
    for(auto &sys : sample.uncertainties){
      tree -> Draw( sys.variable + ">>" +  sys.hist->GetName() , sys.weightString + "*(" + sys.cutString + ")" );

    }
  }

  // // *******************************
  // // ***** Drell-Yan samples *******
  // // *******************************

  // TH1D * histZtt[10];
  // TH1D * histZttSys[10][20];
  // TH1D * histZttSS[10];
  // TH1D * histZttSSrelaxed[10];

  // TH1D * histZll[10];
  // TH1D * histZllSys[10][20];
  // TH1D * histZllSS[10];
  // TH1D * histZllSSrelaxed[10];

  // TString refSamples[5] = {"DYJetsToLL_M-50_13TeV-madgraphMLM",
  // 			   "DY1JetsToLL_M-50_13TeV-madgraphMLM",
  // 			   "DY2JetsToLL_M-50_13TeV-madgraphMLM",
  // 			   "DY3JetsToLL_M-50_13TeV-madgraphMLM",
  // 			   "DY4JetsToLL_M-50_13TeV-madgraphMLM"
  // };
  // double refXSec[5] = {DYNorm*5765,
  // 		       DYNorm*1.164*1012.5,
  // 		       DYNorm*1.164*332.8,
  // 		       DYNorm*1.164*101.8,
  // 		       DYNorm*1.164*54.8};

  // double refEvents[5] = {0,0,0,0,0};

  // for (int iDY=0; iDY<5; ++iDY) {
  //   TFile * file = new TFile(directory+refSamples[iDY]+".root");
  //   TH1D * histWeightsH = (TH1D*)file->Get("histWeightsH");
  //   refEvents[iDY] = histWeightsH->GetSumOfWeights();
  // }
  // TString dySampleNames[9] = {"DYJetsToLL_M-50_13TeV-madgraphMLM",
  // 			      "DYJetsToLL_M-50_13TeV-madgraphMLM",
  // 			      "DYJetsToLL_M-50_13TeV-madgraphMLM",
  // 			      "DYJetsToLL_M-50_13TeV-madgraphMLM",
  // 			      "DYJetsToLL_M-50_13TeV-madgraphMLM",
  // 			      "DY1JetsToLL_M-50_13TeV-madgraphMLM",
  // 			      "DY2JetsToLL_M-50_13TeV-madgraphMLM",
  // 			      "DY3JetsToLL_M-50_13TeV-madgraphMLM",
  // 			      "DY4JetsToLL_M-50_13TeV-madgraphMLM"
  // };

  // double dyNorm[9];
  // dyNorm[0] = lumi*refXSec[0]/refEvents[0];
  // dyNorm[1] = lumi/(refEvents[0]/refXSec[0]+refEvents[1]/refXSec[1]);
  // dyNorm[2] = lumi/(refEvents[0]/refXSec[0]+refEvents[2]/refXSec[2]);
  // dyNorm[3] = lumi/(refEvents[0]/refXSec[0]+refEvents[3]/refXSec[3]);
  // dyNorm[4] = lumi/(refEvents[0]/refXSec[0]+refEvents[4]/refXSec[4]);
  // dyNorm[5] = lumi/(refEvents[0]/refXSec[0]+refEvents[1]/refXSec[1]);
  // dyNorm[6] = lumi/(refEvents[0]/refXSec[0]+refEvents[2]/refXSec[2]);
  // dyNorm[7] = lumi/(refEvents[0]/refXSec[0]+refEvents[3]/refXSec[3]);
  // dyNorm[8] = lumi/(refEvents[0]/refXSec[0]+refEvents[4]/refXSec[4]);

  // TString npartonCuts[9] = {"&&(npartons==0||npartons>4)",
  // 			    "&&npartons==1",
  // 			    "&&npartons==2",
  // 			    "&&npartons==3",
  // 			    "&&npartons==4",
  // 			    "",
  // 			    "",
  // 			    "",
  // 			    ""
  // };

  // TString cutsZtt[9];
  // TString cutsZttSS[9];
  // TString cutsZttSSrelaxed[9];
  
  // TString cutsZttSys[9][20];

  // TString cutsZll[9];
  // TString cutsZllSS[9];
  // TString cutsZllSSrelaxed[9];
  
  // TString cutsZllSys[9][20];

  // for (int iDY=0; iDY<9; ++iDY) {
  //   cutsZtt[iDY]   = Weight+zptmassweight+"(os>0.5"+Cuts+npartonCuts[iDY]+"&&isZTT>0.5)";
  //   cutsZttSS[iDY] = Weight+zptmassweight+qcdweight+"(os<0.5"+Cuts+npartonCuts[iDY]+"&&isZTT>0.5)";
  //   cutsZttSSrelaxed[iDY] = Weight+zptmassweight+qcdweight+"(os<0.5"+CutsSS+npartonCuts[iDY]+"&&isZTT>0.5)";
  //   cutsZll[iDY]   = Weight+zptmassweight+"(os>0.5"+Cuts+npartonCuts[iDY]+"&&isZTT<0.5)";
  //   cutsZllSS[iDY] = Weight+zptmassweight+qcdweight+"(os<0.5"+Cuts+npartonCuts[iDY]+"&&isZTT<0.5)";
  //   cutsZllSSrelaxed[iDY] = Weight+zptmassweight+qcdweight+"(os<0.5"+CutsSS+npartonCuts[iDY]+"&&isZTT<0.5)";
  //   for (int iSys=0; iSys<nSys; ++iSys) {
  //     cutsZttSys[iDY][iSys] = Weight+zptmassweightSys[iSys]+"(os>0.5"+CutsSys[iSys]+npartonCuts[iDY]+"&&isZTT>0.5)";
  //     cutsZllSys[iDY][iSys] = Weight+zptmassweightSys[iSys]+"(os>0.5"+CutsSys[iSys]+npartonCuts[iDY]+"&&isZTT<0.5)";
  //   }

  // }

  // int nSamplesDY = 9;

  // // filling histograms for DY samples
  // for (int i=0; i<nSamplesDY; ++i) { // run over samples

  //   TFile * file = new TFile(directory+dySampleNames[i]+".root");
  //   TTree * tree = (TTree*)file->Get(TauCheck);
  //   double norm = dyNorm[i];

  //   TString histNameZtt          = dySampleNames[i] + Variable + "_ztt_os";
  //   TString histNameZttSS        = dySampleNames[i] + Variable + "_ztt_ss";
  //   TString histNameZttSSrelaxed = dySampleNames[i] + Variable + "_ztt_ss_relaxed";
  //   TString histNameZll          = dySampleNames[i] + Variable + "_zll_os";
  //   TString histNameZllSS        = dySampleNames[i] + Variable + "_zll_ss";
  //   TString histNameZllSSrelaxed = dySampleNames[i] + Variable + "_zll_ss_relaxed";
    
  //   TH2D * histZtt2D          = new TH2D(histNameZtt,"",nBinsX,binsX,nBinsY,binsY);
  //   TH2D * histZttSS2D        = new TH2D(histNameZttSS,"",nBinsX,binsX,nBinsY,binsY);
  //   TH2D * histZttSSrelaxed2D = new TH2D(histNameZttSSrelaxed,"",nBinsX,binsX,nBinsY,binsY);

  //   TH2D * histZll2D          = new TH2D(histNameZll,"",nBinsX,binsX,nBinsY,binsY);
  //   TH2D * histZllSS2D        = new TH2D(histNameZllSS,"",nBinsX,binsX,nBinsY,binsY);
  //   TH2D * histZllSSrelaxed2D = new TH2D(histNameZllSSrelaxed,"",nBinsX,binsX,nBinsY,binsY);

  //   histZtt2D->Sumw2();
  //   histZttSS2D->Sumw2();
  //   histZttSSrelaxed2D->Sumw2();
  //   histZll2D->Sumw2();
  //   histZllSS2D->Sumw2();
  //   histZllSSrelaxed2D->Sumw2();

  //   tree->Draw(Variable+">>"+histNameZtt,  cutsZtt[i]);
  //   tree->Draw(Variable+">>"+histNameZttSS,cutsZttSS[i]);
  //   tree->Draw(Variable+">>"+histNameZttSSrelaxed,cutsZttSSrelaxed[i]);
  //   tree->Draw(Variable+">>"+histNameZll,  cutsZll[i]);
  //   tree->Draw(Variable+">>"+histNameZllSS,cutsZllSS[i]);
  //   tree->Draw(Variable+">>"+histNameZllSSrelaxed,cutsZllSSrelaxed[i]);

  //   histZtt[i]          = (TH1D*)Unfold(histZtt2D);
  //   histZttSS[i]        = (TH1D*)Unfold(histZttSS2D);
  //   histZttSSrelaxed[i] = (TH1D*)Unfold(histZttSSrelaxed2D);
  //   histZll[i]          = (TH1D*)Unfold(histZll2D);
  //   histZllSS[i]        = (TH1D*)Unfold(histZllSS2D);
  //   histZllSSrelaxed[i] = (TH1D*)Unfold(histZllSSrelaxed2D);

  //   // systematics
  //   for (int iSys=0; iSys<nSys; ++iSys ) {
  //     TH2D * histZttSys2D = new TH2D(histNameZtt+sysName[iSys],"",nBinsX,binsX,nBinsY,binsY);
  //     TH2D * histZllSys2D = new TH2D(histNameZll+sysName[iSys],"",nBinsX,binsX,nBinsY,binsY);
  //     histZttSys2D->Sumw2();
  //     histZllSys2D->Sumw2();
  //     tree->Draw(VariableSys[iSys]+">>"+histNameZtt+sysName[iSys],cutsZttSys[i][iSys]);
  //     tree->Draw(VariableSys[iSys]+">>"+histNameZll+sysName[iSys],cutsZllSys[i][iSys]);
  //     histZttSys[i][iSys] = (TH1D*)Unfold(histZttSys2D);
  //     histZllSys[i][iSys] = (TH1D*)Unfold(histZllSys2D);
  //   }
  //   for (int iB=1; iB<=nBins; ++iB) {

  //     double x = histZtt[i]->GetBinContent(iB);
  //     double e = histZtt[i]->GetBinError(iB);
  //     histZtt[i]->SetBinContent(iB,norm*x);
  //     histZtt[i]->SetBinError(iB,norm*e);
  //     x = histZttSS[i]->GetBinContent(iB);
  //     e = histZttSS[i]->GetBinError(iB);
  //     histZttSS[i]->SetBinContent(iB,norm*x);
  //     histZttSS[i]->SetBinError(iB,norm*e);
  //     x = histZttSSrelaxed[i]->GetBinContent(iB);
  //     e = histZttSSrelaxed[i]->GetBinError(iB);
  //     histZttSSrelaxed[i]->SetBinContent(iB,norm*x);
  //     histZttSSrelaxed[i]->SetBinError(iB,norm*e);
      
  //     x = histZll[i]->GetBinContent(iB);
  //     e = histZll[i]->GetBinError(iB);
  //     histZll[i]->SetBinContent(iB,norm*x);
  //     histZll[i]->SetBinError(iB,norm*e);
  //     x = histZllSS[i]->GetBinContent(iB);
  //     e = histZllSS[i]->GetBinError(iB);
  //     histZllSS[i]->SetBinContent(iB,norm*x);
  //     histZllSS[i]->SetBinError(iB,norm*e);
  //     x = histZllSSrelaxed[i]->GetBinContent(iB);
  //     e = histZllSSrelaxed[i]->GetBinError(iB);
  //     histZllSSrelaxed[i]->SetBinContent(iB,norm*x);
  //     histZllSSrelaxed[i]->SetBinError(iB,norm*e);
      
  //     for (int iSys=0; iSys<nSys; ++iSys) {
  // 	x = histZttSys[i][iSys]->GetBinContent(iB);
  // 	e = histZttSys[i][iSys]->GetBinError(iB);
  // 	histZttSys[i][iSys]->SetBinContent(iB,norm*x);
  // 	histZttSys[i][iSys]->SetBinError(iB,norm*e);
  // 	x = histZllSys[i][iSys]->GetBinContent(iB);
  // 	e = histZllSys[i][iSys]->GetBinError(iB);
  // 	histZllSys[i][iSys]->SetBinContent(iB,norm*x);
  // 	histZllSys[i][iSys]->SetBinError(iB,norm*e);
  //     }
  //   }
  //   std::cout << dySampleNames[i] << " -> ZTT = " << histZtt[i]->GetEntries() << " : " << histZtt[i]->GetSumOfWeights() 
  // 	      << "    ZLL = " << histZll[i]->GetEntries() << " : " << histZll[i]->GetSumOfWeights() << std::endl;
  //   //    delete file;
  // }

  // hist[1]   = histZtt[0];
  // histSS[1] = histZttSS[0];
  // histSSrelaxed[1] = histZttSSrelaxed[0];
  // hist[2]   = histZll[0];
  // histSS[2] = histZllSS[0];
  // histSSrelaxed[2] = histZllSSrelaxed[0];
  
  // for (int iSys=0; iSys<nSys; ++iSys) {
  //   histSys[1][iSys] = histZttSys[0][iSys];
  //   histSys[2][iSys] = histZllSys[0][iSys];
  // }

  // for (int iDY=1; iDY<9; ++iDY) {
  //   hist[1]->Add(hist[1],histZtt[iDY]);
  //   hist[2]->Add(hist[2],histZll[iDY]);
  //   histSS[1]->Add(histSS[1],histZttSS[iDY]);
  //   histSS[2]->Add(histSS[2],histZllSS[iDY]);
  //   histSSrelaxed[1]->Add(histSSrelaxed[1],histZttSSrelaxed[iDY]);
  //   histSSrelaxed[2]->Add(histSSrelaxed[2],histZllSSrelaxed[iDY]);
  //   for (int iSys=0; iSys<nSys; ++iSys) {
  //     histSys[1][iSys]->Add(histSys[1][iSys],histZttSys[iDY][iSys]);
  //     histSys[2][iSys]->Add(histSys[2][iSys],histZllSys[iDY][iSys]);
  //   }
  // }

  // // *********************************
  // // ****** End of DY+Jets samples ***
  // // *********************************


  // // *******************************
  // // ***** W+Jets samples *******
  // // *******************************

  // TH1D * histW[10];
  // TH1D * histWSys[10][20];
  // TH1D * histWSS[10];
  // TH1D * histWSSrelaxed[10];

  // // redefine reference cross sections
  // // and reference samples

  // refSamples[0] = "WJetsToLNu_13TeV-madgraphMLM";
  // refSamples[1] = "W1JetsToLNu_13TeV-madgraphMLM";
  // refSamples[2] = "W2JetsToLNu_13TeV-madgraphMLM";
  // refSamples[3] = "W3JetsToLNu_13TeV-madgraphMLM";
  // refSamples[4] = "W4JetsToLNu_13TeV-madgraphMLM";

  // refXSec[0] = 61527;
  // refXSec[1] = 1.221*9644.5;
  // refXSec[2] = 1.221*3144.5;
  // refXSec[3] = 1.221*954.8;
  // refXSec[4] = 1.221*485.6;

  // refEvents[0] = 0;
  // refEvents[1] = 0;
  // refEvents[2] = 0;
  // refEvents[3] = 0;
  // refEvents[4] = 0;

  // for (int iDY=0; iDY<5; ++iDY) {
  //   TFile * file = new TFile(directory+refSamples[iDY]+".root");
  //   TH1D * histWeightsH = (TH1D*)file->Get("histWeightsH");
  //   refEvents[iDY] = histWeightsH->GetSumOfWeights();
  // }
  // TString wSampleNames[9] = {"WJetsToLNu_13TeV-madgraphMLM",
  // 			     "WJetsToLNu_13TeV-madgraphMLM",
  // 			     "WJetsToLNu_13TeV-madgraphMLM",
  // 			     "WJetsToLNu_13TeV-madgraphMLM",
  // 			     "WJetsToLNu_13TeV-madgraphMLM",
  // 			     "W1JetsToLNu_13TeV-madgraphMLM",
  // 			     "W2JetsToLNu_13TeV-madgraphMLM",
  // 			     "W3JetsToLNu_13TeV-madgraphMLM",
  // 			     "W4JetsToLNu_13TeV-madgraphMLM"
  // };

  // double wNorm[9];
  // wNorm[0] = lumi*refXSec[0]/refEvents[0];
  // wNorm[1] = lumi/(refEvents[0]/refXSec[0]+refEvents[1]/refXSec[1]);
  // wNorm[2] = lumi/(refEvents[0]/refXSec[0]+refEvents[2]/refXSec[2]);
  // wNorm[3] = lumi/(refEvents[0]/refXSec[0]+refEvents[3]/refXSec[3]);
  // wNorm[4] = lumi/(refEvents[0]/refXSec[0]+refEvents[4]/refXSec[4]);
  // wNorm[5] = lumi/(refEvents[0]/refXSec[0]+refEvents[1]/refXSec[1]);
  // wNorm[6] = lumi/(refEvents[0]/refXSec[0]+refEvents[2]/refXSec[2]);
  // wNorm[7] = lumi/(refEvents[0]/refXSec[0]+refEvents[3]/refXSec[3]);
  // wNorm[8] = lumi/(refEvents[0]/refXSec[0]+refEvents[4]/refXSec[4]);

  // TString cutsW[9];
  // TString cutsWSS[9];
  // TString cutsWSSrelaxed[9];
  
  // TString cutsWSys[9][20];

  // for (int iDY=0; iDY<9; ++iDY) {
  //   cutsW[iDY]   = Weight+"(os>0.5"+Cuts+npartonCuts[iDY]+")";
  //   cutsWSS[iDY] = Weight+qcdweight+"(os<0.5"+Cuts+npartonCuts[iDY]+")";
  //   cutsWSSrelaxed[iDY] = Weight+qcdweight+"(os<0.5"+CutsSS+npartonCuts[iDY]+")";
  //   for (int iSys=0; iSys<nSys; ++iSys) {
  //     cutsWSys[iDY][iSys] = Weight+"(os>0.5"+CutsSys[iSys]+npartonCuts[iDY]+")";
  //   }
  // }

  // // filling histograms for WJets samples
  // for (int i=0; i<nSamplesDY; ++i) { // run over samples

  //   TFile * file = new TFile(directory+wSampleNames[i]+".root");
  //   TTree * tree = (TTree*)file->Get(TauCheck);
  //   double norm = wNorm[i];

  //   TString histNameW   = wSampleNames[i] + Variable + "_w_os";
  //   TString histNameWSS = wSampleNames[i] + Variable + "_w_ss";
  //   TString histNameWSSrelaxed = wSampleNames[i] + Variable + "_w_ss_relaxed";

  //   TH2D * histW2D = new TH2D(histNameW,"",nBinsX,binsX,nBinsY,binsY);
  //   TH2D * histWSS2D = new TH2D(histNameWSS,"",nBinsX,binsX,nBinsY,binsY);
  //   TH2D * histWSSrelaxed2D = new TH2D(histNameWSSrelaxed,"",nBinsX,binsX,nBinsY,binsY);

  //   histW2D->Sumw2();
  //   histWSS2D->Sumw2();
  //   histWSSrelaxed2D->Sumw2();

  //   tree->Draw(Variable+">>"+histNameW,  cutsW[i]);
  //   tree->Draw(Variable+">>"+histNameWSS,cutsWSS[i]);
  //   tree->Draw(Variable+">>"+histNameWSSrelaxed,cutsWSSrelaxed[i]);

  //   histW[i]   = (TH1D*)Unfold(histW2D);
  //   histWSS[i] = (TH1D*)Unfold(histWSS2D);
  //   histWSSrelaxed[i] = (TH1D*)Unfold(histWSSrelaxed2D);

  //   // systematics
  //   for (int iSys=0; iSys<nSys; ++iSys ) {
  //     TH2D * histWSys2D = new TH2D(histNameW+sysName[iSys],"",nBinsX,binsX,nBinsY,binsY);
  //     histWSys2D->Sumw2();
  //     tree->Draw(VariableSys[iSys]+">>"+histNameW+sysName[iSys],cutsWSys[i][iSys]);
  //     histWSys[i][iSys] = (TH1D*)Unfold(histWSys2D);
  //   }
  //   for (int iB=1; iB<=nBins; ++iB) {

  //     double x = histW[i]->GetBinContent(iB);
  //     double e = histW[i]->GetBinError(iB);
  //     histW[i]->SetBinContent(iB,norm*x);
  //     histW[i]->SetBinError(iB,norm*e);
  //     x = histWSS[i]->GetBinContent(iB);
  //     e = histWSS[i]->GetBinError(iB);
  //     histWSS[i]->SetBinContent(iB,norm*x);
  //     histWSS[i]->SetBinError(iB,norm*e);
  //     x = histWSSrelaxed[i]->GetBinContent(iB);
  //     e = histWSSrelaxed[i]->GetBinError(iB);
  //     histWSSrelaxed[i]->SetBinContent(iB,norm*x);
  //     histWSSrelaxed[i]->SetBinError(iB,norm*e);
      
  //     for (int iSys=0; iSys<nSys; ++iSys) {
  // 	x = histWSys[i][iSys]->GetBinContent(iB);
  // 	e = histWSys[i][iSys]->GetBinError(iB);
  // 	histWSys[i][iSys]->SetBinContent(iB,norm*x);
  // 	histWSys[i][iSys]->SetBinError(iB,norm*e);
  //     }
  //   }
  //   std::cout << wSampleNames[i] << " -> W = " << histW[i]->GetEntries() << " : " << histW[i]->GetSumOfWeights() 
  // 	      << std::endl;
  //   //    delete file;
  // }

  // hist[5]   = histW[0];
  // histSS[5] = histWSS[0];
  // histSSrelaxed[5] = histWSSrelaxed[0];
  
  // for (int iSys=0; iSys<nSys; ++iSys) {
  //   histSys[5][iSys] = histWSys[0][iSys];
  // }


  // for (int iDY=1; iDY<9; ++iDY) {
  //   hist[5]->Add(hist[5],histW[iDY]);
  //   histSS[5]->Add(histSS[5],histWSS[iDY]);
  //   histSSrelaxed[5]->Add(histSSrelaxed[5],histWSSrelaxed[iDY]);
  //   for (int iSys=0; iSys<nSys; ++iSys) {
  //     histSys[5][iSys]->Add(histSys[5][iSys],histWSys[iDY][iSys]);
  //   }
  // }

  // // ********************************
  // // ***** END OF W+Jets SAMPLES ****
  // // ********************************

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
