import os

input_dir_2016 = "./NTuples_2016/"
input_dir_2017 = "./NTuples_2017/"
input_dir_2018 = "./NTuples_2018/"

output_dir = "NTuples_Run2"

processes = ['MuonEG','Embedded','DYJets','WJets','TTbar','SingleTop','Diboson','ggH','VBFH','ZH','WH','ggHWW','VBFHWW','ttH']

cmd = "mkdir "+ output_dir
os.system(cmd)

for process in processes :
    cmd = "hadd -f "+output_dir+"/em-NOMINAL_ntuple_"+process+".root "+input_dir_2016+"em-NOMINAL_ntuple_"+process+".root " + input_dir_2017+"em-NOMINAL_ntuple_"+process+".root " + input_dir_2018+"em-NOMINAL_ntuple_"+process+".root"
    os.system(cmd) 