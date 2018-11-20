import os
import ROOT as R
import numpy as np
import matplotlib.pyplot as plt
import argparse

def main(): 

    parser = argparse.ArgumentParser()
    parser.add_argument('-m', '--model',   help='ML model to use' ,choices = ['keras','xgb'],  default = 'keras')
    parser.add_argument('-c', '--channel', help='Decay channel' ,choices = ['mt','et','tt', 'em'], default = 'mt')
    args = parser.parse_args()

    channel = args.channel
    model = args.model

    filepath = "/".join(["output/2016/htt_{0}.inputs-sm-13TeV-ML.root".format(channel)])
    print filepath
    if not os.path.exists(filepath):
        print "file not found!"
        return 
    print "Loading datacard: " + filepath
    rootfile=R.TFile(filepath, "READ")


    classes=[]
    for key in rootfile.GetListOfKeys():
    	classes.append(key.GetName())

    confusion =  np.zeros( (len(classes),len(classes)) )
    tempdict = {}

    TDirs = [TDir.GetName() for TDir in rootfile.GetListOfKeys()]

    for TDir in TDirs: 
        folder = rootfile.Get( TDir )
        hists = [ hist.GetName() for hist in folder.GetListOfKeys() if not "_13TeV" in hist.GetName()]
        for hist in hists: 
            tempdict.update({hist:folder.Get(hist).Integral()})

        confusion[TDirs.index(TDir)] = compressDict(tempdict,classes,channel)

    # plotting confusion matrices
    print "Writing confusion matrices to output/2016/figures/"
    plot_confusion(confusion,classes,"output/2016/figures/{0}_{1}_confusion.png".format(model,channel), "std")

    conf_pur1, conf_pur2 = get_purity_representations(confusion)
    plot_confusion(conf_pur1, classes, "output/2016/figures/{0}_{1}_confusion_pur1.png".format(model,channel))
    plot_confusion(conf_pur2, classes, "output/2016/figures/{0}_{1}_confusion_pur2.png".format(model,channel), "pur")

    conf_eff1, conf_eff2 = get_efficiency_representations(confusion)
    plot_confusion(conf_eff1, classes, "output/2016/figures/{0}_{1}_confusion_eff1.png".format(model,channel))
    plot_confusion(conf_eff2, classes, "output/2016/figures/{0}_{1}_confusion_eff2.png".format(model,channel), "eff")

    rootfile.Close()


def compressDict(tempdict,classes,channel):

    tmp = np.zeros(len(classes))
    tmp[classes.index('{0}_tt'.format(channel))]=tempdict['TT']
    tmp[classes.index('{0}_st'.format(channel))]=tempdict['ST']
    tmp[classes.index('{0}_misc'.format(channel))]=tempdict['W']+tempdict['ZL']
    tmp[classes.index('{0}_qqh'.format(channel))]=tempdict['qqH125']
    tmp[classes.index('{0}_ss'.format(channel))]=tempdict['QCD']
    tmp[classes.index('{0}_ztt'.format(channel))]=tempdict['ZTT']
    tmp[classes.index('{0}_ggh'.format(channel))]=tempdict['ggH125']
    tmp[classes.index('{0}_vv'.format(channel))]=tempdict['VV']

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
            ma[i, j] = m[i, j] / m[i, i]
            mb[i, j] = m[i, j] / np.sum(m[i, :])
    return ma, mb


def get_efficiency_representations(m):
    ma = np.zeros(m.shape)
    mb = np.zeros(m.shape)
    for i in range(m.shape[0]):
        for j in range(m.shape[1]):
            ma[i, j] = m[i, j] / m[j, j]
            mb[i, j] = m[i, j] / np.sum(m[:, j])
    return ma, mb


if __name__ == '__main__':
    main()