#include "LArSIMpleTrueParticle.hh"

#include "G4ThreeVector.hh"

LArSIMpleTrueParticle::LArSIMpleTrueParticle() : 
  fPosition(G4ThreeVector()), fDirection(G4ThreeVector()), fEnergy(0.0), fPDGCode(0)
{

}

LArSIMpleTrueParticle::LArSIMpleTrueParticle(const G4ThreeVector &pos, const G4ThreeVector &dir, const double energy, const int pdg) :
  fPosition(pos), fDirection(dir), fEnergy(energy), fPDGCode(pdg)
{

}

LArSIMpleTrueParticle::~LArSIMpleTrueParticle()
{

}

