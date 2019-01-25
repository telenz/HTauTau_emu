#!/usr/bin/env python
# -*- coding: utf-8 -*-

import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True  # disable ROOT internal argument parser
import argparse
import numpy as np


def parse_arguments():
    parser = argparse.ArgumentParser(
        description="Fix binning of unrolled em shapes.")
    parser.add_argument("input", type=str, help="Path to input shapes.")
    parser.add_argument("output", type=str, help="Path to output shapes.")
    return parser.parse_args()


def main(args):
    f = ROOT.TFile(args.input, "READ")
    fout = ROOT.TFile(args.output, "RECREATE")
    for key in f.GetListOfKeys():
        dirname = key.GetName()
        d = f.Get(dirname)
        f.cd(dirname)
        fout.cd()
        fout.mkdir(dirname)
        fout.cd(dirname)
        for key2 in d.GetListOfKeys():
            name = key2.GetName()
            title = key2.GetName()
            h = d.Get(name)
            if not "unrolled" in dirname:
               h.Write(name)
               continue
            num_bins = h.GetNbinsX()
            x = np.array(range(10, 101, 5))/100.
            x[0] = 0.125
            bins = [x[0]]
            offset = x[-1]-x[0]
            if num_bins == 18*9: # ggh (without 101 and 102)
                num_unrolling = 9 # unrolling without 101 and 102
            elif num_bins == 18*5: # qqh
                num_unrolling = 5
            else:
                raise Exception("2D binning does not match ggh or qqh.")
            for i in range(num_unrolling):
                for j in range(1,len(x)):
                    bins.append(x[j]+offset*i)
            v = ROOT.std.vector("double")(len(bins))
            for i, c in enumerate(bins):
                v[i] = c
            hnew = ROOT.TH1D(name, title, len(bins)-1, v.data())
            for i in range(1, num_bins+1):
                hnew.SetBinContent(i, h.GetBinContent(i));
                hnew.SetBinError(i, h.GetBinError(i));
            hnew.SetEntries(h.GetEntries())
            hnew.Write()
    fout.Write()
    f.Close()


if __name__ == "__main__":
    args = parse_arguments()
    main(args)
