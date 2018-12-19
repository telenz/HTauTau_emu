TH1D * Unfold(TH2D * histInput) {

  int nBinsX = histInput->GetNbinsX();
  int nBinsY = histInput->GetNbinsY();

  int nBins = nBinsX * nBinsY;

  TString NameNew = TString(histInput->GetName())+TString("_unfolded");

  TH1D * histOutput = new TH1D(NameNew,"",nBins,0,float(nBins));
  int iBin = 1;
  for (int j=1; j<=nBinsY; ++j) {
    for (int i=1; i<=nBinsX; ++i) {
      histOutput->SetBinContent(iBin,histInput->GetBinContent(i,j));
      histOutput->SetBinError(iBin,histInput->GetBinError(i,j));
      iBin++;
    }
  }

  return histOutput;

}
