//******************************************************************
/*
Date - 11/6/2013
Author - B.Waidyawansa

A class to handle the hit collections of the sensitive detectors.



*/
//******************************************************************

#ifndef SHMSDetectorHit_h
#define SHMSDetectorHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4ThreeVector.hh"
#include "G4Allocator.hh"

class SHMSDetectorHit : public G4VHit
{
  
public:
  SHMSDetectorHit();
  ~SHMSDetectorHit();


  void Draw();
  void Print();
  
  // Get/Set functions
  //
  // World position
  inline void SetWorldPos(G4ThreeVector xyz) 
  { worldPos = xyz; }
  inline G4ThreeVector GetWorldPos() const 
  { return worldPos; }
  
  // local position
  inline void SetLocalPos(G4ThreeVector xyz) 
  { localPos = xyz; }
  inline G4ThreeVector GetLocalPos() const 
  { return localPos; }

  // kinetic energy
  inline void SetKineticEnergy(G4double e)
  { kineE = e; }
  inline G4double GetKineticEnergy()
  { return kineE; }

  // intial kinetic energy
  inline void SetKineticEnergy0(G4double e)
  { kineE0 = e; }
  inline G4double GetKineticEnergy0()
  { return kineE0; }

  // energy at the scattering vertex
  inline void SetVertexEnergy(G4double e)
  { vertexE = e; }
  inline G4double GetVertexEnergy()
  { return vertexE; }

  // energy deposition 
  inline void SetEnergyDeposit(G4double e)
  { eDep += e; }
  inline G4double GetEnergyDeposit()
  { return eDep; }

  // particle type
  inline void  SetParticleFlag(G4double f)  
  { particleFlag = f; }
  inline G4double GetParticleFlag() const 
  { return particleFlag; }

  // parent type
  inline void  SetParentID(G4double f)  
  { parentID = f; }
  inline G4double GetParentID() const 
  { return parentID; }


private:

  G4ThreeVector worldPos;
  G4ThreeVector localPos;
  G4double kineE;
  G4double kineE0;
  G4double vertexE;
  G4double eDep;
  G4double particleFlag;
  G4double parentID;  

};

typedef G4THitsCollection<SHMSDetectorHit> SHMSDetectorHitsCollection;
extern G4Allocator<SHMSDetectorHit> SHMSDetectorHitAllocator;

#endif



