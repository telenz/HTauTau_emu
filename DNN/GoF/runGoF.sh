#!bin/bash

# this script executes a gof test for a given variable $var
ERA=2016
DATACARD=${ERA}_workspace.root
SEED=1234
MASS=125
NUM_TOYS=300
var="m_vis"

# Produce the datacard for the em channel (please add the em channel in category gof in the morphing script)
MorphingSM2017 --base_path=/nfs/dust/cms/user/tlenz/13TeV/2017/SM_HTauTau/HTauTau_emu/DNN/GoF/  --input_folder_em=output --real_data=true --jetfakes=0 --embedding=0 --postfix="-m_vis" --channel="em" --auto_rebin=true --stxs_signals="stxs_stage0" --categories="gof" --gof_category_name="em_inclusive" --era=2016 --output="2016_smhtt" --regional_jec=false --ggh_wg1=false

# Create workspace
combineTool.py -M T2W -o workspace.root -i ${CMSSW_BASE}/src/CombineHarvester/HTTSM2017/output/2016_smhtt/cmb/125/

# Rename workspace
cp ${CMSSW_BASE}/src/CombineHarvester/HTTSM2017/output/2016_smhtt/cmb/125/workspace.root ${CMSSW_BASE}/src/CombineHarvester/HTTSM2017/output/2016_smhtt/cmb/125/2016_workspace.root

cd ${CMSSW_BASE}/src/CombineHarvester/HTTSM2017/output/2016_smhtt/cmb/125/ 

# Get test statistic value
combine -M GoodnessOfFit --algo=saturated -m $MASS -d $DATACARD

# Throw toys
combine -M GoodnessOfFit --algo=saturated -m $MASS -d $DATACARD -s $SEED -t $NUM_TOYS

# Collect results
combineTool.py -M CollectGoodnessOfFit --input higgsCombineTest.GoodnessOfFit.mH$MASS.root higgsCombineTest.GoodnessOfFit.mH$MASS.$SEED.root --output gof.json

# Plot
plotGof.py --statistic saturated --mass $MASS.0 --output gof gof.json

