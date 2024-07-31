/**
 *  @file   LArSIMple/include/LArSIMpleTrueParticle.hh
 * 
 *  @brief  Header file for the true particle class.
 * 
 *  $Log: $
 */


#ifndef LArSIMpleTrueParticle_h
#define LArSIMpleTrueParticle_h

#include "G4ThreeVector.hh"

/**
 *  @brief Class representing a true particle from a neutrino input file
 */
class LArSIMpleTrueParticle
{
public:
    /**
     *  @brief  Default constructor
     */
    LArSIMpleTrueParticle();

    /**
     *  @brief  Constructor with position, direction, energy and pdg code
     *
     *  @param  pos the position of the particle
     *  @param  dir the direction of the particle
     *  @param  energy the energy of the particle
     *  @param  pdg the PDG code of the particle
     */
    LArSIMpleTrueParticle(const G4ThreeVector &pos, const G4ThreeVector &dir, const double energy, const int pdg);

    /**
     *  @brief  Destructor
     */
    ~LArSIMpleTrueParticle();

    /**
     *  @brief  Get the position vector of the particle
     *
     *  @return The position of the particle
     */
    G4ThreeVector GetPosition() const;

    /**
     *  @brief  Get the direction vector of the particle
     *
     *  @return The direction of the particle
     */
    G4ThreeVector GetDirection() const;

    /**
     *  @brief  Get the energy of the particle
     *
     *  @return The energy of the particle
     */
    double GetEnergy() const;

    /**
     *  @brief  Get the PDG code of the particle
     *
     *  @return The PDG code of the particle
     */
    int GetPDGCode() const;

private:
    G4ThreeVector fPosition;
    G4ThreeVector fDirection;
    double fEnergy;
    int fPDGCode;
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline G4ThreeVector LArSIMpleTrueParticle::GetPosition() const
{
    return fPosition;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline G4ThreeVector LArSIMpleTrueParticle::GetDirection() const
{
    return fDirection;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline double LArSIMpleTrueParticle::GetEnergy() const
{
    return fEnergy;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline int LArSIMpleTrueParticle::GetPDGCode() const
{
    return fPDGCode;
}

#endif
