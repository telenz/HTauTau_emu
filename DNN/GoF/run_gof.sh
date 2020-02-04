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
OUTPUT_FOLDER=output/${ERA}_${VAR}_smhtt
CMSSW_LOCATION=/nfs/dust/cms/user/mameyer/SM_HiggsTauTau/CombineHarvester/Run2Analysis/CMSSW_10_2_16/src/
#CMSSW_LOCATION=/nfs/dust/cms/user/mameyer/SM_HiggsTauTau/CombineHarvester/LegacyAnalysis/CMSSW_8_1_0/src
#CMSSW_LOCATION=/nfs/dust/cms/user/tlenz/13TeV/2018/CMSSW/SMHTauTau/CMSSW_8_1_0/src

export SCRAM_ARCH=slc6_amd64_gcc700
cd ${CMSSW_LOCATION}
source /cvmfs/cms.cern.ch/cmsset_default.sh
cmsenv

cd ${CMSSW_BASE}/src/CombineHarvester/SMRun2Legacy

echo "ERA is " $ERA

echo "Input folder is " $INPUT_FOLDER
echo "var is " $VAR

MorphingSMRun2Legacy \
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
    --ggh_wg1=true \
    --rebin_categories=false

# Create workspace
combineTool.py -M T2W -o ${ERA}_workspace.root -m $MASS -i ${CMSSW_BASE}/src/CombineHarvester/SMRun2Legacy/${OUTPUT_FOLDER}/cmb/125/ --parallel 8

workspace_location=${CMSSW_BASE}/src/CombineHarvester/SMRun2Legacy/${OUTPUT_FOLDER}/cmb/125/
cd ${workspace_location}

for ALGO in saturated AD KS; do

    # Get test statistic value
    combineTool.py -M GoodnessOfFit --algo=$ALGO -m $MASS -d $DATACARD -n ".$ALGO" --plots --X-rtd MINIMIZER_analytic --cminDefaultMinimizerStrategy 0 --fixedSignalStrength=0

    # Throw toys
    combineTool.py -M GoodnessOfFit --algo=$ALGO -m $MASS --there -d $DATACARD -s $SEED -t $NUM_TOYS -n ".$ALGO.toys" --parallel 8 --verbose 0 --X-rtd MINIMIZER_analytic --cminDefaultMinimizerStrategy 0 --fixedSignalStrength=0

    # Collect results
    combineTool.py -M CollectGoodnessOfFit --input higgsCombine.$ALGO.GoodnessOfFit.mH$MASS.root higgsCombine.$ALGO.toys.GoodnessOfFit.mH$MASS.$SEED.root --output gof-${VAR}.$ALGO.json

    # Plot
    plotGof.py --statistic ${ALGO} --mass $MASS.0 --output gof-${VAR}.${ALGO} gof-${VAR}.${ALGO}.json

    cp gof-${VAR}.${ALGO}.json ${BASE_PATH}/${INPUT_FOLDER}/.
    cp *.png ${BASE_PATH}/${INPUT_FOLDER}/.

    # Get p-value printed out
    echo ""
    grep "p" *.json
    echo ""

done