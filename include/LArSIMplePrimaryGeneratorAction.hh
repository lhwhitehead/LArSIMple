#ifndef LArSIMplePrimaryGeneratorAction_h
#define LArSIMplePrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4ParticleGun;
class G4GeneralParticleSource;
class G4Event;

class LArSIMplePrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {

public:
  LArSIMplePrimaryGeneratorAction();
  ~LArSIMplePrimaryGeneratorAction();
  
public:
  void GeneratePrimaries(G4Event* anEvent);
  
private:
  G4GeneralParticleSource* particleGun;
  
};

#endif

