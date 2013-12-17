//******************************************************************
/*
Date - 10/11/2013
Author - B.Waidyawansa


A GEANT4 simulation package to simulate SHMS fringe fields.
Based on geant4 /examples/extended/persistency/gdml/G02

Usage : ./shms_geant <gdml file name>


11-06-13 Buddhini Added the gdml file to be read in via the command prompt instead of 
                  hardcoding the name in the SHMSDetectorConstruction class.

11-28-13 Buddhini Switched to using geant4 physics list "QGSP_BERT_HP instead of the customized phyics
                  list SHMSPhysicsList.cc. No need for customization. 
                  Also added a shield wall around the beam pipe and dump made out of SSteel. All tracks entering
                  SSteel are killed.
*/
//******************************************************************

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "globals.hh"
#include "G4PhysListFactory.hh"
#include "G4VModularPhysicsList.hh"

// User defined classes
//
#include "SHMSDetectorConstruction.hh"
#include "SHMSPrimaryGeneratorAction.hh"
#include "SHMSRunAction.hh"
//#include "SHMSPhysicsList.hh"
#include "SHMSEventAction.hh"
#include "SHMSSteppingAction.hh"
#include "SHMSAnalysisManager.hh"

//For Visualization
//
#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

// User interface
//
#ifdef G4UI_USE_QT
    #include "G4UIQt.hh"
#else
    #warning "No Qt support."
#endif

#ifdef G4UI_USE_XM
    #include "G4UIXm.hh"
#endif


int main(int argc, char** argv)
{
  G4String fGeomFile;

  // Get the GDML file for geometry
  if(argc==2){
    fGeomFile = argv[1];
  }
  else {
    G4cout<<"The correct usage is ./shms_geant <gdml file name>"<<G4endl;
    G4cout<<"No GDML file specified! exiting program.."<<G4endl;
    exit(1);
  }

       
  // Construct the default run manager
  //
  G4RunManager* runManager = new G4RunManager;

  // Set mandatory user initialization
  //
  SHMSDetectorConstruction* detector = new SHMSDetectorConstruction(fGeomFile);
  runManager->SetUserInitialization(detector);

  G4PhysListFactory factory;
  G4VModularPhysicsList* phys = 0;
  
  // Reference PhysicsList via its name
  phys = factory.GetReferencePhysList("QGSP_BERT_HP");
  runManager->SetUserInitialization(phys);  

  SHMSPrimaryGeneratorAction* primary = new SHMSPrimaryGeneratorAction();
  runManager->SetUserAction(primary);

  SHMSAnalysisManager*  histo = new SHMSAnalysisManager(detector);

  // Set optional user action classes
  //
  SHMSEventAction* EventAct    = new SHMSEventAction();  
  runManager->SetUserAction(EventAct);
  
  SHMSRunAction* RunAct = new SHMSRunAction(detector,histo,EventAct);
  runManager->SetUserAction(RunAct);

  SHMSSteppingAction* StepAct = new SHMSSteppingAction(RunAct,detector,histo);
  runManager->SetUserAction(StepAct);

  
  // Visualization, if required
  //
#ifdef G4VIS_USE
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
#endif

  // Open a interactive session: will stay there until the user types "exit"
  //
  G4UImanager* UImanager = G4UImanager::GetUIpointer();  

  // Initialize G4 kernel
  //
  runManager->Initialize();
 
  // Define UI session for interactive mode.
  //
  if (argc==2)   
    {
#ifdef G4UI_USE
      G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
      UImanager->ApplyCommand("/control/execute vis.mac");       
#endif
      ui->SessionStart();
      delete ui;
#endif
           
    }
  
  
  // job termination

#ifdef G4VIS_USE
  delete visManager;
#endif

  delete runManager;  
  return 0;

}
