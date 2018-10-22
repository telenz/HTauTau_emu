#include "HttStylesNew.cc"
#include "CMS_lumi.C"
// categories
// em_inclusive
// em_0jet_low
// em_0jet_high
// em_1jet_low
// em_1jet_high
// em_vbf_low
// em_vbf_high

class sysUncertainty {
  
public :

  TString name;
  TString cutString;
  TString topWeight;
  TString zptmassWeight;
  TString treeVar;
  TH1D *hist;
  TH1D *histSS;

  sysUncertainty(TString sampleName, TString variable, TString sysName){
    name = sysName;
    hist = new TH1D(sampleName+variable+sysName)
  }
};

class sample {
  
public :
  
  TString name;
  TString cutString;
  TString cutStringSS;
  TString cutStringSSrelaxed;
  TH1D *hist;
  TH1D *histSS;
  TH1D *histSSrelaxed;
  vector<sysUncertainty> *uncertainties;
  
  sysUncertainty(TString sampleName){
    sample = sampleName;
    name = sysName;
    hist = new TH1D(sampleName+variable+sysName)
  }
};

void CreateDatacardsSM(TString directory = "../../Inputs/NTuples_2016",
		       TString category = "em_1jet_low",
		       TString Suffix = "btag_1jet_low") {


  TH1::SetDefaultSumw2();
  TH2::SetDefaultSumw2();

  float xmin = 0;
  float xmax = 240;
  TString Weight = "mcweight*puweight*effweight*0.978824*";
  TString xtitle = "m_{T} [GeV]";
  TString ytitle = "Events / bin";
  TString DataFile = "MuonEG_Run2016";
  bool btagWeight = false;

  TString TauCheck("TauCheck");

  SetStyle();

  TString Cuts        = "&&dzeta>-35&&pt_1>13&&pt_2>10&&TMath::Max(pt_1,pt_2)>24";
  TString CutsSS      = "&&dzeta>-35&&pt_1>13&&pt_2>10&&TMath::Max(pt_1,pt_2)>24";

  TString CutsCategory         = "&&iso_1<0.15&&iso_2<0.2&&extraelec_veto<0.5&&extramuon_veto<0.5";
  TString CutsCategoryJesUp    = "&&iso_1<0.15&&iso_2<0.2&&extraelec_veto<0.5&&extramuon_veto<0.5";
  TString CutsCategoryJesDown  = "&&iso_1<0.15&&iso_2<0.2&&extraelec_veto<0.5&&extramuon_veto<0.5";

  TString CutsCategorySS      = "&&iso_1<0.5&&iso_2>0.2&&iso_2<0.5&&extraelec_veto<0.5&&extramuon_veto<0.5";

  TString btagVeto("&&nbtag==0");
  //  TString btagVeto("");
  TString WeightNoBTag = Weight; 
  if (btagWeight) 
    Weight += "btag0weight*"; 

  TString CutsCategorySys[12];

  TString Variable = "m_vis";

  if (Suffix.Contains("mvis")) {
    Variable = "m_vis";
    xtitle = "m_{vis} [GeV]";
  }
  else if (Suffix.Contains("msv")) {
    Variable = "m_sv";
    xtitle = "m_{#tau#tau} [GeV]";
  }
  
  bool logY = true;

  TString qcdweight("qcdweight*");
  TString topweight("topptweight*");
  TString zptmassweight("zptmassweight*");

  int nBins = 30;
  float bins[50];
  for (int iB=0; iB<=nBins; ++iB)
    bins[iB] = 10*float(iB);

  int nBins_1jet = 12;
  float bins_1jet[13] = {0,40,60,70,80,90,100,110,120,130,150,200,250};

  int nBins_vbf = 12;
  float bins_vbf[13] = {0,20,40,60,80,100,120,140,160,180,200,220,240};

  bool blindData = false;

  int nBlindBins = 10;
  int blindDataBins[10] = {8,9,11,12,13,14,15,16,17,18};

  int nBlindBins_1jet = 7;
  int blindDataBins_1jet[7] = {9,10,11,12,13,14,15};

  int nBlindBins_vbf = 3;
  int blindDataBins_vbf[3] = {6,7,8};

  if (category=="em_0jet_low") {
    CutsCategory   += "&&njets==0&&pt_2<35&&pt_2>15&&dzeta>-35";
    CutsCategorySS += "&&njets==0&&pt_2<35&&pt_2>15&&dzeta>-35";
  }
  else if (category=="em_0jet_high") {
    CutsCategory   += "&&njets==0&&pt_2>=35&&dzeta>-35";
    CutsCategorySS += "&&njets==0&&pt_2>=35&&dzeta>-35";
  }
  else if (category=="em_1jet_low") {
    CutsCategory   += "&&(njets==1 || (njets==2&&mjj<500) || njets>2)&&pt_2<35&&pt_2>15&&dzeta>-35";
    CutsCategorySS += "&&(njets==1 || (njets==2&&mjj<500) || njets>2)&&pt_2<35&&pt_2>15&&dzeta>-35";
    //    nBins = nBins_1jet;
    //    for (int iB=0; iB<=nBins; ++iB) bins[iB] = bins_1jet[iB]; 
    
  }
  else if (category=="em_1jet_high") {
    CutsCategory   += "&&(njets==1 || (njets==2&&mjj<500) || njets>2)&&pt_2>=35&&dzeta>-35";
    CutsCategorySS += "&&(njets==1 || (njets==2&&mjj<500) || njets>2)&&pt_2>=35&&dzeta>-35";
    //    nBins = nBins_1jet;
    //    for (int iB=0; iB<=nBins; ++iB) bins[iB] = bins_1jet[iB]; 
  }
  else if (category=="em_vbf_low") {
    CutsCategory   += "&& njets==2 && mjj>=500 && mjj < 800 && pt_2>=15 && dzeta>-10";
    CutsCategorySS += "&& njets==2 && mjj>=500 && mjj < 800 && pt_2>=15 && dzeta>-10";
    //    nBins = nBins_vbf;
    //    for (int iB=0; iB<=nBins; ++iB) bins[iB] = bins_vbf[iB]; 
  }
  else if (category=="em_vbf_high") {
    CutsCategory   += "&& njets==2 && mjj>=800 && pt_2>15 && dzeta>-10";
    CutsCategorySS += "&& njets==2 && mjj>=800 && pt_2>15 && dzeta>-10";
    nBins = nBins_vbf;
    for (int iB=0; iB<=nBins; ++iB) bins[iB] = bins_vbf[iB]; 
  }
  //  2D categories
  else if (category=="em_0jet") {
    CutsCategory   += "&& njets==0 && pt_2>15 && dzeta>-35";
    CutsCategorySS += "&& njets==0 && pt_2>15 && dzeta>-35";
  }
  else if (category=="em_boosted") {
    CutsCategory   += "&&(njets==1 || (njets==2&&mjj<300) || njets>2)&&pt_2>15&&dzeta>-35";
    CutsCategorySS += "&&(njets==1 || (njets==2&&mjj<300) || njets>2)&&pt_2>15&&dzeta>-35";
    nBins = nBins_1jet;
    for (int iB=0; iB<=nBins; ++iB) bins[iB] = bins_1jet[iB]; 
  }
  else if (category=="em_vbf") {
    CutsCategory   += "&& njets==2 && mjj>=300 && pt_2>15 && dzeta>-10";
    CutsCategorySS += "&& njets==2 && mjj>=300 && pt_2>15 && dzeta>-10";
    nBins = nBins_vbf;
    for (int iB=0; iB<=nBins; ++iB) bins[iB] = bins_vbf[iB]; 
  }

  Cuts   = Cuts   + CutsCategory + btagVeto;
  CutsSS = CutsSS + CutsCategorySS + btagVeto;

  double lumi = 36800;

  vector<TString> sampleNames = {
    DataFile ,
    "DYJets_dnn_em_v1" , 
    "WJets_dnn_em_v1" ,
    "TTbar_dnn_em_v1" ,
    "SingleTop_dnn_em_v1" , 
    "Diboson_dnn_em_v1" , 
    "ggH_dnn_em_v1" , 
    "VBFH_dnn_em_v1" , 
  };

  TH1D * hist[31];
  TH1D * histSS[31];
  TH1D * histSSrelaxed[31];

  

  TString CutsSys[12] = {"&&dzeta>-35 && pt_1>13      && pt_2>15       && TMath::Max(pt_1,pt_2)>24",
			 "&&dzeta>-35 && pt_1>13      && pt_2>15       && TMath::Max(pt_1,pt_2)>24",
			 "&&dzeta>-35 && pt_1>13      && pt_Up_2>15    && TMath::Max(pt_1,pt_Up_2)>24",
			 "&&dzeta>-35 && pt_1>13      && pt_Down_2>15  && TMath::Max(pt_1,pt_Down_2)>24",
			 "&&dzeta>-35 && pt_Up_1>13   && pt_2>15       && TMath::Max(pt_Up_1,pt_2)>24",
			 "&&dzeta>-35 && pt_Down_1>13 && pt_2>15       && TMath::Max(pt_Down_1,pt_2)>24",
			 "&&dzeta>-35 && pt_1>13      && pt_2>15       && TMath::Max(pt_1,pt_2)>24",
			 "&&dzeta>-35 && pt_1>13      && pt_2>15       && TMath::Max(pt_1,pt_2)>24",
			 "&&dzeta>-35 && pt_1>13      && pt_2>15       && TMath::Max(pt_1,pt_2)>24",
			 "&&dzeta>-35 && pt_1>13      && pt_2>15       && TMath::Max(pt_1,pt_2)>24",
			 "&&dzeta>-35 && pt_1>13      && pt_2>15       && TMath::Max(pt_1,pt_2)>24",
			 "&&dzeta>-35 && pt_1>13      && pt_2>15       && TMath::Max(pt_1,pt_2)>24"
  };
  for (int iSys=0; iSys<12; ++iSys)
    CutsSys[iSys] = CutsSys[iSys] + CutsCategory;

  TString topweightSys[12] = {"topptweight*topptweight*",
			      "",
			      "topptweight*",
			      "topptweight*",
			      "topptweight*",
			      "topptweight*",
			      "topptweight*",
			      "topptweight*",
			      "topptweight*",
			      "topptweight*",
			      "topptweight*",
			      "topptweight*"
  };

  TString zptmassweightSys[12] = {"zptmassweight*",
				  "zptmassweight*",
				  "zptmassweight*",
				  "zptmassweight*",
				  "zptmassweight*",
				  "zptmassweight*",
				  "zptmassweight*",
				  "zptmassweight*",
				  "zptmassweight*",
				  "zptmassweight*",
				  "zptmassweight*zptmassweight*",
				  ""
  };

  TString cuts[31];
  TString cutsSS[31];
  TString cutsSSrelaxed[31];
  
  TString cutsSys[31][12];
  
  
  // Central cuts ---->
  for (int i=0; i<31; ++i) {
    cuts[i]   = Weight+"(os>0.5"+Cuts+")";
    cutsSS[i] = Weight+qcdweight+"(os<0.5"+Cuts+")";
    cutsSSrelaxed[i] = Weight+qcdweight+"(os<0.5"+CutsSS+")";
  }
  cuts[0] = "(os>0.5"+Cuts+")";
  cuts[1] = Weight+zptmassweight+"(os>0.5"+Cuts+"&&isZTT)";
  cuts[2] = Weight+zptmassweight+"(os>0.5"+Cuts+"&&!isZTT)";
  cuts[3] = Weight+"(os>0.5"+Cuts+"&&isZTT)";
  cuts[4] = Weight+"(os>0.5"+Cuts+"&&!isZTT)";
  cuts[6] = Weight+topweight+"(os>0.5"+Cuts+")";

  cutsSS[0] = qcdweight+"(os<0.5"+Cuts+")";
  cutsSS[1] = Weight+zptmassweight+qcdweight+"(os<0.5"+Cuts+"&&isZTT)";
  cutsSS[2] = Weight+zptmassweight+qcdweight+"(os<0.5"+Cuts+"&&!isZTT)";
  cutsSS[3] = Weight+qcdweight+"(os<0.5"+Cuts+"&&isZTT)";
  cutsSS[4] = Weight+qcdweight+"(os<0.5"+Cuts+"&&!isZTT)";
  cutsSS[6] = Weight+topweight+qcdweight+"(os<0.5"+Cuts+")";

  cutsSSrelaxed[0] = qcdweight+"(os<0.5"+CutsSS+")";
  cutsSSrelaxed[1] = Weight+zptmassweight+qcdweight+"(os<0.5"+CutsSS+"&&isZTT)";
  cutsSSrelaxed[2] = Weight+zptmassweight+qcdweight+"(os<0.5"+CutsSS+"&&!isZTT)";
  cutsSSrelaxed[3] = Weight+qcdweight+"(os<0.5"+CutsSS+"&&isZTT)";
  cutsSSrelaxed[4] = Weight+qcdweight+"(os<0.5"+CutsSS+"&&!isZTT)";
  cutsSSrelaxed[6] = Weight+topweight+qcdweight+"(os<0.5"+CutsSS+")";

  if (btagWeight) {
    cuts[0] = "(os>0.5"+Cuts+"&&nbtag==0)";
    cutsSS[0] = qcdweight+"(os<0.5"+Cuts+"&&nbtag==0)";
    cutsSSrelaxed[0] = qcdweight+"(os<0.5"+CutsSS+"&&nbtag==0)";
    for (int i=28; i<30)
    cuts[i]   = WeightNoBTag+"(os>0.5"+Cuts+")";
    cutsSS[i] = Weight+qcdweight+"(os<0.5"+Cuts+")";
    cutsSSrelaxed[i] = Weight+qcdweight+"(os<0.5"+CutsSS+")";

  }

  // Systematics ->
  for (int iSys=0; iSys<12; ++iSys) {

    for (int i=0; i<31; ++i) 
      cutsSys[i][iSys] = Weight+"(os>0.5"+CutsSys[iSys]+")";

    cutsSys[0][iSys] = "(os>0.5"+CutsSys[iSys]+")";
    cutsSys[1][iSys] = Weight+zptmassweightSys[iSys]+"(os>0.5"+CutsSys[iSys]+"&&isZTT)";
    cutsSys[2][iSys] = Weight+zptmassweightSys[iSys]+"(os>0.5"+CutsSys[iSys]+"&&!isZTT)";
    cutsSys[3][iSys] = Weight+"(os>0.5"+CutsSys[iSys]+"&&isZTT)";
    cutsSys[4][iSys] = Weight+"(os>0.5"+CutsSys[iSys]+"&&!isZTT)";
    cutsSys[6][iSys] = Weight+topweightSys[iSys]+"(os>0.5"+CutsSys[iSys]+")";

  }

  TCanvas * dummyCanv = new TCanvas("dummy","",500,500);
  
  vector<TH1D*> hist;
  vector<TH1D*> histSS;
  vector<TH1D*> histSSrelaxed;

  vector<TString> variableSys = { Variable, // topPtUp
				  Variable, // topPtDown
				  Variable + "_muUp", // muonScaleUp
				  Variable + "_muDown", // muonScaleDown
				  Variable + "_eUp", // eleScaleUp
				  Variable + "_eDown", // eleScaleDown
				  Variable + "_scaleUp",  // metScaleUp
				  Variable + "_scaleDown",  // metScaleDown
				  Variable, // metScaleUp 
				  Variable, // metScaleDown
				  Variable, // zPtMassUp
				  Variable  // zPtMassDown
  };

  vector<TString> qcdweightSys = { "qcdweightup*",
				   "(qcdweight*qcdweight/qcdweightup)*"
  };

  vector<TString> sysNameSS = { "_CMS_htt_em_qcdShape_13TeVUp",
			        "_CMS_htt_em_qcdShape_13TeVDown"
  };

  vector<TString> sysNames = { "_CMS_htt_ttbarShape_13TeVUp",
			       "_CMS_htt_ttbarShape_13TeVDown",
			       "_CMS_scale_m_em_13TeVUp",
			       "_CMS_scale_m_em_13TeVDown",
			       "_CMS_scale_e_em_13TeVUp",
			       "_CMS_scale_e_em_13TeVDown",
			       "_CMS_scale_met_em_13TeVUp",
			       "_CMS_scale_met_em_13TeVDown",
			       "_CMS_reso_met_em_13TeVUp",
			       "_CMS_reso_met_em_13TeVDown",
			       "_CMS_htt_dyShape_13TeVUp",
			       "_CMS_htt_dyShape_13TeVDown"
  };


  vector<vector<TH1D*>> histSys;


  // filling histograms data and bkgd MC
  for(TString const& sampleName : sampleNames) {

    TFile * file = new TFile( directory + sampleName + ".root" );
    TTree * tree = (TTree*) file -> Get(TauCheck);
    TString histName          = sampleName + Variable + "_os";
    TString histNameSS        = sampleName + Variable + "_ss";
    TString histNameSSrelaxed = sampleName + Variable + "_ss_relaxed";
    TH1D *hist_          = new TH1D(histName,"",nBins,bins);
    TH1D *histSS_        = new TH1D(histNameSS,"",nBins,bins);
    TH1D *histSSrelaxed_ = new TH1D(histNameSSrelaxed,"",nBins,bins);
    hist          -> push_back(hist_);
    histSS        -> push_back(histSS_);
    histSSrelaxed -> push_back(histSSrelaxed_);
    tree -> Draw(Variable+">>"+histName,cuts[i]);
    tree -> Draw(Variable+">>"+histNameSS,cutsSS[i]);
    tree -> Draw(Variable+">>"+histNameSSrelaxed,cutsSSrelaxed[i]);

    // systematics
    for ( TString const& sysName : sysNames ) {
      TH1D *histSys_ = new TH1D(histName+sysName,"",nBins,bins);
      histSys.push_back(histSys_); 
      histSys[i][iSys] = new TH1D(histName+sysName[iSys],"",nBins,bins);
      tree -> Draw(variableSys+">>"+histName+sysName,cutsSys[i][iSys]);
    }

    if (i>0) { // MC samples should be normalized
      for (int iB=1; iB<=nBins; ++iB) {
	double x = hist[i]->GetBinContent(iB);
	double e = hist[i]->GetBinError(iB);
    	hist[i]->SetBinContent(iB,norm*x);
    	hist[i]->SetBinError(iB,norm*e);
	x = histSS[i]->GetBinContent(iB);
	e = histSS[i]->GetBinError(iB);
    	histSS[i]->SetBinContent(iB,norm*x);
    	histSS[i]->SetBinError(iB,norm*e);
	x = histSSrelaxed[i]->GetBinContent(iB);
	e = histSSrelaxed[i]->GetBinError(iB);
    	histSSrelaxed[i]->SetBinContent(iB,norm*x);
    	histSSrelaxed[i]->SetBinError(iB,norm*e);
	for (int iSys=0; iSys<12; ++iSys) {
	  x = histSys[i][iSys]->GetBinContent(iB);
	  e = histSys[i][iSys]->GetBinError(iB);
	  histSys[i][iSys]->SetBinContent(iB,norm*x);
	  histSys[i][iSys]->SetBinError(iB,norm*e);
	}
      }
    }
    std::cout << sampleNames[i] << " -> entries = " << hist[i]->GetEntries() << " : sumOfWeights = " << hist[i]->GetSumOfWeights() << std::endl;
    //    delete file;
  }

  // *******************************
  // ***** Drell-Yan samples *******
  // *******************************

  TH1D * histZtt[10];
  TH1D * histZttSys[10][12];
  TH1D * histZttSS[10];
  TH1D * histZttSSrelaxed[10];

  TH1D * histZll[10];
  TH1D * histZllSys[10][12];
  TH1D * histZllSS[10];
  TH1D * histZllSSrelaxed[10];

  TString refSamples[5] = {"DYJetsToLL_M-50_13TeV-madgraphMLM",
			   "DY1JetsToLL_M-50_13TeV-madgraphMLM",
			   "DY2JetsToLL_M-50_13TeV-madgraphMLM",
			   "DY3JetsToLL_M-50_13TeV-madgraphMLM",
			   "DY4JetsToLL_M-50_13TeV-madgraphMLM"
  };
  double refXSec[5] = {5765,
		       1.164*1012.5,
		       1.164*332.8,
		       1.164*101.8,
		       1.164*54.8};

  double refEvents[5] = {0,0,0,0,0};

  for (int iDY=0; iDY<5; ++iDY) {
    TFile * file = new TFile(directory+refSamples[iDY]+".root");
    TH1D * histWeightsH = (TH1D*)file->Get("histWeightsH");
    refEvents[iDY] = histWeightsH->GetSumOfWeights();
  }
  TString dySampleNames[9] = {"DYJetsToLL_M-50_13TeV-madgraphMLM",
			      "DYJetsToLL_M-50_13TeV-madgraphMLM",
			      "DYJetsToLL_M-50_13TeV-madgraphMLM",
			      "DYJetsToLL_M-50_13TeV-madgraphMLM",
			      "DYJetsToLL_M-50_13TeV-madgraphMLM",
			      "DY1JetsToLL_M-50_13TeV-madgraphMLM",
			      "DY2JetsToLL_M-50_13TeV-madgraphMLM",
			      "DY3JetsToLL_M-50_13TeV-madgraphMLM",
			      "DY4JetsToLL_M-50_13TeV-madgraphMLM"
  };

  double dyNorm[9];
  dyNorm[0] = lumi*refXSec[0]/refEvents[0];
  dyNorm[1] = lumi/(refEvents[0]/refXSec[0]+refEvents[1]/refXSec[1]);
  dyNorm[2] = lumi/(refEvents[0]/refXSec[0]+refEvents[2]/refXSec[2]);
  dyNorm[3] = lumi/(refEvents[0]/refXSec[0]+refEvents[3]/refXSec[3]);
  dyNorm[4] = lumi/(refEvents[0]/refXSec[0]+refEvents[4]/refXSec[4]);
  dyNorm[5] = lumi/(refEvents[0]/refXSec[0]+refEvents[1]/refXSec[1]);
  dyNorm[6] = lumi/(refEvents[0]/refXSec[0]+refEvents[2]/refXSec[2]);
  dyNorm[7] = lumi/(refEvents[0]/refXSec[0]+refEvents[3]/refXSec[3]);
  dyNorm[8] = lumi/(refEvents[0]/refXSec[0]+refEvents[4]/refXSec[4]);

  TString npartonCuts[9] = {"&&(npartons==0||npartons>4)",
			    "&&npartons==1",
			    "&&npartons==2",
			    "&&npartons==3",
			    "&&npartons==4",
			    "",
			    "",
			    "",
			    ""
  };

  TString cutsZtt[9];
  TString cutsZttSS[9];
  TString cutsZttSSrelaxed[9];
  
  TString cutsZttSys[9][12];

  TString cutsZll[9];
  TString cutsZllSS[9];
  TString cutsZllSSrelaxed[9];
  
  TString cutsZllSys[9][12];

  for (int iDY=0; iDY<9; ++iDY) {
    cutsZtt[iDY]   = Weight+zptmassweight+"(os>0.5"+Cuts+npartonCuts[iDY]+"&&isZTT>0.5)";
    cutsZttSS[iDY] = Weight+zptmassweight+qcdweight+"(os<0.5"+Cuts+npartonCuts[iDY]+"&&isZTT>0.5)";
    cutsZttSSrelaxed[iDY] = Weight+zptmassweight+qcdweight+"(os<0.5"+CutsSS+npartonCuts[iDY]+"&&isZTT>0.5)";
    cutsZll[iDY]   = Weight+zptmassweight+"(os>0.5"+Cuts+npartonCuts[iDY]+"&&isZTT<0.5)";
    cutsZllSS[iDY] = Weight+zptmassweight+qcdweight+"(os<0.5"+Cuts+npartonCuts[iDY]+"&&isZTT<0.5)";
    cutsZllSSrelaxed[iDY] = Weight+zptmassweight+qcdweight+"(os<0.5"+CutsSS+npartonCuts[iDY]+"&&isZTT<0.5)";
    for (int iSys=0; iSys<12; ++iSys) {
      cutsZttSys[iDY][iSys] = Weight+zptmassweightSys[iSys]+"(os>0.5"+CutsSys[iSys]+npartonCuts[iDY]+"&&isZTT>0.5)";
      cutsZllSys[iDY][iSys] = Weight+zptmassweightSys[iSys]+"(os>0.5"+CutsSys[iSys]+npartonCuts[iDY]+"&&isZTT<0.5)";
    }

  }

  int nSamplesDY = 9;

  // filling histograms for DY samples
  for (int i=0; i<nSamplesDY; ++i) { // run over samples
    TFile * file = new TFile(directory+dySampleNames[i]+".root");
    TTree * tree = (TTree*)file->Get(TauCheck);
    double norm = dyNorm[i];
    TString histNameZtt   = dySampleNames[i] + Variable + "_ztt_os";
    TString histNameZttSS = dySampleNames[i] + Variable + "_ztt_ss";
    TString histNameZttSSrelaxed = dySampleNames[i] + Variable + "_ztt_ss_relaxed";
    TString histNameZll   = dySampleNames[i] + Variable + "_zll_os";
    TString histNameZllSS = dySampleNames[i] + Variable + "_zll_ss";
    TString histNameZllSSrelaxed = dySampleNames[i] + Variable + "_zll_ss_relaxed";
    histZtt[i]   = new TH1D(histNameZtt,"",nBins,bins);
    histZttSS[i] = new TH1D(histNameZttSS,"",nBins,bins);
    histZttSSrelaxed[i] = new TH1D(histNameZttSSrelaxed,"",nBins,bins);
    histZll[i]   = new TH1D(histNameZll,"",nBins,bins);
    histZllSS[i] = new TH1D(histNameZllSS,"",nBins,bins);
    histZllSSrelaxed[i] = new TH1D(histNameZllSSrelaxed,"",nBins,bins);
    histZtt[i]->Sumw2();
    histZttSS[i]->Sumw2();
    histZttSSrelaxed[i]->Sumw2();
    histZll[i]->Sumw2();
    histZllSS[i]->Sumw2();
    histZllSSrelaxed[i]->Sumw2();
    tree->Draw(Variable+">>"+histNameZtt,  cutsZtt[i]);
    tree->Draw(Variable+">>"+histNameZttSS,cutsZttSS[i]);
    tree->Draw(Variable+">>"+histNameZttSSrelaxed,cutsZttSSrelaxed[i]);
    tree->Draw(Variable+">>"+histNameZll,  cutsZll[i]);
    tree->Draw(Variable+">>"+histNameZllSS,cutsZllSS[i]);
    tree->Draw(Variable+">>"+histNameZllSSrelaxed,cutsZllSSrelaxed[i]);

    // systematics
    for (int iSys=0; iSys<12; ++iSys ) {
      histZttSys[i][iSys] = new TH1D(histNameZtt+sysName[iSys],"",nBins,bins);
      histZllSys[i][iSys] = new TH1D(histNameZll+sysName[iSys],"",nBins,bins);
      histZttSys[i][iSys]->Sumw2();
      histZllSys[i][iSys]->Sumw2();
      tree->Draw(variableSys[iSys]+">>"+histNameZtt+sysName[iSys],cutsZttSys[i][iSys]);
      tree->Draw(variableSys[iSys]+">>"+histNameZll+sysName[iSys],cutsZllSys[i][iSys]);
    }
    for (int iB=1; iB<=nBins; ++iB) {

      double x = histZtt[i]->GetBinContent(iB);
      double e = histZtt[i]->GetBinError(iB);
      histZtt[i]->SetBinContent(iB,norm*x);
      histZtt[i]->SetBinError(iB,norm*e);
      x = histZttSS[i]->GetBinContent(iB);
      e = histZttSS[i]->GetBinError(iB);
      histZttSS[i]->SetBinContent(iB,norm*x);
      histZttSS[i]->SetBinError(iB,norm*e);
      x = histZttSSrelaxed[i]->GetBinContent(iB);
      e = histZttSSrelaxed[i]->GetBinError(iB);
      histZttSSrelaxed[i]->SetBinContent(iB,norm*x);
      histZttSSrelaxed[i]->SetBinError(iB,norm*e);
      
      x = histZll[i]->GetBinContent(iB);
      e = histZll[i]->GetBinError(iB);
      histZll[i]->SetBinContent(iB,norm*x);
      histZll[i]->SetBinError(iB,norm*e);
      x = histZllSS[i]->GetBinContent(iB);
      e = histZllSS[i]->GetBinError(iB);
      histZllSS[i]->SetBinContent(iB,norm*x);
      histZllSS[i]->SetBinError(iB,norm*e);
      x = histZllSSrelaxed[i]->GetBinContent(iB);
      e = histZllSSrelaxed[i]->GetBinError(iB);
      histZllSSrelaxed[i]->SetBinContent(iB,norm*x);
      histZllSSrelaxed[i]->SetBinError(iB,norm*e);
      
      for (int iSys=0; iSys<12; ++iSys) {
	x = histZttSys[i][iSys]->GetBinContent(iB);
	e = histZttSys[i][iSys]->GetBinError(iB);
	histZttSys[i][iSys]->SetBinContent(iB,norm*x);
	histZttSys[i][iSys]->SetBinError(iB,norm*e);
	x = histZllSys[i][iSys]->GetBinContent(iB);
	e = histZllSys[i][iSys]->GetBinError(iB);
	histZllSys[i][iSys]->SetBinContent(iB,norm*x);
	histZllSys[i][iSys]->SetBinError(iB,norm*e);
      }
    }
    std::cout << dySampleNames[i] << " -> ZTT = " << histZtt[i]->GetEntries() << " : " << histZtt[i]->GetSumOfWeights() 
	      << "    ZLL = " << histZll[i]->GetEntries() << " : " << histZll[i]->GetSumOfWeights() << std::endl;
    //    delete file;
  }

  hist[1]   = histZtt[0];
  histSS[1] = histZttSS[0];
  histSSrelaxed[1] = histZttSSrelaxed[0];
  hist[2]   = histZll[0];
  histSS[2] = histZllSS[0];
  histSSrelaxed[2] = histZllSSrelaxed[0];
  
  for (int iSys=0; iSys<12; ++iSys) {
    histSys[1][iSys] = histZttSys[0][iSys];
    histSys[2][iSys] = histZllSys[0][iSys];
  }

  for (int iDY=1; iDY<9; ++iDY) {
    hist[1]->Add(hist[1],histZtt[iDY]);
    hist[2]->Add(hist[2],histZll[iDY]);
    histSS[1]->Add(histSS[1],histZttSS[iDY]);
    histSS[2]->Add(histSS[2],histZllSS[iDY]);
    histSSrelaxed[1]->Add(histSSrelaxed[1],histZttSSrelaxed[iDY]);
    histSSrelaxed[2]->Add(histSSrelaxed[2],histZllSSrelaxed[iDY]);
    for (int iSys=0; iSys<12; ++iSys) {
      histSys[1][iSys]->Add(histSys[1][iSys],histZttSys[iDY][iSys]);
      histSys[2][iSys]->Add(histSys[2][iSys],histZllSys[iDY][iSys]);
    }
  }

  // *******************************
  // ***** W+Jets samples *******
  // *******************************

  TH1D * histW[10];
  TH1D * histWSys[10][12];
  TH1D * histWSS[10];
  TH1D * histWSSrelaxed[10];

  // redefine reference cross sections
  // and reference samples

  refSamples[0] = "WJetsToLNu_13TeV-madgraphMLM";
  refSamples[1] = "W1JetsToLNu_13TeV-madgraphMLM";
  refSamples[2] = "W2JetsToLNu_13TeV-madgraphMLM";
  refSamples[3] = "W3JetsToLNu_13TeV-madgraphMLM";
  refSamples[4] = "W4JetsToLNu_13TeV-madgraphMLM";

  refXSec[0] = 61527;
  refXSec[1] = 1.221*9644.5;
  refXSec[2] = 1.221*3144.5;
  refXSec[3] = 1.221*954.8;
  refXSec[4] = 1.221*485.6;

  refEvents[0] = 0;
  refEvents[1] = 0;
  refEvents[2] = 0;
  refEvents[3] = 0;
  refEvents[4] = 0;

  for (int iDY=0; iDY<5; ++iDY) {
    TFile * file = new TFile(directory+refSamples[iDY]+".root");
    TH1D * histWeightsH = (TH1D*)file->Get("histWeightsH");
    refEvents[iDY] = histWeightsH->GetSumOfWeights();
  }
  TString wSampleNames[9] = {"WJetsToLNu_13TeV-madgraphMLM",
			     "WJetsToLNu_13TeV-madgraphMLM",
			     "WJetsToLNu_13TeV-madgraphMLM",
			     "WJetsToLNu_13TeV-madgraphMLM",
			     "WJetsToLNu_13TeV-madgraphMLM",
			     "W1JetsToLNu_13TeV-madgraphMLM",
			     "W2JetsToLNu_13TeV-madgraphMLM",
			     "W3JetsToLNu_13TeV-madgraphMLM",
			     "W4JetsToLNu_13TeV-madgraphMLM"
  };

  double wNorm[9];
  wNorm[0] = lumi*refXSec[0]/refEvents[0];
  wNorm[1] = lumi/(refEvents[0]/refXSec[0]+refEvents[1]/refXSec[1]);
  wNorm[2] = lumi/(refEvents[0]/refXSec[0]+refEvents[2]/refXSec[2]);
  wNorm[3] = lumi/(refEvents[0]/refXSec[0]+refEvents[3]/refXSec[3]);
  wNorm[4] = lumi/(refEvents[0]/refXSec[0]+refEvents[4]/refXSec[4]);
  wNorm[5] = lumi/(refEvents[0]/refXSec[0]+refEvents[1]/refXSec[1]);
  wNorm[6] = lumi/(refEvents[0]/refXSec[0]+refEvents[2]/refXSec[2]);
  wNorm[7] = lumi/(refEvents[0]/refXSec[0]+refEvents[3]/refXSec[3]);
  wNorm[8] = lumi/(refEvents[0]/refXSec[0]+refEvents[4]/refXSec[4]);

  TString cutsW[9];
  TString cutsWSS[9];
  TString cutsWSSrelaxed[9];
  
  TString cutsWSys[9][12];

  for (int iDY=0; iDY<9; ++iDY) {
    cutsW[iDY]   = Weight+"(os>0.5"+Cuts+npartonCuts[iDY]+")";
    cutsWSS[iDY] = Weight+qcdweight+"(os<0.5"+Cuts+npartonCuts[iDY]+")";
    cutsWSSrelaxed[iDY] = Weight+qcdweight+"(os<0.5"+CutsSS+npartonCuts[iDY]+")";
    for (int iSys=0; iSys<12; ++iSys) {
      cutsWSys[iDY][iSys] = Weight+"(os>0.5"+CutsSys[iSys]+npartonCuts[iDY]+")";
    }
  }

  // filling histograms for WJets samples
  for (int i=0; i<nSamplesDY; ++i) { // run over samples
    TFile * file = new TFile(directory+wSampleNames[i]+".root");
    TTree * tree = (TTree*)file->Get(TauCheck);
    double norm = wNorm[i];
    TString histNameW   = wSampleNames[i] + Variable + "_w_os";
    TString histNameWSS = wSampleNames[i] + Variable + "_w_ss";
    TString histNameWSSrelaxed = wSampleNames[i] + Variable + "_w_ss_relaxed";
    histW[i]   = new TH1D(histNameW,"",nBins,bins);
    histWSS[i] = new TH1D(histNameWSS,"",nBins,bins);
    histWSSrelaxed[i] = new TH1D(histNameWSSrelaxed,"",nBins,bins);
    histW[i]->Sumw2();
    histWSS[i]->Sumw2();
    histWSSrelaxed[i]->Sumw2();
    tree->Draw(Variable+">>"+histNameW,  cutsW[i]);
    tree->Draw(Variable+">>"+histNameWSS,cutsWSS[i]);
    tree->Draw(Variable+">>"+histNameWSSrelaxed,cutsWSSrelaxed[i]);

    // systematics
    for (int iSys=0; iSys<12; ++iSys ) {
      histWSys[i][iSys] = new TH1D(histNameW+sysName[iSys],"",nBins,bins);
      histWSys[i][iSys]->Sumw2();
      tree->Draw(variableSys[iSys]+">>"+histNameW+sysName[iSys],cutsWSys[i][iSys]);
    }
    for (int iB=1; iB<=nBins; ++iB) {

      double x = histW[i]->GetBinContent(iB);
      double e = histW[i]->GetBinError(iB);
      histW[i]->SetBinContent(iB,norm*x);
      histW[i]->SetBinError(iB,norm*e);
      x = histWSS[i]->GetBinContent(iB);
      e = histWSS[i]->GetBinError(iB);
      histWSS[i]->SetBinContent(iB,norm*x);
      histWSS[i]->SetBinError(iB,norm*e);
      x = histWSSrelaxed[i]->GetBinContent(iB);
      e = histWSSrelaxed[i]->GetBinError(iB);
      histWSSrelaxed[i]->SetBinContent(iB,norm*x);
      histWSSrelaxed[i]->SetBinError(iB,norm*e);
      
      for (int iSys=0; iSys<12; ++iSys) {
	x = histWSys[i][iSys]->GetBinContent(iB);
	e = histWSys[i][iSys]->GetBinError(iB);
	histWSys[i][iSys]->SetBinContent(iB,norm*x);
	histWSys[i][iSys]->SetBinError(iB,norm*e);
      }
    }
    std::cout << wSampleNames[i] << " -> W = " << histW[i]->GetEntries() << " : " << histW[i]->GetSumOfWeights() 
	      << std::endl;
    //    delete file;
  }

  hist[5]   = histW[0];
  histSS[5] = histWSS[0];
  histSSrelaxed[5] = histWSSrelaxed[0];
  
  for (int iSys=0; iSys<12; ++iSys) {
    histSys[5][iSys] = histWSys[0][iSys];
  }

  for (int iDY=1; iDY<9; ++iDY) {
    hist[5]->Add(hist[5],histW[iDY]);
    histSS[5]->Add(histSS[5],histWSS[iDY]);
    histSSrelaxed[5]->Add(histSSrelaxed[5],histWSSrelaxed[iDY]);
    for (int iSys=0; iSys<12; ++iSys) {
      histSys[5][iSys]->Add(histSys[5][iSys],histWSys[iDY][iSys]);
    }
  }

  // 

  hist[1]->Add(hist[1],hist[3]); // ZTT low mass + high mass
  hist[2]->Add(hist[2],hist[4]); // ZLL low mass + high mass

  for (int iSys=0; iSys<12; ++iSys) {
    histSys[1][iSys]->Add(histSys[1][iSys],histSys[3][iSys]);
    histSys[2][iSys]->Add(histSys[2][iSys],histSys[4][iSys]);
  }

  //  adding up single top and VV backgrounds
  for (int iH=8; iH<19; ++iH) {
    hist[7]->Add(hist[7],hist[iH]);
    for (int iSys=0; iSys<12; ++iSys)
      histSys[7][iSys]->Add(histSys[7][iSys],histSys[iH][iSys]);
  }

  // adding up W+Jets and W+gamma samples
  for (int iH=19; iH<22; ++iH) {
    hist[5]->Add(hist[5],hist[iH]);
    for (int iSys=0; iSys<12; ++iSys)
      histSys[5][iSys]->Add(histSys[5][iSys],histSys[iH][iSys]);
  }

  // subtracting background from SS
  for (int iH=1; iH<22; ++iH) {
    histSS[0]->Add(histSS[0],histSS[iH],1,-1);
    histSSrelaxed[0]->Add(histSSrelaxed[0],histSSrelaxed[iH],1,-1);
  }

  for (int iB=1; iB<=nBins; ++iB) {
    float xSS = histSS[0]->GetBinContent(iB);
    float xSSrelaxed = histSSrelaxed[0]->GetBinContent(iB);
    if (xSS<0)
      histSS[0]->SetBinContent(iB,0.);
    if (xSSrelaxed<0)
      histSSrelaxed[0]->SetBinContent(iB,0.);
  }

  // adding up WplusH and WminusH                                                                                                                                                                           
  hist[25]->Add(hist[25],hist[26]);
  for (int iSys=0; iSys<12; ++iSys)
    histSys[25][iSys]->Add(histSys[25][iSys],histSys[26][iSys]);


  /*
  // applying dyExtrapolationFactor
  for (int iB=1; iB<=nBins; ++iB) {
    hist[1]->SetBinContent(iB,dyExtrapolationFactor*hist[1]->GetBinContent(iB));
    hist[1]->SetBinError(iB,dyExtrapolationFactor*hist[2]->GetBinError(iB));
    hist[2]->SetBinContent(iB,dyExtrapolationFactor*hist[2]->GetBinContent(iB));
    hist[2]->SetBinError(iB,dyExtrapolationFactor*hist[2]->GetBinError(iB));
    for (int iSys=2; iSys<12; ++iSys) {
      histSys[1][iSys]->SetBinContent(iB,dyExtrapolationFactor*histSys[1][iSys]->GetBinContent(iB));
      histSys[1][iSys]->SetBinError(iB,dyExtrapolationFactor*histSys[1][iSys]->GetBinError(iB));
    }
  }
  */

  float qcdNorm = histSS[0]->GetSumOfWeights();
  float qcdNormRelaxed = histSSrelaxed[0]->GetSumOfWeights(); 
  float ratioQCD = qcdNorm / qcdNormRelaxed;

  std::cout << "QCDNorm = " << qcdNorm << "   QCDNormRelaxed = " << qcdNormRelaxed << "   ratio = " << ratioQCD << std::endl;

  histSS[0]->SetLineColor(2);
  histSSrelaxed[0]->SetLineColor(4);

  for (int iB=1; iB<=nBins; ++iB) {
    float xSSrelaxed = histSSrelaxed[0]->GetBinContent(iB);
    float xSSErelaxed = histSSrelaxed[0]->GetBinError(iB);
    std::cout << "QCD relaxed :  " << iB << "  =  " << xSSrelaxed << " +/- " << xSSErelaxed << std::endl;
    float xHist = ratioQCD*histSSrelaxed[0]->GetBinContent(iB);
    float eHist = ratioQCD*histSSrelaxed[0]->GetBinError(iB);
    histSSrelaxed[0]->SetBinContent(iB,xHist);
    histSSrelaxed[0]->SetBinError(iB,eHist);
  }

  /*
  TCanvas * c1 = new TCanvas("c1","",600,600);

  float yMax = 2*histSS[0]->GetMaximum();
  histSS[0]->GetYaxis()->SetRangeUser(0.,yMax);

  histSS[0]->Draw();
  histSSrelaxed[0]->Draw("same");

  c1->Update();
  */

  std::cout << "Cross check of QCD norm. : " << histSSrelaxed[0]->GetSumOfWeights()/histSS[0]->GetSumOfWeights() << std::endl;


  TH1D * histData = (TH1D*)hist[0]->Clone("data_obs");
  TH1D * QCD = (TH1D*)histSSrelaxed[0]->Clone("QCD");
  TH1D * ZTT = (TH1D*)hist[1]->Clone("ZTT");
  TH1D * ZLL = (TH1D*)hist[2]->Clone("ZLL");
  TH1D * W   = (TH1D*)hist[5]->Clone("W");
  TH1D * TT  = (TH1D*)hist[6]->Clone("TT");
  TH1D * VV  = (TH1D*)hist[7]->Clone("VV");
  TH1D * EWKZ = (TH1D*)hist[22]->Clone("EWKZ");
  TH1D * ggH125 = (TH1D*)hist[23]->Clone("ggH125");
  TH1D * qqH125 = (TH1D*)hist[24]->Clone("qqH125");
  TH1D * WH125 = (TH1D*)hist[25]->Clone("WH125");
  TH1D * ZH125  = (TH1D*)hist[27]->Clone("ZH125");
  TH1D * TTH125 = (TH1D*)hist[28]->Clone("TTH125");
  TH1D * ggHWW125 = (TH1D*)hist[29]->Clone("HWW_gg125");
  TH1D * qqHWW125 = (TH1D*)hist[30]->Clone("HWW_qq125");

  TString BaseName = "htt_em.inputs-sm-13TeV_" +Suffix;
  TString rootFileName = BaseName+".root";
  TFile * fileInputs = new TFile(rootFileName,"recreate"); 
  fileInputs->mkdir(category);
  fileInputs->cd(category);
  histData->Write("data_obs");
  TT->Write("TT");
  ZTT->Write("ZTT");
  ZLL->Write("ZL");
  W->Write("W");
  QCD->Write("QCD");
  VV->Write("VV");
  EWKZ->Write("EWKZ");
  ggH125->Write("ggH125");
  qqH125->Write("qqH125");
  WH125->Write("WH125");
  ZH125->Write("ZH125");
  TTH125->Write("TTH125");
  ggHWW125->Write("HWW_gg125");
  qqHWW125->Write("HWW_qq125");

  for (int iSys=4; iSys<6; ++iSys) {
    histSys[1][iSys]->Write("ZTT"+sysName[iSys]);
    histSys[2][iSys]->Write("ZL"+sysName[iSys]);
    histSys[5][iSys]->Write("W"+sysName[iSys]);
    histSys[6][iSys]->Write("TT"+sysName[iSys]);
    histSys[7][iSys]->Write("VV"+sysName[iSys]);
    histSys[22][iSys]->Write("EWKZ"+sysName[iSys]);
    histSys[23][iSys]->Write("ggH125"+sysName[iSys]);
    histSys[24][iSys]->Write("qqH125"+sysName[iSys]);
    histSys[25][iSys]->Write("WH125"+sysName[iSys]);
    histSys[27][iSys]->Write("ZH125"+sysName[iSys]);
    histSys[28][iSys]->Write("TTH125"+sysName[iSys]);
    histSys[29][iSys]->Write("HWW_gg125"+sysName[iSys]);
    histSys[30][iSys]->Write("HWW_qq125"+sysName[iSys]);
  }
  for (int iSys=0; iSys<2; ++iSys) {
    histSys[6][iSys]->Write("TT"+sysName[iSys]);
  }
  for (int iSys=10; iSys<12; ++iSys) {
    histSys[1][iSys]->Write("ZTT"+sysName[iSys]);
    histSys[2][iSys]->Write("ZL"+sysName[iSys]);
  }

  fileInputs->Close();

  ostringstream str;
  str << BaseName << ".txt";
  string nn = str.str();
  const char * p = nn.c_str();

  std::ofstream textFile(p);
  textFile << "imax 1   number of channels" << std::endl;
  textFile << "jmax *   number of backgrounds" << std::endl;
  textFile << "kmax *   number of nuisance parameters" << std::endl;
  textFile << "-----------------" << std::endl;
  textFile << "observation " << histData->GetSumOfWeights() << std::endl;
  textFile << "-----------------" << std::endl;
  textFile << "shapes * * " << rootFileName << "  " << category << "/$PROCESS    " << category << "/$PROCESS_$SYSTEMATIC " << std::endl;
  textFile << "-----------------" << std::endl;
  textFile << "bin                               emu    emu    emu    emu    emu    emu" << std::endl;
  textFile << "process                           ZTT     TT    QCD      W     VV    ZLL" << std::endl;
  textFile << "process                             0      1      2      3      4      5" << std::endl;
  textFile << "rate     " 
	   << ZTT->GetSumOfWeights() << "  " 
	   << TT->GetSumOfWeights() << "  "
	   << QCD->GetSumOfWeights() << "  " 
	   << W->GetSumOfWeights() << "  " 
	   << VV->GetSumOfWeights() << "  " 
	   << ZLL->GetSumOfWeights() << std::endl;
  textFile << "-----------------------------" << std::endl;
  textFile << "CMS_eff_m                   lnN   1.03   1.03      -   1.03   1.03      -" << std::endl;
  textFile << "CMS_eff_e                   lnN   1.04   1.04      -   1.04   1.04      -" << std::endl;
  textFile << "CMS_ztt_ttjNorm             lnN      -   1.07      -      -      -      -" << std::endl;
  textFile << "CMS_ztt_wNorm               lnN      -      -      -   1.15      -      -" << std::endl;
  textFile << "CMS_ztt_vvNorm              lnN      -      -      -      -   1.20      -" << std::endl;
  textFile << "CMS_ztt_em_zllNorm          lnN      -      -      -      -      -   1.20" << std::endl;
  textFile << "CMS_ztt_em_qcdNorm          lnN      -      -   1.15      -      -      -" << std::endl;
  textFile << "lumi_13TeV                  lnN   1.03   1.03      -   1.03   1.03      -" << std::endl;
  textFile << "CMS_scale_m               shape   1.00   1.00      -   1.00   1.00      -" << std::endl;
  textFile << "CMS_scale_e               shape   1.00   1.00      -   1.00   1.00      -" << std::endl;
  //  textFile << "CMS_topPt                 shape      -   1.00      -      -      -      -" << std::endl;
  //  textFile << "CMS_ztt_em_qcdShape       shape      -      -   1.00      -      -      -" << std::endl;
  textFile << "CMS_ztt_boson_scale_met     lnN   1.02      -      -   1.02      -      -" << std::endl;
  textFile << "CMS_ztt_ewkTop_scale_met    lnN      -   1.03      -      -   1.03      -" << std::endl;
  textFile << "CMS_ztt_boson_reso_met      lnN   1.02      -      -   1.02      -      -" << std::endl;
  textFile << "CMS_ztt_ewkTop_reso_met     lnN      -   1.01      -      -   1.01      -" << std::endl;

  std::cout << "QCD : " << QCD->GetSumOfWeights() << std::endl;
  std::cout << "VV  : " << VV->GetSumOfWeights() << std::endl;
  std::cout << "W   : " << W->GetSumOfWeights() << std::endl;
  std::cout << "TT  : " << TT->GetSumOfWeights() << std::endl;
  std::cout << "ZLL : " << ZLL->GetSumOfWeights() << std::endl;
  std::cout << "ZTT : " << ZTT->GetSumOfWeights() << std::endl;

  TH1D * dummy = (TH1D*)ZTT->Clone("dummy");
  float errLumi = 0.03;
  float errMuon = 0.03;
  float errElectron = 0.04;
  float errQCD = 0.15;
  float errVV = 0.2;
  float errW = 0.15;
  float errTT = 0.1;
  for (int iB=1; iB<=nBins; ++iB) {
    float eQCD = errQCD*QCD->GetBinContent(iB);
    float eVV = errVV*VV->GetBinContent(iB);
    float eW = errW*W->GetBinContent(iB);
    float eTT = errTT*TT->GetBinContent(iB);
    float err2 = eQCD*eQCD + eVV*eVV + eW*eW + eTT*eTT;
    float errTot = TMath::Sqrt(err2);
    dummy->SetBinError(iB,errTot);
  }

  VV->Add(VV,QCD);
  W->Add(W,VV);
  TT->Add(TT,W);
  ZLL->Add(ZLL,TT);
  ZTT->Add(ZTT,ZLL);
  std::cout << "BKG : " << ZTT->GetSumOfWeights() << std::endl;
  std::cout << "DAT : " << histData->GetSumOfWeights() << std::endl;
  // MET SCale
  std::cout << std::endl;
  std::cout << "MET Scale -> " << std::endl;
  std::cout << "VV  : " << histSys[7][6]->GetSumOfWeights()/hist[7]->GetSumOfWeights() << std::endl;
  std::cout << "W   : " << histSys[5][6]->GetSumOfWeights()/hist[5]->GetSumOfWeights() << std::endl;
  std::cout << "TT  : " << histSys[6][6]->GetSumOfWeights()/hist[6]->GetSumOfWeights() << std::endl;
  std::cout << "ZLL : " << histSys[2][6]->GetSumOfWeights()/hist[2]->GetSumOfWeights() << std::endl;
  std::cout << "ZTT : " << histSys[1][6]->GetSumOfWeights()/hist[1]->GetSumOfWeights() << std::endl;
  // MET Resolution
  std::cout << std::endl;
  std::cout << "MET Resolution -> " << std::endl;
  std::cout << "VV  : " << histSys[7][8]->GetSumOfWeights()/hist[7]->GetSumOfWeights() << std::endl;
  std::cout << "W   : " << histSys[5][8]->GetSumOfWeights()/hist[5]->GetSumOfWeights() << std::endl;
  std::cout << "TT  : " << histSys[6][8]->GetSumOfWeights()/hist[6]->GetSumOfWeights() << std::endl;
  std::cout << "ZLL : " << histSys[2][8]->GetSumOfWeights()/hist[2]->GetSumOfWeights() << std::endl;
  std::cout << "ZTT : " << histSys[1][8]->GetSumOfWeights()/hist[1]->GetSumOfWeights() << std::endl;
  std::cout << std::endl;

  TH1D * bkgdErr = (TH1D*)ZTT->Clone("bkgdErr");
  bkgdErr->SetFillStyle(3013);
  bkgdErr->SetFillColor(1);
  bkgdErr->SetMarkerStyle(21);
  bkgdErr->SetMarkerSize(0);  
  
  for (int iB=1; iB<=nBins; ++iB) {
    QCD->SetBinError(iB,0);
    VV->SetBinError(iB,0);
    TT->SetBinError(iB,0);
    W->SetBinError(iB,0);
    ZLL->SetBinError(iB,0);
    ZTT->SetBinError(iB,0);
    float eStat =  bkgdErr->GetBinError(iB);
    float X = bkgdErr->GetBinContent(iB);
    float eLumi = errLumi * X;
    float eMuon = errMuon * X;
    float eElectron = errElectron * X;
    float eBkg = dummy->GetBinError(iB);
    float Err = TMath::Sqrt(eStat*eStat+eLumi*eLumi+eBkg*eBkg+eMuon*eMuon+eElectron*eElectron);
    bkgdErr->SetBinError(iB,Err);
    bkgdErr->SetBinContent(iB,ZTT->GetBinContent(iB));
    std::cout << iB << "  :  " << bkgdErr->GetBinContent(iB) << " " << bkgdErr->GetBinError(iB) << std::endl;
    ggH125->SetBinError(iB,0);
    qqH125->SetBinError(iB,0);
  }

  ggH125->SetLineColor(2);
  qqH125->SetLineColor(4);

  InitData(histData); histData->GetXaxis()->SetRangeUser(xmin,xmax);
  InitHist(QCD,TColor::GetColor("#FFCCFF"));
  InitHist(W,TColor::GetColor("#DE5A6A"));
  InitHist(TT,TColor::GetColor("#9999CC"));
  InitHist(VV,TColor::GetColor("#6F2D35"));
  InitHist(ZLL,TColor::GetColor("#4496C8"));
  InitHist(ZTT,TColor::GetColor("#FFCC66"));
  histData->GetXaxis()->SetTitle(xtitle);
  histData->GetYaxis()->SetTitle(ytitle);
  histData->GetYaxis()->SetTitleOffset(1.5);
  histData->GetYaxis()->SetTitleSize(0.06);
  float yUpper = histData->GetMaximum();
  if (logY)
    histData->GetYaxis()->SetRangeUser(0.5,1000*yUpper);
  else
    histData->GetYaxis()->SetRangeUser(0,1.2*yUpper);

  histData->SetMarkerSize(1.2);
  histData->GetXaxis()->SetLabelSize(0);
  histData->GetYaxis()->SetLabelSize(0.06);

  //  nData = histData->GetSum();
  //  float nMC   = TT->GetSum();
  //  float eData = TMath::Sqrt(nData);

  // Blinding data
  if (blindData) {
    for (int iB=1; iB<=nBins; ++iB) {
      for (int iBlind=0; iBlind<nBlindBins; ++iBlind) {
	if (iB==blindDataBins[iBlind]) {
	  histData->SetBinContent(iB,100000000);
	  histData->SetBinError(iB,0);
	}
      }
    }
  }


  TCanvas * canv1 = MakeCanvas("canv1", "", 700, 800);
  TPad *upper = new TPad("upper", "pad",0,0.31,1,1);
  upper->Draw();
  upper->cd();
  upper->SetFillColor(0);
  upper->SetBorderMode(0);
  upper->SetBorderSize(10);
  upper->SetTickx(1);
  upper->SetTicky(1);
  upper->SetLeftMargin(0.17);
  upper->SetRightMargin(0.05);
  upper->SetBottomMargin(0.02);
  upper->SetFrameFillStyle(0);
  upper->SetFrameLineStyle(0);
  upper->SetFrameLineWidth(2);
  upper->SetFrameBorderMode(0);
  upper->SetFrameBorderSize(10);
  upper->SetFrameFillStyle(0);
  upper->SetFrameLineStyle(0);
  upper->SetFrameLineWidth(2);
  upper->SetFrameBorderMode(0);
  upper->SetFrameBorderSize(10);

  histData->Draw("e1");
  ZTT->Draw("sameh");
  ZLL->Draw("sameh");
  TT->Draw("sameh");
  W->Draw("sameh");
  //  VV->Draw("sameh");
  QCD->Draw("sameh");
  histData->Draw("e1same");
  bkgdErr->Draw("e2same");
  ggH125->Draw("sameh");
  qqH125->Draw("sameh");
  float chi2 = 0;
  for (int iB=1; iB<=nBins; ++iB) {
    float xData = histData->GetBinContent(iB);
    float xMC = ZTT->GetBinContent(iB);
    if (xMC>1e-1) {
      float diff2 = (xData-xMC)*(xData-xMC);
      chi2 += diff2/xMC;
    }
  }
  std::cout << std::endl;
  std::cout << "Chi2 = " << chi2 << std::endl;
  std::cout << std::endl;

  TLegend * leg = new TLegend(0.5,0.5,0.8,0.90);
  //  TLegend * leg = new TLegend(0.3,0.4,0.52,0.78);
  SetLegendStyle(leg);
  leg->SetTextSize(0.04);
  leg->AddEntry(histData,"Data","lp");
  leg->AddEntry(ZTT,"Z#rightarrow #tau#tau","f");
  leg->AddEntry(ZLL,"Z#rightarrow ll","f");
  leg->AddEntry(TT,"t#bar{t}","f");
  leg->AddEntry(W,"electroweak","f");
  //  leg->AddEntry(VV,"electroweak","f");
  leg->AddEntry(QCD,"QCD","f");
  leg->AddEntry(ggH125,"ggH125(x10)","l");
  leg->AddEntry(qqH125,"qqH125(x10)","l");
  leg->Draw();
  writeExtraText = true;
  extraText = "Preliminary";
  CMS_lumi(upper,4,33); 
  plotchannel("e#mu");

  char KT[100];
  sprintf(KT,"#chi^{2} : %5.1f",chi2);
  TLatex * cms = new TLatex(0.25,0.85,KT);
  cms->SetNDC();
  cms->SetTextSize(0.05);
  //  cms->Draw();

  if (logY) upper->SetLogy(true);
    
  upper->Draw("SAME");
  upper->RedrawAxis();
  upper->Modified();
  upper->Update();
  canv1->cd();

  TH1D * ratioH = (TH1D*)histData->Clone("ratioH");
  TH1D * ratioErrH = (TH1D*)bkgdErr->Clone("ratioErrH");
  ratioH->SetMarkerColor(1);
  ratioH->SetMarkerStyle(20);
  ratioH->SetMarkerSize(1.2);
  ratioH->SetLineColor(1);
  ratioH->GetYaxis()->SetRangeUser(0.4,1.6);
  ratioH->GetYaxis()->SetNdivisions(505);
  ratioH->GetXaxis()->SetLabelFont(42);
  ratioH->GetXaxis()->SetLabelOffset(0.04);
  ratioH->GetXaxis()->SetLabelSize(0.14);
  ratioH->GetXaxis()->SetTitleSize(0.13);
  ratioH->GetXaxis()->SetTitleOffset(1.2);
  ratioH->GetYaxis()->SetTitle("obs/exp");
  ratioH->GetYaxis()->SetLabelFont(42);
  ratioH->GetYaxis()->SetLabelOffset(0.015);
  ratioH->GetYaxis()->SetLabelSize(0.13);
  ratioH->GetYaxis()->SetTitleSize(0.14);
  ratioH->GetYaxis()->SetTitleOffset(0.5);
  ratioH->GetXaxis()->SetTickLength(0.07);
  ratioH->GetYaxis()->SetTickLength(0.04);
  ratioH->GetYaxis()->SetLabelOffset(0.01);

  for (int iB=1; iB<=nBins; ++iB) {
    float x1 = histData->GetBinContent(iB);
    float x2 = ZTT->GetBinContent(iB);
    ratioErrH->SetBinContent(iB,1.0);
    ratioErrH->SetBinError(iB,0.0);
    float xBkg = bkgdErr->GetBinContent(iB);
    float errBkg = bkgdErr->GetBinError(iB);
    if (xBkg>0) {
      float relErr = errBkg/xBkg;
      ratioErrH->SetBinError(iB,relErr);

    }
    if (x1>0&&x2>0) {
      float e1 = histData->GetBinError(iB);
      float ratio = x1/x2;
      float eratio = e1/x2;
      ratioH->SetBinContent(iB,ratio);
      ratioH->SetBinError(iB,eratio);
    }
    else {
      ratioH->SetBinContent(iB,1000);
    }
  }

  // ------------>Primitives in pad: lower
  TPad * lower = new TPad("lower", "pad",0,0,1,0.30);
  lower->Draw();
  lower->cd();
  lower->SetFillColor(0);
  lower->SetBorderMode(0);
  lower->SetBorderSize(10);
  lower->SetGridy();
  lower->SetTickx(1);
  lower->SetTicky(1);
  lower->SetLeftMargin(0.17);
  lower->SetRightMargin(0.05);
  lower->SetTopMargin(0.026);
  lower->SetBottomMargin(0.35);
  lower->SetFrameFillStyle(0);
  lower->SetFrameLineStyle(0);
  lower->SetFrameLineWidth(2);
  lower->SetFrameBorderMode(0);
  lower->SetFrameBorderSize(10);
  lower->SetFrameFillStyle(0);
  lower->SetFrameLineStyle(0);
  lower->SetFrameLineWidth(2);
  lower->SetFrameBorderMode(0);
  lower->SetFrameBorderSize(10);

  ratioH->Draw("e1");
  ratioErrH->Draw("e2same");

  lower->Modified();
  lower->RedrawAxis();
  canv1->cd();
  canv1->Modified();
  canv1->cd();
  canv1->SetSelected(canv1);

  canv1->Print("figures/"+Suffix+"_1D.png");
}
