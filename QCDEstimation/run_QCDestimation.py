import os
print "Deleting old weight file ..."
cmd="rm output/QCDweights.root"
os.system(cmd)
print "Determining OS/SS transfer factors as function of deltaR(e,mu) in bins of different jet multiplicities ..."
cmd="python QCDWeights.py"
os.system(cmd)
print "Determining non-closure corrections..."
cmd="python NonClosureCorrection.py"
os.system(cmd)
print "Determining isolation corrections..."
cmd="python IsolationCorrection.py"
os.system(cmd)
