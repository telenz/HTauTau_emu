#!bin/bash

# Script which executes a gof test for a given variable $var

# Please specify the following input variables
#ERA=2017
#EMB=0
#VAR="pt_2"
#INPUT_FOLDER=var_1d
DATACARD=${ERA}_workspace.root
SEED=1234
MASS=125
NUM_TOYS=300
CURRENT_PATH=$(pwd)
BASE_PATH=${CURRENT_PATH}/output/${ERA}
OUTPUT_FOLDER=${ERA}_${VAR}_smhtt
#CMSSW_LOCATION=/nfs/dust/cms/user/mameyer/SM_HiggsTauTau/CombineHarvester/2017/CMSSW_8_1_0/src
CMSSW_LOCATION=/nfs/dust/cms/user/tlenz/13TeV/2017/CMSSW/CMSSW_8_1_0/src

export SCRAM_ARCH=slc6_amd64_gcc530
cd ${CMSSW_LOCATION}
source /cvmfs/cms.cern.ch/cmsset_default.sh
cmsenv

cd ${CMSSW_BASE}/src/CombineHarvester/HTTSM2017

echo "ERA is " $ERA

echo "Input folder is " $INPUT_FOLDER
echo "var is " $VAR

MorphingSM2017 \
    --base_path=${BASE_PATH} \
    --input_folder_em=$INPUT_FOLDER \
    --real_data=true \
    --classic_bbb=true \
    --jetfakes=false \
    --embedding=${EMB} \
    --postfix="-$VAR" \
    --channel="em" \
    --auto_rebin=false \
    --stxs_signals=stxs_stage0 \
    --categories=gof \
    --gof_category_name="em_inclusive" \
    --era=${ERA} \
    --output=$OUTPUT_FOLDER \
    --regional_jec=true \
    --ggh_wg1=true

# Create workspace
combineTool.py -M T2W -o ${ERA}_workspace.root -m $MASS -i ${CMSSW_BASE}/src/CombineHarvester/HTTSM2017/output/${OUTPUT_FOLDER}/cmb/125/

workspace_location=${CMSSW_BASE}/src/CombineHarvester/HTTSM2017/output/${OUTPUT_FOLDER}/cmb/125/
cd ${workspace_location}

# Get test statistic value
combine -M GoodnessOfFit --algo=saturated -m $MASS -d $DATACARD

# Throw toys
combine -M GoodnessOfFit --algo=saturated -m $MASS -d $DATACARD -s $SEED -t $NUM_TOYS

# Collect results
combineTool.py -M CollectGoodnessOfFit --input higgsCombineTest.GoodnessOfFit.mH$MASS.root higgsCombineTest.GoodnessOfFit.mH$MASS.$SEED.root --output gof-${VAR}.json

# Plot
plotGof.py --statistic saturated --mass $MASS.0 --output gof-${VAR} gof-${VAR}.json

cp gof-${VAR}.json ${BASE_PATH}/${INPUT_FOLDER}/.

# Get p-value printed out
echo ""
grep "p" *.json
echo ""

