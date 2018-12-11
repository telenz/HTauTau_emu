#!/bin/bash

# $1 era
VARIABLES=variables.yaml


python plot_gof.py $VARIABLES '/nfs/dust/cms/user/mameyer/SM_HiggsTauTau/HTauTau_emu/DNN/GoF/output/2017/var_2d' 'em' ${1}

