#include "TH2D.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include <iostream>
#include <vector>
#include "HttStylesNew.cc"
#include "Unfold.C"
#include "CMS_lumi.C"

class analyzer_instance_2D {

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

  analyzer_instance_2D(TString name = ""){
    name = name;
    /* hist          = new TH2D(name+"_os","",100,0,200,100,0,200); */
    /* histSS        = new TH2D(name+"_ss","",100,0,200,100,0,200); */
    /* histSSrelaxed = new TH2D(name+"_ss_relaxed","",100,0,200,100,0,200); */
  }
};


class sysUncertainty : public analyzer_instance_2D {

public :
  
  TString sysName;
  
  sysUncertainty(TString sysName = ""){
    name = sysName;
    /* hist          = new TH2D("","",100,0,200,100,0,200); */
    /* histSSrelaxed = new TH2D("","",100,0,200,100,0,200); */
  }
};

class sample : public analyzer_instance_2D {

public :  

  TString filename;
  vector<sysUncertainty> uncertainties;
  double norm = 1;
 
 sample(TString sampleName = "", TString filepath = "")
    {
      name     = sampleName;
      filename = filepath;
      cout<<filename<<endl;
      /* hist          = new TH2D(name+"_os","",100,0,200,100,0,200); */
      /* histSSrelaxed = new TH2D(name+"_ss_relaxed","",100,0,200,100,0,200); */
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
