//******************************************************************
/*
Date - 10/15/2013
Author - B.Waidyawansa

SHMSEventAction class implementation using examples/extended/electromagnetic/TestEM4



*/
//******************************************************************

#include "SHMSEventAction.hh"
#include "g4root.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4GenericMessenger.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "SHMSSteppingAction.hh"
#include "SHMSDetectorConstruction.hh"
#include "SHMSDetectorSD.hh"

SHMSEventAction* SHMSEventAction::fgInstance = 0;

SHMSEventAction* SHMSEventAction::Instance()
{
// Static acces function via G4RunManager 
  return fgInstance;
}   
                              
SHMSEventAction::SHMSEventAction()
{
  // Clear the energy arrays
  sd_energy.clear();
  sd_energy2.clear();

  // for now use a fixed size vector
  sd_energy.resize(10);
  sd_energy2.resize(10);

  fgInstance = this;
}

SHMSEventAction::~SHMSEventAction()
{}

void SHMSEventAction::BeginOfEventAction( const G4Event* event)
{
  G4int eventID = event->GetEventID();
  G4cout << "\n### Begin Event Action - event " << eventID << G4endl;

  // Get the number of sensitive detectors in the collection.
  SDman = G4SDManager::GetSDMpointer();
  nDets = SDman->GetCollectionCapacity();

  if((int) hitsCollectionID.size()==0)   
    {    
      for (int i=0; i<nDets; i++) {

	G4String collName = "detector";
	collName+=G4UIcommand::ConvertToString(i+1);
	SHMSDetectorSD *sddet = (SHMSDetectorSD*)(SDman->FindSensitiveDetector(collName));
	sd_names.push_back(sddet->GetIdentifier());
	collName+="/hitsColl";
	hitsCollectionID.push_back (SDman->GetCollectionID(collName));
      }
    }

  G4cout << "### Begin Event Action - Created Hits collection  " << G4endl;

}


void SHMSEventAction::EndOfEventAction(const G4Event* event)
{          
  
  // get analysis manager
  analysisManager = SHMSAnalysisManager::Instance();

  // Get hits collection for this event
  G4HCofThisEvent * HCE = event->GetHCofThisEvent();
  std::vector<SHMSDetectorHitsCollection*> THC;

  if(HCE) {
    for (int i=0; i<nDets; i++) {
      THC.push_back ((SHMSDetectorHitsCollection*)(HCE->GetHC(hitsCollectionID[i])));
    }
  }
  else
    G4cout<<"Hit collection for this event was not found!!!!\n";

  G4cout << "### Fill ntuples & histograms " << G4endl;


  // loop over number of sensitive detectors and fill/calculate variables
  G4double edep=0;
  for (int i=0; i<nDets; i++) {
             
    if(THC[i]){
      int n_hit = THC[i]->entries();
      if (n_hit>0) {
	for(G4int k=0;k<n_hit;k++) {

	  SHMSDetectorHit* aHit = (*THC[i])[k];
	  // Save all hits in the sensitive detectors without any selections
	  analysisManager-> FillNtuples(aHit,sd_names.at(i));
	  analysisManager-> FillHistograms(aHit,sd_names.at(i));

	  //Get total energy deposited for this event
	  edep = aHit->GetEnergyDeposit();

	  // accumulate all energy deposition in the events
	  // This will be later passed on the RunAction class
	  sd_energy.at(i)+=edep;
	  sd_energy2.at(i)+=edep*edep;

	  //G4cout<<" edep ="<<sd_energy.at(i)<<" edep^2"<<sd_energy2.at(i)<<G4endl;
	}
      }
    }

    // Fill energy deposition histograms per event
    TH1* histo1 = (TH1D*)gDirectory->Get(Form("Tot_Edep_%s",(sd_names.at(i)).data()));
    if(!histo1){
      G4cout<<" Unable to find the total energy deposit histogram for "<<sd_names.at(i)<<G4endl;
      return;
    }
    else{
      histo1 ->Fill(sd_energy.at(i)/MeV,1); 
    }

  }
  G4cout << "### End of Event Action " << G4endl;
    
}




