import ROOT as rt

def openTree(filename,treename):
    f1 = rt.TFile(filename)
    tree = f1.Get(treename)
    return (f1,tree)

def createH1(xaxisname , nbins, xmin, xmax):
    h1 = rt.TH1D("h_"+xaxisname,"h_"+xaxisname, nbins , xmin, xmax)
    h1.SetLineColor(1)
    h1.SetLineWidth(3)
    h1.GetYaxis().SetTitleSize(20)
    h1.GetYaxis().SetTitleFont(43)
    h1.GetYaxis().SetTitleOffset(1.55)
    h1.GetXaxis().SetTitle(xaxisname)
    h1.SetTitle("")
    h1.SetStats(0)
    return h1


def createCanvas() :
    c = rt.TCanvas("c", "canvas", 800, 800)
    return c

def hist2Lists(hist) :

    list_x=[]
    list_y=[]
    list_yerr=[]

    for bin in range(1,hist.GetNbinsX()) : 
        list_x.append(hist.GetBinCenter(bin))
        list_y.append(hist.GetBinContent(bin))
        list_yerr.append(hist.GetBinError(bin))

    return list_x , list_y , list_yerr

def InitHist(hist, color) :
    hist.SetFillColor(color);
    hist.SetFillStyle(1001);
    hist.SetLineColor(1);
    hist.SetLineStyle(1);
    hist.SetLineWidth(2);
    hist.SetMarkerColor(1);
    hist.SetMarkerStyle(21);
    hist.SetMarkerSize(0.0);
    # hist.SetStats(rt.kFalse);
    return hist
