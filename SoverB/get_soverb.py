#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import yaml
import ROOT
import numpy
import copy
from array import array
from multiprocessing import Pool

import argparse
import logging
logger = logging.getLogger()

def setup_logging(output_file, level=logging.DEBUG):
    logger.setLevel(level)
    formatter = logging.Formatter("%(name)s - %(levelname)s - %(message)s")

    handler = logging.StreamHandler()
    handler.setFormatter(formatter)
    logger.addHandler(handler)

    file_handler = logging.FileHandler(output_file, "w")
    file_handler.setFormatter(formatter)
    logger.addHandler(file_handler)

def parse_arguments():
    parser = argparse.ArgumentParser(
        description="Calculate fake factors and create friend trees.")
    parser.add_argument(
        "--directory",
        required=True,
        type=str,
        help="Directory with Artus outputs.")
    parser.add_argument(
        "--output-directory",
        required=True,
        type=str,
        help="Directory to write.")
    parser.add_argument(
        "--era", type=str, required=True, help="Experiment era.")
    parser.add_argument(
        "--num-threads",
        default=24,
        type=int,
        help="Number of threads to be used.")
    parser.add_argument(
        "--postfit-file",
        default=None,
        type=str,
        help=
        "File with combine postfit distributions."
    )
    return parser.parse_args()

def calculate_soverb(config):
   
    args = config[0]
    datafile = config[1]
  
    postfit_file = ROOT.TFile(args.postfit_file, "READ")
    category_map = {
        #~ "et": [1, 2, 12, 15, 11, 13, 14, 16], # be careful with ordering as NN categories are accessed via index!!!
        #~ "mt": [1, 2, 12, 15, 11, 13, 14, 16],
        "em": [1, 2, 12, 14, 18, 19, 13, 16], 
        #~ "tt": [1, 2, 12, 17, 16]
    }
    hists = {
        #~ "et" : [{}, {}, {}, {}, {}, {}, {}, {}],
        #~ "mt" : [{}, {}, {}, {}, {}, {}, {}, {}],
        "em" : [{}, {}, {}, {}, {}, {}, {}, {}]
        #~ "tt" : [{}, {}, {}, {}, {}]
        }
       
    for channel in category_map.keys():
        for i, nick in enumerate(category_map[channel]):
            histsig = postfit_file.Get("htt_{ch}_{cat}_Run{era}_prefit/ggH".format(ch=channel, cat=nick, era=args.era))
            histsig.Add(postfit_file.Get("htt_{ch}_{cat}_Run{era}_prefit/qqH".format(ch=channel, cat=nick, era=args.era)))
            
            hists[channel][i]["sig"] = histsig
            hists[channel][i]["bkg"] = postfit_file.Get("htt_{ch}_{cat}_Run{era}_prefit/TotalBkg".format(ch=channel, cat=nick, era=args.era)) #already contains signal in CMSSW_8 combine
    
    input_file = ROOT.TFile(os.path.join(args.directory, datafile), "READ")
   
    if not os.path.exists(os.path.join(args.output_directory, os.path.dirname(datafile))):
        os.mkdir(os.path.join(args.output_directory, os.path.dirname(datafile)))
    output_file = ROOT.TFile(
            os.path.join(args.output_directory, datafile), "RECREATE")
            
    output_file.cd()
    output_tree = ROOT.TTree("TauCheck", "TauCheck")
    s = numpy.zeros(1, dtype=float)
    b = numpy.zeros(1, dtype=float)
    soversb = numpy.zeros(1, dtype=float)
    output_tree.Branch("s", s, "s/D")
    output_tree.Branch("b", b, "b/D")
    output_tree.Branch("soversb", soversb, "soversb/D")
    
    input_tree = input_file.Get("TauCheck")
    
    for event in input_tree:
        cat = int(getattr(event, "predicted_class"))
        
        val = getattr(event, "predicted_prob")
        if cat == 0: #unrolled ggH histograms
            width = 0.8 if channel=="tt" else 0.875
            n_ptcats = 4 if (channel=="tt" or channel == "em") else 3
            if event.njets>=1:
                val+=width
                if event.njets>=2:
                    val+=width*n_ptcats
                if event.pt_tt>60:
                    val+=width
                if event.pt_tt>120:
                    val+=width
                if event.pt_tt>200 and (channel=="tt" or channel == "em"):
                    val+=width
        if cat == 1: #unrolled qqH histograms
            width = 0.8 if channel=="tt" else 0.875
            if event.jpt_1>200:
                val+=width*4
            else:
                if event.njets>=2 and event.mjj>400 and event.jdeta>2.8 and event.pt_ttjj>0 and event.pt_ttjj<25:
                    val+=0.0
                elif event.njets>=2 and event.mjj>400 and event.jdeta>2.8 and event.pt_ttjj>25:
                    val+=width
                elif event.njets>=2 and event.mjj>60 and event.mjj<120:
                    val+=width*2
                else:
                    val+=width*3
                    
        idx = hists[channel][cat]["sig"].GetXaxis().FindBin(val)
        sig = hists[channel][cat]["sig"].GetBinContent(idx)
        sb = hists[channel][cat]["bkg"].GetBinContent(idx) #already contains signal in CMSSW_8 combine
        s[0] = sig
        b[0] = sb - sig
        soversb[0] = 0.0 if sb == 0.0 else sig / sb
        output_tree.Fill()
    output_tree.Write()
                
    postfit_file.Close()
    input_file.Close()
    
if __name__ == "__main__":
    args = parse_arguments()
    setup_logging("{}_get_soverb.log".format(args.era),
                  logging.INFO)
    files = []
    
    for entry in os.listdir(args.directory):
        files.append(entry)
    
    #create output directory
    if os.path.exists(args.output_directory):
        logger.critical(
            "Output directory %s already exists. I don't want to overwrite it!"
            % args.output_directory)
        raise Exception
    else:
        os.mkdir(args.output_directory)
        logger.info("Createt output directory")

    logger.info("Create friend trees...")

    pool = Pool(processes=args.num_threads)
    pool.map(calculate_soverb, [[args, datafile]
                                  for datafile in files])
    pool.close()
    pool.join()
    del pool