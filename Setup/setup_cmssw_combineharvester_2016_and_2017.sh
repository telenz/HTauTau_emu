# The following lines will set up a CMSSW release with CombineHarvester with the SM2016-dev branch (taken mostly from : https://github.com/cms-analysis/CombineHarvester/blob/SM2016-dev/HTTSM2016/Protocol_SM_Htt_2016.md)

export SCRAM_ARCH=slc6_amd64_gcc481
scram project CMSSW CMSSW_7_4_7
cd CMSSW_7_4_7/src
cmsenv
git clone git@github.com:/cms-analysis/HiggsAnalysis-CombinedLimit.git HiggsAnalysis/CombinedLimit
cd HiggsAnalysis/CombinedLimit
git fetch origin
git checkout v6.3.1
cd ../..
git clone git@github.com:/cms-analysis/CombineHarvester.git CombineHarvester
cd CombineHarvester
git checkout SM2016-dev
scram b -j 8
scram b -j 8

# The following lines will set up a CMSSW release with CombineHarvester with the SM2017-dev branch (taken from : https://github.com/cms-analysis/CombineHarvester/blob/SMHTT2017-dev/HTTSM2017/README.md)

export SCRAM_ARCH=slc6_amd64_gcc481 #bash, for tcsh: setenv SCRAM_ARCH slc6_amd64_gcc481
scram project CMSSW CMSSW_7_4_7
cd CMSSW_7_4_7/src
cmsenv
git clone git@github.com:/cms-analysis/HiggsAnalysis-CombinedLimit.git HiggsAnalysis/CombinedLimit
cd HiggsAnalysis/CombinedLimit
git fetch origin
git checkout v6.3.2
cd ../..
git clone git@github.com:/cms-analysis/CombineHarvester.git CombineHarvester -b SMHTT2017-dev
scram b -j 8