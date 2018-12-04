#!bin/bash

# Script which executes a gof test for a given variable $var

# Pleas specify the following input variables
#ERA=2016
DATACARD=${ERA}_workspace.root
SEED=1234
MASS=125
NUM_TOYS=300
CAT="ML"
BASE_PATH=/nfs/dust/cms/user/tlenz/13TeV/2017/SM_HTauTau/HTauTau_emu/DNN/Datacards/output/
INPUT_FOLDER=${ERA}
OUTPUT_FOLDER=2016_smhtt
CMSSW_LOCATION=/nfs/dust/cms/user/tlenz/13TeV/2017/CMSSW/CombineHarvester/2017/CMSSW_7_4_7/src
CURRENT_PATH=$(pwd)


cd ${CMSSW_LOCATION}
source /cvmfs/cms.cern.ch/cmsset_default.sh
cmsenv

cd ${CMSSW_BASE}/src/CombineHarvester/HTTSM2017
cd output
rm -rf *
cd -

# Produce the datacard for the em channel (please add the em channel in category gof in the morphing script)
MorphingSM2017 --base_path=$BASE_PATH  --input_folder_em=${INPUT_FOLDER} --real_data=true --jetfakes=0 --embedding=${EMB} --postfix="-$CAT" --channel="em" --auto_rebin=true --stxs_signals="stxs_stage0" --categories="stxs_stage0" --era=${ERA} --output=$OUTPUT_FOLDER --regional_jec=true --ggh_wg1=true


# Creating workspace
DATACARD_PATH=output/${ERA}_smhtt/cmb/125
combineTool.py -M T2W -o ${ERA}_workspace.root -i ${DATACARD_PATH} --parallel 12

# Calculating signal strength constraints
combine -M MaxLikelihoodFit -m 125 ${DATACARD_PATH}/${ERA}_workspace.root \
    --robustFit 1 -n $ERA \
    --minimizerAlgoForMinos=Minuit2,Migrad

# Prefit shapes
# The text datacard is referenced only to add the original binning of the shapes
# to the output histograms of the command. Otherwise, the bins are numbered with
# integers.
PostFitShapesFromWorkspace -m 125 -w ${DATACARD_PATH}/${ERA}_workspace.root \
    -d output/${ERA}_smhtt/cmb/125/combined.txt.cmb -o ${ERA}_datacard_shapes_prefit.root

# Postfit shapes
# The file mlfit${ERA}.root is the result of the MaxLikelihoodFit above
PostFitShapesFromWorkspace -m 125 -w ${DATACARD_PATH}/${ERA}_workspace.root \
    -d output/${ERA}_smhtt/cmb/125/combined.txt.cmb -o ${ERA}_datacard_shapes_postfit_sb.root \
    -f mlfit${ERA}.root:fit_s --postfit

cd ${CURRENT_PATH}

cp ${CMSSW_BASE}/src/CombineHarvester/HTTSM2017/${ERA}_datacard_shapes_postfit_sb.root output/${ERA}