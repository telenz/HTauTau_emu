#include "HttStylesNew.cc"
#include "CMS_lumi.C"
#include <iomanip>
void Plot(
          TString directory  = "/nfs/dust/cms/user/mameyer/SM_HiggsTauTau/HTauTau_emu/Inputs/NTuples_2016/",
          TString DataFile   = "MuonEG_Run2016",
          TString Variable   = "dzeta",
          TString Suffix     = "inclusive",
          int nBins          =  30,
          float xmin         =  -45,
          float xmax         =  105,
          TString Weight     = "mcweight*puweight*effweight*0.978824*0.985*xsec_lumi_weight*btag0weight*",
          TString Cuts       = "&&iso_1<0.15&&iso_2<0.2&&extraelec_veto<0.5&&extramuon_veto<0.5&&pt_1>13&&pt_2>10&TMath::Max(pt_1,pt_2)>24&&metFilters>0.5&&nbtag==0&&mTdileptonMET<60&&dzeta>-35",
          TString xtitle     = "D_{#zeta} [GeV]",
          TString ytitle     = "Events / 5 GeV"
          ) {

   TH1::SetDefaultSumw2();
   TH2::SetDefaultSumw2();
   SetStyle();
 
   //general settings
   TString version = "v1";
   TString channel = "em";
   bool plotLeg = true;
   int position = 0; // 0 - right, 1 - left, 2 - central
   bool showSignal = false;
   bool logY = false;
   
   //scales for certain processes
   double TTnorm = 1.0;
   double Wnorm  = 1.0;
   double scaleSignal = 50;

   //settings for blinding
   bool blindData = false;
   int nbMin = 12;
   int nbMax = 20;
 
   TString topweight("topptweightRun2*");
   TString qcdweight("2.31*"); //("2.31*");
   TString zptmassweight("zptmassweight*");
   
   //define samples
   const int nSamples = 9;
   int nBG = 6;
   
   TString sampleNames[nSamples] = {
     "MuonEG_Run2016_dnn_" + channel + "_" + version ,   // data (0)
     "DYJets_dnn_"         + channel + "_" + version ,   // ZTT (1) 
     "DYJets_dnn_"         + channel + "_" + version ,   // ZLL (2) 
     "WJets_dnn_"          + channel + "_" + version ,   // WJets (3)
     "TTbar_dnn_"          + channel + "_" + version ,   // TTbar (4)
     "SingleTop_dnn_"      + channel + "_" + version ,   // SingleTop (5)
     "Diboson_dnn_"        + channel + "_" + version ,   // Diboson (6)
     "ggH_dnn_"            + channel + "_" + version ,   // ggH (7)
     "VBFH_dnn_"           + channel + "_" + version ,   // VBFH (8)
  };
 
   //define cuts
   TString cuts[nSamples];
   TString cutsSS[nSamples];
   
   for (int i=0; i<nSamples; ++i) {
      cuts[i] = Weight+"(os>0.5"+Cuts+")";
      cutsSS[i] = Weight+qcdweight+"(os<0.5"+Cuts+")";
   }
   cuts[0] = "(os>0.5"+Cuts+")";
   cuts[1] = Weight+zptmassweight+"(os>0.5"+Cuts+"&&isZTT)";
   cuts[2] = Weight+zptmassweight+"(os>0.5"+Cuts+"&&!isZTT)";
   cuts[4]  = Weight+topweight+"(os>0.5"+Cuts+")";
   
   cutsSS[0] = qcdweight+"(os<0.5"+Cuts+")";
   cutsSS[1] = Weight+zptmassweight+qcdweight+"(os<0.5"+Cuts+"&&isZTT)";
   cutsSS[2] = Weight+zptmassweight+qcdweight+"(os<0.5"+Cuts+"&&!isZTT)";
   cutsSS[4]  = Weight+topweight+qcdweight+"(os<0.5"+Cuts+")";
   
   
   TH1D * hist[nSamples];
   TH1D * histSS[nSamples];

  TCanvas * dummyCanv = new TCanvas("dummy","",500,500);
  
  // filling histograms
  for (int i=0; i<nSamples; ++i) {
    //    std::cout << sampleNames[i] << std::endl;
     TFile * file = new TFile(directory+sampleNames[i]+".root");
     TTree * tree = (TTree*)file->Get("TauCheck");
     TString histName = sampleNames[i] + Variable + "_ss";
     TString histNameSS = sampleNames[i] + Variable + "_os";
    hist[i] = new TH1D(histName,"",nBins,xmin,xmax);
    histSS[i] = new TH1D(histNameSS,"",nBins,xmin,xmax);
    tree->Draw(Variable+">>"+histName,cuts[i]);
    tree->Draw(Variable+">>"+histNameSS,cutsSS[i]);
    std::cout << sampleNames[i] << " " << hist[i]->GetEntries() << " " << hist[i]->Integral(0,nBins+1) << std::endl;
  }
  std::cout<<cuts[4]<<std::endl;
  std::cout<<"Data integral is: "<<hist[4]->Integral()<<std::endl;
  
  float dataSS = histSS[0]->GetSumOfWeights();
  float dataSSfull = histSS[0]->Integral(0,nBins+1);
  
  // subtracting background from SS
  float nonQCD = 0;
  float nonQCDfull = 0;
  for (int i=1; i<=nBG; ++i){
     std::cout<<"Data integral: "<<histSS[0]->Integral()<<std::endl;
     std::cout<<i <<" BG integral: "<<histSS[i]->Integral()<<std::endl;
     histSS[0]->Add(histSS[0],histSS[i],1,-1);
     nonQCD = nonQCD + histSS[i]->GetSumOfWeights();
     nonQCDfull = nonQCDfull + histSS[i]->Integral(0,nBins+1);
  }
  for (int iB=1; iB<=nBins; ++iB)
     std::cout << iB << " : " << histSS[0]->GetBinContent(iB) << std::endl;
      
  float Wfraction     = histSS[3]->GetSumOfWeights()/dataSS;
  float WfractionFull = histSS[3]->Integral(0,nBins+1)/dataSSfull;
  
  float nonQCDfraction = nonQCD/dataSS;
  float nonQCDfractionFull = nonQCDfull/dataSSfull;


    std::cout << "SS region" << std::endl;
    std::cout << "VV (MC)      : " << histSS[6]->GetSumOfWeights() << " : "<< histSS[6]->Integral(0,nBins+1) << std::endl;
    std::cout << "W  (MC)      : " << histSS[3]->GetSumOfWeights() << " : "<< histSS[3]->Integral(0,nBins+1) << std::endl;
    std::cout << "TT (MC)      : " << histSS[4]->GetSumOfWeights() << " : "<< histSS[4]->Integral(0,nBins+1) <<  std::endl;
    std::cout << "DY (MC)      : " << histSS[1]->GetSumOfWeights()+histSS[2]->GetSumOfWeights()  << " : "<< histSS[1]->Integral(0,nBins+1)+histSS[2]->Integral(0,nBins+1) << std::endl;
    std::cout << "non-QCD (MC) : " << nonQCD << " : " << nonQCDfull << std::endl;
    std::cout << "data         : " << dataSS << " : " << dataSSfull << std::endl;
    std::cout << "W+Jets  fraction : " << Wfraction << " : " << WfractionFull << std::endl;
    std::cout << "non-QCD fraction : " << nonQCDfraction << " : " << nonQCDfractionFull << std::endl; 
    std::cout << std::endl;
    
    //  adding up single top and VV backgrounds
    hist[5]->Add(hist[5],hist[6]);
    histSS[5]->Add(histSS[5],histSS[6]);

    TH1D * histData = (TH1D*)hist[0]->Clone("data_obs");
    TH1D * QCD = (TH1D*)histSS[0]->Clone("QCD");
    TH1D * ZTT = (TH1D*)hist[1]->Clone("ZTT");
    TH1D * ZLL = (TH1D*)hist[2]->Clone("ZLL");
    TH1D * W   = (TH1D*)hist[3]->Clone("W");
    TH1D * TT  = (TH1D*)hist[4]->Clone("TT");
    TH1D * VV  = (TH1D*)hist[5]->Clone("VV");
    TH1D * ggH = (TH1D*)hist[7]->Clone("ggH");
    TH1D * qqH = (TH1D*)hist[8]->Clone("qqH");
    
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

    InitData(histData); histData->GetXaxis()->SetRangeUser(xmin,xmax);  
    InitHist(QCD,TColor::GetColor("#FFCCFF"));
    InitHist(ZLL,TColor::GetColor("#DE5A6A"));
    InitHist(TT,TColor::GetColor("#9999CC"));
    InitHist(VV,TColor::GetColor("#DE5A6A"));
    InitHist(W,TColor::GetColor("#DE5A6A"));
    InitHist(ZLL,TColor::GetColor("#DE5A6A"));
    InitHist(ZTT,TColor::GetColor("#FFCC66"));

    VV->Add(VV,W);
    VV->Add(VV,ZLL);

    THStack *stack = new THStack("Background","");
    stack -> Add(QCD);
    stack -> Add(VV);
    stack -> Add(TT);
    stack -> Add(ZTT);
    
       
    std::cout << "BKG : " << ((TH1F*)stack->GetStack()->Last())->GetSumOfWeights() << " : " << ((TH1F*)stack->GetStack()->Last())->Integral(0,nBins+1) << std::endl;
    std::cout << "DAT : " << histData->GetSumOfWeights() << " : " << histData->Integral(0,nBins+1) << std::endl;
    std::cout << "DAT/BKG = " << histData->GetSumOfWeights()/((TH1F*)stack->GetStack()->Last())->GetSumOfWeights() << "+/-" 
              << TMath::Sqrt(histData->GetSumOfWeights())/((TH1F*)stack->GetStack()->Last())->GetSumOfWeights() << std::endl;
    
    
    TH1D * bkgdErr = (TH1D*)stack->GetStack()->Last()->Clone("bkgdErr");
    bkgdErr->SetMarkerSize(0);
    bkgdErr->SetFillStyle(3004);
    bkgdErr->SetFillColor(923);
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
       ggH->SetBinError(iB,0);
       qqH->SetBinError(iB,0);
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
    stack->Draw("same hist");
    
    histData->Draw("e1same");
    bkgdErr->Draw("e2same");
    canv1->Update();
    if (showSignal) {
       ggH->Draw("hsame");
       qqH->Draw("hsame");
    }    
    
    TLegend *leg = new TLegend(0.65,0.45,0.85,0.8);
    if (position==1)
       leg = new TLegend(0.24,0.5,0.4,0.9);
    SetLegendStyle(leg);
    leg->SetTextSize(0.05);
    leg->AddEntry(histData,"Data","lp");
    leg->AddEntry(ZTT,"Z#rightarrow #tau#tau","f");
    //leg->AddEntry(ZLL,"Z#rightarrow ll","f");
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
    writeExtraText = false;
    extraText = "Preliminary";
    CMS_lumi(upper,4,33); 
    plotchannel("e#mu",0.25,0.84);
    
    char KT[100];
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
    ratioH -> Divide((TH1D*)stack->GetStack()->Last()); // Divide by the sum of the THStack
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
       ratioErrH->SetBinContent(iB,1.0);
       ratioErrH->SetBinError(iB,0.0);
       float xBkg = bkgdErr->GetBinContent(iB);
       float errBkg = bkgdErr->GetBinError(iB);
       if (xBkg>0) {
          float relErr = errBkg/xBkg;
          ratioErrH->SetBinError(iB,relErr);
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
