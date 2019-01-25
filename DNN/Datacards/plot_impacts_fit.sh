#!bin/bash
#ERA=2016
OUTPUT_FOLDER=${ERA}_smhtt
CMSSW_LOCATION=/nfs/dust/cms/user/mameyer/SM_HiggsTauTau/CombineHarvester/2017/CMSSW_8_1_0/src
workspace_location=${CMSSW_LOCATION}/CombineHarvester/HTTSM2017/output/${OUTPUT_FOLDER}/cmb/125/
CombineHarvesterLocation=${CMSSW_LOCATION}/CombineHarvester/HTTSM2017/
CURRENT_PATH=$(pwd)

export SCRAM_ARCH=slc6_amd64_gcc530
cd ${CMSSW_LOCATION}
source /cvmfs/cms.cern.ch/cmsset_default.sh
cmsenv


cd $CombineHarvesterLocation

python $CMSSW_BASE/src/HiggsAnalysis/CombinedLimit/test/diffNuisances.py -a \
    -f html fitDiagnostics${ERA}.root > ${ERA}_diff_nuisances.html

combineTool.py -M Impacts -m 125 -d ${workspace_location}/${ERA}_workspace.root --doInitialFit \
    --robustFit 1 -t -1 --expectSignal=1 --parallel 20

combineTool.py -M Impacts -m 125 -d ${workspace_location}/${ERA}_workspace.root --doFits \
    --parallel 20 --robustFit 1 -t -1 --expectSignal=1

combineTool.py -M Impacts -m 125 -d ${workspace_location}/${ERA}_workspace.root --output ${ERA}_impacts.json

plotImpacts.py -i ${ERA}_impacts.json -o ${ERA}_impacts

cd ${CURRENT_PATH}