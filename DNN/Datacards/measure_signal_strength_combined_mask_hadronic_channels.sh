#!bin/bash

em=0;et=1;tt=1;mt=1 

# Please specify the following input variables
ERA=2017
EMB=1
DATACARD=${ERA}_workspace.root
SEED=1234
MASS=125
CAT="ML"
BASE_PATH=/nfs/dust/cms/user/tlenz/13TeV/2017/SM_HTauTau/HTauTau_emu/DNN/Datacards/output/
#BASE_PATH=/nfs/dust/cms/user/mameyer/SM_HiggsTauTau/HTauTau_emu/DNN/Datacards/output/
INPUT_FOLDER=${ERA}
OUTPUT_FOLDER=${ERA}_smhtt
#CMSSW_LOCATION=/nfs/dust/cms/user/mameyer/SM_HiggsTauTau/CombineHarvester/2017/CMSSW_8_1_0/src
CMSSW_LOCATION=/nfs/dust/cms/user/tlenz/13TeV/2017/CMSSW/CMSSW_8_1_0/src
CURRENT_PATH=$(pwd)

export SCRAM_ARCH=slc6_amd64_gcc530
cd ${CMSSW_LOCATION}
source /cvmfs/cms.cern.ch/cmsset_default.sh
cmsenv

cd ${CMSSW_BASE}/src/CombineHarvester/HTTSM2017
rm -rf output/*


# Produce the datacard for the em channel (please add the em channel in category gof in the morphing script)
ulimit -s unlimited
$CMSSW_BASE/bin/slc6_amd64_gcc530/MorphingSM2017 \
    --base_path=$BASE_PATH \
    --input_folder_em=$INPUT_FOLDER \
    --input_folder_et=$INPUT_FOLDER \
    --input_folder_mt=$INPUT_FOLDER \
    --input_folder_tt=$INPUT_FOLDER \
    --real_data=false \
    --classic_bbb=false \
    --jetfakes=true \
    --embedding=${EMB} \
    --postfix="-$CAT" \
    --channel="em,et,mt,tt" \
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
cd output/${ERA}_smhtt/et/125/
for FILE in *.txt
do
    sed -i '$s/$/\n * autoMCStats 0.0/' $FILE
done
cd -
cd output/${ERA}_smhtt/mt/125/
for FILE in *.txt
do
    sed -i '$s/$/\n * autoMCStats 0.0/' $FILE
done
cd -
cd output/${ERA}_smhtt/tt/125/
for FILE in *.txt
do
    sed -i '$s/$/\n * autoMCStats 0.0/' $FILE
done
cd -

rm -f ${CMSSW_BASE}/src/CombineHarvester/HTTSM2017/output/${OUTPUT_FOLDER}/cmb/125/${ERA}_workspace.root

# Create workspace
combineTool.py -M T2W -o ${ERA}_workspace.root -i ${CMSSW_BASE}/src/CombineHarvester/HTTSM2017/output/${ERA}_smhtt/cmb/125/  \
    --parallel 8 -m 125\
    -P HiggsAnalysis.CombinedLimit.PhysicsModel:multiSignalModel \
    --PO '"map=^.*/ggH.?$:r_ggH[1,-5,5]"' \
    --PO '"map=^.*/qqH.?$:r_qqH[1,-5,5]"' \
    --channel-masks

# old : combineTool.py -M T2W -o workspace.root -m $MASS -i ${CMSSW_BASE}/src/CombineHarvester/HTTSM2017/output/2016_smhtt/cmb/125/

workspace_location=${CMSSW_BASE}/src/CombineHarvester/HTTSM2017/output/${ERA}_smhtt/cmb/125/

combineTool.py -M MultiDimFit -m 125 -d ${workspace_location}/${ERA}_workspace.root \
    --algo singles --robustFit 1 \
    --X-rtd MINIMIZER_analytic --cminDefaultMinimizerStrategy 0 \
    -t -1 --expectSignal 1 \
    -n $ERA \
    --setParameters mask_htt_em_12_Run${ERA}=${em},mask_htt_em_13_Run${ERA}=${em},mask_htt_em_14_Run${ERA}=${em},mask_htt_em_16_Run${ERA}=${em},mask_htt_em_18_Run${ERA}=${em},mask_htt_em_19_Run${ERA}=${em},mask_htt_em_1_Run${ERA}=${em},mask_htt_em_2_Run${ERA}=${em},mask_htt_et_11_Run${ERA}=${et},mask_htt_et_12_Run${ERA}=${et},mask_htt_et_13_Run${ERA}=${et},mask_htt_et_14_Run${ERA}=${et},mask_htt_et_15_Run${ERA}=${et},mask_htt_et_16_Run${ERA}=${et},mask_htt_et_1_Run${ERA}=${et},mask_htt_et_2_Run${ERA}=${et},mask_htt_mt_11_Run${ERA}=${mt},mask_htt_mt_12_Run${ERA}=${mt},mask_htt_mt_13_Run${ERA}=${mt},mask_htt_mt_14_Run${ERA}=${mt},mask_htt_mt_15_Run${ERA}=${mt},mask_htt_mt_16_Run${ERA}=${mt},mask_htt_mt_1_Run${ERA}=${mt},mask_htt_mt_2_Run${ERA}=${mt},mask_htt_tt_12_Run${ERA}=${tt},mask_htt_tt_16_Run${ERA}=${tt},mask_htt_tt_17_Run${ERA}=${tt},mask_htt_tt_1_Run${ERA}=${tt},mask_htt_tt_2_Run${ERA}=${tt}

combine -M Significance -t -1 --expectSignal 1  --X-rtd MINIMIZER_analytic --cminDefaultMinimizerStrategy 0 --significance -m 125 -n $ERA  --toysFrequentist output/${ERA}_smhtt/cmb/125/${ERA}_workspace.root --setParameters mask_htt_em_12_Run${ERA}=${em},mask_htt_em_13_Run${ERA}=${em},mask_htt_em_14_Run${ERA}=${em},mask_htt_em_16_Run${ERA}=${em},mask_htt_em_18_Run${ERA}=${em},mask_htt_em_19_Run${ERA}=${em},mask_htt_em_1_Run${ERA}=${em},mask_htt_em_2_Run${ERA}=${em},mask_htt_et_11_Run${ERA}=${et},mask_htt_et_12_Run${ERA}=${et},mask_htt_et_13_Run${ERA}=${et},mask_htt_et_14_Run${ERA}=${et},mask_htt_et_15_Run${ERA}=${et},mask_htt_et_16_Run${ERA}=${et},mask_htt_et_1_Run${ERA}=${et},mask_htt_et_2_Run${ERA}=${et},mask_htt_mt_11_Run${ERA}=${mt},mask_htt_mt_12_Run${ERA}=${mt},mask_htt_mt_13_Run${ERA}=${mt},mask_htt_mt_14_Run${ERA}=${mt},mask_htt_mt_15_Run${ERA}=${mt},mask_htt_mt_16_Run${ERA}=${mt},mask_htt_mt_1_Run${ERA}=${mt},mask_htt_mt_2_Run${ERA}=${mt},mask_htt_tt_12_Run${ERA}=${tt},mask_htt_tt_16_Run${ERA}=${tt},mask_htt_tt_17_Run${ERA}=${tt},mask_htt_tt_1_Run${ERA}=${tt},mask_htt_tt_2_Run${ERA}=${tt}


cd ${CURRENT_PATH}
