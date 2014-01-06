//******************************************************************
/*
Date - 10/15/2013
Author - B.Waidyawansa

SHMSEventAction class

In EndOfEventAction(), the accumulated  energy 
deposit from particles on the sensitive detectors are stored in the 
histograms

*/
//******************************************************************

#ifndef SHMSEventAction_h
#define SHMSEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4THitsMap.hh"
#include "SHMSAnalysisManager.hh"

class G4GenericMessenger;

class SHMSEventAction : public G4UserEventAction
{
public:
  SHMSEventAction();
  ~SHMSEventAction();

  // static access method
  static SHMSEventAction* Instance();

public:
  virtual void BeginOfEventAction(const G4Event *event);
  virtual void   EndOfEventAction(const G4Event *event);

  const std::vector<G4double> GetEnergySum() const { return sd_energy; };
  const std::vector<G4double> GetEnergySum2()const { return sd_energy2; };

private:
  static SHMSEventAction*  fgInstance;  
  SHMSAnalysisManager* analysisManager;
  G4SDManager * SDman;

  std::vector<G4int> hitsCollectionID;
  std::vector<G4String> sd_names;
  std::vector<G4double>sd_energy; // to store total energy per event
  std::vector<G4double> sd_energy2; // to store total energy^2 per event

  G4int nDets;
  G4double fEnergySum;
  G4double fEnergySum2;

};

#endif
