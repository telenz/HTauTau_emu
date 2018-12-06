#!/bin/bash

#$1 filelist
#$2 era

let "n = 0"
rm -rf $1_files
mkdir $1_files
cp HTC_submit.sh $1_files/
Splitter $1 1
cd $1_files/
for i in `ls -1v $1_*`
 do
    var="$( cat $i )"
    echo $var
    echo submitting job $n for file $i from list $1
    cat > $1_$n.sh <<EOF
source /cvmfs/cms.cern.ch/cmsset_default.sh
export SCRAM_ARCH=slc6_amd64_gcc630
cd ${CMSSW_BASE}/src
cmsenv
cd -
cd /nfs/dust/cms/user/tlenz/13TeV/2017/SM_HTauTau/HTauTau_emu/DNN/GoF/
python run_gof_tests_2d_for_htc.py -emb -e '$2' -var '$var'

EOF
    chmod u+x $1_$n.sh
    ./HTC_submit.sh $1_$n.sh $1_$n
    let "n = n + 1"
done
echo Total $n jobs submitted
cd ../

#run_gof_tests_2d.py -e '2016' -emb -var ${VAR}
