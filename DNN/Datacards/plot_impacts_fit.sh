#!bin/bash

OUTPUT_FOLDER=2016_smhtt
CMSSW_LOCATION=/nfs/dust/cms/user/mameyer/SM_HiggsTauTau/CombineHarvester/2017/CMSSW_7_4_7/src/
workspace_location=${CMSSW_LOCATION}/CombineHarvester/HTTSM2017/output/${OUTPUT_FOLDER}/cmb/125/
CombineHarvesterLocation=${CMSSW_LOCATION}/CombineHarvester/HTTSM2017/
CURRENT_PATH=$(pwd)


cd ${CMSSW_LOCATION}
source /cvmfs/cms.cern.ch/cmsset_default.sh
cmsenv


cd $CombineHarvesterLocation
combineTool.py -M Impacts -m 125 -d ${workspace_location}/${ERA}_workspace.root --doInitialFit --robustFit 1 -t -1 --expectSignal=1 --parallel 20 --minimizerAlgoForMinos=Minuit2,Migrad
combineTool.py -M Impacts -m 125 -d ${workspace_location}/${ERA}_workspace.root --doFits --parallel 20 --robustFit 1 -t -1 --expectSignal=1 --minimizerAlgoForMinos=Minuit2,Migrad # --X-rtd FITTER_NEW_CROSSING_ALGO --X-rtd FITTER_NEVER_GIVE_UP # with this options it does not seem to find an end
combineTool.py -M Impacts -m 125 -d ${workspace_location}/${ERA}_workspace.root --output ${workspace_location}/${ERA}_impacts.json
plotImpacts.py -i ${workspace_location}/${ERA}_impacts.json -o ${ERA}_impacts
cd ${CURRENT_PATH}