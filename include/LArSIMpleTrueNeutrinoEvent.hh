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

    /**
     *  @brief  Add a neutrino to the true event
     *
     *  @param  neutrino the neutrino to add
     */
    void AddNeutrino(LArSIMpleTrueParticle neutrino);

    /**
     *  @brief  Add a neutrino to the true event
     *
     *  @param  pos the neutrino position
     *  @param  dir the neutrino direction
     *  @param  energy the neutrino energy
     *  @param  pdg the pdg code of the neutrino
     */
    void AddNeutrino(G4ThreeVector pos, G4ThreeVector dir, double energy, int pdg);

    /**
     *  @brief  Add the nuclear target to the true event
     *
     *  @param  target the nuclear target to add
     */
    void AddTarget(LArSIMpleTrueParticle target);

    /**
     *  @brief  Add the nuclear target to the true event
     *
     *  @param  pos the target position position
     *  @param  dir the target nucleus direction
     *  @param  energy the target nucleus energy
     *  @param  pdg the pdg code of the target nucleus
     */
    void AddTarget(G4ThreeVector pos, G4ThreeVector dir, double energy, int pdg);

    /**
     *  @brief  Add a final state particle to the true event
     *
     *  @param  particle the final state particle
     */
    void AddFinalStateParticle(LArSIMpleTrueParticle particle);

    /**
     *  @brief  Add a final state particle to the true event
     *
     *  @param  pos the particle position
     *  @param  dir the particle direction
     *  @param  energy the particle energy
     *  @param  pdg the particle pdg code
     */
    void AddFinalStateParticle(G4ThreeVector pos, G4ThreeVector dir, double energy, int pdg);

    /**
     *  @brief  Set the neutrino interaction vertex position
     *
     *  @param  pos the vertex position
     */
    void SetInteractionVertex(G4ThreeVector pos);

    /**
     *  @brief  Returns the neutrino interaction vertex position
     *
     *  @return the vertex position
     */
    G4ThreeVector GetInteractionVertex() const;

    /**
     *  @brief  Returns the neutrino PDG code
     *
     *  @return the PDG code
     */
    int GetNeutrinoPDGCode() const;

    /**
     *  @brief  Returns the neutrino energy
     *
     *  @return the neutrino energy
     */
    float GetNeutrinoEnergy() const;

    /**
     *  @brief  Returns the neutrino flavour
     *
     *  @return the neutrino flavour
     */
    LArSIMpleNeutrinoFlavourType GetNeutrinoFlavour() const;

    /**
     *  @brief  Returns the neutrino interaction type
     *
     *  @return the interaction type
     */
    LArSIMpleNeutrinoInteractionType GetInteractionType() const;

    /**
     *  @brief  Sets the neutrino interaction type
     *
     *  @param  intType the interaction type
     */
    void SetInteractionType(const LArSIMpleNeutrinoInteractionType intType);

    /**
     *  @brief  Returns the number of final-state particles
     *
     *  @return the number of final-state particles
     */
    unsigned int GetNumberOfFinalStateParticles() const;

    /**
     *  @brief  Returns the final-state particles
     *
     *  @return the final-state particles
     */
    std::vector<LArSIMpleTrueParticle> GetFinalStateParticles() const;

    /**
     *  @brief  Returns the leading lepton
     *
     *  @return the leading lepton
     */
    LArSIMpleTrueParticle GetLeadingLepton() const;

    /**
     *  @brief  Returns the number of final-state protons
     *
     *  @return the number of final-state protons
     */
    unsigned int GetNProtons() const;

    /**
     *  @brief  Returns the number of final-state pi+ mesons
     *
     *  @return the number of final-state pi+ mesons
     */
    unsigned int GetNPiPlus() const;

    /**
     *  @brief  Returns the number of final-state pi- mesons
     *
     *  @return the number of final-state pi- mesons
     */
    unsigned int GetNPiMinus() const;

    /**
     *  @brief  Returns the number of final-state pi0 mesons
     *
     *  @return the number of final-state pi0 mesons
     */
    unsigned int GetNPiZero() const;

    /**
     *  @brief  Returns the number of final-state neutrons
     *
     *  @return the number of final-state neutrons
     */
    unsigned int GetNNeutrons() const;

    /**
     *  @brief  Returns the number of final-state photons
     *
     *  @return the number of final-state photons
     */
    unsigned int GetNPhotons() const;

    /**
     *  @brief  Print a summary of the neutrino interaction
     */
    void Print() const;

private:
    /**
     *  @brief  Sets the neutrino flavour
     */
    void SetFlavour();

    /**
     *  @brief  Returns the number of final-state particles with a given PDG code
     *
     *  @param  pdg the pdg code of the particle type to count
     *
     *  @return the number of particles
     */
    unsigned int GetNParticles(const int pdg) const;

    LArSIMpleTrueParticle fNeutrino;                          ///< True particle representing the neutrino
    LArSIMpleTrueParticle fTarget;                            ///< True particle representing the target nucleon

    G4ThreeVector fInteractionVertex;                         ///< The neutrino interaction vertex position
    LArSIMpleNeutrinoFlavourType fFlavour;                    ///< The neutrino flavour
    LArSIMpleNeutrinoInteractionType fInteractionType;        ///< The neutrino interaction type

    std::vector<LArSIMpleTrueParticle> fFinalStateParticles;  ///< A vector of final-state particles
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
