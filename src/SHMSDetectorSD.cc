//******************************************************************
/*
Date - 10/15/2013
Author - B.Waidyawansa

SHMSDetectorSD  class implementation

12-03-13 Buddhini Removed creator process. Added parent ID.

*/
//******************************************************************

#include "SHMSDetectorSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4VProcess.hh"

SHMSDetectorSD::SHMSDetectorSD(G4String name, G4String type)
:G4VSensitiveDetector(name)
{
  G4String HCname;
  collectionName.insert(HCname="hitsColl");
  identifier = type;
}

SHMSDetectorSD::~SHMSDetectorSD()
{ }

void SHMSDetectorSD::Initialize(G4HCofThisEvent* HCE)
{
  hitsCollection = new SHMSDetectorHitsCollection(SensitiveDetectorName,collectionName[0]); 
  HCID = -1;
  if(HCID<0)
    { HCID = G4SDManager::GetSDMpointer()->GetCollectionID(hitsCollection); }
  HCE->AddHitsCollection( HCID, hitsCollection); 
}

G4bool SHMSDetectorSD::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{

  const G4Track* fTrack = aStep->GetTrack();
  
  // Identify and flag the particle
  //
  G4double flagParticle=0.;
  if (fTrack->GetDynamicParticle()->GetDefinition() ->GetParticleName() == "e-")       flagParticle = 1;    
  if (fTrack->GetDynamicParticle()->GetDefinition() ->GetParticleName() == "e+")       flagParticle = 2;
  if (fTrack->GetDynamicParticle()->GetDefinition() ->GetParticleName() == "gamma")    flagParticle = 3;
  if (fTrack->GetDynamicParticle()->GetDefinition() ->GetParticleName() == "neutron")  flagParticle = 4;


  //Get parent id. parent id is 0 for the primary particle and 1 for the secondary particle and so on.
  G4double flagParent;
  if(fTrack->GetParentID() < 3) flagParent = fTrack->GetParentID(); // for now save only parent and first primary
  else 
    flagParent = -1;

  /*
  A bit about : PreStepPoint and PostStepPoint.
  
  NOTE: In Geant4, "particles are NOT transported in the tracking category".
  
  It means that tracking is not been done from one point to the other.
  Instead, G4 uses concept of 'step', which is related with physics process.
  It calculates most probable process and do it.  As a result, it results in
  two points; PreStepPoint and PostStepPoint.
    
  PreStepPoint contains track information before a process has been occurred,
  while PostStepPoint has information after the process has been processed.
  
  PreStepPoint
  
  --> G4 calculates probabilities of the physics processes
  --> Take smallest process step length among the processes
  --> Calculate other processes during the step
  
  --> 
  PostStepPoint
  
  So to get the information related with the current 'position', such as 
  current volume, we have to use PreStepPoint.  
  
  But if we need information releted with the 'processes', we have to use
  PostStepPoint.
  
  Positional Information: PreStepPoint  : current volume, ...
  Procedural Information: PostStepPoint : process name, eLoss, step length, ...
  */
  
  G4StepPoint* preStepPoint = aStep->GetPreStepPoint();

  // Get track position at the boundary
  G4ThreeVector worldPositionBoundary;
  G4ThreeVector localPositionBoundary;

  // if the preStepPoint is at the boundary of the volume
  if (preStepPoint->GetStepStatus() == fGeomBoundary){
      worldPositionBoundary = preStepPoint->GetPosition();
      localPositionBoundary = preStepPoint->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(worldPositionBoundary);
  }

  G4ThreeVector worldPosition = preStepPoint->GetPosition();
  G4ThreeVector localPosition = preStepPoint->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(worldPosition);
  G4ThreeVector vertexMomentum = fTrack->GetVertexMomentumDirection();
  
  // scattering angle
  G4double scat_ang = sqrt(vertexMomentum.x()*vertexMomentum.x() + 
			   vertexMomentum.y()*vertexMomentum.y());

  // Kinetic energy of the electron at the vertex
  G4double vertexE = fTrack->GetVertexKineticEnergy();

  // Kinetic energy of the original electron which interacted inside the target
  G4double kineE0 = fTrack->GetVertexKineticEnergy();

  // Kinetic energy of the electron when it hit the sensitive detector
  G4double kineE = fTrack->GetKineticEnergy();

  // Total energy deposited in the step
  G4double eDep = aStep->GetTotalEnergyDeposit();
 
  // Create and define Hit attributes to store above parameters
  //
  SHMSDetectorHit* newHit = new SHMSDetectorHit();
 
  newHit->SetWorldPos(worldPosition);
  newHit->SetLocalPos(localPosition);
  newHit->SetVertexEnergy(vertexE);
  newHit->SetKineticEnergy(kineE);
  newHit->SetKineticEnergy0(kineE0);
  newHit->SetEnergyDeposit(eDep);  
  newHit->SetParticleFlag(flagParticle);
  newHit->SetParentID(flagParent);
     
  hitsCollection->insert( newHit );

  return true;
}

void SHMSDetectorSD::EndOfEvent(G4HCofThisEvent*)
{}
