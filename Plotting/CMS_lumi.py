import ROOT as rt

def draw(pad, year):
     
     lumi2016 = "2016, 35.9 fb^{-1}";
     lumi2017 = "2017, 41.5 fb^{-1}";
     lumi2018 = "2018, 59.7 fb^{-1}";
     
     pad.cd()
     
     if year is "2016":
        lumiText = lumi2016 + " (13 TeV)"
     elif year is "2017":
        lumiText = lumi2017 + " (13 TeV)"
     else:
        lumiText = lumi2018 + " (13 TeV)"
        
     t = pad.GetTopMargin()
     r = pad. GetRightMargin();
     lumiTextOffset = 0.1

     latex = rt.TLatex()
     latex.SetNDC();
     latex.SetTextAngle(0);
     latex.SetTextColor(rt.kBlack);    

     latex.SetTextFont(42);
     latex.SetTextAlign(31); 
     latex.SetTextSize(0.5*t);    
     latex.DrawLatex(1-r,1-t+lumiTextOffset*t,lumiText);     
        
     
     