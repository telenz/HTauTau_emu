# -*- coding: utf-8 -*-

import ROOT as r
import sys
import glob
import os

cut_dict = {
    "em_nominal" : "pt_1>13 && TMath::Max(pt_1,pt_2)>24 && dzeta>-35 && mTdileptonMET<60 && q_1*q_2 < 0 && nbtag == 0"
}

if __name__ == '__main__':
 
    check_samples = False

    f = sys.argv[1]
    output = f.split("/")[-1]
    rootf = r.TFile.Open("file://"+f,"read")
    rootout = r.TFile(str(output)+"new","recreate")
    for channel in cut_dict:
        rootout.mkdir(channel)
        rootout.cd(channel)
        oldtree = rootf.Get("TauCheck")
        newtree = oldtree.CopyTree(cut_dict[channel])
        print channel,": original - ",oldtree.GetEntries(),", postsync - ",newtree.GetEntries()
        newtree.Write("",r.TObject.kOverwrite)