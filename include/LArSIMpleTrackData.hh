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
    bool CanTrackBeFolded(const G4Track *track) const;

    int fTrackID;                                       ///< The track id of the associated G4Track object
    int fParentID;                                      ///< The track id of the parent particle
    int fPDG;                                           ///< The PDG code of this track
    std::string fProcess;                               ///< The process that created this track as a string
    LArSIMpleProcessTable::ProcessCode fProcessCode;    ///< The process that created this track as a process code
    bool fIsFoldable;                                   ///< Whether the truth information can be folded back
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

inline bool LArSIMpleTrackData::IsFoldable() const
{
    return fIsFoldable;
}

#endif
