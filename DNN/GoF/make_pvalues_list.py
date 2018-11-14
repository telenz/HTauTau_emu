import os

input_dir = "output/var_1d/"


for filename in os.listdir(input_dir) :
    if not filename.endswith(".json") :
        continue
    var = filename.split("gof-")[1]
    var = var.split(".json")[0]
    f = open(input_dir + filename , 'r')
    lines=f.readlines()

    for line in lines:
        if not '\"p\"' in line:
            continue
        pvalue = line
        pvalue = pvalue.split(": ")[1]
        pvalue = pvalue.split(",")[0]
        pvalue = pvalue[0:4]
        print ('{:<10} {:<10}'.format(var,pvalue))
        
    
