#include "TH2D.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include <iostream>
#include <vector>
#include "HttStylesNew.cc"
#include "Unfold.C"
#include "CMS_lumi.C"

class analyzedInstance {

public :
  TString name;
  TString cutString = "";
  TString cutStringSS = "";
  TString cutStringSSrelaxed = "";
  TString weightString = "";
  TString weightStringSS = "";
  TString weightStringSSrelaxed = "";
  TString topweight     = "";
  TString zptmassweight = "";
  TString ggscaleweight = "";
  TString qcdweight = "";
  TH2D *hist;
  TH2D *histSS;
  TH2D *histSSrelaxed;
  TString variable;
  sysUncertainty(TString sysName=""){
    name = sysName;
    hist          = new TH2D(name+"_os","",100,0,200,100,0,200);
    histSS        = new TH2D(name+"_ss","",100,0,200,100,0,200);
    histSSrelaxed = new TH2D(name+"_ss_relaxed","",100,0,200,100,0,200);
  }
};


class sysUncertainty {

public :
  TString name;
  TString cutString = "";
  TString weightString = "";
  TString topweight = "topptweightRun2*";
  TString zptmassweight = "zptmassweight*";
  TString ggscaleweight = "";
  TString treeVar;
  TH2D *hist;
  TH2D *histSSrelaxed;
  TString variable;
  sysUncertainty(TString sysName=""){
    name = sysName;
    hist          = new TH2D("","",100,0,200,100,0,200);
    histSSrelaxed = new TH2D("","",100,0,200,100,0,200);
  }
};

class sample {

public :  
  TString name;
  TString filename;
  TString cutString;
  TString cutStringSS;
  TString cutStringSSrelaxed;
  TString weightString;
  TString weightStringSS;
  TString weightStringSSrelaxed;
  TH2D *hist;
  TH2D *histSS;
  TH2D *histSSrelaxed;
  TString variable;
  vector<sysUncertainty> uncertainties;
  double norm = 1;
 
  sample(TString sampleName="", TString filename=""){
    name = sampleName;
    filename = filename;
    hist          = new TH2D(name+"_os","",100,0,200,100,0,200);
    histSSrelaxed = new TH2D(name+"_ss_relaxed","",100,0,200,100,0,200);
  }
};

class category {

public :   
  TString name = "";
  TString cutString   = "";
  TString cutStringSS = "";
  TString variable   = "";
  float *binsX;
  float *binsY;
  vector<sample> *sampleList;

  category(TString catName){
    name = catName;
  }
};
