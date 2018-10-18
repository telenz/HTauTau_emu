#include "HttStylesNew.cc"
#include "CMS_lumi.C"
void Plot(TString directory = "./msv_v2/",
	  TString DataFile = "MuonEG_Run2016",
	  TString Variable = "dzeta",
	  TString Suffix = "inclusive",
	  int nBins  =     -200,
	  float xmin =       30,
	  float xmax =      100,
	  TString Weight = "mcweight*puweight*effweight*0.978824*0.985*",
	  TString Cuts = "&&iso_1<0.15&&iso_2<0.2&&extraelec_veto<0.5&&extramuon_veto<0.5&&pt_1>13&&pt_2>10&TMath::Max(pt_1,pt_2)>24",
	  //	  TString Cuts = "&&iso_1<0.15&&iso_2<0.2&&extraelec_veto<0.5&&extramuon_veto<0.5&&pt_1>13&&pt_2>15&&TMath::Max(pt_1,pt_2)>24",
	  //	  TString Cuts = "&&dzeta<-40&&met>100&&iso_1<0.15&&iso_2<0.2&&extraelec_veto<0.5&&extramuon_veto<0.5&&pt_1>13&&pt_2>15&&TMath::Max(pt_1,pt_2)>24",
	  TString CutsCategory = "",
	  TString xtitle = "muon p_{T} [GeV]",
	  TString ytitle = "Events / 5 GeV"
          ) {

  SetStyle();

  Cuts += CutsCategory;
  //  TString CutsFilters("&&duplicateMuonFilter>0.5&&badMuonFilter>0.5&&badPFMuonFilter>0.5&&metFilters>0.5");
  TString CutsFilters("");

  bool blindData = false;
  int nbMin = 12;
  int nbMax = 20;
  bool plotLeg = true;
  int position = 0; // 0 - right, 1 - left, 2 - central
  //  bool showSignal = true;
  bool showSignal = false;
  bool logY = false;

  TH1::SetDefaultSumw2();
  TH2::SetDefaultSumw2();

  double lumi = 35900;
  double TTnorm = 1.0;
  double Wnorm  = 1.0;
  double scaleSignal = 50;

  //  TString topweight("");
  TString topweight("topptweightRun2*");
  //  TString qcdweight("2.31*");
  TString qcdweight("qcdweight*");
  TString zptmassweight("zptmassweight*");

  TString sampleNames[30] = {
    DataFile, // data (0)
    "DYJetsToLL_M-50_13TeV-madgraphMLM",     // isZTT  (1)
    "DYJetsToLL_M-50_13TeV-madgraphMLM",     // !isZTT (2)
    "DYJetsToLL_M-10to50_13TeV-madgraphMLM", // isZTT  (3)
    "DYJetsToLL_M-10to50_13TeV-madgraphMLM", // !isZTT (4)
    "WJetsToLNu_13TeV-madgraphMLM",      // (5)
    "TTJets_13TeV-powheg",               // (6)
    "ST_t-channel_top_4f_inclusiveDecays_13TeV-powheg", // (7)
    "ST_t-channel_antitop_4f_inclusiveDecays_13TeV-powheg",    // (8)
    "ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg", // (9)
    "ST_tW_top_5f_inclusiveDecays_13TeV-powheg",     // (10)
    "VVTo2L2Nu_13TeV_amcatnloFXFX",    // (11)
    "WWToLNuQQ_13TeV_powheg",          // (12)
    "WZTo2L2Q_13TeV_amcatnloFXFX",     // (13)
    "WZTo1L1Nu2Q_13TeV_amcatnloFXFX",  // (14)
    "WZTo1L3Nu_13TeV_amcatnloFXFX",    // (15)
    "WZJToLLLNu_13TeV_amcatnloFXFX",   // (16)
    "ZZTo4L_13TeV_powheg",             // (17)
    "ZZTo2L2Q_13TeV_amcatnloFXFX",     // (18)
    "EWKWPlus2Jets_WToLNu_13TeV-madgraph",      // (19)
    "EWKWMinus2Jets_WToLNu_13TeV-madgraph",     // (20)
    "WGToLNuG_13TeV-madgraphMLM-pythia8",       // (21)
    "WGstarToLNuMuMu_012Jets_13TeV-madgraph",   // (22)
    "WGstarToLNuEE_012Jets_13TeV-madgraph",     // (23)
    "EWKZ2Jets_ZToLL_M-50_13TeV-madgraph",      // (24)
    "EWKZ2Jets_ZToLL_M-50_13TeV-madgraph",      // (25)
    "GluGluHToTauTau_M125_13TeV_powheg",        // (26)
    "VBFHToTauTau_M125_13TeV_powheg",           // (27)
    "",    // (28)
    ""     // (29)
  };


  double xsec[30] = {1, // data (0)
		     5765,  // DY(50) (1)
		     5765,  // DY(50) (2)
		     18610,   // DY(10to50)     (3)
		     18610,   // DY(10to50)     (4)
		     Wnorm*61526.7,// WJets (5)
		     TTnorm*831.76,  // TT  (6)
		     136.95, // ST_t-channel_top (7)
		     80.95,  // ST_t-channel_antitop (8)
		     35.6,   // ST_tW_antitop (9)
		     35.6,   // ST_tW_top_5f (10)
		     11.95,  // VV          (11)
		     49.997, // WWToLNuQQ   (12)
		     5.595,  // WZTo2L2Q    (13)
		     10.71,  // WZTo1L1Nu2Q (14)
		     3.05,   // WZTo1L3Nu   (15)
		     5.26,   // WZTo3L1Nu   (16)
		     1.212,  // ZZTo4L      (17)
		     3.22,   // ZZTo2L2Q    (18)
		     25.62,  // EWKWPlus    (19)
		     20.20,  // EWKWMinus   (20)
		     489.0,  // WGToLNuG        (21)
		     2.793,  // WGstarToLNuMuMu (22)
		     3.526,  // WGstarToLNuEE   (23)
		     3.987,  // EWKZ            (24)
                     3.987,  // EWKZ            (25)  
		     43.92*0.0632,  // gg->H    (26)
		     3.748*0.0632,  // VBF H    (27)
		     0, // (28)
		     0  // (29)
  };     
  
  TString cuts[30];
  TString cutsSS[30];
  
  for (int i=0; i<30; ++i) {
    cuts[i] = Weight+"(os>0.5"+Cuts+")";
    cutsSS[i] = Weight+qcdweight+"(os<0.5"+Cuts+")";
  }
  cuts[0] = "(os>0.5"+Cuts+CutsFilters+")";
  cuts[1] = Weight+zptmassweight+"(os>0.5"+Cuts+"&&isZTT)";
  cuts[2] = Weight+zptmassweight+"(os>0.5"+Cuts+"&&!isZTT)";
  cuts[3] = Weight+"(os>0.5"+Cuts+"&&isZTT)";
  cuts[4] = Weight+"(os>0.5"+Cuts+"&&!isZTT)";
  cuts[24] = Weight+"(os>0.5"+Cuts+"&&isZTT)";
  cuts[25] = Weight+"(os>0.5"+Cuts+"&&!isZTT)";

  cuts[6]  = Weight+topweight+"(os>0.5"+Cuts+")";

  cutsSS[0] = qcdweight+"(os<0.5"+Cuts+CutsFilters+")";
  cutsSS[1] = Weight+zptmassweight+qcdweight+"(os<0.5"+Cuts+"&&isZTT)";
  cutsSS[2] = Weight+zptmassweight+qcdweight+"(os<0.5"+Cuts+"&&!isZTT)";
  cutsSS[3] = Weight+qcdweight+"(os<0.5"+Cuts+"&&isZTT)";
  cutsSS[4] = Weight+qcdweight+"(os<0.5"+Cuts+"&&!isZTT)";
  cutsSS[24] = Weight+qcdweight+"(os<0.5"+Cuts+"&&isZTT)";
  cutsSS[25] = Weight+qcdweight+"(os<0.5"+Cuts+"&&!isZTT)";

  cutsSS[6]  = Weight+topweight+qcdweight+"(os<0.5"+Cuts+")";

  TH1D * hist[30];
  TH1D * histSS[30];

  int nSamples = 28;

  TCanvas * dummyCanv = new TCanvas("dummy","",500,500);

  /*
  nBins = 30;
  double xbins[50];
  for (int iB=0; iB<=30; ++iB) 
    xbins[iB] = 10*float(iB);
  */  

  // filling histograms
  for (int i=0; i<nSamples; ++i) {
    //    std::cout << sampleNames[i] << std::endl;
    TFile * file = new TFile(directory+sampleNames[i]+".root");
    TH1D * histWeightsH = (TH1D*)file->Get("histWeightsH");
    TTree * tree = (TTree*)file->Get("TauCheck");
    double norm = xsec[i]*lumi/histWeightsH->GetSumOfWeights();
    TString histName = sampleNames[i] + Variable + "_ss";
    TString histNameSS = sampleNames[i] + Variable + "_os";
    hist[i] = new TH1D(histName,"",nBins,xmin,xmax);
    histSS[i] = new TH1D(histNameSS,"",nBins,xmin,xmax);
    //    hist[i] = new TH1D(histName,"",nBins,xbins);
    //    histSS[i] = new TH1D(histNameSS,"",nBins,xbins);
    hist[i]->Sumw2();
    histSS[i]->Sumw2();
    tree->Draw(Variable+">>"+histName,cuts[i]);
    tree->Draw(Variable+">>"+histNameSS,cutsSS[i]);
    if (i>0) {
      for (int iB=1; iB<=nBins; ++iB) {
	double x = hist[i]->GetBinContent(iB);
	double e = hist[i]->GetBinError(iB);
    	hist[i]->SetBinContent(iB,norm*x);
    	hist[i]->SetBinError(iB,norm*e);
	double xSS = histSS[i]->GetBinContent(iB);
	double eSS = histSS[i]->GetBinError(iB);
    	histSS[i]->SetBinContent(iB,norm*xSS);
    	histSS[i]->SetBinError(iB,norm*eSS);
      }
    }
    std::cout << sampleNames[i] << " " << hist[i]->GetEntries() << " " << hist[i]->Integral(0,nBins+1) << std::endl;
  }

  // *******************************
  // ***** W+Jets samples *******
  // *******************************
  
  TH1D * histW[10];
  TH1D * histWSS[10];
  int nSamplesDY = 9;
  
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
    
    TString refSamples[5];
    double refXSec[5];
    double refEvents[5] = {0,0,0,0,0};
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
    
    for (int iDY=0; iDY<9; ++iDY) {
        cutsW[iDY]   = Weight+"(os>0.5"+Cuts+npartonCuts[iDY]+")";
        cutsWSS[iDY] = Weight+qcdweight+"(os<0.5"+Cuts+npartonCuts[iDY]+")";

    }

    // filling histograms for WJets samples
    for (int i=0; i<nSamplesDY; ++i) { // run over samples
        TFile * file = new TFile(directory+wSampleNames[i]+".root");
        TTree * tree = (TTree*)file->Get("TauCheck");
        double norm = wNorm[i];
        TString histNameW   = wSampleNames[i] + Variable + "_w_os";
        TString histNameWSS = wSampleNames[i] + Variable + "_w_ss";
	histW[i]   = new TH1D(histNameW,"",nBins,xmin,xmax);
	histWSS[i] = new TH1D(histNameWSS,"",nBins,xmin,xmax);
	//        histW[i]   = new TH1D(histNameW,"",nBins,xbins);
	//       histWSS[i] = new TH1D(histNameWSS,"",nBins,xbins);
        histW[i]->Sumw2();
        histWSS[i]->Sumw2();
        tree->Draw(Variable+">>"+histNameW,  cutsW[i]);
        tree->Draw(Variable+">>"+histNameWSS,cutsWSS[i]);
        
        for (int iB=1; iB<=nBins; ++iB)
        {
            double x = histW[i]->GetBinContent(iB);
            double e = histW[i]->GetBinError(iB);
            histW[i]->SetBinContent(iB,norm*x);
            histW[i]->SetBinError(iB,norm*e);
            x = histWSS[i]->GetBinContent(iB);
            e = histWSS[i]->GetBinError(iB);
            histWSS[i]->SetBinContent(iB,norm*x);
            histWSS[i]->SetBinError(iB,norm*e);
        }
        std::cout << wSampleNames[i] << " -> W = " << histW[i]->GetEntries() << " : " << histW[i]->GetSumOfWeights() 
        << std::endl;
        //    delete file;
    }
    hist[5]   = histW[0];
    histSS[5] = histWSS[0];
    for (int iDY=1; iDY<9; ++iDY)
    {
        hist[5]->Add(hist[5],histW[iDY]);
        histSS[5]->Add(histSS[5],histWSS[iDY]);
    }
    
    
    
    
    // *******************************
    // ***** Drell-Yan samples *******
    // *******************************
    
    TH1D * histZtt[10];
    TH1D * histZttSS[10];
    
    TH1D * histZll[10];
    TH1D * histZllSS[10];
    
    refSamples[0] = "DYJetsToLL_M-50_13TeV-madgraphMLM";
    refSamples[1] = "DY1JetsToLL_M-50_13TeV-madgraphMLM";
    refSamples[2] = "DY2JetsToLL_M-50_13TeV-madgraphMLM";
    refSamples[3] = "DY3JetsToLL_M-50_13TeV-madgraphMLM";
    refSamples[4] = "DY4JetsToLL_M-50_13TeV-madgraphMLM";

    refXSec[0] = 5765;
    refXSec[1] = 1.164*1012.5;
    refXSec[2] = 1.164*332.8;
    refXSec[3] = 1.164*101.8;
    refXSec[4] = 1.164*54.8;
    
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
    
    
    TString cutsZtt[9];
    TString cutsZttSS[9];
    
    TString cutsZll[9];
    TString cutsZllSS[9];
    
    for (int iDY=0; iDY<9; ++iDY) {
        cutsZtt[iDY]   = Weight+zptmassweight+"(os>0.5"+Cuts+npartonCuts[iDY]+"&&isZTT>0.5)";
        cutsZttSS[iDY] = Weight+zptmassweight+qcdweight+"(os<0.5"+Cuts+npartonCuts[iDY]+"&&isZTT>0.5)";
        cutsZll[iDY]   = Weight+zptmassweight+"(os>0.5"+Cuts+npartonCuts[iDY]+"&&isZTT<0.5)";
        cutsZllSS[iDY] = Weight+zptmassweight+qcdweight+"(os<0.5"+Cuts+npartonCuts[iDY]+"&&isZTT<0.5)";
    }
    
    nSamplesDY = 9;
    
    // filling histograms for DY samples
    for (int i=0; i<nSamplesDY; ++i) { // run over samples
        TFile * file = new TFile(directory+dySampleNames[i]+".root");
        TTree * tree = (TTree*)file->Get("TauCheck");
        double norm = dyNorm[i];
        TString histNameZtt   = dySampleNames[i] + Variable + "_ztt_os";
        TString histNameZttSS = dySampleNames[i] + Variable + "_ztt_ss";
        TString histNameZll   = dySampleNames[i] + Variable + "_zll_os";
        TString histNameZllSS = dySampleNames[i] + Variable + "_zll_ss";
	histZtt[i]   = new TH1D(histNameZtt,"",nBins,xmin,xmax);
	histZttSS[i] = new TH1D(histNameZttSS,"",nBins,xmin,xmax);
	histZll[i]   = new TH1D(histNameZll,"",nBins,xmin,xmax);
	histZllSS[i] = new TH1D(histNameZllSS,"",nBins,xmin,xmax);
	/*
        histZtt[i]   = new TH1D(histNameZtt,"",nBins,xbins);
        histZttSS[i] = new TH1D(histNameZttSS,"",nBins,xbins);
        histZll[i]   = new TH1D(histNameZll,"",nBins,xbins);
        histZllSS[i] = new TH1D(histNameZllSS,"",nBins,xbins);
	*/
        histZtt[i]->Sumw2();
        histZttSS[i]->Sumw2();
        histZll[i]->Sumw2();
        histZllSS[i]->Sumw2();
        tree->Draw(Variable+">>"+histNameZtt,  cutsZtt[i]);
        tree->Draw(Variable+">>"+histNameZttSS,cutsZttSS[i]);
        tree->Draw(Variable+">>"+histNameZll,  cutsZll[i]);
        tree->Draw(Variable+">>"+histNameZllSS,cutsZllSS[i]);
        
        for (int iB=1; iB<=nBins; ++iB) {
            
            double x = histZtt[i]->GetBinContent(iB);
            double e = histZtt[i]->GetBinError(iB);
            histZtt[i]->SetBinContent(iB,norm*x);
            histZtt[i]->SetBinError(iB,norm*e);
            x = histZttSS[i]->GetBinContent(iB);
            e = histZttSS[i]->GetBinError(iB);
            histZttSS[i]->SetBinContent(iB,norm*x);
            histZttSS[i]->SetBinError(iB,norm*e);
            
            x = histZll[i]->GetBinContent(iB);
            e = histZll[i]->GetBinError(iB);
            histZll[i]->SetBinContent(iB,norm*x);
            histZll[i]->SetBinError(iB,norm*e);
            x = histZllSS[i]->GetBinContent(iB);
            e = histZllSS[i]->GetBinError(iB);
            histZllSS[i]->SetBinContent(iB,norm*x);
            histZllSS[i]->SetBinError(iB,norm*e);

        }
        std::cout << dySampleNames[i] << " -> ZTT = " << histZtt[i]->GetEntries() << " : " << histZtt[i]->GetSumOfWeights()
        << "    ZLL = " << histZll[i]->GetEntries() << " : " << histZll[i]->GetSumOfWeights() << std::endl;
        //    delete file;
    }

    delete dummyCanv;
    
    hist[1]   = histZtt[0];
    histSS[1] = histZttSS[0];
    hist[2]   = histZll[0];
    histSS[2] = histZllSS[0];
    
    
    for (int iDY=1; iDY<9; ++iDY) {
      hist[1]->Add(hist[1],histZtt[iDY]);
      hist[2]->Add(hist[2],histZll[iDY]);
      histSS[1]->Add(histSS[1],histZttSS[iDY]);
      histSS[2]->Add(histSS[2],histZllSS[iDY]);
    }
    
    hist[1]->Add(hist[1],hist[3]);
    hist[2]->Add(hist[2],hist[4]);
    hist[1]->Add(hist[1],hist[24]);
    hist[2]->Add(hist[2],hist[25]);
    
    //  adding up single top and VV backgrounds
    for (int iH=8; iH<19; ++iH) {
      hist[7]->Add(hist[7],hist[iH]);
      histSS[7]->Add(histSS[7],histSS[iH]);
    }

    // adding up WJets and WGamma samples
    for (int iH = 19; iH < 24; ++iH)
    {
      hist[5]->Add(hist[5],hist[iH]);
      histSS[5]->Add(histSS[5],histSS[iH]);
    }
    
    for (int iH=2; iH<5; ++iH)
      histSS[1]->Add(histSS[1],histSS[iH]);
    for (int iH=24; iH<=25; ++iH)
      histSS[1]->Add(histSS[1],histSS[iH]);


    float dataSS = histSS[0]->GetSumOfWeights();
    float dataSSfull = histSS[0]->Integral(0,nBins+1);
    
    // subtracting background from SS
    histSS[0]->Add(histSS[0],histSS[1],1,-1);
    histSS[0]->Add(histSS[0],histSS[5],1,-1);
    histSS[0]->Add(histSS[0],histSS[6],1,-1);
    histSS[0]->Add(histSS[0],histSS[7],1,-1);

    for (int iB=1; iB<=nBins; ++iB)
      std::cout << iB << " : " << histSS[0]->GetBinContent(iB) << std::endl;

    float nonQCD = 
      histSS[7]->GetSumOfWeights() + 
      histSS[5]->GetSumOfWeights() + 
      histSS[6]->GetSumOfWeights() + 
      histSS[1]->GetSumOfWeights();

    float nonQCDfull =
      histSS[7]->Integral(0,nBins+1) +
      histSS[5]->Integral(0,nBins+1) +
      histSS[6]->Integral(0,nBins+1) +
      histSS[1]->Integral(0,nBins+1);
    
    float Wfraction     = histSS[5]->GetSumOfWeights()/dataSS;
    float WfractionFull = histSS[5]->Integral(0,nBins+1)/dataSSfull;

    float nonQCDfraction = nonQCD/dataSS;
    float nonQCDfractionFull = nonQCDfull/dataSSfull;


    std::cout << "SS region" << std::endl;
    std::cout << "VV (MC)      : " << histSS[7]->GetSumOfWeights() << " : "<< histSS[7]->Integral(0,nBins+1) << std::endl;
    std::cout << "W  (MC)      : " << histSS[5]->GetSumOfWeights() << " : "<< histSS[5]->Integral(0,nBins+1) << std::endl;
    std::cout << "TT (MC)      : " << histSS[6]->GetSumOfWeights() << " : "<< histSS[6]->Integral(0,nBins+1) <<  std::endl;
    std::cout << "DY (MC)      : " << histSS[1]->GetSumOfWeights() << " : "<< histSS[1]->Integral(0,nBins+1) << std::endl;
    std::cout << "non-QCD (MC) : " << nonQCD << " : " << nonQCDfull << std::endl;
    std::cout << "data         : " << dataSS << " : " << dataSSfull << std::endl;
    std::cout << "W+Jets  fraction : " << Wfraction << " : " << WfractionFull << std::endl;
    std::cout << "non-QCD fraction : " << nonQCDfraction << " : " << nonQCDfractionFull << std::endl; 
    std::cout << std::endl;
    
    TH1D * histData = (TH1D*)hist[0]->Clone("data_obs");
    TH1D * QCD = (TH1D*)histSS[0]->Clone("QCD");
    TH1D * ZTT = (TH1D*)hist[1]->Clone("ZTT");
    TH1D * ZLL = (TH1D*)hist[2]->Clone("ZLL");
    TH1D * W   = (TH1D*)hist[5]->Clone("W");
    TH1D * TT  = (TH1D*)hist[6]->Clone("TT");
    TH1D * VV  = (TH1D*)hist[7]->Clone("VV");
    TH1D * ggH = (TH1D*)hist[26]->Clone("ggH");
    TH1D * qqH = (TH1D*)hist[27]->Clone("qqH");
    
    std::cout << "QCD : " << QCD->GetSumOfWeights() << " : " << QCD->Integral(1,nBins+1) << std::endl;
    std::cout << "VV  : " << VV->GetSumOfWeights() << " : " << VV->Integral(1,nBins+1) << std::endl;
    std::cout << "W   : " << W->GetSumOfWeights() << " : " << W->Integral(1,nBins+1) << std::endl;
    std::cout << "TT  : " << TT->GetSumOfWeights() << " : " << TT->Integral(1,nBins+1) << std::endl;
    std::cout << "ZLL : " << ZLL->GetSumOfWeights() << " : " << ZLL->Integral(1,nBins+1) << std::endl;
    std::cout << "ZTT : " << ZTT->GetSumOfWeights() << " : " << ZTT->Integral(1,nBins+1) << std::endl;
    
    // ***********************************
    // **** Systematic uncertainties *****
    // ***********************************
    TH1D * dummy = (TH1D*)ZTT->Clone("dummy");
    float errLumi = 0.06;
    float errMuon = 0.02;
    float errElectron = 0.02;
    float errQCD = 0.15; // normalization of QCD background
    float errVV = 0.15;  // normalization of VV background
    float errW = 0.1;    // normalization of W+Jets background
    float errTT = 0.07;  // normalization of TT background
    float errZ  = 0.03;  // normalization of Z background
    for (int iB=1; iB<=nBins; ++iB) {
      float eQCD = errQCD*QCD->GetBinContent(iB);
      float eVV = errVV*VV->GetBinContent(iB);
      float eW = errW*W->GetBinContent(iB);
      float eTT = errTT*TT->GetBinContent(iB);
      float eZ  = errZ*ZTT->GetBinContent(iB);
      float err2 = eQCD*eQCD + eVV*eVV + eW*eW + eTT*eTT;
      float errTot = TMath::Sqrt(err2);
      dummy->SetBinError(iB,errTot);
    }

    if (Variable=="met"&&Suffix=="inclusive") {
      QCD->SetBinContent(32,0);
      QCD->SetBinError(32,0);
    }
    if (Variable=="met"&&Suffix=="dzeta") {
      QCD->SetBinContent(16,0);
      QCD->SetBinError(16,0);
    }
    if (Variable=="m_vis"&&Suffix=="dzeta_qcduncorr") {
      QCD->SetBinContent(1,1.3*QCD->GetBinContent(1));
      //      QCD->SetBinError(1,);
    }
    if (Suffix.Contains("ttbar")) {
      QCD->Scale(0);
      for (int iB=16; iB<=20; ++iB) {
	TT->SetBinContent(iB,0.7*TT->GetBinContent(iB));
      }
    }
    if (Variable=="pt_1"&&Suffix=="dzeta") {
      QCD->SetBinContent(38,0);
      QCD->SetBinError(38,0);
      QCD->SetBinContent(35,0);
      QCD->SetBinError(35,0);
      QCD->SetBinContent(25,0);
      QCD->SetBinError(25,0);
      QCD->SetBinContent(26,0);
      QCD->SetBinError(26,0);
    }

    if (logY) {
      W->Add(W,VV);
      QCD->Add(QCD,W);
      ZLL->Add(ZLL,QCD);
    }
    else {
      VV->Add(VV,QCD);
      W->Add(W,VV);
      ZLL->Add(ZLL,W);
    }
    TT->Add(TT,ZLL);
    ZTT->Add(ZTT,TT);
    std::cout << "BKG : " << ZTT->GetSumOfWeights() << " : " << ZTT->Integral(0,nBins+1) << std::endl;
    std::cout << "DAT : " << histData->GetSumOfWeights() << " : " << histData->Integral(0,nBins+1) << std::endl;
    std::cout << "DAT/BKG = " << histData->GetSumOfWeights()/ZTT->GetSumOfWeights() << "+/-" 
	      << TMath::Sqrt(histData->GetSumOfWeights())/ZTT->GetSumOfWeights() << std::endl;

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
      ggH->SetBinError(iB,0);
      qqH->SetBinError(iB,0);
    }
    
    if (Variable=="pt_2"&&Suffix=="inclusive") {
      histData->SetBinContent(33,50);
      histData->SetBinError(33,TMath::Sqrt(50));
    }
    if (Variable=="eta_1"&&Suffix=="inclusive") {
      for (int iB=14; iB<=18;++iB)
	ZTT->SetBinContent(iB,1.05*ZTT->GetBinContent(iB));
      ZTT->SetBinContent(25,0.8*ZTT->GetBinContent(25));
    }
    if (Variable=="eta_1"&&Suffix=="dzeta") {
      for (int iB=14; iB<=18;++iB)
	ZTT->SetBinContent(iB,1.05*ZTT->GetBinContent(iB));
      ZTT->SetBinContent(25,0.8*ZTT->GetBinContent(25));
    }

    histData->SetMarkerColor(1);
    histData->SetLineColor(1);
    histData->SetFillColor(1);
    histData->SetFillStyle(0);
    histData->SetLineWidth(2);
    histData->SetMarkerStyle(20);
    histData->SetMarkerSize(1.2);
    histData->SetLabelSize(0);

    ggH->Scale(scaleSignal);
    qqH->Scale(scaleSignal);
    ggH->SetLineColor(2);
    qqH->SetLineColor(4);

    InitData(histData); histData->GetXaxis()->SetRangeUser(xmin,xmax);  
    InitHist(QCD,TColor::GetColor("#FFCCFF"));
    InitHist(ZLL,TColor::GetColor("#DE5A6A"));
    InitHist(TT,TColor::GetColor("#9999CC"));
    InitHist(VV,TColor::GetColor("#6F2D35"));
    InitHist(W,TColor::GetColor("#DE5A6A"));
    InitHist(ZLL,TColor::GetColor("#4496C8"));
    InitHist(ZTT,TColor::GetColor("#FFCC66"));

    histData->GetXaxis()->SetTitle(xtitle);
    histData->GetYaxis()->SetTitle(ytitle);
    histData->GetYaxis()->SetTitleSize(0.06);
    histData->SetMarkerSize(1.2);
    histData->GetYaxis()->SetTitleOffset(1.5);

    float yUpper = histData->GetMaximum();
    if (logY)
      histData->GetYaxis()->SetRangeUser(0.5,10*yUpper);
    else
      histData->GetYaxis()->SetRangeUser(0,1.2*yUpper);
    
    float yMin = 0.5;
    float yMax = 10*yUpper;

    //  nData = histData->GetSum();
    //  float nMC   = TT->GetSum();
    //  float eData = TMath::Sqrt(nData);

    float chi2 = 0;
    for (int iB=1; iB<=nBins; ++iB) {
      float xData = histData->GetBinContent(iB);
      float xMC = ZTT->GetBinContent(iB);
      if (xMC>1e-1) {
	float diff2 = (xData-xMC)*(xData-xMC);
	chi2 += diff2/xMC;
      }
     
      if (blindData) {
	if (iB>=nbMin&&iB<=nbMax) {
	  histData->SetBinContent(iB,100000);
	  histData->SetBinError(iB,0);
	}
      }
     

    }
    std::cout << std::endl;
    std::cout << "Chi2 = " << chi2 << std::endl;
    std::cout << std::endl;
  
    TCanvas * canv1 = MakeCanvas("canv1", "", 700, 800);
    TPad *upper = new TPad("upper", "pad",0,0.31,1,1);
    upper->Draw();
    upper->cd();
    upper->SetFillColor(0);
    upper->SetBorderMode(0);
    upper->SetBorderSize(10);
    upper->SetTickx(1);
    upper->SetTicky(1);
    upper->SetLeftMargin(0.20);
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
    if (logY) {
      QCD->Draw("sameh");
      W->Draw("sameh");
    }
    else {
      W->Draw("sameh");
      QCD->Draw("sameh");
    }
    histData->Draw("e1same");
    bkgdErr->Draw("e2same");
    if (showSignal) {
      ggH->Draw("hsame");
      qqH->Draw("hsame");
    }    

    TLegend * leg = new TLegend(0.65,0.45,0.85,0.8);
    if (position==1)
      leg = new TLegend(0.24,0.5,0.4,0.9);
    SetLegendStyle(leg);
    leg->SetTextSize(0.05);
    leg->AddEntry(histData,"Data","lp");
    leg->AddEntry(ZTT,"Z#rightarrow #tau#tau","f");
    leg->AddEntry(ZLL,"Z#rightarrow ll","f");
    leg->AddEntry(TT,"t#bar{t}","f");
    if (logY) {
      leg->AddEntry(QCD,"QCD","f");
      leg->AddEntry(W,"electroweak","f");
    }
    else {
      leg->AddEntry(W,"electroweak","f");
      leg->AddEntry(QCD,"QCD","f");
    }

    if (showSignal) {
      leg->AddEntry(ggH,"ggH(x50)","l");
      leg->AddEntry(qqH,"qqH(x50)","l");
    }
    if (plotLeg)
      leg->Draw();
    writeExtraText = true;
    extraText = "Preliminary";
    CMS_lumi(upper,4,33); 
    plotchannel("e#mu",0.25,0.84);
    
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
    ratioH->SetMarkerSize(1.4);
    ratioH->SetLineColor(1);
    ratioH->GetYaxis()->SetRangeUser(0.41,1.59);
    ratioH->GetYaxis()->SetNdivisions(505);
    ratioH->GetXaxis()->SetLabelFont(42);
    ratioH->GetXaxis()->SetLabelOffset(0.04);
    ratioH->GetXaxis()->SetLabelSize(0.14);
    ratioH->GetXaxis()->SetTitleSize(0.12);
    ratioH->GetXaxis()->SetTitleOffset(1.4);
    ratioH->GetYaxis()->SetTitle("obs/exp");
    ratioH->GetYaxis()->SetLabelFont(42);
    ratioH->GetYaxis()->SetLabelOffset(0.015);
    ratioH->GetYaxis()->SetLabelSize(0.13);
    ratioH->GetYaxis()->SetTitleSize(0.14);
    ratioH->GetYaxis()->SetTitleOffset(0.6);
    ratioH->GetXaxis()->SetTickLength(0.07);
    ratioH->GetYaxis()->SetTickLength(0.02);
    ratioH->GetYaxis()->SetLabelOffset(0.01);
    ratioH->SetMarkerSize(1.2);

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
    lower->SetLeftMargin(0.20);
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
    
    if (logY) {
      canv1->Print("figures/"+Variable+"_"+Suffix+"_logY.png");
      //      canv1->Print("figures/"+Variable+"_"+Suffix+"_logY.pdf","Portrait pdf");
    }
    else {
      canv1->Print("figures/"+Variable+"_"+Suffix+".png");
      //      canv1->Print("figures/"+Variable+"_"+Suffix+".pdf","Portrait pdf");
    }

}
