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
  double norm = 1;

  TString variable = "m_vis : pt_2";

  TH2D *hist;
  TH2D *histSS;
  TH2D *histSSrelaxed;

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
  TString variable    = "";

  int nBinsX = 1;
  int nBinsY = 1;
  float *binsX;
  float *binsY;

  vector<Sample> *sampleList;

  Category(TString catName){
    name = catName;
  }
};
// ********************************************************************************************
