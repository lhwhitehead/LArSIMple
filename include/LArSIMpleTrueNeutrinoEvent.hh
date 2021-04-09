#ifndef LArSIMpleTrueNeutrinoEvent_h
#define LArSIMpleTrueNeutrinoEvent_h

#include <vector>
#include "LArSIMpleTrueParticle.hh"

enum class LArSIMpleNeutrinoFlavourType
{
  kNuE,
  kNuMu,
  kNuTau,
  kAntiNuE,
  kAntiNuMu,
  kAntiNuTau
};

enum class LArSIMpleNeutrinoInteractionType
{
  kCCQE,
  kCCRES,
  kCCDIS,
  kCCOther,
  kNCQE,
  kNCRES,
  kNCDIS,
  kNCOther
};

class LArSIMpleTrueNeutrinoEvent
{

  public:
  LArSIMpleTrueNeutrinoEvent();
  ~LArSIMpleTrueNeutrinoEvent();

  void AddNeutrino(const LArSIMpleTrueParticle neutrino) {fNeutrino = neutrino;};
  void AddNeutrino(const G4ThreeVector &pos, const G4ThreeVector &dir, const double energy, const int pdg);
  void AddTarget(const LArSIMpleTrueParticle target) {fTarget = target;};
  void AddTarget(const G4ThreeVector &pos, const G4ThreeVector &dir, const double energy, const int pdg);
  void AddFinalStateParticle(const LArSIMpleTrueParticle particle) {fFinalStateParticles.push_back(particle);};
  void AddFinalStateParticle(const G4ThreeVector &pos, const G4ThreeVector &dir, const double energy, const int pdg);

  LArSIMpleNeutrinoFlavourType GetNeutrinoFlavour() const {return fFlavour;};
  LArSIMpleNeutrinoInteractionType GetInteractionType() const {return fInteractionType;};
  void SetInteractionType(const LArSIMpleNeutrinoInteractionType intType) {fInteractionType = intType;};

  unsigned int GetNumberOfFinalStateParticles() const {return fFinalStateParticles.size();};
  LArSIMpleTrueParticle GetLeadingLepton() const;

  unsigned int GetNProtons() const  {return this->GetNParticles(2212);};
  unsigned int GetNPiPlus() const   {return this->GetNParticles(211);};
  unsigned int GetNPiMinus() const  {return this->GetNParticles(-211);};
  unsigned int GetNPiZero() const   {return this->GetNParticles(111);};
  unsigned int GetNNeutrons() const {return this->GetNParticles(2112);};
  unsigned int GetNPhotons() const  {return this->GetNParticles(22);};

  private:

  void SetFlavour();
  unsigned int GetNParticles(const int pdg) const;

  LArSIMpleTrueParticle fNeutrino;
  LArSIMpleTrueParticle fTarget;

  LArSIMpleNeutrinoFlavourType fFlavour;
  LArSIMpleNeutrinoInteractionType fInteractionType;

  std::vector<LArSIMpleTrueParticle> fFinalStateParticles;

};

#endif

