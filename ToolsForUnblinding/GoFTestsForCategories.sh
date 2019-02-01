#!/bin/bash

ERA=2016
CHANNEL=em
BIN=13
ALGO=saturated

CMSSW_LOCATION=/nfs/dust/cms/user/mameyer/SM_HiggsTauTau/CombineHarvester/2017/CMSSW_8_1_0/src
#BASE_PATH=/nfs/dust/cms/user/tlenz/13TeV/2017/SM_HTauTau/HTauTau_emu/DNN/Datacards/output/
BASE_PATH=/nfs/dust/cms/user/mameyer/SM_HiggsTauTau/HTauTau_emu/DNN/Datacards/output/
CURRENT_PATH=$(pwd)

export SCRAM_ARCH=slc6_amd64_gcc530
cd ${CMSSW_LOCATION}
source /cvmfs/cms.cern.ch/cmsset_default.sh
cmsenv

cd -

cd ${CMSSW_BASE}/src/CombineHarvester/HTTSM2017
cd output
rm -rf *
cd -

$CMSSW_BASE/bin/slc6_amd64_gcc530/MorphingSM2017 \
    --base_path=$BASE_PATH \
    --input_folder_em=$INPUT_FOLDER \
    --real_data=true \
    --classic_bbb=false \
    --jetfakes=false \
    --embedding=${EMB} \
    --postfix="-$CAT" \
    --channel="em" \
    --auto_rebin=true \
    --stxs_signals=stxs_stage0 \
    --categories=stxs_stage1 \
    --era=${ERA}\
    --output=$OUTPUT_FOLDER \
    --regional_jec=true \
    --ggh_wg1=true


cd output/${ERA}_smhtt/cmb/125/
for FILE in *.txt
do
    sed -i '$s/$/\n * autoMCStats 0.0/' $FILE
done
cd -

cd output/${ERA}_smhtt/em/125/
for FILE in *.txt
do
    sed -i '$s/$/\n * autoMCStats 0.0/' $FILE
done
cd -

cd $CURRENT_PATH

for ALGO in saturated AD KS; do
    # Clean ROOT files
    rm -f higgs*.root *.json *.pdf *.png ${PWD}/workspace.root

    # Make workspace
    combineTool.py -M T2W -o $CURRENT_PATH/workspace.root -i /nfs/dust/cms/user/mameyer/SM_HiggsTauTau/CombineHarvester/2017/CMSSW_8_1_0/src/CombineHarvester/HTTSM2017/output/${ERA}_smhtt/cmb/125/htt_${CHANNEL}_${BIN}_Run${ERA}.txt --parallel 20

    # Get test statistic
    combineTool.py -M GoodnessOfFit --algorithm $ALGO -m 125 -d ${PWD}/workspace.root \
        -n ".$ALGO" --plots \
        --X-rtd MINIMIZER_analytic --cminDefaultMinimizerStrategy 0

    # Throw toys
    combineTool.py -M GoodnessOfFit --algorithm $ALGO -m 125 --there -d ${PWD}/workspace.root \
        -n ".$ALGO.toys" -t 50 -s 1230:1249:1 --parallel 20 --verbose 0 \
        --X-rtd MINIMIZER_analytic --cminDefaultMinimizerStrategy 0

    # Collect results
    combineTool.py -M CollectGoodnessOfFit \
        --input higgsCombine.${ALGO}.GoodnessOfFit.mH125.root \
            higgsCombine.${ALGO}.toys.GoodnessOfFit.mH125.*.root -o ${CHANNEL}_${ERA}_${BIN}.${ALGO}.json

    # Plot
    plotGof.py --statistic ${ALGO} --mass 125.0 --output ${CHANNEL}_${ERA}_${BIN}.${ALGO} ${CHANNEL}_${ERA}_${BIN}.${ALGO}.json

    # Save results
    mkdir -p results/
    cp *${ALGO}.png *${ALGO}.pdf results/
done