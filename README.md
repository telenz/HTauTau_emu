## Framework to analyze 2016/2017 H->tautau emu-channel data

### 1.) Make small analysis ntuples (DNN ntuples)

The ntuples can be processed with Inputs/create_dnn_ntuples.cpp. Inputs (like xsecs and input ntuples) to this script are specified in settings_for_eras.h.

### 2.) Goodness-of-Fit tests

   1d GoF tests can be done with DNN/GoF/run_gof_tests.py. There the variable list is specified and the jobs are processed in parallel.
   For the 2d GoF tests use DNN/GoF/run_gof_tests_2d_for_htc.py. The jobs are submitted to the cluster with DNN/GoF/submit_gof.sh. This script takes as input a list with all variable sets, like DNN/GoF/gof_list_2016_2d and the era.

### 3.) Datacard

   The datacards are processed with DNN/Datacards/produce_category_plots_runDatacardparallel.py which requires the config name as argument.