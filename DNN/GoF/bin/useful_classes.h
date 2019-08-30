#ifndef USEFUL_CLASSES_H
#define USEFUL_CLASSES_H

#include "TH2D.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include <iostream>
#include <vector>
#include <map>

// ********************************************************************************************
class Sample {

public :  

  TString name;
  TString filename;

  TString cutString    = "";
  TString weightString = "1*";
  TString cutStringSS    = "";
  TString weightStringSS = "1*";

  TString topweight     = "1*";
  TString zptmassweight = "1*";
  TString ggscaleweight = "1*";
  TString qcdweight     = "1*";
  TString norm          = "1*";

  TString variable = "";

  TH1D *hist_1d;
  TH1D *histSS_1d;

  TH2D *hist_2d;
  TH2D *histSS_2d;

  TString htxs_stage1_flag = "-1";

  bool is_signal = false;
  bool subtract_from_ss = true;

  map< TString , Sample > uncertainties;

  /* Sample(TString sampleName = "", TString filepath){ */
  /*   name     = sampleName; */
  /*   filename = filepath; */
  /* } */

  Sample(TString sampleName = ""){
    name     = sampleName;
  }

};
// ********************************************************************************************

// ********************************************************************************************
class SampleForPlotting {

public :

  TString name = "";
  vector<TString> subsamples = {};
  TString hist_name = "";

  bool isData = false;
  bool isSignal = false;
  TH1D *hist = 0;

  TString color = "#FFCCFF";

  TString legend_entry = "";

  SampleForPlotting(TString sample_name = "", vector<TString> sample_list={}){
    name     = sample_name;
    subsamples = sample_list;
  }
};
// ********************************************************************************************

// ********************************************************************************************
class Category {

public :   
  TString name = "";
  TString suffix = "";
  unsigned int class_nr = 0;
  int htxs_reco_flag    = -1;

  TString variable    = "";
  bool plot_2d = false;
  TString cutstring   = "";
  TString cutstring_ss = "";
  TString weight = "";
  TString qcdweight = "1*";
  TString gg_scale_weight_up   = "";
  TString gg_scale_weight_down = "";

  int nbins_1d   = 20;
  int nbins_2d_x = 20;
  int nbins_2d_y = 20;
  vector<float> bins_1d;
  vector<float> binning_1d;
  vector<float> binning_2d_x;
  vector<float> binning_2d_y;
  vector<float> bins_x_2d;
  vector<float> bins_y_2d;

  map<TString,Sample> sample_list;

  Category(TString catName = ""){
    name = catName;
  }
};
// ********************************************************************************************

vector<float> calc_binning_1d(bool take_percentile_subrange, bool apply_equidistant_binning, Category cat , TString directory){

  TFile *file = new TFile( directory + "/" + cat.sample_list.at("0_Data").filename );
  TTree *tree = (TTree*) file->Get("TauCheck");

  float range_low  = 0;
  float range_high = 0;
  vector<double> percentile_ranges;
  for(int i=0; i<=10; i++) percentile_ranges.push_back(0.01+i*0.098);

  float min_val = tree->GetMinimum(cat.variable);
  float max_val = tree->GetMaximum(cat.variable);
  if(min_val == -10) min_val=0;
  cout<<endl<<"Minimum value in tree = "<<min_val<<endl;
  cout<<"Maximum value in tree = "<<max_val<<endl<<endl;
  
  TH1D* hist_aux = new TH1D("hist_aux", "", 1000000, min_val, max_val);
  tree -> Draw( cat.variable + ">> hist_aux" , "1*("+cat.variable+Form(">%f",min_val)+ cat.cutstring + ")" );

  // 0.) Get result if simply binning from config should be used
  if(!take_percentile_subrange && !apply_equidistant_binning){
    return cat.binning_1d;
  }
  if(!take_percentile_subrange){
    cout<<"Which binning should be applied ? Please check your settings in the config. Either take_percentile_subrange AND apply_equidistant_binning is set to false OR take_percentile_subrange is set to true!"<<endl;
  }

  // 1.) Find 0.01 and 0.99 percentiles to get the range of the histogram
  // Check if variable is of type float
  int var;
  int valid_type = tree->SetBranchAddress(cat.variable,&var);
  if(valid_type==0){
    cout<<"Variable is not appropriate for percentile range. binning_1d as specified in config is used !"<<endl;
    return cat.binning_1d;
  }

  unsigned int idx_bins=0;
  int count =0;
  float integral = hist_aux->GetSumOfWeights();
  bool range_low_not_set = true;
  for(int ibin=1; ibin<hist_aux->GetNbinsX()+1; ibin++){
    count += hist_aux->GetBinContent(ibin);
    if(count>=0.01*integral && range_low_not_set){
      range_low  = hist_aux->GetBinCenter(ibin);
      range_low_not_set = false;
    }
    else if(count>=0.99*integral){
      range_high = hist_aux->GetBinCenter(ibin);
      break;
    }
  }
  cout<<"histo range starts at = "<<range_low<<endl;
  cout<<"histo range ends at   = "<<range_high<<endl;

  // 2.) Get result for equidistant binning
  vector<float> binning;
  if(apply_equidistant_binning){
    for(int i=0; i<=cat.nbins_1d; i++) binning.push_back(range_low + i*(range_high-range_low)/cat.nbins_1d);
    return binning;
  }

  // 3.) Get results for flat binning
  idx_bins=0;
  count =0;
  binning.clear();
  for(int ibin=1; ibin<hist_aux->GetNbinsX()+1; ibin++){
    count += hist_aux->GetBinContent(ibin);
    if(count>=percentile_ranges[idx_bins]*integral){
      binning.push_back(hist_aux->GetBinCenter(ibin));
      cout<<"bin range starts at = "<<hist_aux->GetBinCenter(ibin)<<endl;
      if(idx_bins+1 == percentile_ranges.size()) break;
      idx_bins +=1;
    }
  }

  return binning;
}

// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
std::pair<vector<float>,vector<float>> calc_binning_2d(bool take_percentile_subrange, bool apply_equidistant_binning, Category cat , TString directory){


  TFile *file = new TFile( directory + "/" + cat.sample_list.at("0_Data").filename );
  TTree *tree = (TTree*) file->Get("TauCheck");

  vector<float> binning_2d_x;
  vector<float> binning_2d_y;

  float range_x_low  = 0;
  float range_x_high = 0;
  float range_y_low  = 0;
  float range_y_high = 0;
  vector<double> percentile_ranges;
  for(int i=0; i<=10; i++) percentile_ranges.push_back(0.01+i*0.098);

  TString var_y = cat.variable( 0 , cat.variable.First(":") );
  TString var_x = cat.variable( cat.variable.First(":")+1 , cat.variable.Length());
  float min_val_x = tree->GetMinimum(var_x);
  float max_val_x = tree->GetMaximum(var_x);
  float min_val_y = tree->GetMinimum(var_y);
  float max_val_y = tree->GetMaximum(var_y);
  if(min_val_x == -10) min_val_x=0;
  if(min_val_y == -10) min_val_y=0;
  cout<<endl<<"Minimum value of "<<var_x<<" in tree = "<<min_val_x<<endl;
  cout<<"Maximum value of "<<var_x<<" in tree = "<<max_val_x<<endl;
  cout<<"Minimum value of "<<var_y<<" in tree = "<<min_val_y<<endl;
  cout<<"Maximum value of "<<var_y<<" in tree = "<<max_val_y<<endl<<endl;

  TH1D* hist_aux_x = new TH1D("hist_aux_x", "", 1000000, min_val_x, max_val_x);
  TH1D* hist_aux_y = new TH1D("hist_aux_y", "", 1000000, min_val_y, max_val_y);
  tree -> Draw( var_x + ">> hist_aux_x" , "1*("+var_x+Form(">%f",min_val_x)+ cat.cutstring + ")" );
  tree -> Draw( var_y + ">> hist_aux_y" , "1*("+var_y+Form(">%f",min_val_y)+ cat.cutstring + ")" );

  // 0.) Get result if simply binning from config should be used
  if(!take_percentile_subrange && !apply_equidistant_binning){
    return std::make_pair(cat.binning_2d_x, cat.binning_2d_y);
  }

  if(!take_percentile_subrange){
    cout<<"Which binning should be applied ? Please check your settings in the config. Either take_percentile_subrange AND apply_equidistant_binning is set to false OR take_percentile_subrange is set to true!"<<endl;
  }

  // 1.) Find 0.01 and 0.99 percentiles to get the range of the histogram
  // Check if variable is of type float
  int var1, var2;
  int valid_type_x = tree->SetBranchAddress(var_x,&var1);
  int valid_type_y = tree->SetBranchAddress(var_y,&var2);
  if(valid_type_x==0 && valid_type_y==0){
    cout<<"Variable x and y are not appropriate for percentile range. binning_2_x _yd as specified in config is used !"<<endl;
    return std::make_pair(cat.binning_2d_x, cat.binning_2d_y);
  }
  int count_events = 0;
  float integral_x = hist_aux_x->GetSumOfWeights();
  bool range_low_not_set = true;
  for(int ibin=1; ibin<hist_aux_x->GetNbinsX()+1; ibin++){
    count_events += hist_aux_x->GetBinContent(ibin);
    if(count_events>=0.01*integral_x&&range_low_not_set){
      range_x_low  = hist_aux_x->GetBinCenter(ibin);
      range_low_not_set = false;
    }
    if(count_events>=0.99*integral_x){
      range_x_high = hist_aux_x->GetBinCenter(ibin);
      break;
    }
  }
  count_events =0;
  float integral_y = hist_aux_y->GetSumOfWeights();
  range_low_not_set = true;
  for(int ibin=1; ibin<hist_aux_y->GetNbinsX()+1; ibin++){
    count_events += hist_aux_y->GetBinContent(ibin);
    if(count_events>=0.01*integral_y &&range_low_not_set){
      range_y_low  = hist_aux_y->GetBinCenter(ibin);
      range_low_not_set = false;
    }
    if(count_events>=0.99*integral_y){
      range_y_high = hist_aux_y->GetBinCenter(ibin);
      break;
    }
  }
  cout<<"histo x range starts at = "<<range_x_low<<endl;
  cout<<"histo x range ends at   = "<<range_x_high<<endl;
  cout<<"histo y range starts at = "<<range_y_low<<endl;
  cout<<"histo y range ends at   = "<<range_y_high<<endl;

  // 2.) Get result for equidistant binning
  vector<float> binning;
  if(apply_equidistant_binning){
    for(int i=0; i<=cat.nbins_2d_x; i++) binning_2d_x.push_back(range_x_low + i*(range_x_high-range_x_low)/cat.nbins_2d_x);
    for(int i=0; i<=cat.nbins_2d_y; i++) binning_2d_y.push_back(range_y_low + i*(range_y_high-range_y_low)/cat.nbins_2d_y);
    if( valid_type_x==0 ){
      cout<<"Variable x is not appropriate for percentile range. binning_2d_x as specified in config is used !"<<endl;
      return std::make_pair(cat.binning_2d_x, binning_2d_y);
    }
    if( valid_type_y==0 ){
      cout<<"Variable x is not appropriate for percentile range. binning_2d_y as specified in config is used !"<<endl;
      return std::make_pair(binning_2d_x, cat.binning_2d_y);
    }
    return std::make_pair(binning_2d_x, binning_2d_y);
  }

  // 3.) Get results for flat binning
  unsigned int idx_bins=0;
  count_events =0;
  for(int ibin=1; ibin<hist_aux_x->GetNbinsX()+1; ibin++){
    count_events += hist_aux_x->GetBinContent(ibin);
    if(count_events >= percentile_ranges[idx_bins]*integral_x){
      binning_2d_x.push_back(hist_aux_x->GetBinCenter(ibin));
      cout<<"bin range starts at = "<<hist_aux_x->GetBinCenter(ibin)<<endl;
      if(idx_bins+1 == percentile_ranges.size()) break;
      idx_bins +=1;
    }
  }
  idx_bins = 0;
  count_events = 0;
  for(int ibin=1; ibin<hist_aux_y->GetNbinsX()+1; ibin++){
    count_events += hist_aux_y->GetBinContent(ibin);
    if(count_events >= percentile_ranges[idx_bins]*integral_y){
      binning_2d_y.push_back(hist_aux_y->GetBinCenter(ibin));
      cout<<"bin range starts at = "<<hist_aux_y->GetBinCenter(ibin)<<endl;
      if(idx_bins+1 == percentile_ranges.size()) break;
      idx_bins +=1;
    }
  }
  if( valid_type_x==0 ) return std::make_pair(cat.binning_2d_x, binning_2d_y);
  if( valid_type_y==0 ) return std::make_pair(binning_2d_x, cat.binning_2d_y);
  return std::make_pair(binning_2d_x, binning_2d_y);
}

#endif
