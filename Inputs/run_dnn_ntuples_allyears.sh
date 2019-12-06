#! bin/bash

sed -i 's/void create_dnn_ntuples( TString era = "2017" )/void create_dnn_ntuples( TString era = "2016" )/g' create_dnn_ntuples.cpp

root -l -q -b create_dnn_ntuples.cpp+

sed -i 's/void create_dnn_ntuples( TString era = "2016" )/void create_dnn_ntuples( TString era = "2017" )/g' create_dnn_ntuples.cpp


root -l -q -b create_dnn_ntuples.cpp+

sed -i 's/void create_dnn_ntuples( TString era = "2017" )/void create_dnn_ntuples( TString era = "2018" )/g' create_dnn_ntuples.cpp

root -l -q -b create_dnn_ntuples.cpp+

sed -i 's/void create_dnn_ntuples( TString era = "2018" )/void create_dnn_ntuples( TString era = "2017" )/g' create_dnn_ntuples.cpp