/**
 *  @file   LArSIMple/include/LArSIMpleTrackData.hh
 * 
 *  @brief  Header file for the track data class.
 * 
 *  $Log: $
 */

#ifndef LArSIMpleTrackData_h
#define LArSIMpleTrackData_h

#include "LArSIMpleProcessTable.hh"

#include "G4ThreeVector.hh"

#include <string>

class G4Track;

/**
 *  @brief Class that stores additional information about the true track objects
 */
class LArSIMpleTrackData
{
public:
    /**
     *  @brief  Default constructor
     */
    LArSIMpleTrackData();

    /**
     *  @brief  Constructor using a G4Track object
     *
     *  @param  track the GEANT4 track
     */
    LArSIMpleTrackData(const G4Track *track);

    /**
     *  @brief  Copy constructor
     *
     *  @param  rhs LArSIMpleTrackData object to copy
     */
    LArSIMpleTrackData(const LArSIMpleTrackData &rhs);

    /**
     *  @brief  Assignment operator
     *
     *  @param  rhs LArSIMpleTrackData object to compare
     */
    LArSIMpleTrackData operator=(const LArSIMpleTrackData &rhs);

    /**
     *  @brief  Destructor
     */
    ~LArSIMpleTrackData();

    /**
     *  @brief  Set the track end point information
     *
     *  @param  track the G4Track to provide the end point information
     */
    void SetEndPointInfo(const G4Track *track);

    /**
     *  @brief  Get the GEANT4 track id of the associated G4Track
     *
     *  @return The GEANT4 track id
     */
    int GetTrackID() const;

    /**
     *  @brief  Get the GEANT4 track id of the parent particle
     *
     *  @return The GEANT4 track id of the parent particle
     */
    int GetParentID() const;

    /**
     *  @brief  Get the PDG code of the track
     *
     *  @return The PDG code
     */
    int GetPDG() const;

    /**
     *  @brief  Set the particle as a primary particle. This is needed for decay photons of primary pi0s
     */
    void SetPrimary();

    /**
     *  @brief  Query if the true track is a primary particle or not
     *
     *  @return whether the track is primary or not
     */
    bool IsPrimary() const;

    /**
     *  @brief  Get the creation process of the track
     *
     *  @return The creation process string
     */
    std::string GetProcess() const;

    /**
     *  @brief  Get the creation process code of the track
     *
     *  @return The creation process code
     */
    LArSIMpleProcessTable::ProcessCode GetProcessCode() const;

    /**
     *  @brief  Get the mass of the particle
     *
     *  @return The particle mass
     */
    double GetMass() const;

    /**
     *  @brief  Get the vertex position of the track
     *
     *  @return The vertex position
     */
    G4ThreeVector GetVertexPosition() const;

    /**
     *  @brief  Get the vertex direction of the track
     *
     *  @return The vertex direction
     */
    G4ThreeVector GetVertexDirection() const;

    /**
     *  @brief  Get the kinetic energy at the track vertex
     *
     *  @return The kinetic energy at the vertex point
     */
    double GetVertexKineticEnergy() const;

    /**
     *  @brief  Get the momentum at the track vertex
     *
     *  @return The momentum at the vertex point
     */
    double GetVertexMomentum() const;

    /**
     *  @brief  Get the end position of the track
     *
     *  @return The end position
     */
    G4ThreeVector GetEndPosition() const;

    /**
     *  @brief  Get the end direction of the track
     *
     *  @return The end direction
     */
    G4ThreeVector GetEndDirection() const;

    /**
     *  @brief  Get the kinetic energy at the end position of the track
     *
     *  @return The kinetic energy at the track end point
     */
    double GetEndKineticEnergy() const;

    /**
     *  @brief  Get the momentum at the end position of the track
     *
     *  @return The momentum at the track end point
     */
    double GetEndMomentum() const;

    /**
     *  @brief  Get whether the track truth be folded back to associate all particles in a shower to the particle that initiated the shower
     *
     *  @return Whether the track truth be folded back
     */
    bool IsFoldable() const;

private:
    /**
     *  @brief  Can the track truth be folded back to associate all particles in a shower to the particle that initiated the shower
     *
     *  @param  track the GEANT4 track to attempt to fold back
     *
     *  @return Whether the track truth be folded back
     */
    bool CanTrackBeFolded() const;

    int fTrackID;                                    ///< The track id of the associated G4Track object
    int fParentID;                                   ///< The track id of the parent particle
    int fPDG;                                        ///< The PDG code of this track
    bool fIsPrimary;                                 ///< Is this a primary particle?
    std::string fProcess;                            ///< The process that created this track as a string
    LArSIMpleProcessTable::ProcessCode fProcessCode; ///< The process that created this track as a process code
    double fMass;
    G4ThreeVector fVertexPosition;
    G4ThreeVector fVertexDirection;
    double fVertexKineticEnergy;
    double fVertexMomentum;
    G4ThreeVector fEndPosition;
    G4ThreeVector fEndDirection;
    double fEndKineticEnergy;
    double fEndMomentum;

    bool fIsFoldable; ///< Whether the truth information can be folded back
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline int LArSIMpleTrackData::GetTrackID() const
{
    return fTrackID;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline int LArSIMpleTrackData::GetParentID() const
{
    return fParentID;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline int LArSIMpleTrackData::GetPDG() const
{
    return fPDG;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMpleTrackData::SetPrimary()
{
    fIsPrimary = true;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline bool LArSIMpleTrackData::IsPrimary() const
{
    return fIsPrimary;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline std::string LArSIMpleTrackData::GetProcess() const
{
    return fProcess;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline LArSIMpleProcessTable::ProcessCode LArSIMpleTrackData::GetProcessCode() const
{
    return fProcessCode;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline double LArSIMpleTrackData::GetMass() const
{
    return fMass;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline G4ThreeVector LArSIMpleTrackData::GetVertexPosition() const
{
    return fVertexPosition;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline G4ThreeVector LArSIMpleTrackData::GetVertexDirection() const
{
    return fVertexDirection;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline double LArSIMpleTrackData::GetVertexKineticEnergy() const
{
    return fVertexKineticEnergy;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline double LArSIMpleTrackData::GetVertexMomentum() const
{
    return fVertexMomentum;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline G4ThreeVector LArSIMpleTrackData::GetEndPosition() const
{
    return fEndPosition;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline G4ThreeVector LArSIMpleTrackData::GetEndDirection() const
{
    return fEndDirection;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline double LArSIMpleTrackData::GetEndKineticEnergy() const
{
    return fEndKineticEnergy;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline double LArSIMpleTrackData::GetEndMomentum() const
{
    return fEndMomentum;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline bool LArSIMpleTrackData::IsFoldable() const
{
    return fIsFoldable;
}

#endif
