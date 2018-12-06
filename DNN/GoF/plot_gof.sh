#!/bin/bash

# $1 era
VARIABLES=variables.yaml

python plot_gof.py $VARIABLES 'output/2016/var_2d' 'em' ${1}
