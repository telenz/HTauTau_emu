#!bin/bash

# Script which executes a gof test for a given variable $var

# Pleas specify the following input variables
ERA=2016
EMB=0
DATACARD=${ERA}_workspace.root
SEED=1234
MASS=125
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
MorphingSM2017 --base_path=$BASE_PATH  --input_folder_em=$INPUT_FOLDER --real_data=false --jetfakes=0 --embedding=${EMB} --postfix="-$CAT" --channel="em" --auto_rebin=true --stxs_signals="stxs_stage1" --categories="stxs_stage1" --era=${ERA} --output=$OUTPUT_FOLDER --regional_jec=true --ggh_wg1=true

# Create workspace
combineTool.py -M T2W -o ${ERA}_workspace.root -i ${CMSSW_BASE}/src/CombineHarvester/HTTSM2017/output/2016_smhtt/cmb/125/ --parallel 12 \
    -P HiggsAnalysis.CombinedLimit.PhysicsModel:multiSignalModel \
    --PO '"map=^.*/ggH_0J.?$:r_ggH_0J[1,-30,30]"' \
    --PO '"map=^.*/ggH_1J_PTH_0_60.?$:r_ggH_1J_PTH_0_60[1,-30,30]"' \
    --PO '"map=^.*/ggH_1J_PTH_60_120.?$:r_ggH_1J_PTH_60_120[1,-30,30]"' \
    --PO '"map=^.*/ggH_1J_PTH_120_200.?$:r_ggH_1J_PTH_120_200[1,-30,30]"' \
    --PO '"map=^.*/ggH_1J_PTH_GT200.?$:r_ggH_1J_PTH_GT200[1,-30,30]"' \
    --PO '"map=^.*/ggH_GE2J_PTH_0_60.?$:r_ggH_GE2J_PTH_0_60[1,-30,30]"' \
    --PO '"map=^.*/ggH_GE2J_PTH_60_120.?$:r_ggH_GE2J_PTH_60_120[1,-30,30]"' \
    --PO '"map=^.*/ggH_GE2J_PTH_120_200.?$:r_ggH_GE2J_PTH_120_200[1,-30,30]"' \
    --PO '"map=^.*/ggH_GE2J_PTH_GT200.?$:r_ggH_GE2J_PTH_GT200[1,-30,30]"' \
    --PO '"map=^.*/ggH_VBFTOPO_JET3.?$:r_ggH_VBFTOPO_JET3[1,-30,30]"' \
    --PO '"map=^.*/ggH_VBFTOPO_JET3VETO.?$:r_ggH_VBFTOPO_JET3VETO[1,-30,30]"' \
    --PO '"map=^.*/qqH_VBFTOPO_JET3VETO.?$:r_qqH_VBFTOPO_JET3VETO[1,-30,30]"' \
    --PO '"map=^.*/qqH_VBFTOPO_JET3.?$:r_qqH_VBFTOPO_JET3[1,-30,30]"' \
    --PO '"map=^.*/qqH_REST.?$:r_qqH_REST[1,-30,30]"' \
    --PO '"map=^.*/qqH_VH2JET.?$:r_qqH_VH2JET[1,-100,100]"' \
    --PO '"map=^.*/qqH_PTJET1_GT200.?$:r_qqH_PTJET1_GT200[1,-30,30]"'

workspace_location=${CMSSW_BASE}/src/CombineHarvester/HTTSM2017/output/${OUTPUT_FOLDER}/cmb/125/

combineTool.py -M MultiDimFit -m 125 -d ${workspace_location}/${ERA}_workspace.root \
        --algo singles -t -1 --expectSignal 1 \
        --X-rtd FITTER_NEW_CROSSING_ALGO --X-rtd FITTER_NEVER_GIVE_UP \
        --robustFit 1 -n $ERA \
        --minimizerAlgoForMinos=Minuit2,Migrad

cd ${CURRENT_PATH}
