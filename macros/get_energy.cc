/*
Author - B.Waidyawansa
Date - 12/17/2013

GEt energy deposition from eDep_MeV histograms
*/


#include<TString.h>
#include<TFile.h>
#include<TChain.h>
#include<TChainElement.h>
#include<TH2.h>
#include<TProfile.h>
#include<TPaletteAxis.h>
#include<TH1.h>
#include<TStyle.h>
#include<iostream>
#include<TPaveText.h>
#include<TCanvas.h>
#include<TKey.h>
#include<TRandom.h>


void get_edep()
{
 
  //Delete all the objects stored in the current directory memory
  gDirectory->Delete("*");
  //  gStyle->SetCanvasPreferGL(1); this cuases the canvas to save as a blank canvas. So saveAs option is not working..
  gStyle->SetPalette(1); 
  gStyle->SetOptStat(0); 
  
  // pads parameters
  gStyle->SetPadColor(kWhite); 
  gStyle->SetPadBorderMode(0);
  gStyle->SetPadBorderSize(0);
  gStyle->SetPadGridX(kTRUE);
  gStyle->SetPadGridY(kTRUE);
  gStyle->SetPadTopMargin(0.12);
  gStyle->SetPadBottomMargin(0.12);
  gStyle->SetPadLeftMargin(0.08);  
  gStyle->SetPadRightMargin(0.18);  
  gStyle->SetTitleSize(0.05);

  gStyle->SetLabelSize(0.05,"y");

	   
  // Create a canvas 
  TCanvas * C = new TCanvas("canvas","",1000,1000);
  C->Divide(1);
  C->SetFillColor(0);
             

  // Open and chain the root file

  TChain * chain = new TChain("Box");
  TFile *f = new TFile("~/rootfiles/debuging_setup1_ntuple.root");
  if (f->IsZombie()) {
    std::cout << "Error opening root file chain "<< std::endl;
    exit(1);
  }
  else{
    chain->Add("~/rootfiles/debuging_setup1_ntuple.root");
    if(chain == NULL){
      std::cout<< "Unable to find the ntuple HBCoil"<< std::endl; 
      exit(1);
    }
    else{
      std::cout<<"Obtaining data from ntuple : HBCoil"<<"\n";


      //////////////////////////////////
      // Draw total heat deposition in Watt/muA
      //////////////////////////////////
      C->cd(2);


      //   First get the statistical rms on energy deposited per step.
      //this is going to be the uncertainty in energy per step
      chain->Draw("eDep_MeV>> hist0","","");
      TH1* h0 = (TH1*)gDirectory->Get("hist0");
      Double_t rms_edep = h0->GetRMS(); // MeV

      // get energy
      std::cout<<"Get total energy ..\n"; 
      Double_t entries = 0.0;
      Double_t bins = h0->GetNbinsX();
      Double_t val = 0;
      Double_t sum = 0;
      Double_t terr = 0;
      for(Int_t i=0;i<bins;i++) {
	entries = h0->GetBinContent(i+1); 
	val = (h0->GetBinCenter(i+1))* entries;
	//	err = rms_edep/sqrt(entries);
	//	err = (h->GetBinWidth(i+1))/sqrt(entries);
	sum +=val;
      }

      // total error on the energy determination
      Double_t tot_entries =  h0->GetEntries();
      Double_t error = rms_edep/sqrt(tot_entries);

      TPaveText *pt = new TPaveText(0.3,0.6,0.85,0.8,"NDC");
      pt->AddText(Form("Total heat deposited = %2.2f #pm %2.2f MeV",sum,error));
      pt->Draw();
      gPad->Update();

    std::cout<<"Done!"<<std::endl;
    }
  }
}


