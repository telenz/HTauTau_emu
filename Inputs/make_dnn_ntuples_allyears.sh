#! bin/bash

root -b -l <<EOF
.L create_dnn_ntuples.cpp+
EOF

root -l -b -q create_dnn_ntuples.cpp"(\"2016\")" &> out_2016.txt &
root -l -b -q create_dnn_ntuples.cpp"(\"2017\")"&> out_2017.txt &
root -l -b -q create_dnn_ntuples.cpp"(\"2018\")"&> out_2018.txt &