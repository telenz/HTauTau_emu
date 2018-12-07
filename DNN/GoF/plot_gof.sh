#!/bin/bash

# $1 era
VARIABLES=variables.yaml


python plot_gof.py $VARIABLES '/nfs/dust/cms/user/tlenz/13TeV/2017/SM_HTauTau/HTauTau_emu/DNN/GoF/output/2017/var_2d' 'em' ${1}

