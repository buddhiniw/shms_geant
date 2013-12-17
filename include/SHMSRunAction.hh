//******************************************************************
/*
Date - 10/11/2013
Author - B.Waidyawansa

SHMSRunAction class

It accumulates statistic and computes dispersion of the energy deposit 
of charged particles with use of analysis tools:
H1D histograms are created in BeginOfRunAction() for the following 
physics quantities:
- Edep in coils
The same values are also saved in the ntuple.
The histograms and ntuple are saved in the output file in root format

In EndOfRunAction(), the accumulated statistic and computed 
dispersion is printed.

*/
//******************************************************************

#ifndef SHMSRunAction_h
#define SHMSRunAction_h 1


#include "globals.hh"
#include "G4UserRunAction.hh"
#include "SHMSDetectorConstruction.hh"
#include "SHMSAnalysisManager.hh"
#include "SHMSEventAction.hh"

class G4Run;

class SHMSRunAction : public G4UserRunAction
{
public:
  
  SHMSRunAction(SHMSDetectorConstruction*, SHMSAnalysisManager*, SHMSEventAction*);
  ~SHMSRunAction();

  void BeginOfRunAction(const G4Run* aRun);
  void EndOfRunAction(const G4Run*);

private:

  SHMSDetectorConstruction* Detector;    
  SHMSAnalysisManager* Output;
  SHMSEventAction* Event;
};

#endif
