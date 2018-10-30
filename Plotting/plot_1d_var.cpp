#include "../DNN/GoF/HttStylesNew.cc"
#include "../DNN/GoF/useful_classes.h"
#include "../DNN/GoF/CMS_lumi.h"
#include <iomanip>
#include <iostream>
#include "THStack.h"
#include "TROOT.h"
void plot_1d_var(
		 TString variable    = "m_sv",
		 TString category    = "em_inclusive",
		 TString directory   = "/nfs/dust/cms/user/tlenz/13TeV/2017/SM_HTauTau/HTauTau_emu/DNN/GoF/output/var_1d/"
          ) {

   // Set some options
   bool plotLeg = true;
   bool logy = false;
   double scaleSignal = 50;
   bool blindData = false;

   TH1::SetDefaultSumw2();
   TH2::SetDefaultSumw2();
   SetPixelStyle();
   

   // Read file
   TString filename = directory + "/htt_em.inputs-sm-13TeV-" + variable + ".root";
   TFile *file      = new TFile( filename , "READ");

   // Define sample categorization
   SampleForPlotting data("Data",{"data_obs"});
   SampleForPlotting ztt("ZTT",{"ZTT"});
   SampleForPlotting zl("ZL",{"ZL","EWKZ","VV","W","ST"});
   SampleForPlotting tt("TT",{"TT"});
   SampleForPlotting qcd("QCD",{"QCD"});

   ztt.color = "#FFCC66";
   zl.color  = "#DE5A6A";
   tt.color  = "#9999CC";
   qcd.color = "#FFCCFF";

   ztt.legend_entry  = "Z#rightarrow #tau#tau";
   zl.legend_entry   = "electroweak";
   tt.legend_entry   = "t#bar{t}";
   qcd.legend_entry  = "QCD";
   data.legend_entry = "Data";
 
   data.isData = true;

   vector<SampleForPlotting*> sample_vec = { &qcd , &zl , &tt , &ztt , &data };


   TH1D* hist = 0;

   for(SampleForPlotting *smpl : sample_vec){
     for(TString & subsmpl : smpl->subsamples){

       TString histogram_path = category + "/" + subsmpl;
       hist = (TH1D*)file -> Get(histogram_path);
       if(hist == nullptr){
	 cout << "Histogram " << histogram_path << " doesn't exists in the root file " << filename << ". Exit Code." << endl;
	 exit(-1);
       }
       if(smpl->hist == nullptr) smpl->hist = (TH1D*) hist->Clone();
       else                      smpl->hist -> Add(hist);
     }
     if(smpl->isData) continue;
     InitHist(smpl->hist,TColor::GetColor(smpl->color));
   }
   
   // ModTDRStyle();

   // Fill all background to a THStack
   THStack *stack = new THStack("Background","");   
   for(auto  *smpl : sample_vec){
     if( smpl->isData   == true ) continue;
     if( smpl->isSignal == true ) continue;
     stack -> Add(smpl->hist);
   }

   // Initialize a background error histogram
   TH1D * bkgdErr = (TH1D*)stack->GetStack()->Last()->Clone("bkgdErr");
   bkgdErr -> SetMarkerSize(0);
   bkgdErr -> SetFillStyle(3004);
   bkgdErr -> SetFillColor(923);
   bkgdErr -> SetMarkerSize(0);
   
   // Make a canvas
   TCanvas * canv1 = MakeCanvas("canv1", "", 700, 800);

   // ModTDRStyle(); 

   TPad *upper = new TPad("upper", "pad",0,0.31,1,1);
   upper->Draw();
   upper->cd();

   float yUpper = data.hist->GetMaximum();
   data.hist -> GetXaxis() -> SetTitle(variable);
   data.hist -> GetYaxis() -> SetTitle("Events");
   if(logy) upper -> SetLogy();
   else     data.hist -> GetYaxis() -> SetRangeUser(0,1.2*yUpper);


   data.hist -> Draw("e1");
   stack     -> Draw("same hist");
   data.hist -> Draw("e1 same");
   // bkgdErr->Draw("e2same");
   canv1->Update();

   // Draw legend
   TLegend *leg = new TLegend(0.65,0.45,0.9,0.9);
   SetLegendStyle(leg);
   for(auto *smpl : sample_vec){
     if(smpl->isData)        leg->AddEntry( smpl->hist , smpl->legend_entry , "lp");
     else if(smpl->isSignal) leg->AddEntry( smpl->hist , smpl->legend_entry , "l");
     else                    leg->AddEntry( smpl->hist , smpl->legend_entry , "f");
   }
   leg->Draw();
   writeExtraText = false;
   extraText = "Preliminary";
   // CMS_lumi(upper,4,33); 
   plotchannel("e#mu",0.25,0.84);
    
   char KT[100];
   TLatex * cms = new TLatex(0.25,0.85,KT);
   cms->SetNDC();
   cms->SetTextSize(0.05);
   // cms->Draw();
    
   upper->Draw("SAME");
   upper->RedrawAxis();
   upper->Modified();
   upper->Update();
   canv1->cd();
    
   TH1D * ratioH    = (TH1D*) data.hist->Clone("ratioH");
   TH1D * ratioErrH = (TH1D*) bkgdErr->Clone("ratioErrH");
   ratioH -> Divide((TH1D*)stack->GetStack()->Last()); // Divide by the sum of the THStack
   ratioH->GetYaxis()->SetRangeUser(0.4,1.6);
   ratioH->GetYaxis()->SetNdivisions(505);
   ratioH->GetYaxis()->SetTitle("obs/exp");
   ratioH->GetXaxis()->SetTitle("");

   // ------------>Primitives in pad: lower
   TPad * lower = new TPad("lower", "pad",0,0,1,0.30);
   lower->Draw();
   lower->cd();
   lower->SetGridy();
   ratioH->Draw("e1");
   ratioErrH->Draw("e2same");

   lower->Modified();
   lower->RedrawAxis();
   canv1->cd();
   canv1->Modified();
   canv1->cd();
   canv1->SetSelected(canv1);
    
   canv1->Print("figures/"+variable+"_"+category+".png");
}
