#!bin/bash

# Script which executes a gof test for a given variable $var

# Pleas specify the following input variables
ERA=2016
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
MorphingSM2017 --base_path=$BASE_PATH  --input_folder_em=$INPUT_FOLDER --real_data=false --jetfakes=0 --embedding=0 --postfix="-$CAT" --channel="em" --auto_rebin=true --stxs_signals="stxs_stage0" --categories="stxs_stage0" --era=${ERA} --output=$OUTPUT_FOLDER --regional_jec=false --ggh_wg1=false

# Create workspace
combineTool.py -M T2W -o workspace.root -i ${CMSSW_BASE}/src/CombineHarvester/HTTSM2017/output/2016_smhtt/cmb/125/ --parallel 10 -m 125 \
    -P HiggsAnalysis.CombinedLimit.PhysicsModel:multiSignalModel \
    --PO '"map=^.*/ggH.?$:r_ggH[1,-10,10]"' \
    --PO '"map=^.*/qqH.?$:r_qqH[1,-10,10]"'

# old : combineTool.py -M T2W -o workspace.root -m $MASS -i ${CMSSW_BASE}/src/CombineHarvester/HTTSM2017/output/2016_smhtt/cmb/125/

# Rename workspace
workspace_location=${CMSSW_BASE}/src/CombineHarvester/HTTSM2017/output/${OUTPUT_FOLDER}/cmb/125/
cp ${workspace_location}/workspace.root ${workspace_location}/2016_workspace.root

combineTool.py -M MultiDimFit -m 125 -d ${workspace_location}/${ERA}_workspace.root \
        --algo singles -t -1 --expectSignal 1 \
        --X-rtd FITTER_NEW_CROSSING_ALGO --X-rtd FITTER_NEVER_GIVE_UP \
        --robustFit 1 -n $ERA \
        --minimizerAlgoForMinos=Minuit2,Migrad

cd ${CURRENT_PATH}
