
var=$1


if [ "$1" = "--help" ]
then
  echo -e "--------------------\nUsage: pass variable(s) to script, which will for each variable"
  echo -e  "*) write rootfiles of channels mt, et, tt to shapes/Vienna with Plotter.py"
  echo -e "*) run Fit all for channels mt, et, tt\n--------------------"
fi

#### PLOT PART
#eval `scram runtime -sh` # set cmsenv for Plotter
#for CHANNEL in tt
#do
#  echo -e "**********************************\n*   Plot Variable: $var   *\n**********************************"
#  echo -e "--------------------\nChannel:  ${CHANNEL}\n--------------------"
#  python produce.py $var -c ${CHANNEL} --syst --debug -o /afs/hephy.at/work/m/mspanring/CMSSW_7_4_7/src/CombineHarvester/HTTSM2017/shapes/Vienna/
#done


cd $CMSSW_BASE
cd /nfs/dust/cms/user/tlenz/13TeV/2017/CMSSW/CMSSW_7_4_7/src/CombineHarvester/HTTSM2016/
eval `scram runtime -sh`


rm -rf output/${var}
for CHANNEL in em; do

  # cd $CMSSW_BASE/src/CombineHarvester/HTTSM2017/output  
  # ./Fit datacard mlfit plot --postfit 1 --channel ${CHANNEL} --var ${var} --method def ff  --closure=true  --log=00  
  # cd $CMSSW_BASE/src/CombineHarvester/HTTSM2017

    MorphingSM2017 --input_folder_em=DESY/ --channel=${CHANNEL} --postfix="-${var}" --real_data=true --output_folder DESY

    ALGO=saturated
    combineTool.py -M GoodnessOfFit --algorithm $ALGO -m 125 --there -d output/${var}/${CHANNEL}/125/*.txt -n ".$ALGO" 
    combineTool.py -M GoodnessOfFit --algorithm $ALGO -m 125 --there -d output/${var}/${CHANNEL}/125/*.txt -n ".$ALGO.toys" -t 50 -s 0:7:1 --parallel 8 --verbose 0


    combineTool.py -M CollectGoodnessOfFit --input output/${var}/${CHANNEL}/125/higgsCombine.saturated.GoodnessOfFit.mH125.root output/${var}/${CHANNEL}/125/higgsCombine.saturated.toys.GoodnessOfFit.mH125.*.root -o ${goffolder}/htt_${CHANNEL}_${var}_saturated.json
    echo $v, $(cat ${goffolder}/htt_${CHANNEL}_${var}_saturated.json | grep p)
    echo $v, $(cat ${goffolder}/htt_${CHANNEL}_${var}_saturated.json | grep p) >> ${goffolder}/${CHANNEL}_${var}_pvalue.txt
    python ../CombineTools/scripts/plotGof.py --statistic ${ALGO} --mass 125.0 ${goffolder}/htt_${CHANNEL}_${var}_saturated.json --title-right="41.86 fb^{-1} (13 TeV)" --output="${goffolder}/${CHANNEL}_${var}-Saturated" --title-left=${texname}
done


# For 2016 something like this should work
MorphingSM2016 --output_folder=test --postfix="-2D" --control_region=1 --manual_rebin=false --real_data=false --mm_fit=false --ttbar_fit=true