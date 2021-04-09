#include "LArSIMpleTrueNeutrinoEvent.hh"
#include "LArSIMpleTrueParticle.hh"

LArSIMpleTrueNeutrinoEvent::LArSIMpleTrueNeutrinoEvent()
{

}

LArSIMpleTrueNeutrinoEvent::~LArSIMpleTrueNeutrinoEvent()
{

}

void LArSIMpleTrueNeutrinoEvent::AddNeutrino(LArSIMpleTrueParticle neutrino)
{
  fNeutrino = neutrino;
  this->SetFlavour();
}

void LArSIMpleTrueNeutrinoEvent::AddNeutrino(G4ThreeVector pos, G4ThreeVector dir, double energy, int pdg)
{
  this->AddNeutrino(LArSIMpleTrueParticle(pos,dir,energy,pdg));
}

void LArSIMpleTrueNeutrinoEvent::AddTarget(G4ThreeVector pos, G4ThreeVector dir, double energy, int pdg)
{
  this->AddTarget(LArSIMpleTrueParticle(pos,dir,energy,pdg));
}

void LArSIMpleTrueNeutrinoEvent::AddFinalStateParticle(G4ThreeVector pos, G4ThreeVector dir, double energy, int pdg)
{
  this->AddFinalStateParticle(LArSIMpleTrueParticle(pos,dir,energy,pdg));
}

void LArSIMpleTrueNeutrinoEvent::SetFlavour()
{
  const int pdg = fNeutrino.GetPDGCode();
  switch(pdg)
  {
    case -16 : fFlavour = LArSIMpleNeutrinoFlavourType::kAntiNuTau; break;
    case -14 : fFlavour = LArSIMpleNeutrinoFlavourType::kAntiNuMu; break;
    case -12 : fFlavour = LArSIMpleNeutrinoFlavourType::kAntiNuE; break;
    case 12  : fFlavour = LArSIMpleNeutrinoFlavourType::kNuE; break;
    case 14  : fFlavour = LArSIMpleNeutrinoFlavourType::kNuMu; break;
    case 16  : fFlavour = LArSIMpleNeutrinoFlavourType::kNuTau; break;
  }
}

unsigned int LArSIMpleTrueNeutrinoEvent::GetNParticles(const int pdg) const
{
  unsigned int nMatches(0);

  for(const LArSIMpleTrueParticle &part : fFinalStateParticles)
  {
    if(part.GetPDGCode() == pdg)
      ++nMatches;
  }

  return nMatches;
}


