#!bin/bash

# Script which executes a gof test for a given variable $var

# Pleas specify the following input variables
ERA=2016
DATACARD=${ERA}_workspace.root
SEED=1234
MASS=125
NUM_TOYS=300
VAR="pt_2"
BASE_PATH=/nfs/dust/cms/user/tlenz/13TeV/2017/SM_HTauTau/HTauTau_emu/DNN/GoF/output/
INPUT_FOLDER=var_1d
OUTPUT_FOLDER=2016_smhtt

cd ${CMSSW_BASE}/src/CombineHarvester/HTTSM2017

# Produce the datacard for the em channel (please add the em channel in category gof in the morphing script)
MorphingSM2017 --base_path=$BASE_PATH  --input_folder_em=$INPUT_FOLDER --real_data=true --jetfakes=0 --embedding=0 --postfix="-$VAR" --channel="em" --auto_rebin=true --stxs_signals="stxs_stage0" --categories="gof" --gof_category_name="em_inclusive" --era=2016 --output=$OUTPUT_FOLDER --regional_jec=false --ggh_wg1=false

# Create workspace
combineTool.py -M T2W -o workspace.root -m $MASS -i ${CMSSW_BASE}/src/CombineHarvester/HTTSM2017/output/2016_smhtt/cmb/125/

# Rename workspace
workspace_location=${CMSSW_BASE}/src/CombineHarvester/HTTSM2017/output/${OUTPUT_FOLDER}/cmb/125/
cp ${workspace_location}/workspace.root ${workspace_location}/2016_workspace.root

cd ${workspace_location}

# Get test statistic value
combine -M GoodnessOfFit --algo=saturated -m $MASS -d $DATACARD

# Throw toys
combine -M GoodnessOfFit --algo=saturated -m $MASS -d $DATACARD -s $SEED -t $NUM_TOYS

# Collect results
combineTool.py -M CollectGoodnessOfFit --input higgsCombineTest.GoodnessOfFit.mH$MASS.root higgsCombineTest.GoodnessOfFit.mH$MASS.$SEED.root --output gof-${VAR}.json

# Plot
plotGof.py --statistic saturated --mass $MASS.0 --output gof-${VAR} gof-${VAR}.json

# Get p-value printed out
echo ""
grep "p" *.json
echo ""

