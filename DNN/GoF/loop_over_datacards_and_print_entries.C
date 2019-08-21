#include "bin/HttStylesNew.cc"
#include <sstream> 
#include "TFile.h"
#include "TKey.h"
#include <iostream>

void loop_over_datacards_and_print_entries() {


  TFile *in = new TFile("output/2017/var_1d/htt_em.inputs-sm-Run2017-pt_2.root","READ");
  in->cd("em_inclusive/");

  TIter next(gDirectory->GetListOfKeys());
  TKey *key = 0;
  while ((key = (TKey*)next()))
    {
      TClass *c = gROOT->GetClass(key->GetClassName());
      if (!c->InheritsFrom("TH1")) continue;
      TH1D *h = (TH1D*) key->ReadObj();
      h->SetDirectory(0);
      TString hName = h->GetName();
      
      for(int i=1; i<=h->GetNbinsX(); i++){
	cout<<hName<<" bin " << i << " = "<<h->GetBinContent(i)<<endl;
	cout<<h->GetBinLowEdge(i)<<endl;
      }
      cout<<endl;
      delete h;
    }
  delete key;
  
}
