ERA=$1
CHANNEL=$2

eval `scramv1 runtime -sh`
cd ${CMSSW_BASE}/src/CombineHarvester/HTTSM2017

rm -f ${CMSSW_BASE}/src/CombineHarvester/HTTSM2017/output/${ERA}_smhtt/cmb/125/${ERA}_workspace.root

combineTool.py -M T2W -o ${ERA}_workspace.root -i output/${ERA}_smhtt/cmb/125  \
       --parallel 8 -m 125  \
       -P HiggsAnalysis.CombinedLimit.PhysicsModel:multiSignalModel  \
       --PO '"map=^.*/ggH.?$:r_ggH[1,-5,5]"'  \
       --PO '"map=^.*/qqH.?$:r_qqH[1,-5,5]"'

combineTool.py -M MultiDimFit -m 125 -d output/${ERA}_smhtt/cmb/125/${ERA}_workspace.root  \
       --algo singles  -t -1 \
       --expectSignal 1  \
       --robustFit 1 -n ${ERA}  \
       --X-rtd MINIMIZER_analytic --cminDefaultMinimizerStrategy 0


root -l fitDiagnosticscmb.root <<<"fit_b->Print()" | grep covaria