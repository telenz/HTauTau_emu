ERA=$1
CHANNEL=$2

eval `scramv1 runtime -sh`
cd ${CMSSW_BASE}/src/CombineHarvester/HTTSM2017

rm -f ${CMSSW_BASE}/src/CombineHarvester/HTTSM2017/output/${ERA}_smhtt/cmb/125/${ERA}_workspace.root

combineTool.py -M T2W -o ${ERA}_workspace.root -i output/${ERA}_smhtt/cmb/125    \                                           --parallel 8  -m 125  \                                                                                     
combine -M Significance --X-rtd MINIMIZER_analytic --cminDefaultMinimizerStrategy 0 --significance -m 125 -n $ERA  output/${ERA}_smhtt/cmb/125/${ERA}_workspace.root

