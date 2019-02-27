CMSSW_LOCATION=/nfs/dust/cms/user/mameyer/SM_HiggsTauTau/CombineForUnbliding/CMSSW_8_1_0/src
#CMSSW_LOCATION=/nfs/dust/cms/user/tlenz/13TeV/2017/CMSSW/CMSSW_8_1_0/src
DATACARD_PATH=${CMSSW_LOCATION}/CombineHarvester/HTTSM2017/output/${ERA}_smhtt/cmb/125/
BASE_PATH=/nfs/dust/cms/user/mameyer/SM_HiggsTauTau/HTauTau_emu/DNN/Datacards/
#BASE_PATH=/nfs/dust/cms/user/tlenz/13TeV/2017/SM_HTauTau/HTauTau_emu/DNN/Datacards/

export SCRAM_ARCH=slc6_amd64_gcc530
cd ${CMSSW_LOCATION}
source /cvmfs/cms.cern.ch/cmsset_default.sh
cmsenv

cd ${CMSSW_BASE}/src/CombineHarvester/HTTSM2017/

PostFitShapesFromWorkspace -m 125 -w ${DATACARD_PATH}/${ERA}_workspace.root \
    -d output/${ERA}_smhtt/cmb/125/combined.txt.cmb -o ${ERA}_datacard_shapes_prefit.root

PostFitShapesFromWorkspace -m 125 -w ${DATACARD_PATH}/${ERA}_workspace.root \
    -d output/${ERA}_smhtt/cmb/125/combined.txt.cmb -o ${ERA}_datacard_shapes_postfit_sb.root \
    -f fitDiagnostics${ERA}.root:fit_s --postfit

cd ${CURRENT_PATH}

cp ${CMSSW_BASE}/src/CombineHarvester/HTTSM2017/${ERA}_datacard_shapes_postfit_sb.root ${BASE_PATH}/output/${ERA}
cp ${CMSSW_BASE}/src/CombineHarvester/HTTSM2017/${ERA}_datacard_shapes_prefit.root ${BASE_PATH}/output/${ERA}

