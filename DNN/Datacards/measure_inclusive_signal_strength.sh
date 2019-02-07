#!bin/bash

# Please specify the following input variables
#ERA=2017
#EMB=1
DATACARD=${ERA}_workspace.root
SEED=1234
MASS=125
CAT="ML"
BASE_PATH=/nfs/dust/cms/user/mameyer/SM_HiggsTauTau/HTauTau_emu/DNN/Datacards/output/
#BASE_PATH=/nfs/dust/cms/user/tlenz/13TeV/2017/SM_HTauTau/HTauTau_emu/DNN/Datacards/output/
INPUT_FOLDER=${ERA}
OUTPUT_FOLDER=${ERA}_smhtt
CMSSW_LOCATION=/nfs/dust/cms/user/mameyer/SM_HiggsTauTau/CombineForUnbliding/CMSSW_8_1_0/src
#CMSSW_LOCATION=/nfs/dust/cms/user/tlenz/13TeV/2017/CMSSW/CombineHarvester/2017/CMSSW_7_4_7/src
CURRENT_PATH=$(pwd)

export SCRAM_ARCH=slc6_amd64_gcc530
cd ${CMSSW_LOCATION}
source /cvmfs/cms.cern.ch/cmsset_default.sh
cmsenv

cd ${CMSSW_BASE}/src/CombineHarvester/HTTSM2017
#rm -rf output/*


# Produce the datacard for the em channel (please add the em channel in category gof in the morphing script)
ulimit -s unlimited
$CMSSW_BASE/bin/slc6_amd64_gcc530/MorphingSM2017 \
    --base_path=$BASE_PATH \
    --input_folder_em=$INPUT_FOLDER \
    --real_data=true \
    --classic_bbb=false \
    --binomial_bbb=true \
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

rm -f ${CMSSW_BASE}/src/CombineHarvester/HTTSM2017/output/${OUTPUT_FOLDER}/cmb/125/${ERA}_workspace.root

# Creating workspace
DATACARD_PATH=output/${ERA}_smhtt/cmb/125
combineTool.py -M T2W -o ${ERA}_workspace.root -i ${DATACARD_PATH} --parallel 12

combineTool.py -M MultiDimFit -m 125 -d ${DATACARD_PATH}/${ERA}_workspace.root \
    --algo singles\
    --robustFit 1 -n $ERA \
    --X-rtd MINIMIZER_analytic --cminDefaultMinimizerStrategy 0 \
    --rMin -50.0 --rMax 50.0


  #--algo singles -t -1 --expectSignal 1 \
# Calculating signal strength constraints
combine -M FitDiagnostics -m 125 ${DATACARD_PATH}/${ERA}_workspace.root \
    --robustFit 1 -n $ERA \
    --robustHesse 1 \
    --rMin -50.0 --rMax 50.0 \
    --X-rtd MINIMIZER_analytic --cminDefaultMinimizerStrategy 0

cd ${CURRENT_PATH}
