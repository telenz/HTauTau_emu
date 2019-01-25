
INPUT=$1
ERA=$2
REAL=$3
CMSSW_LOCATION=/nfs/dust/cms/user/mameyer/SM_HiggsTauTau/CombineHarvester/2017/CMSSW_8_1_0/src
BASE_PATH=${CMSSW_LOCATION}/CombineHarvester/HTTSM2017/shapes/
CAT="ML"

export SCRAM_ARCH=slc6_amd64_gcc530
cd ${CMSSW_LOCATION}
source /cvmfs/cms.cern.ch/cmsset_default.sh
cmsenv

eval `scramv1 runtime -sh`
cd ${CMSSW_BASE}/src/CombineHarvester/HTTSM2017

rm -r output/*_smhtt

$CMSSW_BASE/bin/slc6_amd64_gcc530/MorphingSM2017 \
    --base_path=$BASE_PATH \
    --input_folder_em=$INPUT \
    --real_data=${REAL} \
    --classic_bbb=false \
    --jetfakes=false \
    --embedding=true \
    --postfix="-$CAT" \
    --channel="em" \
    --auto_rebin=true \
    --stxs_signals=stxs_stage0 \
    --categories=stxs_stage1 \
    --era=${ERA}\
    --output=${ERA}_smhtt \
    --regional_jec=true \
    --ggh_wg1=true

cd output/${ERA}_smhtt/cmb/125/
for FILE in *.txt
do
    sed -i '$s/$/\n * autoMCStats 0.0/' $FILE
done
cd -

mkdir -p output/cmb_smhtt/cmb/125
mkdir -p output/cmb_smhtt/cmb/common

cp -p output/${ERA}_smhtt/cmb/125/*txt  output/cmb_smhtt/cmb/125/
cp -p output/${ERA}_smhtt/cmb/common/*  output/cmb_smhtt/cmb/common/
