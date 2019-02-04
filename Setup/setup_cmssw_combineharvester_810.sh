export SCRAM_ARCH=slc6_amd64_gcc530
export VO_CMS_SW_DIR=/cvmfs/cms.cern.ch
source $VO_CMS_SW_DIR/cmsset_default.sh

export SCRAM_ARCH=slc6_amd64_gcc530
scram project CMSSW CMSSW_8_1_0
cd CMSSW_8_1_0/src
cmsenv

git clone https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit.git HiggsAnalysis/CombinedLimit
cd HiggsAnalysis/CombinedLimit
git fetch origin
git checkout v7.0.12
cd ../..

git clone https://github.com/cms-analysis/CombineHarvester.git CombineHarvester
cd CombineHarvester
git checkout SMHTT2017-dev
cd -

cd CombineHarvester/HTTSM2017
git clone https://:@gitlab.cern.ch:8443/cms-htt/SM-ML-2017.git shapes
cd -

# Build
scram b -j 24
scram b python

