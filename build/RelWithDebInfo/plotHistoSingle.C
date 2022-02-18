// ROOT macro file for plotting example B4 histograms 
// 
// Can be run from ROOT session:
// root[0] .x plotHisto.C

void plotHistoSingle(){
  gROOT->Reset();
  gROOT->SetStyle("Plain");
 // auto  mycanvas = new TCanvas();
  
  // Draw histos filled by Geant4 simulation 
  //   

  // Open file filled by Geant4 simulation 
TFile f("hist5.root");

  // Create a canvas and divide it into 2x2 pads
  TCanvas* c1 = new TCanvas("c1", "", 20, 20, 1000, 1000);
  c1->Divide(2,2);
  
  // Draw Eabs histogram in the pad 1
  c1->cd(1);
  TH2F* hist1 = (TH2F*)f.Get("E1");
  hist1->SetTitle("1");
  hist1->Draw("HIST");
  
  c1->cd(2);
  TH2F* hist2 = (TH2F*)f.Get("E2");
  hist2->SetTitle("2");
  hist2->Draw("HIST");


  c1->cd(3);
  TH1D* hist5 = (TH1D*)f.Get("E5");
  hist5->SetTitle("3");
  hist5->Draw("HIST");

  c1->cd(4);
  TH1D* hist6 = (TH1D*)f.Get("E6");
  hist6->SetTitle("4");
  hist6->Draw("HIST");
 
 



 
c1->Print("d37_30_9.pdf");
}

// /vis/scene/endOfEventAction accumulate -1
