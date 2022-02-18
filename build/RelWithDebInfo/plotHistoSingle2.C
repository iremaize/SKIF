// ROOT macro file for plotting example B4 histograms 
// 
// Can be run from ROOT session:
// root[0] .x plotHisto.C

void plotHistoSingle2(){
  gROOT->Reset();
  gROOT->SetStyle("Plain");
 // auto  mycanvas = new TCanvas();
  
  // Draw histos filled by Geant4 simulation 
  //   

  // Open file filled by Geant4 simulation
  // Create a canvas and divide it into 2x2 pads
  TCanvas* c1 = new TCanvas("c1", "", 20, 20, 1000, 1000);
  c1->Divide(2,1);

  c1->cd(1);
  auto rdf = ROOT::RDF::MakeCsvDataFrame("01deg_0kg_E5.csv");
  auto h = rdf.Histo1D("x");
  h->Draw();
 
 



 
c1->Print("graph_with_law.pdf");
}

// /vis/scene/endOfEventAction accumulate -1
