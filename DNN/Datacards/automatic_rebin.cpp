#include "TFile.h"
#include "TKey.h"
#include "TCollection.h"
#include "TIterator.h"
#include "TString.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TDirectory.h"
#include <iostream>

void automatic_rebin(){

  TFile* in_file = new TFile("/nfs/dust/cms/user/mameyer/SM_HiggsTauTau/HTauTau_emu/DNN/Datacards/output/2017/htt_em.inputs-sm-Run2017-ML_stage1_v2.root");
  TFile* out_file = new TFile("rebinned_output_file.root","RECREATE");

  for (auto key_dir : *in_file->GetListOfKeys() ) {

    auto key_dir_2 = static_cast<TKey*>(key_dir);
    auto obj_dir = key_dir_2->ReadObj();
    if ( !obj_dir->InheritsFrom("TDirectory") ){
      cout<<"There is a non-direectory in the input root file."<<endl;
      continue;
    }

    TString dir_name = obj_dir -> GetName();
    std::cout << "name: " << dir_name << std::endl;
    out_file->mkdir(dir_name);
    out_file->cd(dir_name);
    in_file->cd(dir_name);
    for (auto keyO : *gDirectory->GetListOfKeys()) {
      auto key = static_cast<TKey*>(keyO);
      auto obj = key->ReadObj();
      TH1* h   = static_cast<TH1*>(obj);
      cout << "histo name = " << key->GetName() << endl;
      h -> Rebin(5);
      out_file -> cd(dir_name);
      h        -> Write(key->GetName());  
      in_file  -> cd(dir_name);
    }
  }
out_file->Write();
}
