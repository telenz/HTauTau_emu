import ROOT as rt
import imp
wsptools = imp.load_source('wsptools', 'workspaceTools.py')
w = rt.RooWorkspace('w')

def GetFromTFile(str):
    f = ROOT.TFile(str.split(':')[0])
    obj = f.Get(str.split(':')[1]).Clone()
    f.Close()
    return obj

def main():

    loc = 'output/QCDweights.root'

    #FIXME NJET BINS? wsptools.SafeWrapHist(w, ['expr::m_pt_max100("min(10,100)")', 'expr::e_pt_max100("min(13,100)")'],  GetFromTFile(loc+':'), 'em_qcd_factors')
    wsptools.SafeWrapHist(w, ['expr::m_pt_max100("min(10,100)")', 'expr::e_pt_max100("min(13,100)")'],  GetFromTFile(loc+':NonClosureCorrection'), 'em_qcd_factors_bothaiso')
    wsptools.SafeWrapHist(w, ['expr::m_pt_max40("min(10,40)")','expr::e_pt_max40("min(13,40)")'],  GetFromTFile(loc+':IsolationCorrection'), 'em_qcd_extrap_uncert')

    w.factory('expr::em_qcd_osss_binned("((@0<0.15)*((@1==0)*(2.505-0.1545*@2) + (@1>0)*(2.896-0.3304*@2))*@3 + (@0>=0.15)*((@1==0)*(3.048-0.1726*@2) + (@1>0)*(3.398-0.3965*@2))*@4)*@5" ,iso[0],njets[0],dR[0],em_qcd_factors,em_qcd_factors_bothaiso, em_qcd_extrap_uncert)')

    w.factory('expr::em_qcd_osss_0jet_rateup("((@0<0.15)*((@1==0)*(2.660-0.1545*@2) + (@1>0)*(2.896-0.3304*@2))*@3 + (@0>=0.15)*((@1==0)*(3.173-0.1726*@2) + (@1>0)*(3.398-0.3965*@2))*@4)*@5" ,iso[0],njets[0],dR[0],em_qcd_factors,em_qcd_factors_bothaiso, em_qcd_extrap_uncert)')
    w.factory('expr::em_qcd_osss_0jet_ratedown("((@0<0.15)*((@1==0)*(2.350-0.1545*@2) + (@1>0)*(2.896-0.3304*@2))*@3 + (@0>=0.15)*((@1==0)*(2.923-0.1726*@2) + (@1>0)*(3.398-0.3965*@2))*@4)*@5" ,iso[0],njets[0],dR[0],em_qcd_factors,em_qcd_factors_bothaiso, em_qcd_extrap_uncert)')
    w.factory('expr::em_qcd_osss_0jet_shapeup  ("((@0<0.15)*((@1==0)*(2.505-0.1075*@2) + (@1>0)*(2.896-0.3304*@2))*@3 + (@0>=0.15)*((@1==0)*(3.048-0.2110*@2) + (@1>0)*(3.398-0.3965*@2))*@4)*@5" ,iso[0],njets[0],dR[0],em_qcd_factors,em_qcd_factors_bothaiso, em_qcd_extrap_uncert)')
    w.factory('expr::em_qcd_osss_0jet_shapedown("((@0<0.15)*((@1==0)*(2.505-0.2015*@2) + (@1>0)*(2.896-0.3304*@2))*@3 + (@0>=0.15)*((@1==0)*(3.048-0.2110*@2) + (@1>0)*(3.398-0.3965*@2))*@4)*@5" ,iso[0],njets[0],dR[0],em_qcd_factors,em_qcd_factors_bothaiso, em_qcd_extrap_uncert)')


    w.factory('expr::em_qcd_osss_1jet_rateup("((@0<0.15)*((@1==0)*(2.505-0.1545*@2) + (@1>0)*(2.978-0.3304*@2))*@3 + (@0>=0.15)*((@1==0)*(3.048-0.1726*@2) + (@1>0)*(3.459-0.3965*@2))*@4)*@5" ,iso[0],njets[0],dR[0],em_qcd_factors,em_qcd_factors_bothaiso, em_qcd_extrap_uncert)')
    w.factory('expr::em_qcd_osss_1jet_ratedown("((@0<0.15)*((@1==0)*(2.505-0.1545*@2) + (@1>0)*(2.814-0.3304*@2))*@3 + (@0>=0.15)*((@1==0)*(3.048-0.1726*@2) + (@1>0)*(3.337-0.3965*@2))*@4)*@5" ,iso[0],njets[0],dR[0],em_qcd_factors,em_qcd_factors_bothaiso, em_qcd_extrap_uncert)')
    w.factory('expr::em_qcd_osss_1jet_shapeup("((@0<0.15)*((@1==0)*(2.505-0.1545*@2) + (@1>0)*(2.896-0.3019*@2))*@3 + (@0>=0.15)*((@1==0)*(3.048-0.1726*@2) + (@1>0)*(3.398-0.3753*@2))*@4)*@5" ,iso[0],njets[0],dR[0],em_qcd_factors,em_qcd_factors_bothaiso, em_qcd_extrap_uncert)')
    w.factory('expr::em_qcd_osss_1jet_shapedown("((@0<0.15)*((@1==0)*(2.505-0.1545*@2) + (@1>0)*(2.896-0.3589*@2))*@3 + (@0>=0.15)*((@1==0)*(3.048-0.1726*@2) + (@1>0)*(3.398-0.4177*@2))*@4)*@5" ,iso[0],njets[0],dR[0],em_qcd_factors,em_qcd_factors_bothaiso, em_qcd_extrap_uncert)')


    wsptools.MakeBinnedCategoryFuncMap(w, 'njets', [0,1,10000],'em_qcd_osss_shapeup_binned', ['em_qcd_osss_0jet_shapeup','em_qcd_osss_1jet_shapeup'])

    wsptools.MakeBinnedCategoryFuncMap(w, 'njets', [0,1,10000],'em_qcd_osss_shapedown_binned', ['em_qcd_osss_0jet_shapedown','em_qcd_osss_1jet_shapedown'])

    wsptools.MakeBinnedCategoryFuncMap(w, 'njets', [0,1,10000],'em_qcd_osss_rateup_binned', ['em_qcd_osss_0jet_rateup','em_qcd_osss_1jet_rateup'])

    wsptools.MakeBinnedCategoryFuncMap(w, 'njets', [0,1,10000],'em_qcd_osss_ratedown_binned', ['em_qcd_osss_0jet_ratedown','em_qcd_osss_1jet_ratedown'])


    w.factory('expr::em_qcd_extrap_up("@0*@1",em_qcd_osss_binned,em_qcd_extrap_uncert)')
    w.factory('expr::em_qcd_extrap_down("@0/@1",em_qcd_osss_binned,em_qcd_extrap_uncert)')

if __name__ == '__main__' :
    main()
