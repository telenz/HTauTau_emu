import os
from glob import glob
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('-e', dest='era', help='ERA' ,choices = ['2016','2017'], default = '2016')
args = parser.parse_args()

era= args.era

input_dir = "output/"+era+"/var_1d"

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
        print ('{:<10} : {:<10}'.format(var,pvalue))
        
    
