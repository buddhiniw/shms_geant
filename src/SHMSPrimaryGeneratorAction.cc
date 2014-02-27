/******************************************************************

Date - 10/11/2013
Author - B.Waidyawansa

SHMSPrimaryGeneratorAction class implementation

 
11-05-2013 Buddhini - Hardcode location of the particle gun w.r.t bender body coordinate system. 
12-04-2013 Buddhini - Hardcoded location of the gun w.r.t. origin on the optics system located at the bender.
                      This will enable us to use any target without having to readjust X and Y gun location.
01-09-2013 Buddhini - Removed the while loop used around the X0,Y0 random event generators. It was causing the particle hit location on the target to be uniform instead of random. X0 and Y0 was almost always 0.

******************************************************************/


#include "SHMSPrimaryGeneratorAction.hh"

#include "globals.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "Randomize.hh"


// booliean keys to turn on/off particle gun types
#define TEST 0        // to test the magnetic field with no target
#define WITH_TGT 1    // for the standard simulation with target

// turn on debugging messages          
#define DEBUG 0          



SHMSPrimaryGeneratorAction::SHMSPrimaryGeneratorAction()
{
  // Particle gun and particle table 
  //

  fParticleGun = new G4ParticleGun(1);
  fParticleTable = G4ParticleTable::GetParticleTable();

  // Default particle
  //
  G4ParticleDefinition* particle = fParticleTable->FindParticle("e-");
  fParticleGun->SetParticleDefinition(particle);
  
  // electron energy is 11 GeV.
  double gun_energy = 11000; // MeV
  fParticleGun->SetParticleEnergy(gun_energy*MeV);
  G4cout << "***************************" << G4endl
	 << "*                          " << G4endl
	 << "*  Gun energy is "<<gun_energy<<" MeV    " << G4endl
	 << "*                          " << G4endl
	 << "***************************" << G4endl;
  /*
    12-04-2013 Buddhini - Hardcode location of the particle gun w.r.t optics coordinate system. 
    Place gun at least 30 cm upstream of target. --> gun is at bender_to_target+30 cm along beam axis.
 
    From geometry (in shms_geom.gdml) units are cm;
    target_to_hbbore_center = 176 cm
    angle between beam axis and bender axis Z = 5.5 = alpha
    angle between beam axis and optics axis Z = 3 = beta

    X = (target_to_hbbore_center*cos(beta) + 30*cos(beta+alpha)
    Y = 0
    Z = (target_to_hbbore_center*sin(beta) + 30*sin(beta+alpha)
    
  */
  gun_x_pos = 13.65;
  gun_y_pos = 0;
  gun_z_pos = 205.43;

}

SHMSPrimaryGeneratorAction::~SHMSPrimaryGeneratorAction()
{
  delete fParticleGun;
}

void SHMSPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //this function is called at the begining of event
  //Start position of primaries at the origin of the world coordinates
 
  G4cout<<"\n### Generate Primaries .. "<<G4endl;

  if(TEST){
    // To test the magnetic field
    //

    // particles originate at the target.
    fParticleGun->SetParticlePosition(G4ThreeVector(13.65*cm,0.0*cm,-205.43*cm));

    // test by shooting 3 particles with with 1.5 deg spacing
  //   fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0,0,1));
//     fParticleGun->GeneratePrimaryVertex(anEvent);

    G4ThreeVector momDir1;
    momDir1.setRThetaPhi(1.,-9.5*deg,0*deg);
    fParticleGun->SetParticleMomentumDirection(momDir1);
    fParticleGun->GeneratePrimaryVertex(anEvent);

    G4ThreeVector momDir2;
    momDir2.setRThetaPhi(1.,-8.5*deg,0*deg);
    fParticleGun->SetParticleMomentumDirection(momDir2);
    fParticleGun->GeneratePrimaryVertex(anEvent);

    G4ThreeVector momDir3;
    momDir3.setRThetaPhi(1.,-6.5*deg,0*deg);
    fParticleGun->SetParticleMomentumDirection(momDir3);
    fParticleGun->GeneratePrimaryVertex(anEvent);
    
    G4ThreeVector momDir4;
    momDir4.setRThetaPhi(1.,-4.5*deg,0*deg);
    fParticleGun->SetParticleMomentumDirection(momDir4);
    fParticleGun->GeneratePrimaryVertex(anEvent);

    G4ThreeVector momDir5;
    momDir5.setRThetaPhi(1.,-2.5*deg,0*deg);
    fParticleGun->SetParticleMomentumDirection(momDir5);
    fParticleGun->GeneratePrimaryVertex(anEvent);

    G4ThreeVector momDir6;
    momDir6.setRThetaPhi(1.,2.5*deg,0*deg);
    fParticleGun->SetParticleMomentumDirection(momDir6);
    fParticleGun->GeneratePrimaryVertex(anEvent);

    G4ThreeVector momDir7;
    momDir7.setRThetaPhi(1.,4.5*deg,0*deg);
    fParticleGun->SetParticleMomentumDirection(momDir7);
    fParticleGun->GeneratePrimaryVertex(anEvent);


  }
  
  if(WITH_TGT){

    // Initial beam position uniformly spread on a disk
    //
   G4double z0=-gun_z_pos;
   G4double x0=0;
   G4double y0=0;
   
   // beam size is 2x2 mm^2 raster
   G4double sizeMax = 0.2; 

   x0 = CLHEP::RandFlat::shoot(-sizeMax,sizeMax);
   y0 = CLHEP::RandFlat::shoot(-sizeMax,sizeMax);


   G4double raster_x = gun_x_pos +x0;
   G4double raster_y = gun_y_pos +y0;

   if(DEBUG){
     G4cout<<"$$$$ x0 "<<x0<<G4endl;
     G4cout<<"$$$$ y0 "<<y0<<G4endl;
     G4cout<<"$$$$ z0 "<<z0<<G4endl;
     G4cout<<"$$$$ raster x "<<raster_x<<G4endl;
     G4cout<<"$$$$ raster y "<<raster_y<<G4endl;
   }

   //    fParticleGun->SetParticlePosition(G4ThreeVector(x0+gun_x_pos,y0+gun_y_pos,z0));
   fParticleGun->SetParticlePosition(G4ThreeVector(raster_x*cm,raster_y*cm,z0*cm));

   G4ThreeVector momDir;
   momDir.setRThetaPhi(1.,-8.5*deg,0*deg);
   fParticleGun->SetParticleMomentumDirection(momDir);
   fParticleGun->GeneratePrimaryVertex(anEvent);


  }
  
}

