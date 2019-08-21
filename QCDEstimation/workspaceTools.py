#!/usr/bin/env python
import ROOT
from array import array
import math
import numpy as np

# Boilerplate
ROOT.PyConfig.IgnoreCommandLineOptions = True
ROOT.gROOT.SetBatch(ROOT.kTRUE)
ROOT.RooWorkspace.imp = getattr(ROOT.RooWorkspace, 'import')
ROOT.TH1.AddDirectory(0)


def TGraphAsymmErrorsToTH1D(graph):
    nbins = graph.GetN()
    bin_edges = []
    for i in xrange(0, nbins):
        bin_edges.append(graph.GetX()[i]-graph.GetEXlow()[i])
    bin_edges.append(graph.GetX()[i]+graph.GetEXhigh()[nbins-1])
    hist = ROOT.TH1D(graph.GetName(), graph.GetTitle(), nbins, array('d', bin_edges))
    for i in xrange(1, nbins+1):
        hist.SetBinContent(i, graph.GetY()[i-1])
        hist.SetBinError(i, (graph.GetEYhigh()[i-1] + graph.GetEYlow()[i-1]) / 2.)
    return hist

def UncertsFromGraphs(graph_data,graph_mc):
    nbins = graph_data.GetN()
    bin_edges = []
    for i in xrange(0, nbins):
        bin_edges.append(graph_data.GetX()[i]-graph_data.GetEXlow()[i])
    bin_edges.append(graph_data.GetX()[i]+graph_data.GetEXhigh()[nbins-1])
    hist_up = ROOT.TH1D(graph_data.GetName()+'_up', graph_data.GetTitle(), nbins, array('d', bin_edges))
    hist_down = ROOT.TH1D(graph_data.GetName()+'_down', graph_data.GetTitle(), nbins, array('d', bin_edges))
    for i in xrange(1, nbins+1):
        content_data=graph_data.GetY()[i-1]
        content_data_up=graph_data.GetEYhigh()[i-1]
        content_data_down=graph_data.GetEYlow()[i-1]

        content_mc=graph_mc.GetY()[i-1]
        content_mc_up=graph_mc.GetEYhigh()[i-1]
        content_mc_down=graph_mc.GetEYlow()[i-1]
        # assume data and MC uncertainties are uncorrelated, data up-shift shifts SF up whereas data down-shift shifts SF up and vise versa
        up = (1.+math.sqrt((content_data_up/content_data)**2 + (content_mc_down/content_mc)**2))
        down = (1.-math.sqrt((content_data_down/content_data)**2 + (content_mc_up/content_mc)**2))
        hist_up.SetBinContent(i,up)
        hist_down.SetBinContent(i,down)
    return (hist_up,hist_down)

# Special version of the above function that infers the binning without using EXlow/high
# Instead relies on the assumption that first bin starts at zero
def TGraphAsymmErrorsToTH1DForTaus(graph):
    nbins = graph.GetN()
    bin_edges = []
    bin_edges.append(0.)
    for i in xrange(0, nbins):
        bin_edges.append(graph.GetX()[i]+(graph.GetX()[i] - bin_edges[-1]))
    print bin_edges
    hist = ROOT.TH1D(graph.GetName(), graph.GetTitle(), nbins, array('d', bin_edges))
    last_non_zero = 0.0
    for i in xrange(1, nbins+1):
        hist.SetBinContent(i, graph.GetY()[i-1])
        hist.SetBinError(i, (graph.GetEYhigh()[i-1] + graph.GetEYlow()[i-1]) / 2.)
        if graph.GetY()[i-1] != 0.:
            last_non_zero = graph.GetY()[i-1]
    # print 'Before fix'
    # hist.Print('range')
    for i in reversed(xrange(1, nbins+1)):
        if hist.GetBinContent(i) == 0.:
            hist.SetBinContent(i, last_non_zero)
        else:
            break
    # print 'After fix'
    # hist.Print('range')
    return hist

def Convert2DOverflows(h):

   nx = h.GetNbinsX()+1
   xbins=[]
   for i in range(1,nx+1):
     xbins.append(h.GetXaxis().GetBinLowEdge(i))
   xbins.append(xbins[nx-1]+h.GetXaxis().GetBinWidth(nx))

   ny = h.GetNbinsY()+1
   ybins=[]
   for i in range(1,ny+1):
     ybins.append(h.GetYaxis().GetBinLowEdge(i))
   ybins.append(ybins[ny-1]+h.GetYaxis().GetBinWidth(ny))

   name=h.GetName()
   htmp = ROOT.TH2F(name, h.GetTitle(), nx, np.asarray(xbins), ny, np.asarray(ybins))
   htmp.SetXTitle(h.GetXaxis().GetTitle())
   htmp.SetYTitle(h.GetYaxis().GetTitle())
   for i in range(1,nx+1):
     for j in range(1,ny+1):
       htmp.Fill(htmp.GetXaxis().GetBinCenter(i),htmp.GetYaxis().GetBinCenter(j), h.GetBinContent(i,j))
   htmp.SetEntries(h.GetEntries())
   return htmp


def SafeWrapHist(wsp, binvars, hist, name=None, bound=True, incOF=False):
    # Use the histogram name for this function unless a new name has
    # been specified

    if len(binvars) == 2 and incOF: hist=Convert2DOverflows(hist)

    if name is None:
        name = hist.GetName()
    # Bit of technical RooFit thing:
    # We want to use two sets of x and y variables. The first set will be
    # named specifically for the histogram in question. When we create the
    # RooDataHist RooFit will adjust the ranges and binning of these
    # RooRealVars to match the input histogram. We'll store these in
    # 'h_arglist'. The second set will contain the variables that are actually
    # used for looking up values in the RooHistFunc that will wrap around the
    # RooDataHist. These variables can be used for different RooHistFuncs with
    # different binnings and/or ranges in each, and could even be defined as
    # functions of other variables. We'll store these in 'f_arglist'.
    h_arglist = ROOT.RooArgList()
    f_arglist = ROOT.RooArgList()
    # Keep track of the relevant histogram axes (for the 1D, 2D or 3D cases)
    axes = [hist.GetXaxis()]

    if len(binvars) >= 2:
        axes.append(hist.GetYaxis())
    if len(binvars) >= 3:
        axes.append(hist.GetZaxis())
    for i, var in enumerate(binvars):
        # If the axis variable doesn't exist in the workspace already we need
        # to create it
        if not wsp.arg(var):
            # Check if the user has defined a function here
            if var.startswith('expr::'):
                funcarg = wsp.factory(var)
                var = funcarg.GetName()
            # otherwise create a normal variable
            else:
                wsp.factory('%s[0]' % var)
        # If the bound option is set we first pass the axis variables through
        # a function that adjusts the value to the xaxis minimum or maximum if
        # the user has set a value outside the axis range
        if bound:
            f_arglist.add(wsp.factory('expr::%s_range_%s("TMath::Range(%f,%f,@0)", %s)' % (
                name, var,
                axes[i].GetBinLowEdge(1),
                axes[i].GetBinUpEdge(axes[i].GetNbins()),
                var)))
        else:
            f_arglist.add(wsp.arg(var))
        # Now create the histogram-specific binning variable
        h_arglist.add(wsp.factory('%s_binningvar_%s[0]' % (name, var)))
    # First create the RooDataHist
    rdh = ROOT.RooDataHist(name, hist.GetTitle(), h_arglist, hist)
    # Then the RooHistFunc - it will create a 1->1 correspondence between the lookup variables
    # (f_arglist) and the variables that defined the histogram binning (h_arglist)
    rhf = ROOT.RooHistFunc(name, hist.GetTitle(), f_arglist, h_arglist, rdh)
    # Finally we can import it into the workspace
    wsp.imp(rhf)
    wsp.imp(hist.Clone('hist_'+name))


def MakeBinnedCategory(wsp, name, bins):
    var = wsp.factory('%s[%g,%g,%g]' % (name, bins[0], bins[0], bins[-1]))
    var.setBinning(ROOT.RooBinning(len(bins)-1, array('d', bins)))
    cat = wsp.factory('RooBinningCategory::%s_cat(%s)' % (name, name))
    return cat


def MakeBinnedCategoryFuncMap(wsp, name, bins, funcName, funcs):
    MakeBinnedCategory(wsp, name, bins)
    expr = []
    for i in xrange(len(bins)-1):
        expr.append('(@0==%i)*@%i' % (i, i+1))
    fn = wsp.factory('expr::%s("%s",%s_cat,%s)' % (funcName, '+'.join(expr), name, ','.join(funcs)))
    return fn


def ProcessDESYLeptonSFs(filename, postfix, name):
    f = ROOT.TFile(filename)
    etaBinsH = f.Get('etaBinsH')
    eta_bins = set()
    pt_bins = set()
    graphs = []
    for i in xrange(1, etaBinsH.GetNbinsX()+1):
        label = etaBinsH.GetXaxis().GetBinLabel(i)

        graph = f.Get('ZMass%s_%s' % (label, postfix))
        gr_bins = set()
        for j in xrange(0, graph.GetN()):
            gr_bins.add(graph.GetX()[j]-graph.GetEXlow()[j])
            gr_bins.add(graph.GetX()[j]+graph.GetEXhigh()[j])
        print 'Graph: %s, bins: %s' % (graph.GetName(), len(gr_bins))
        graph.Print()
        pt_bins.update(gr_bins)
        graphs.append(graph)

        eta_bins.add(etaBinsH.GetXaxis().GetBinLowEdge(i))
        eta_bins.add(etaBinsH.GetXaxis().GetBinUpEdge(i))
    result = ROOT.TH2D(name, name,
                       len(pt_bins)-1, array('d', sorted(pt_bins)),
                       len(eta_bins)-1, array('d', sorted(eta_bins)))
    for i in xrange(1, len(eta_bins)):
        for j in xrange(1, len(pt_bins)):
                result.SetBinContent(j, i, graphs[i-1].GetY()[j-1])
                result.SetBinError(j, i, (graphs[i-1].GetEYhigh()[j-1] + graphs[i-1].GetEYlow()[j-1]) / 2.)
    result.Print('range')
    return result



# file = ROOT.TFile('input/scale_factors/Muon_SF_spring16temp.root')
# hists = []
# hists.append(file.Get('Mu22_Data_Eff'))
# hists.append(file.Get('Mu19_Data_Eff'))
# file.Close()


# ws = ROOT.RooWorkspace('ws')

# for hist in hists:
#     # Here we are saying that the x-axis lookup is done by taking the the abs
#     # value of m_eta first, and the y-axis lookup is just done directly with
#     # the m_pt variable
#     SafeWrapHist(ws, ['expr::m_abs_eta("TMath::Abs(@0)",m_eta[0])', 'm_pt'], hist)


# ws.Print()
# ws.writeToFile('input/scale_factors/Muon_SF_spring16temp_workspace.root')
# # I don't know why I need to delete this but it segfaults if I don't
# ws.Delete()
