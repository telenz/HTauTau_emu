#include "TH2D.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include <iostream>
#include <vector>
#include <map>
#include "HttStylesNew.cc"
#include "Unfold.C"
#include "CMS_lumi.C"

// ********************************************************************************************
class Sample {

public :  

  TString name;
  TString filename;

  TString cutString    = "";
  TString weightString = "1*";
  TString cutStringSS    = "";
  TString weightStringSS = "1*";
  TString cutStringSSrelaxed    = "";
  TString weightStringSSrelaxed = "1*";

  TString topweight     = "1*";
  TString zptmassweight = "1*";
  TString ggscaleweight = "1*";
  TString norm = "1*";

  TString variable_1d = "m_vis";
  TString variable_2d = "m_vis : pt_2";

  TH1D *hist_1d;
  TH1D *histSS_1d;
  TH1D *histSSrelaxed_1d;

  TH2D *hist_2d;
  TH2D *histSS_2d;
  TH2D *histSSrelaxed_2d;

  map< TString , Sample > uncertainties;

  Sample(TString sampleName = "", TString filepath = ""){
    name     = sampleName;
    filename = filepath;
  }
};
// ********************************************************************************************

// ********************************************************************************************
class Category {

public :   
  TString name = "";
  TString suffix = "";

  TString cutString   = "";
  TString cutStringSS = "";
  TString variable_1d = "";
  TString variable_2d = "";

  int nbins_1d  = 1;
  int nbins_x_2d = 1;
  int nbins_y_2d = 1;
  float *bins_1d;
  float *bins_x_2d;
  float *bins_y_2d;

  vector<Sample> *sampleList;

  Category(TString catName){
    name = catName;
  }
};
// ********************************************************************************************
