/**
 *  @file   LArSIMple/include/LArSIMpleTrueNeutrinoEvent.hh
 * 
 *  @brief  Header file for the true neutrino event class.
 * 
 *  $Log: $
 */

#ifndef LArSIMpleTrueNeutrinoEvent_h
#define LArSIMpleTrueNeutrinoEvent_h

#include "G4ThreeVector.hh"
#include "LArSIMpleTrueParticle.hh"
#include <vector>

/**
 *  @brief Enumeration for neutrino flavour types
 */
enum class LArSIMpleNeutrinoFlavourType
{
    kNuE,
    kNuMu,
    kNuTau,
    kAntiNuE,
    kAntiNuMu,
    kAntiNuTau
};

/**
 *  @brief Enumeration for true neutrino interaction types
 */
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

/**
 *  @brief Class representing a neutrino interaction and all interaction products
 */
class LArSIMpleTrueNeutrinoEvent
{
public:
    /**
     *  @brief  Default constructor
     */
    LArSIMpleTrueNeutrinoEvent();

    /**
     *  @brief  Destructor
     */
    ~LArSIMpleTrueNeutrinoEvent();

    void AddNeutrino(LArSIMpleTrueParticle neutrino);
    void AddNeutrino(G4ThreeVector pos, G4ThreeVector dir, double energy, int pdg);
    void AddTarget(LArSIMpleTrueParticle target);
    void AddTarget(G4ThreeVector pos, G4ThreeVector dir, double energy, int pdg);
    void AddFinalStateParticle(LArSIMpleTrueParticle particle);
    void AddFinalStateParticle(G4ThreeVector pos, G4ThreeVector dir, double energy, int pdg);

    void SetInteractionVertex(G4ThreeVector pos);
    G4ThreeVector GetInteractionVertex() const;

    int GetNeutrinoPDGCode() const;
    float GetNeutrinoEnergy() const;
    LArSIMpleNeutrinoFlavourType GetNeutrinoFlavour() const;
    LArSIMpleNeutrinoInteractionType GetInteractionType() const;
    void SetInteractionType(const LArSIMpleNeutrinoInteractionType intType);

    unsigned int GetNumberOfFinalStateParticles() const;
    std::vector<LArSIMpleTrueParticle> GetFinalStateParticles() const;
    LArSIMpleTrueParticle GetLeadingLepton() const;

    unsigned int GetNProtons() const;
    unsigned int GetNPiPlus() const;
    unsigned int GetNPiMinus() const;
    unsigned int GetNPiZero() const;
    unsigned int GetNNeutrons() const;
    unsigned int GetNPhotons() const;

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

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMpleTrueNeutrinoEvent::AddTarget(LArSIMpleTrueParticle target)
{
    fTarget = target;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMpleTrueNeutrinoEvent::AddTarget(G4ThreeVector pos, G4ThreeVector dir, double energy, int pdg)
{
    this->AddTarget(LArSIMpleTrueParticle(pos, dir, energy, pdg));
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMpleTrueNeutrinoEvent::AddFinalStateParticle(LArSIMpleTrueParticle particle)
{
    fFinalStateParticles.emplace_back(particle);
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMpleTrueNeutrinoEvent::SetInteractionVertex(G4ThreeVector pos)
{
    fInteractionVertex = pos;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline G4ThreeVector LArSIMpleTrueNeutrinoEvent::GetInteractionVertex() const
{
    return fInteractionVertex;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline int LArSIMpleTrueNeutrinoEvent::GetNeutrinoPDGCode() const
{
    return fNeutrino.GetPDGCode();
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline float LArSIMpleTrueNeutrinoEvent::GetNeutrinoEnergy() const
{
    return fNeutrino.GetEnergy();
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline LArSIMpleNeutrinoFlavourType LArSIMpleTrueNeutrinoEvent::GetNeutrinoFlavour() const
{
    return fFlavour;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline LArSIMpleNeutrinoInteractionType LArSIMpleTrueNeutrinoEvent::GetInteractionType() const
{
    return fInteractionType;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMpleTrueNeutrinoEvent::SetInteractionType(const LArSIMpleNeutrinoInteractionType intType)
{
    fInteractionType = intType;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline unsigned int LArSIMpleTrueNeutrinoEvent::GetNumberOfFinalStateParticles() const
{
    return fFinalStateParticles.size();
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline std::vector<LArSIMpleTrueParticle> LArSIMpleTrueNeutrinoEvent::GetFinalStateParticles() const
{
    return fFinalStateParticles;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline unsigned int LArSIMpleTrueNeutrinoEvent::GetNProtons() const
{
    return this->GetNParticles(2212);
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline unsigned int LArSIMpleTrueNeutrinoEvent::GetNPiPlus() const
{
    return this->GetNParticles(211);
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline unsigned int LArSIMpleTrueNeutrinoEvent::GetNPiMinus() const
{
    return this->GetNParticles(-211);
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline unsigned int LArSIMpleTrueNeutrinoEvent::GetNPiZero() const
{
    return this->GetNParticles(111);
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline unsigned int LArSIMpleTrueNeutrinoEvent::GetNNeutrons() const
{
    return this->GetNParticles(2112);
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline unsigned int LArSIMpleTrueNeutrinoEvent::GetNPhotons() const
{
    return this->GetNParticles(22);
}

#endif
