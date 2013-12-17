//******************************************************************
/*
Date - 10/23/2013
Author - B.Waidyawansa

SHMSSteppingAction class implementation
based on geant4.9.6.p02/examples/advanced/purging_magnet/

12-17-13 Buddhini Added a track kill mechanism for particles entering shields made out of
                  the hypothetical material "Moria" (you shall not pass!)


*/
//******************************************************************

#include "SHMSSteppingAction.hh"
#include "SHMSRunAction.hh"
#include "SHMSDetectorConstruction.hh"
#include "SHMSPrimaryGeneratorAction.hh"
#include "SHMSAnalysisManager.hh"

#include "G4SystemOfUnits.hh"
#include "G4SteppingManager.hh"
#include "G4VTouchable.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VParticleChange.hh"
#include "G4Track.hh"


SHMSSteppingAction* SHMSSteppingAction::fgInstance = 0;

SHMSSteppingAction* SHMSSteppingAction::Instance()
{
// Static acces function via G4RunManager 
  return fgInstance;
}   

SHMSSteppingAction::SHMSSteppingAction(SHMSRunAction* run, SHMSDetectorConstruction* det,
				       SHMSAnalysisManager* hist)
  :fRun(run),fDetector(det),fHisto(hist)
{
  fgInstance = this;
}

SHMSSteppingAction::~SHMSSteppingAction()
{}

void SHMSSteppingAction::UserSteppingAction(const G4Step* aStep)
{ 

  G4Track* fTrack = aStep->GetTrack();

  // KILL the particle if it hits a shielding made out of the hypothetical material, Moria */
  G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
  G4VPhysicalVolume* thePrePV = preStepPoint->GetPhysicalVolume();


  if(thePrePV->GetLogicalVolume()->GetMaterial()->GetName().compare("Moria")==0)
    {
      // kill all the tracks entering shielding
      fTrack->SetTrackStatus(fStopAndKill);
      return;
    } 
}
