/*
Author - B.Waidyawansa
Date - 12-03-2013

A macro to open the shms_sim rootfiles and draw the following;

1. Heat deposition on the DumpFace and DumpWindow
2. Hit distribution ,,   ,,

*/


void draw_dump_hits(TString file, TString field_status, Double_t Ne, TString target)
{
 
  //Delete all the objects stored in the current directory memory
  gDirectory->Delete("*");
  gStyle->SetCanvasPreferGL(1);
  gStyle->SetPalette(1); 

  // pads parameters
  gStyle->SetPadColor(0); 
  gStyle->SetPadBorderMode(0);
  gStyle->SetPadBorderSize(0);
  gStyle->SetPadGridX(kTRUE);
  gStyle->SetPadGridY(kTRUE);
  gStyle->SetPadTopMargin(0.12);
  gStyle->SetPadBottomMargin(0.12);
  gStyle->SetPadLeftMargin(0.08);  
  gStyle->SetPadRightMargin(0.18);  
  gStyle->SetTitleSize(0.05);


  Bool_t heat_dep = true;

  // Open and chain the root file
  Char_t filename[200]; 
  sprintf(filename,"~/rootfiles/rootfiles/%s.000_ntuple.root",file.Data());

  TChain * chain1 = new TChain("DumpFace");
  TChain * chain2 = new TChain("DumpWindow");

  TFile *f = new TFile(filename);
  if (f->IsZombie()) {
    std::cout << "Error opening root file chain "<< filename << std::endl;
    exit(1);
  }
 
  sprintf(filename,"~/rootfiles/rootfiles/%s.000_ntuple.root",file.Data());
  std::cout << "Opening root file "<< filename << std::endl;
  chain1->Add(filename);
  chain2->Add(filename);

  if(chain1 == NULL){
    std::cout<< "Unable to find the ntuples "<< std::endl; 
    exit(1);
  }
  
  std::cout<<"Obtaining data from ntuples"<<std::endl;
  
  // Create a canvas 
  TCanvas * C = new TCanvas("canvas","",1300,1000);
  C->Divide(2,2);
  C->SetFillColor(0);
  
  std::cout<<" Number of beam electrons used for the simulation - "<<Ne<<std::endl;
  
  //////
  /// Draw hit distribution on the surface
  
  TString convert = Form("eDep_MeV/%f",Ne); 
  C->cd(1); 
  // Dump Window
  chain2->Draw("local_y_cm:local_x_cm>>hist2","local_z_cm<-0.05 && parentID==0");
  TH2F*  h2 = (TH2F*)(gDirectory->Get("hist2"));
  h2->Draw("");
  h2->SetTitle("Hits on the Dump Window Surface");
  h2->GetXaxis()->SetTitle("X (cm)");
  h2->GetYaxis()->SetTitle("Y (cm)");
  gPad->Update();

  
  C->cd(2); 
  // Dump Face
  chain1->Draw("local_y_cm:local_x_cm >> hist12","local_z_cm<-0.2 && parentID==0",""); 
  TH2F* h12 = (TH2F*)(gDirectory->Get("hist12"));
  h12->Draw("");
  h12->SetMarkerColor(kGreen-2);
  h12->SetTitle("Hits on the Dump Face Surface");
  h12->GetXaxis()->SetTitle("X (cm)");
  h12->GetYaxis()->SetTitle("Y (cm)");
  gPad->Update();


  
  /////////
  // Draw total heat deposition in Watt/muA
  C->cd(3);
  // Dump window
  chain2->Draw("local_y_cm:local_x_cm>>hist4",Form("%s*(local_z_cm<0.06)",convert.Data()),"colz"); // -0.05 cm is based on the geometry of the dump window
  
  TH2F* h4 = (TH2*)(gDirectory->Get("hist4"));
  h4->SetStats(0);
  h4->SetTitle("Total heat deposited on the Dump Window");
  h4->GetYaxis()->SetTitle("Y (cm) ");
  h4->GetXaxis()->SetTitle("X (cm)");
  gPad->Update();
  TPaletteAxis *palette1 = (TPaletteAxis*)((h4->GetListOfFunctions())->FindObject("palette"));
  palette1->GetAxis()->SetTitle("W#muA^{-1}");
  palette1->GetAxis()->SetTitleOffset(1.5);
  gPad->Modified();

  C->cd(4);
  // Dump window
  chain1->Draw("local_y_cm:local_x_cm>>hist3",Form("%s*(local_z_cm<0.7)",convert.Data()),"colz"); // -0.05 cm is based on the geometry of the dump window
  
  TH2F* h3 = (TH2*)(gDirectory->Get("hist3"));
  h3->SetStats(0);
  h3->SetTitle("Total heat deposited on the Dump Face");
  h3->GetYaxis()->SetTitle("Y (cm) ");
  h3->GetXaxis()->SetTitle("X (cm)");
  gPad->Update();
  TPaletteAxis *palette2 = (TPaletteAxis*)h3->GetListOfFunctions()->FindObject("palette");
  palette2->GetAxis()->SetTitle("W#muA^{-1}");
  palette2->GetAxis()->SetTitleOffset(1.5);
  gPad->Modified();

  C->Modified();
  C->Update();
  C->SaveAs(Form("dump_field_%s_%s_sim_plots.png",field_status.Data(),target.Data()));
  C->SaveAs(Form("dump_field_%s_%s_sim_plots.C",field_status.Data(),target.Data()));


  std::cout<<"Done!"<<std::endl;
}
