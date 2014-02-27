//******************************************************************
/*
Date - 10/11/2013
Author - B.Waidyawansa

SHMSRunAction class implementation using examples/extended/electromagnetic/TestEM4



*/
//******************************************************************

#include "SHMSRunAction.hh"
#include "G4Run.hh"
#include "G4ParticleDefinition.hh"
#include "SHMSEventAction.hh"
#include <iostream>
#include <fstream>


SHMSRunAction::SHMSRunAction(SHMSDetectorConstruction* det, SHMSAnalysisManager* ana,SHMSEventAction* eve)
  :fDetector(det),fOutput(ana),fEvent(eve)
{}

SHMSRunAction::~SHMSRunAction()
{}


void SHMSRunAction::BeginOfRunAction(const G4Run* aRun)
{  
  G4cout << "\n### Run Action - Run " << aRun->GetRunID() << " start.." << G4endl;
  // Open ROOT file to store ntuples and histograms
  fOutput->OpenROOTFile();
}


void SHMSRunAction::EndOfRunAction(const G4Run* aRun)
{
  //save ntuples and histograms
  fOutput->SaveROOTFile();

  // compute total energy deposited on each sensitive volume during this run
  //
  G4double fEdepRms = 0;

  G4int nEvents = aRun->GetNumberOfEvent();
  if (nEvents == 0) return;

  // Get the energy data arrays
  fEnergySum  = SHMSEventAction::Instance()->GetEnergySum();
  fEnergySum2 = SHMSEventAction::Instance()->GetEnergySum2();

  // Open file to save total energy
  char name[100]; 
  sprintf(name,"/data1/cdaq/buddhini/rootfiles/%s_output.txt",(fDetector->GetGeomFile()).data());
  G4cout<<" file "<<name<<G4endl;
  oFile.open(name);
  oFile<<"Events in this run = "<<nEvents<<std::endl;
  oFile << "Total energy deposition for this run -\n";
  oFile << "detector, energy (MeV), rms (MeV)\n";

  std::vector<G4String> detNames = fDetector->GetDetNames();
  Double_t  nDet = detNames.size();
  for(G4int i=0;i<nDet;i++){ // for each sensitive detector
    G4double var = fEnergySum2.at(i) - (fEnergySum.at(i)*fEnergySum.at(i));
    //G4cout<<" sum2 = "<<fEnergySum2.at(i)<<" sum*sum "<<fEnergySum.at(i)*fEnergySum.at(i)<<G4endl;
    if (var > 0) fEdepRms = std::sqrt(var); 
    else fEdepRms = 0.0;
    oFile<<detNames[i]<<","<<fEnergySum.at(i)<<","<<fEdepRms<<std::endl;
    
  }  
  oFile.close();

  G4cout << "### End of Run Action.." << G4endl;

}
