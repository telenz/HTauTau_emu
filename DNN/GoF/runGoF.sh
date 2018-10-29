#!bin/bash
MorphingSM2017 --base_path=/nfs/dust/cms/user/mameyer/SM_HiggsTauTau/HTauTau_emu/DNN/GoF/output/  --real_data=true --jetfakes=0 --embedding=0 --postfix="-m_vis" --channel="em" --auto_rebin=true --stxs_signals="stxs_stage0" --categories="gof" --gof_category_name="em_inclusive" --era=2016 --output="2016_smhtt"


cd /nfs/dust/cms/user/mameyer/SM_HiggsTauTau/CombineHarvester/CMSSW_7_4_7/src/HiggsAnalysis/CombinedLimit/
make

cd /nfs/dust/cms/user/mameyer/SM_HiggsTauTau/CombineHarvester/CMSSW_7_4_7/src
scram b -j8

combineTool.py -M T2W -o workspace.root -i ${CMSSW_BASE}/src/CombineHarvester/HTTSM2017/bin/output/2016_smhtt/cmb/

cp ${CMSSW_BASE}/src/CombineHarvester/HTTSM2017/bin/output/2016_smhtt/cmb/workspace.root 2016_workspace.root

combine -M GoodnessOfFit --algo=saturated -m 125 -d 2016_workspace.root

ERA=$1
DATACARD=${ERA}_workspace.root
SEED=1234
MASS=125
NUM_TOYS=300

# Get test statistic value
combine -M GoodnessOfFit --algo=saturated -m $MASS -d $DATACARD

# Throw toys
combine -M GoodnessOfFit --algo=saturated -m $MASS -d $DATACARD -s $SEED -t $NUM_TOYS

# Collect results
combineTool.py -M CollectGoodnessOfFit --input higgsCombineTest.GoodnessOfFit.mH$MASS.root higgsCombineTest.GoodnessOfFit.mH$MASS.$SEED.root --output gof.json

# Plot
plotGof.py --statistic saturated --mass $MASS.0 --output gof gof.json

