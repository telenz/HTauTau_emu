import os
from glob import glob

input_dir = "output/var_1d_correct_qcd_estimation"

files_list = glob(os.path.join(input_dir, '*.json'))
print files_list
for filename in sorted(files_list):

    var = filename.split("gof-")[1]
    var = var.split(".json")[0]

    f = open(filename , 'r')
    lines=f.readlines()
    for line in lines:     
        if not '\"p\"' in line:
            continue
        pvalue = line
        pvalue = pvalue.split(": ")[1]
        pvalue = pvalue.split(",")[0]
        pvalue = pvalue[0:4]
        print ('{:<10} {:<10}'.format(var,pvalue))
        
    
