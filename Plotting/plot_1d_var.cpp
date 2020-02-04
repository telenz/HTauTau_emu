#include "../DNN/GoF/bin/HttStylesNew.cc"
#include "../DNN/GoF/bin/useful_classes.h"
#include "../DNN/GoF/bin/CMS_lumi.C"
#include <iomanip>
#include <iostream>
#include "THStack.h"
#include "TROOT.h"
#include "TMath.h"
#include "TColor.h"
#include "TSystem.h"

void plot_1d_var(
		 TString variable    = "m_sv",
		 TString category    = "em_inclusive",
		 bool draw_signal    = false,
		 bool draw_log_scale = false,
		 TString directory   = "/nfs/dust/cms/user/tlenz/13TeV/2017/SM_HTauTau/HTauTau_emu/DNN/GoF/output/var_1d/",
		 TString era         = "2016",
		 bool use_embedded   = false,
       bool log_scale_x    = false
		 ) {

  // General uncertainties
  float err_lumi     = 0.025;
  if(era=="2017")  err_lumi=0.023;
  float err_trigger  = 0.020;
  float err_muon_eff = 0.014;
  float err_elec_eff = 0.014;

  // Sample-specific uncertainties
  float err_vv_xsec  = 0.05;
  float err_st_xsec  = 0.05;
  float err_tt_xsec  = 0.06;
  float err_w_xsec   = 0.04;
  float err_dy_xsec  = 0.04;
  float err_qcd_norm = 0.10;
  float err_emb_norm = 0.04;

  vector<float> err_norm_general = { err_lumi , err_trigger , err_muon_eff , err_elec_eff };

   // Set some options
   bool plotLeg = true;
   double scaleSignal = 1;
   bool blindData = true;

   TH1::SetDefaultSumw2();
   TH2::SetDefaultSumw2();
   SetPixelStyle();
   

   // Read file
   TString filename = directory + "/"+era+"em-synced-"+ variable + ".root";
   TFile *file      = new TFile( filename , "READ");

   // Define sample categorization
   SampleForPlotting data("Data",{"data_obs"});
   SampleForPlotting ztt("ZTT",{"ZTT"});
   SampleForPlotting emb("EMB",{"EMB"});
   SampleForPlotting zl("ZL",{"ZL"});
   SampleForPlotting vv("VV",{"VVL","VVT"});
   SampleForPlotting st("ST",{"ST"});
   SampleForPlotting w("W",{"W"});
   SampleForPlotting tt("TT",{"TTL","TTT"});
   SampleForPlotting qcd("QCD",{"QCD"});
   SampleForPlotting qqh("qqH",{"qqH125"});
   SampleForPlotting ggh("ggH",{"ggH125"});
   SampleForPlotting zh("ZH",{"ZH125"});
   SampleForPlotting wh("WH",{"WH125"});

   if(use_embedded){
     vv.subsamples.clear();
     vv.subsamples.push_back("VVL");
     tt.subsamples.clear();
     tt.subsamples.push_back("TTL");
   }

   ztt.color = "#FFCC66";
   emb.color = "#FFCC66";
   zl.color  = "#DE5A6A";
   w.color   = "#03A8F5";
   vv.color  = "#A8CCA4";
   st.color  = "#BEE6E7";
   tt.color  = "#9999CC";
   qcd.color = "#FFCCFF";
   ggh.color = "#05B0BB"; //blue:"#03A8F5";
   qqh.color = "#BB051E";
   zh.color  = "#EA32E9";
   wh.color  = "#CCFFCC";

   ztt.legend_entry  = "Z#rightarrow #tau#tau (MC)";
   emb.legend_entry  = "Z#rightarrow #tau#tau (emb.)";
   zl.legend_entry   = "Z#rightarrow ll   ";
   w.legend_entry    = "W   ";
   vv.legend_entry   = "VV   ";
   st.legend_entry   = "ST   ";
   tt.legend_entry   = "t#bar{t}   ";
   qcd.legend_entry  = "QCD   ";
   data.legend_entry = "Data   ";
   qqh.legend_entry  = "qqH   ";
   ggh.legend_entry  = "ggH   ";
   zh.legend_entry   = "ZH   ";
   wh.legend_entry   = "WH   ";
 
   data.isData  = true;
   qqh.isSignal = true;
   ggh.isSignal = true;
   zh.isSignal  = true;
   wh.isSignal  = true;

   vector<SampleForPlotting*> sample_vec;
   if(draw_signal){
     if(!use_embedded) sample_vec = { &wh , &zh , &qqh , &ggh , &qcd , &vv , &st , &zl ,  &w  , &tt , &ztt , &data  };
     else              sample_vec = { &wh , &zh , &qqh , &ggh , &qcd , &vv , &st , &zl ,  &w  , &tt , &emb , &data  };
   }
   else{
     if(!use_embedded) sample_vec = { &qcd , &vv , &st , &zl , &w , &tt , &ztt , &data  };
     else              sample_vec = { &qcd , &vv , &st , &zl , &w , &tt , &emb , &data  };
   }

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
     if(smpl->isSignal){
       smpl->hist->SetLineColor(TColor::GetColor(smpl->color));
       continue;
     }
     InitHist(smpl->hist,TColor::GetColor(smpl->color));
     smpl->hist -> SetLineColor(TColor::GetColor(smpl->color));
   }

   // Fill all background to a THStack
   THStack *stack = new THStack("Background","");   
   for(auto  *smpl : sample_vec){
     if( smpl->isData   == true ) continue;
     if( smpl->isSignal == true ) continue;
     stack -> Add(smpl->hist);
   }
   
   // Give warning if there are empty bins
   TH1 *last = (TH1*)stack->GetStack()->Last();
   // for (int iB=1; iB<=last->GetNbinsX(); ++iB) {
   //   if(last->GetBinContent(iB) <=0) cout<<endl<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! WARNNING : empty bins !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl<<endl;
   // }

   // Initialize a background error histogram
   TH1D * bkgdErr = (TH1D*) stack -> GetStack() -> Last() -> Clone("bkgdErr");
   bkgdErr -> SetMarkerSize(0);
   bkgdErr -> SetFillStyle(3004);
   bkgdErr -> SetFillColor(923);
   bkgdErr -> SetMarkerSize(0);

   for (int iB = 1 ; iB <= data.hist->GetNbinsX() ; ++iB) {

     for(auto  *smpl : sample_vec){
       if( smpl->isData   == true ) continue;
       if( smpl->isSignal == true ) continue;
       smpl->hist->SetBinError(iB,0);
     }
     float err_stat = bkgdErr -> GetBinError(iB);
     float x        = bkgdErr -> GetBinContent(iB);
     vector<float> err_norm_abs;
     for(float err : err_norm_general){
       err_norm_abs.push_back( err * x );
     }
     // Add sample-specific uncertainties
     err_norm_abs.push_back( vv.hist->GetBinContent(iB)*err_vv_xsec );
     err_norm_abs.push_back( st.hist->GetBinContent(iB)*err_st_xsec );
     err_norm_abs.push_back( tt.hist->GetBinContent(iB)*err_tt_xsec );
     err_norm_abs.push_back( w.hist->GetBinContent(iB)*err_w_xsec );
     if(use_embedded) err_norm_abs.push_back( emb.hist->GetBinContent(iB)*err_emb_norm );
     else{
       err_norm_abs.push_back( ztt.hist->GetBinContent(iB)*err_dy_xsec );
       err_norm_abs.push_back( zl.hist->GetBinContent(iB)*err_dy_xsec );
     }
     err_norm_abs.push_back( qcd.hist->GetBinContent(iB)*err_qcd_norm );
     float err_total = err_stat*err_stat;
     for(float err_bin : err_norm_abs) err_total += err_bin*err_bin;
     err_total = TMath::Sqrt(err_total);
     bkgdErr -> SetBinError(iB,err_total);
   }

   // Make a canvas
   TCanvas * canv1 = 0;
   if( category.Contains("unrolled") ) canv1 = MakeCanvas("canv1", "", 1400, 800);
   else                                canv1 = MakeCanvas("canv1", "", 700, 800);
   TPad *upper = new TPad("upper", "pad",0,0.31,1,1);
   upper->Draw();
   upper->cd();

   float y_upper = data.hist->GetMaximum();
   float y_lower = qcd.hist->GetMaximum()*0.01;
   data.hist -> GetXaxis() -> SetTitle(variable);
   if( category.Contains("unrolled") ){
     data.hist->GetXaxis()->SetTitle("");
     data.hist->GetXaxis()->SetLabelSize(0);
   }
   data.hist -> GetYaxis() -> SetTitle("Events");
   if(draw_log_scale){
     if(draw_signal) y_lower = 0.1*qqh.hist->GetMaximum();
     if(y_lower == 0) y_lower = 1;
     data.hist -> GetYaxis() -> SetRangeUser(y_lower , 1000*y_upper);
     upper -> SetLogy();
   }
   else  data.hist -> GetYaxis() -> SetRangeUser(0,1.75*y_upper);

   if (log_scale_x) {
      upper->SetLogx();
   }

   // Calculate signal strength (s+b/b) - needed for ratio plot and blinding
   TH1D * signal_strength    = (TH1D*) stack->GetStack()->Last()->Clone("ratioH");
   for(auto  *smpl : sample_vec){
      if(smpl->isSignal) {
         signal_strength -> Add(smpl->hist);
         smpl->hist->Scale(scaleSignal);
      }
   }
   signal_strength -> Divide((TH1D*)stack->GetStack()->Last());
   signal_strength -> SetLineColor(2);
   signal_strength -> SetLineWidth(3);
   signal_strength -> SetFillStyle(0);

   // BLINDING
   // for(int i = 1 ; i <= data.hist->GetNbinsX() ; i++){
   //   if(signal_strength -> GetBinContent(i) > 1.02 ){
   //     data.hist->SetBinContent(i,0);
   //     data.hist->SetBinError(i,0);
   //   }
   // }
   if(blindData){
     if(category.Contains("ggh") || category.Contains("qqh")){
       for(int i = 1 ; i <= data.hist->GetNbinsX() ; i++){
	 data.hist->SetBinContent(i,0);
	 data.hist->SetBinError(i,0);
       }
     }
   }

   data.hist -> Draw("e1");
   stack     -> Draw("same hist");
   data.hist -> Draw("e1 same");
   bkgdErr   -> Draw("e2same");
   if(draw_signal){
     ggh.hist  -> Draw("hist same");
     qqh.hist  -> Draw("hist same");
     zh.hist   -> Draw("hist same");
     wh.hist   -> Draw("hist same");
   }
   canv1     -> Update();

   // Draw legend + other things
   TLegend *leg = 0;
   leg = new TLegend(0.20,0.67,0.80,0.89);
   SetLegendStyle(leg);
   leg->SetNColumns(3);
   if( category.Contains("unrolled") ){
     leg = new TLegend(0.2,0.7,0.8,0.9);
     SetLegendStyle(leg);
     leg->SetNColumns(5);
   }
   vector<TString> already_added_to_legend;
   for (unsigned int i = sample_vec.size(); i-- > 0; ){
     if( find(already_added_to_legend.begin() , already_added_to_legend.end()  , sample_vec[i]->name) != already_added_to_legend.end() ) continue;

     if(sample_vec[i]->isData)        leg->AddEntry( sample_vec[i]->hist , sample_vec[i]->legend_entry , "lp");
     else if(sample_vec[i]->isSignal) leg->AddEntry( sample_vec[i]->hist , sample_vec[i]->legend_entry , "l");
     else                             leg->AddEntry( sample_vec[i]->hist , sample_vec[i]->legend_entry , "f");
     already_added_to_legend.push_back( sample_vec[i]->name );
   }
   leg->Draw();
   writeExtraText = false;
   extraText = "Preliminary";
   if(era=="2016") CMS_lumi(upper,4,33);
   else if(era=="2017") CMS_lumi(upper,5,33);
   TString category_name = category;

   if(category == "em_inclusive" ) category_name = "incl.";
   else if(category == "em_ztt")   category_name = "Z#rightarrow #tau#tau";
   else if(category == "em_tt")    category_name = "t#bar{t}";
   else if(category == "em_st")    category_name = "single top";
   else if(category == "em_misc")  category_name = "misc";
   else if(category == "em_vv")    category_name = "diboson";
   else if(category == "em_ss")    category_name = "same sign";
   else if(category == "em_qqh")   category_name = "qqH";
   else if(category == "em_ggh")   category_name = "ggH";
   plotchannel("e#mu " + category_name + " category",0.15,0.84);
    
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

   for (int iB=1; iB<=data.hist->GetNbinsX(); ++iB) {
     ratioErrH -> SetBinContent(iB,1.0);
     ratioErrH -> SetBinError(iB,0.0);
     float x_bkg   = bkgdErr -> GetBinContent(iB);
     float err_bkg = bkgdErr -> GetBinError(iB);
     if (x_bkg>0) {
       float rel_err = err_bkg/x_bkg;
       ratioErrH -> SetBinError(iB,rel_err);
     }
   }


   // ------------>Primitives in pad: lower
   TPad * lower = new TPad("lower", "pad",0,0,1,0.30);
   lower->Draw();
   lower->cd();
   lower->SetGridy();
   ratioH->Draw("e1");
   ratioErrH->Draw("e2same");
   if(draw_signal) signal_strength->Draw("hist same");

   if (log_scale_x) {
      lower->SetLogx();
   }

   lower->Modified();
   lower->RedrawAxis();
   canv1->cd();
   canv1->Modified();
   canv1->cd();
   canv1->SetSelected(canv1);

   TString out_filename = variable + "_" + category;
   if(draw_log_scale) out_filename += "_log";
   if (log_scale_x) out_filename += "_log_x";

   TString output_dir = directory + "/figures/";
   gSystem -> Exec("mkdir " + output_dir);

   canv1->Print( output_dir + out_filename + ".png");
   canv1->Print( output_dir + out_filename + ".pdf");
}
