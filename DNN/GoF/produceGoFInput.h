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
class Analyzer2D {

public :
  TString name;

  TString cutString    = "";
  TString weightString = "";
  TString cutStringSS    = "";
  TString weightStringSS = "";
  TString cutStringSSrelaxed    = "";
  TString weightStringSSrelaxed = "";

  TString topweight     = "";
  TString zptmassweight = "";
  TString qcdweight     = "";
  TString ggscaleweight = "";

  TH2D *hist;
  TH2D *histSS;
  TH2D *histSSrelaxed;
  
  TString variable = "m_vis : pt_2";

  Analyzer2D(TString name = ""){
    name = name;
  }
};
// ********************************************************************************************

// ********************************************************************************************
class Sample : public Analyzer2D {

public :  
  TString filename;
  map< TString , Sample > uncertainties;
  double norm = 1;
 
  Sample(TString sampleName = "", TString filepath = ""){
    name     = sampleName;
    filename = filepath;
  }
};

// ********************************************************************************************

class Category {

public :   
  TString name = "";
  TString cutString   = "";
  TString cutStringSS = "";
  TString variable   = "";
  float *binsX;
  float *binsY;
  vector<Sample> *sampleList;

  Category(TString catName){
    name = catName;
  }
};
