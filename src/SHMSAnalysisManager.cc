//******************************************************************
/*
Date - 10/23/2013
Author - B.Waidyawansa

SHMSAnalysisManager class implementation
based on geant4.9.6.p02/examples/advanced/purging_magnet/

12-02-13 Buddhini Added EventTree to store event based information such as energy deposition, angle and vertex energy
12-03-13 Buddhini Added parent ID and kinetic energy before step into the ntuple.

*/
//******************************************************************

#include <stdlib.h>
#include <fstream>
#include "SHMSAnalysisManager.hh"
#include "SHMSDetectorHit.hh"

// Booliean keys to control ntuple and histo storage in the rootfile
#define NTUPLES 1
#define HISTOS  1

SHMSAnalysisManager* SHMSAnalysisManager::fgInstance = 0;

SHMSAnalysisManager* SHMSAnalysisManager::Instance()
{
// Static acces function via G4RunManager 
  return fgInstance;
}   


SHMSAnalysisManager::SHMSAnalysisManager(SHMSDetectorConstruction*det)
  :fDetector(det){

  // set the root file name using the gdml file name. Currently, all rootfiles are saved into
  // my folder in data1.
  fileName  = "/data1/cdaq/buddhini/rootfiles/"+fDetector->GetGeomFile()+"_ntuple.root";
  nDet = 0;
  Clear();
}

SHMSAnalysisManager::~SHMSAnalysisManager() 
{ 
  if(theTFile) delete theTFile;
}

void SHMSAnalysisManager::Clear()
{
  if (theTFile) {
    Ntuples.clear();  
    histo1d.clear(); 
  }

}

void SHMSAnalysisManager::OpenROOTFile()
{
   
  G4cout << "\n----> Open rootfile .. " << G4endl;
  theTFile = new TFile(fileName, "RECREATE");

  CreateNtuplesHistos();

}


void SHMSAnalysisManager::SaveROOTFile()
{
  if (theTFile)
    {
      theTFile -> Write(); 
      theTFile -> Close();
    }
  theTFile = 0;
  
}

//Create Ntuples
void SHMSAnalysisManager::CreateNtuplesHistos()
{
  detNames = fDetector->GetDetNames();
  nDet = detNames.size();

  Ntuples.resize(nDet);
  histo1d.resize(nDet);
  
  
  for(G4int i=0;i<nDet;i++)
    {
       // create Ntuples
      if(NTUPLES){
	Ntuples.at(i)  = new TNtuple(detNames.at(i), 
				     "detector hits", 
				     "local_x_cm:local_y_cm:local_z_cm:vertexE_MeV:kineticE_MeV:eDep_MeV:flagParticle:parentID");	

      }

      if(HISTOS){
	
	// Create histogram(s)
	histo1d.at(i) = new TH1D(Form("Tot_Edep_%s",(detNames.at(i)).data()), "Energy Deposition in the Volume per Event; Energy Deposition per Event (MeV); Electrons in the Beam ", 800, 0.0, 0.0);
	
	// enable automatic rebinning at the time of filling
	histo1d.at(i)->SetBit(TH1::kCanRebin);
     }
    }
  
  G4cout << "\n----> Histograms and Ntuples are opened in "<<fileName<< G4endl;
  
}


// This function fills a N-tuple with position and energy of 
// electrons entering the measurement volume.
//
void SHMSAnalysisManager::FillNtuples(SHMSDetectorHit *aHit,G4String ntuple_name)  
{
  if(NTUPLES){
    TNtuple *ntuple = (TNtuple*)gDirectory->Get(ntuple_name);
    
    if(!ntuple) {
      G4cout<<" Unable to find ntuple- "<<ntuple_name<<G4endl;
      return;
    }
    else{
      ntuple->Fill(aHit->GetLocalPos().x()/cm,
		   aHit->GetLocalPos().y()/cm,
		   aHit->GetLocalPos().z()/cm,
		   aHit->GetVertexEnergy()/MeV,
		   aHit->GetKineticEnergy()/MeV,
		   aHit->GetEnergyDeposit()/MeV,
		   aHit->GetParticleFlag(),
		   aHit->GetParentID());
      
    }
  }
  
}

void SHMSAnalysisManager::FillHistograms(SHMSDetectorHit *aHit,G4String hist_name)
{
  // 1-D histograms are filled in the SHMSEventAction class.
  // Nothing to do here yet..
}
