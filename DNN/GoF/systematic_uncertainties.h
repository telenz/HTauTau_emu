#include "useful_classes.h"

Sample create_systematic_uncertainty( TString unc_name, TString combine_harvester_name,
				      bool gof_2d,
				      Sample mother_sample, TTree* input_tree,
				      bool is_variable_shift=true, TString shift_name = "",
				      bool is_weight_uncertainty=false,
				      TString remove_this_string="", TString add_this_string=""){

  Sample uncertainty_sample = mother_sample;
  TString variable = "";
  if(!gof_2d) variable = mother_sample.variable_1d;
  else        variable = mother_sample.variable_2d;

  mother_sample.uncertainties.insert( make_pair( unc_name , uncertainty_sample ) );
  mother_sample.uncertainties[unc_name].name += combine_harvester_name;

  if(is_variable_shift){
    if(input_tree -> GetBranch("dzeta_" +shift_name)) mother_sample.uncertainties[unc_name].cutString.ReplaceAll("dzeta","dzeta_"+shift_name);
    if(input_tree -> GetBranch("pt_1_" +shift_name)) mother_sample.uncertainties[unc_name].cutString.ReplaceAll("pt_1","pt_1_"+shift_name);
    if(input_tree -> GetBranch("pt_2_" +shift_name)) mother_sample.uncertainties[unc_name].cutString.ReplaceAll("pt_2","pt_2_"+shift_name);
    if(input_tree -> GetBranch("mTdileptonMET_" +shift_name)) mother_sample.uncertainties[unc_name].cutString.ReplaceAll("mTdileptonMET","mTdileptonMET_"+shift_name);
    if(input_tree -> GetBranch("nbtag_" +shift_name)) mother_sample.uncertainties[unc_name].cutString.ReplaceAll("nbtag","nbtag_"+shift_name);
    if(!gof_2d){
      if(input_tree -> GetBranch( variable + "_" + shift_name )) mother_sample.uncertainties[unc_name].variable_1d = variable + "_" + shift_name;
      //      else cout<<"No systematic shift for "<<shift_name<<" for variable "<<variable<<" available in tree."<<endl;
    }
    else{
      // split the two variables into two seperate strings
      TString var1, var1_variation, var2, var2_variation;
      var1 = mother_sample.variable_2d( 0 , mother_sample.variable_2d.First(":") );
      var2 = mother_sample.variable_2d( mother_sample.variable_2d.First(":")+1 , mother_sample.variable_2d.Length() );
      var1_variation   = var1;
      var2_variation   = var2;
      if(input_tree -> GetBranch(var1_variation+"_"+shift_name)) var1_variation = var1 + "_" + shift_name;
      if(input_tree -> GetBranch(var2_variation+"_"+shift_name)) var2_variation = var2 + "_" + shift_name;
      mother_sample.uncertainties[unc_name].variable_2d = var1_variation   + ":" + var2_variation;
    }
  }

  if(is_weight_uncertainty){
    mother_sample.uncertainties[unc_name].weightString.ReplaceAll(remove_this_string,add_this_string);
    mother_sample.uncertainties[unc_name].weightStringSS.ReplaceAll(remove_this_string,add_this_string);
  }

  return  mother_sample;
}

