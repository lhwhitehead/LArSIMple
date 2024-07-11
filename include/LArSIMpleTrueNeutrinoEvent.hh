#ifndef LArSIMpleTrueNeutrinoEvent_h
#define LArSIMpleTrueNeutrinoEvent_h

#include "G4ThreeVector.hh"
#include "LArSIMpleTrueParticle.hh"
#include <vector>
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
    kNCOther,
    // Not in nuance codes, but we can include them?
    kCC2P2H,
    kDummy = 999
};

class LArSIMpleTrueNeutrinoEvent
{

public:
    LArSIMpleTrueNeutrinoEvent();
    ~LArSIMpleTrueNeutrinoEvent();

    void AddNeutrino(LArSIMpleTrueParticle neutrino);
    void AddNeutrino(G4ThreeVector pos, G4ThreeVector dir, double energy, int pdg);
    void AddTarget(LArSIMpleTrueParticle target)
    {
        fTarget = target;
    };
    void AddTarget(G4ThreeVector pos, G4ThreeVector dir, double energy, int pdg);
    void AddFinalStateParticle(LArSIMpleTrueParticle particle)
    {
        fFinalStateParticles.emplace_back(particle);
    };
    void AddFinalStateParticle(G4ThreeVector pos, G4ThreeVector dir, double energy, int pdg);

    void SetInteractionVertex(G4ThreeVector pos)
    {
        fInteractionVertex = pos;
    };
    G4ThreeVector GetInteractionVertex() const
    {
        return fInteractionVertex;
    };

    int GetNeutrinoPDGCode() const
    {
        return fNeutrino.GetPDGCode();
    };
    float GetNeutrinoEnergy() const
    {
        return fNeutrino.GetEnergy();
    };
    LArSIMpleNeutrinoFlavourType GetNeutrinoFlavour() const
    {
        return fFlavour;
    };
    LArSIMpleNeutrinoInteractionType GetInteractionType() const
    {
        return fInteractionType;
    };
    void SetInteractionType(const LArSIMpleNeutrinoInteractionType intType)
    {
        fInteractionType = intType;
    };

    unsigned int GetNumberOfFinalStateParticles() const
    {
        return fFinalStateParticles.size();
    };
    std::vector<LArSIMpleTrueParticle> GetFinalStateParticles() const
    {
        return fFinalStateParticles;
    };
    LArSIMpleTrueParticle GetLeadingLepton() const;

    unsigned int GetNProtons() const
    {
        return this->GetNParticles(2212);
    };
    unsigned int GetNPiPlus() const
    {
        return this->GetNParticles(211);
    };
    unsigned int GetNPiMinus() const
    {
        return this->GetNParticles(-211);
    };
    unsigned int GetNPiZero() const
    {
        return this->GetNParticles(111);
    };
    unsigned int GetNNeutrons() const
    {
        return this->GetNParticles(2112);
    };
    unsigned int GetNPhotons() const
    {
        return this->GetNParticles(22);
    };

    void Print() const;

private:
    void SetFlavour();
    unsigned int GetNParticles(const int pdg) const;

    LArSIMpleTrueParticle fNeutrino;
    LArSIMpleTrueParticle fTarget;

    G4ThreeVector fInteractionVertex;
    LArSIMpleNeutrinoFlavourType fFlavour;
    LArSIMpleNeutrinoInteractionType fInteractionType;

    std::vector<LArSIMpleTrueParticle> fFinalStateParticles;
};

#endif
