#include "LArSIMplePrimaryGeneratorAction.hh"

#include "Randomize.hh"

#include "G4GeneralParticleSource.hh"
#include "G4ParticleGun.hh"
#include "G4Event.hh"
#include "globals.hh"


LArSIMplePrimaryGeneratorAction::LArSIMplePrimaryGeneratorAction() {
  
  particleGun = new G4GeneralParticleSource();
  //particleGun = new G4ParticleGun(1);

}


LArSIMplePrimaryGeneratorAction::~LArSIMplePrimaryGeneratorAction() {

  delete particleGun;
}


void LArSIMplePrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {

  particleGun->GeneratePrimaryVertex(anEvent);

}

