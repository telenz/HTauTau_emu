#include "../DNN/GoF/HttStylesNew.cc"
#include "../DNN/GoF/useful_classes.h"
#include "../DNN/GoF/CMS_lumi.C"
#include <iomanip>
#include <iostream>
#include "THStack.h"
#include "TROOT.h"
#include "TMath.h"
#include "TColor.h"

void make_postfit_plots(bool draw_signal    = false,
			bool draw_log_scale = false,
			TString directory   = ".",
			TString era         = "2016",
			bool use_embedded   = false
		 ) {

   // Set some options
   bool plotLeg       = true;
   bool blindData     = false;

   TH1::SetDefaultSumw2();
   TH2::SetDefaultSumw2();
   SetPixelStyle();

   // Read file
   TString filename = directory + "/" + era + "_datacard_shapes_postfit_sb.root";
   TFile *file      = new TFile( filename , "READ");

   
   // Define map of categories
   map<int,TString> category_map = {{ 1, "em_ggh"},
				    { 2, "em_qqh"},
				    {12, "em_ztt"},
				    {13, "em_tt"},
				    {14, "em_ss"},
				    {16, "em_misc"},
				    {18, "em_st"},
				    {19, "em_vv"},
   };

   // Define sample categorization
   SampleForPlotting data("Data",{"data_obs"});
   SampleForPlotting ztt("ZTT",{"ZTT"});
   SampleForPlotting emb("EMB",{"EMB"});
   SampleForPlotting zl("ZL",{"ZL"});
   SampleForPlotting vv("VV",{"VV"});
   SampleForPlotting st("ST",{"ST"});
   SampleForPlotting w("W",{"W"});
   SampleForPlotting tt("TT",{"TT"});
   SampleForPlotting qcd("QCD",{"QCD"});
   SampleForPlotting qqh("qqH",{"qqH125"});
   SampleForPlotting ggh("ggH",{"ggH125"});

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

   ztt.legend_entry  = "Z#rightarrow #tau#tau (MC)";
   emb.legend_entry  = "Z#rightarrow #tau#tau (emb.)";
   zl.legend_entry   = "Z#rightarrow ll";
   w.legend_entry    = "W";
   vv.legend_entry   = "VV";
   st.legend_entry   = "ST";
   tt.legend_entry   = "t#bar{t}";
   qcd.legend_entry  = "QCD";
   data.legend_entry = "Data";
   qqh.legend_entry  = "qqH";
   ggh.legend_entry  = "ggH";
 
   data.isData  = true;
   qqh.isSignal = true;
   ggh.isSignal = true;

   vector<SampleForPlotting*> sample_vec;
   if(draw_signal){
     if(!use_embedded) sample_vec = { &qqh , &ggh , &qcd , &vv , &st , &zl ,  &w  , &tt , &ztt , &data  };
     else              sample_vec = { &qqh , &ggh , &qcd , &vv , &st , &zl ,  &w  , &tt , &emb , &data  };
   }
   else{
     if(!use_embedded) sample_vec = { &qcd , &vv , &st , &zl , &w , &tt , &ztt , &data  };
     else              sample_vec = { &qcd , &vv , &st , &zl , &w , &tt , &emb , &data  };
   }

   TH1D* hist = 0;
   TCanvas * canv1 = MakeCanvas("canv1", "", 700, 800);

   // Loop over all categories
   for(auto cat : category_map){
     cout<<endl<<"Category : "<<cat.second<<" : "<<cat.first<<endl;
     for(SampleForPlotting *smpl : sample_vec){
       for(TString & subsmpl : smpl->subsamples){

	 TString histogram_path = Form ("htt_em_%d_13TeV_postfit/",cat.first) + subsmpl;
	 hist = (TH1D*)file -> Get(histogram_path);
	 if(hist == nullptr){
	   cout << "Histogram " << histogram_path << " doesn't exists in the root file " << filename << ". Exit Code." << endl;
	   exit(-1);
	 }
	 if(smpl->hist == nullptr) smpl->hist = (TH1D*) hist->Clone();
	 else                      smpl->hist -> Add(hist);
       }
       if(smpl->isData == 1) continue;
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
     for (int iB=1; iB<=last->GetNbinsX(); ++iB) {
       if(last->GetBinContent(iB) <=0) cout<<endl<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! WARNNING : empty bins !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl<<endl;
     }

     // Initialize a background error histogram
     TH1D * bkgdErr;
     TString histogram_path = Form ("htt_em_%d_13TeV_postfit/TotalBkg",cat.first);
     bkgdErr = (TH1D*)file -> Get(histogram_path);
     if(bkgdErr == nullptr){
       cout << "Histogram " << histogram_path << " doesn't exists in the root file " << filename << ". Exit Code." << endl;
       exit(-1);
     }
     bkgdErr -> SetMarkerSize(0);
     bkgdErr -> SetFillStyle(3004);
     bkgdErr -> SetFillColor(923);
     bkgdErr -> SetMarkerSize(0);

     // Make a canvas
     canv1->cd();
     TPad *upper = new TPad("upper", "pad",0,0.31,1,1);
     upper->Draw();
     upper->cd();

     float y_upper = data.hist->GetMaximum();
     float y_lower = qcd.hist->GetMaximum()*0.01;
     data.hist -> GetXaxis() -> SetTitle("ML score");
     data.hist -> GetYaxis() -> SetTitle("Events");
     if(draw_log_scale){
       if(draw_signal) y_lower = 0.1*qqh.hist->GetMaximum();
       if(y_lower == 0) y_lower = 1;
       data.hist -> GetYaxis() -> SetRangeUser(y_lower , 100*y_upper);
       upper -> SetLogy();
     }
     else  data.hist -> GetYaxis() -> SetRangeUser(0,1.2*y_upper);

     // Calculate signal strength (s+b/b) - needed for ratio plot and blinding
     TH1D * signal_strength    = (TH1D*) stack->GetStack()->Last()->Clone("ratioH");
     for(auto  *smpl : sample_vec){
       if(smpl->isSignal) signal_strength -> Add(smpl->hist);
     }
     signal_strength -> Divide((TH1D*)stack->GetStack()->Last());
     signal_strength -> SetLineColor(2);
     signal_strength -> SetLineWidth(3);
     signal_strength -> SetFillStyle(0);

     //BLINDING
     for(int i = 1 ; i <= data.hist->GetNbinsX() ; i++){
       if(signal_strength -> GetBinContent(i) > 1.02 ){
	 data.hist->SetBinContent(i,0);
	 data.hist->SetBinError(i,0);
       }
     }
     if(cat.second.Contains("ggh") || cat.second.Contains("qqh")){
       for(int i = 1 ; i <= data.hist->GetNbinsX() ; i++){
	 data.hist->SetBinContent(i,0);
	 data.hist->SetBinError(i,0);
       }
     }

     data.hist -> Draw("e1");
     stack     -> Draw("same hist");
     data.hist -> Draw("e1 same");
     bkgdErr   -> Draw("e2same");
     if(draw_signal){
       ggh.hist  -> Draw("hist same");
       qqh.hist  -> Draw("hist same");
     }
     canv1     -> Update();

     // Draw legend + other things
     TLegend *leg = 0;
     if( data.hist->GetMaximumBin() <= data.hist->GetNbinsX()/2.) leg = new TLegend(0.62,0.45,0.9,0.9);
     else leg = new TLegend(0.2,0.45,0.45,0.9);
     SetLegendStyle(leg);
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
     TString category_name = cat.second;
     ztt.legend_entry  = "Z#rightarrow #tau#tau (MC)";
     emb.legend_entry  = "Z#rightarrow #tau#tau (emb.)";
     zl.legend_entry   = "Z#rightarrow ll";
     w.legend_entry    = "W";
     vv.legend_entry   = "VV";
     st.legend_entry   = "ST";
     tt.legend_entry   = "t#bar{t}";
     qcd.legend_entry  = "QCD";
     data.legend_entry = "Data";
     qqh.legend_entry  = "qqH";
     ggh.legend_entry  = "ggH";

     if(cat.second == "em_inclusive" ) category_name = "incl.";
     else if(cat.second == "em_ztt")   category_name = "Z#rightarrow #tau#tau";
     else if(cat.second == "em_tt")    category_name = "t#bar{t}";
     else if(cat.second == "em_st")    category_name = "single top";
     else if(cat.second == "em_misc")  category_name = "misc";
     else if(cat.second == "em_vv")    category_name = "diboson";
     else if(cat.second == "em_ss")    category_name = "same sign";
     else if(cat.second == "em_qqh")   category_name = "qqH";
     else if(cat.second == "em_ggh")   category_name = "ggH";
     plotchannel("e#mu " + category_name + " category",0.25,0.84);
    
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

     lower->Modified();
     lower->RedrawAxis();
     canv1->cd();
     canv1->Modified();
     canv1->cd();
     canv1->SetSelected(canv1);

     TString out_filename = "MLscore_postfit_" + cat.second;
     if(draw_log_scale) out_filename += "_log";
     canv1->Print(directory + "/figures/" + out_filename + ".png");
   }
}
