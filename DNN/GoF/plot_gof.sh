#!/bin/bash

# $1 era
VARIABLES=variables.yaml


python plot_gof.py $VARIABLES 'em' ${1}

