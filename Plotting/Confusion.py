import os
import ROOT as R
import numpy as np
import matplotlib.pyplot as plt
import argparse
import yaml

def main(): 

    parser = argparse.ArgumentParser()
    parser.add_argument('-e', dest='era', help='ERA' ,choices = ['2016','2017'], default = '2016')
    parser.add_argument('-emb', dest='embedded', help='embedded samples used' , action='store_true')
    args = parser.parse_args()

    era = args.era
    embedded = args.embedded

    filepath = "/".join(["output/{0}/htt_em.inputs-sm-Run{0}-ML.root".format(era)])
    print filepath
    if not os.path.exists(filepath):
        print "file not found!"
        return 
    print "Loading datacard: " + filepath
    rootfile=R.TFile(filepath, "READ")

    classes = [TDir.GetName() for TDir in rootfile.GetListOfKeys() if not "ggh_" in TDir.GetName() and not "qqh_" in TDir.GetName()]
    confusion =  np.zeros( (len(classes),len(classes)) )
    tempdict = {}
    print "\nFollowing categories are considered:"
    for cl in classes:
        folder = rootfile.Get( cl )
        print "-" + cl
        hists = [ hist.GetName() for hist in folder.GetListOfKeys() if not "Up" in hist.GetName() and not "Down" in hist.GetName() and not "ggH_" in hist.GetName() and not "qqH_" in hist.GetName()  and not "HWW" in hist.GetName() and not "ZH" in hist.GetName() and not "WH" in hist.GetName() and not "ttH" in hist.GetName() and not "data" in hist.GetName()  ]
        for hist in hists:
            tempdict.update({hist:folder.Get(hist).Integral()})

        confusion[classes.index(cl)] = compressDict(tempdict,classes,'em',embedded)


    # plotting confusion matrices
    print "Writing confusion matrices to output/"+era+"/figures/"
    plot_confusion(confusion,classes,"output/"+era+"/figures/em_confusion.png", "std")

    conf_pur1, conf_pur2 = get_purity_representations(confusion)
    plot_confusion(conf_pur1, classes, "output/"+era+"/figures/em_confusion_pur1.png")
    plot_confusion(conf_pur2, classes, "output/"+era+"/figures/em_confusion_pur2.png", "pur")

    conf_eff1, conf_eff2 = get_efficiency_representations(confusion)
    plot_confusion(conf_eff1, classes, "output/"+era+"/figures/em_confusion_eff1.png")
    plot_confusion(conf_eff2, classes, "output/"+era+"/figures/em_confusion_eff2.png", "eff")

    names = ["em_confusion_pur1","em_confusion_pur2","em_confusion_eff1","em_confusion_eff2"]
    i=0
    if not os.path.isdir("output/"+era+"/yamlfiles/"):
        os.mkdir("output/"+era+"/yamlfiles/")
    for confmatrix in conf_pur1,conf_pur2,conf_eff1,conf_eff2:
        d = {}
        for i1, c1 in enumerate(classes):
          d[c1] = {}
          for i2, c2 in enumerate(classes):
                d[c1][c2] = float(confmatrix[i2, i1])
          f = open("output/"+era+"/yamlfiles/"+names[i]+".yaml", "w")
          yaml.dump(d, f)
        i=i+1

    rootfile.Close()


def compressDict(tempdict,classes,channel,embedded):

    tmp = np.zeros(len(classes))
    tmp[classes.index('{0}_tt'.format(channel))]=tempdict['TTT']+tempdict['TTL']
    tmp[classes.index('{0}_st'.format(channel))]=tempdict['ST']
    tmp[classes.index('{0}_misc'.format(channel))]=tempdict['W']+tempdict['ZL']
    tmp[classes.index('{0}_qqh'.format(channel))]=tempdict['qqH125']
    tmp[classes.index('{0}_ss'.format(channel))]=tempdict['QCD']
    if embedded == True :
        tmp[classes.index('{0}_ztt'.format(channel))]=tempdict['EMB']
    else :
        tmp[classes.index('{0}_ztt'.format(channel))]=tempdict['ZTT']
    tmp[classes.index('{0}_ggh'.format(channel))]=tempdict['ggH125']
    tmp[classes.index('{0}_db'.format(channel))]=tempdict['VVT']+tempdict['VVL']

    return tmp


def plot_confusion(confusion, classes, filename, form="arb", markup='{:.2f}'):
    plt.figure(figsize=(1.5 * confusion.shape[0], 1.0 * confusion.shape[1]))
    axis = plt.gca()
    for i in range(confusion.shape[0]):  #predicted class
        for j in range(confusion.shape[1]): #true class
            axis.text(
                i + 0.5,
                j + 0.5,
                markup.format(confusion[-1 - j, i]),
                ha='center',
                va='center')
    q = plt.pcolormesh(confusion[::-1], cmap='Wistia')
    cbar = plt.colorbar(q)
    if form == "std":
        cbar.set_label("Sum of event weights", rotation=270, labelpad=50)
    elif form == "pur":
        cbar.set_label("Purity", rotation=270, labelpad=50)
    elif form == "eff":
        cbar.set_label("Efficiency", rotation=270, labelpad=50)
    else:
        cbar.set_label("Arbitrary units", rotation=270, labelpad=50)
    plt.xticks(
        np.array(range(len(classes))) + 0.5, classes, rotation='vertical')
    plt.yticks(
        np.array(range(len(classes))) + 0.5,
        classes[::-1],
        rotation='horizontal')
    plt.xlim(0, len(classes))
    plt.ylim(0, len(classes))
    plt.ylabel('Predicted class')
    plt.xlabel('True class')
    plt.savefig(filename, bbox_inches='tight')


def get_purity_representations(m):
    ma = np.zeros(m.shape)
    mb = np.zeros(m.shape)
    for i in range(m.shape[0]):
        for j in range(m.shape[1]):
            if m[i,i] != 0:
                ma[i, j] = m[i, j] / m[i, i]
            mb[i, j] = m[i, j] / np.sum(m[i, :])
    return ma, mb


def get_efficiency_representations(m):
    ma = np.zeros(m.shape)
    mb = np.zeros(m.shape)
    for i in range(m.shape[0]):
        for j in range(m.shape[1]):
            if m[j,j] != 0:
                ma[i, j] = m[i, j] / m[j, j]
            mb[i, j] = m[i, j] / np.sum(m[:, j])
    return ma, mb


if __name__ == '__main__':
    main()
