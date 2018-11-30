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

  TString variable_1d = "m_vis";
  TString variable_2d = "m_vis : pt_2";
  TString variable = "m_vis";

  TH1D *hist_1d;
  TH1D *histSS_1d;

  TH2D *hist_2d;
  TH2D *histSS_2d;

  map< TString , Sample > uncertainties;

  Sample(TString sampleName = "", TString filepath = ""){
    name     = sampleName;
    filename = filepath;
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

  TString variable_1d = "";
  TString variable_2d = "";
  TString variable    = "";
  bool plot_2d = false;
  TString cutstring   = "";
  TString cutstring_ss = "";
  TString weight = "";
  TString qcdweight = "1*";
  TString gg_scale_weight_up   = "";
  TString gg_scale_weight_down = "";

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
#endif
