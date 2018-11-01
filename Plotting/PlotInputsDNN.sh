#!bin/bash

root -b -l  <<EOF 

.L Plot.C 

Plot("/nfs/dust/cms/user/mameyer/SM_HiggsTauTau/HTauTau_emu/Inputs/NTuples_2016/",
     "MuonEG_Run2016",
     "dzeta",
     "inputDNN",
      30,
      -45,
      105,
     "mcweight*puweight*effweight*0.978824*0.985*xsec_lumi_weight*",
     "&&iso_1<0.15&&iso_2<0.2&&extraelec_veto<0.5&&extramuon_veto<0.5&&pt_1>13&&pt_2>10&TMath::Max(pt_1,pt_2)>24&&metFilters>0.5&&mTdileptonMET<60&&dzeta>-35",
     "D_{#zeta} [GeV]",
     "Events / 5 GeV"
     )

Plot("/nfs/dust/cms/user/mameyer/SM_HiggsTauTau/HTauTau_emu/Inputs/NTuples_2016/","MuonEG_Run2016","m_sv","inputDNN",30,0,300,"mcweight*puweight*effweight*0.978824*0.985*xsec_lumi_weight*","&&iso_1<0.15&&iso_2<0.2&&extraelec_veto<0.5&&extramuon_veto<0.5&&pt_1>13&&pt_2>10&TMath::Max(pt_1,pt_2)>24&&metFilters>0.5&&mTdileptonMET<60&&dzeta>-35","m_{SV} [GeV]",
"Events / 10 GeV")

Plot("/nfs/dust/cms/user/mameyer/SM_HiggsTauTau/HTauTau_emu/Inputs/NTuples_2016/",
     "MuonEG_Run2016",
     "m_vis",
     "inputDNN",
      20,
      0,
      200,
     "mcweight*puweight*effweight*0.978824*0.985*xsec_lumi_weight*",
     "&&iso_1<0.15&&iso_2<0.2&&extraelec_veto<0.5&&extramuon_veto<0.5&&pt_1>13&&pt_2>10&TMath::Max(pt_1,pt_2)>24&&metFilters>0.5&&mTdileptonMET<60&&dzeta>-35",
     "m_{vis} [GeV]",
     "Events / 10 GeV"
     )

Plot("/nfs/dust/cms/user/mameyer/SM_HiggsTauTau/HTauTau_emu/Inputs/NTuples_2016/",
     "MuonEG_Run2016",
     "pt_1",
     "inputDNN",
      30,
      0,
      150,
     "mcweight*puweight*effweight*0.978824*0.985*xsec_lumi_weight*",
     "&&iso_1<0.15&&iso_2<0.2&&extraelec_veto<0.5&&extramuon_veto<0.5&&pt_1>13&&pt_2>10&TMath::Max(pt_1,pt_2)>24&&metFilters>0.5&&mTdileptonMET<60&&dzeta>-35",
     "electron p_{T} [GeV]",
     "Events / 5 GeV"
     )

Plot("/nfs/dust/cms/user/mameyer/SM_HiggsTauTau/HTauTau_emu/Inputs/NTuples_2016/",
     "MuonEG_Run2016",
     "pt_2",
     "inputDNN",
      30,
      0,
      150,
     "mcweight*puweight*effweight*0.978824*0.985*xsec_lumi_weight*",
     "&&iso_1<0.15&&iso_2<0.2&&extraelec_veto<0.5&&extramuon_veto<0.5&&pt_1>13&&pt_2>10&TMath::Max(pt_1,pt_2)>24&&metFilters>0.5&&mTdileptonMET<60&&dzeta>-35",
     "muon p_{T} [GeV]",
     "Events / 5 GeV"
     )

Plot("/nfs/dust/cms/user/mameyer/SM_HiggsTauTau/HTauTau_emu/Inputs/NTuples_2016/",
     "MuonEG_Run2016",
     "eta_1",
     "inputDNN",
      25,
      -2.5,
      2.5,
     "mcweight*puweight*effweight*0.978824*0.985*xsec_lumi_weight*",
     "&&iso_1<0.15&&iso_2<0.2&&extraelec_veto<0.5&&extramuon_veto<0.5&&pt_1>13&&pt_2>10&TMath::Max(pt_1,pt_2)>24&&metFilters>0.5&&mTdileptonMET<60&&dzeta>-35",
     "electron #eta",
     "Events"
     )

Plot("/nfs/dust/cms/user/mameyer/SM_HiggsTauTau/HTauTau_emu/Inputs/NTuples_2016/",
     "MuonEG_Run2016",
     "eta_2",
     "inputDNN",
      25,
      -2.5,
      2.5,
     "mcweight*puweight*effweight*0.978824*0.985*xsec_lumi_weight*",
     "&&iso_1<0.15&&iso_2<0.2&&extraelec_veto<0.5&&extramuon_veto<0.5&&pt_1>13&&pt_2>10&TMath::Max(pt_1,pt_2)>24&&metFilters>0.5&&mTdileptonMET<60&&dzeta>-35",
     "muon #eta",
     "Events"
     )

Plot("/nfs/dust/cms/user/mameyer/SM_HiggsTauTau/HTauTau_emu/Inputs/NTuples_2016/",
     "MuonEG_Run2016",
     "jpt_1",
     "inputDNN",
      25,
      0,
      250,
     "mcweight*puweight*effweight*0.978824*0.985*xsec_lumi_weight*",
     "&&iso_1<0.15&&iso_2<0.2&&extraelec_veto<0.5&&extramuon_veto<0.5&&pt_1>13&&pt_2>10&TMath::Max(pt_1,pt_2)>24&&metFilters>0.5&&mTdileptonMET<60&&dzeta>-35",
     "leading jet p_{T} [GeV]",
     "Events / 10 GeV"
     )

Plot("/nfs/dust/cms/user/mameyer/SM_HiggsTauTau/HTauTau_emu/Inputs/NTuples_2016/",
     "MuonEG_Run2016",
     "jpt_2",
     "inputDNN",
      25,
      0,
      250,
     "mcweight*puweight*effweight*0.978824*0.985*xsec_lumi_weight*",
     "&&iso_1<0.15&&iso_2<0.2&&extraelec_veto<0.5&&extramuon_veto<0.5&&pt_1>13&&pt_2>10&TMath::Max(pt_1,pt_2)>24&&metFilters>0.5&&mTdileptonMET<60&&dzeta>-35",
     "trailing jet p_{T} [GeV]",
     "Events / 10 GeV"
     )

Plot("/nfs/dust/cms/user/mameyer/SM_HiggsTauTau/HTauTau_emu/Inputs/NTuples_2016/",
     "MuonEG_Run2016",
     "njets",
     "inputDNN",
      10,
      -0.5,
      9.5,
     "mcweight*puweight*effweight*0.978824*0.985*xsec_lumi_weight*",
     "&&iso_1<0.15&&iso_2<0.2&&extraelec_veto<0.5&&extramuon_veto<0.5&&pt_1>13&&pt_2>10&TMath::Max(pt_1,pt_2)>24&&metFilters>0.5&&mTdileptonMET<60&&dzeta>-35",
     "N_{jets}",
     "Events"
     )

Plot("/nfs/dust/cms/user/mameyer/SM_HiggsTauTau/HTauTau_emu/Inputs/NTuples_2016/",
     "MuonEG_Run2016",
     "mt_1",
     "inputDNN",
      15,
      0,
      150,
     "mcweight*puweight*effweight*0.978824*0.985*xsec_lumi_weight*",
     "&&iso_1<0.15&&iso_2<0.2&&extraelec_veto<0.5&&extramuon_veto<0.5&&pt_1>13&&pt_2>10&TMath::Max(pt_1,pt_2)>24&&metFilters>0.5&&mTdileptonMET<60&&dzeta>-35",
     "mt_{1} [GeV]",
     "Events / 10 GeV"
     )

Plot("/nfs/dust/cms/user/mameyer/SM_HiggsTauTau/HTauTau_emu/Inputs/NTuples_2016/",
     "MuonEG_Run2016",
     "mt_2",
     "inputDNN",
      15,
      0,
      150,
     "mcweight*puweight*effweight*0.978824*0.985*xsec_lumi_weight*",
     "&&iso_1<0.15&&iso_2<0.2&&extraelec_veto<0.5&&extramuon_veto<0.5&&pt_1>13&&pt_2>10&TMath::Max(pt_1,pt_2)>24&&metFilters>0.5&&mTdileptonMET<60&&dzeta>-35",
     "mt_{2} [GeV]",
     "Events / 10 GeV"
     )

Plot("/nfs/dust/cms/user/mameyer/SM_HiggsTauTau/HTauTau_emu/Inputs/NTuples_2016/",
     "MuonEG_Run2016",
     "pt_tt",
     "inputDNN",
      30,
      0,
      300,
     "mcweight*puweight*effweight*0.978824*0.985*xsec_lumi_weight*",
     "&&iso_1<0.15&&iso_2<0.2&&extraelec_veto<0.5&&extramuon_veto<0.5&&pt_1>13&&pt_2>10&TMath::Max(pt_1,pt_2)>24&&metFilters>0.5&&mTdileptonMET<60&&dzeta>-35",
     "pt_{tt} [GeV]",
     "Events / 10 GeV"
     )

Plot("/nfs/dust/cms/user/mameyer/SM_HiggsTauTau/HTauTau_emu/Inputs/NTuples_2016/",
     "MuonEG_Run2016",
     "mjj",
     "inputDNN",
      30,
      0,
      600,
     "mcweight*puweight*effweight*0.978824*0.985*xsec_lumi_weight*",
     "&&iso_1<0.15&&iso_2<0.2&&extraelec_veto<0.5&&extramuon_veto<0.5&&pt_1>13&&pt_2>10&TMath::Max(pt_1,pt_2)>24&&metFilters>0.5&&mTdileptonMET<60&&dzeta>-35",
     "m_{jj} [GeV]",
     "Events / 20 GeV"
     )

Plot("/nfs/dust/cms/user/mameyer/SM_HiggsTauTau/HTauTau_emu/Inputs/NTuples_2016/",
     "MuonEG_Run2016",
     "met",
     "inputDNN",
      15,
      0,
      150,
     "mcweight*puweight*effweight*0.978824*0.985*xsec_lumi_weight*",
     "&&iso_1<0.15&&iso_2<0.2&&extraelec_veto<0.5&&extramuon_veto<0.5&&pt_1>13&&pt_2>10&TMath::Max(pt_1,pt_2)>24&&metFilters>0.5&&mTdileptonMET<60&&dzeta>-35",
     "#slash{E}_{T} [GeV]",
     "Events / 10 GeV"
     )


.q


EOF
