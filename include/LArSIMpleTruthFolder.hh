/**
 *  @file   LArSIMple/include/LArSIMpleTruthFolder.hh
 * 
 *  @brief  Header file for the truth folding class
 * 
 *  $Log: $
 */

#ifndef LArSIMpleTruthFolder_h
#define LArSIMpleTruthFolder_h

#include <map>

#include "LArSIMpleProcessTable.hh"
#include "LArSIMpleTrackData.hh"

#include "G4Track.hh"

class LArSIMpleTrueNeutrinoEvent;

/**
 *  @brief Class that handles folding back truth information from particles to their parents
 */
class LArSIMpleTruthFolder
{
public:
    /**
     *  @brief  Constructor
     */
    LArSIMpleTruthFolder();

    /**
     *  @brief  Destructor
     */
    virtual ~LArSIMpleTruthFolder();

    /**
     *  @brief  Set whether or not to perform truth folding for delta rays (including pair production)
     *
     *  @param  val whether to performing truth folding or not
     */
    void SetFoldDeltaRays(const bool val);

    /**
     *  @brief  Add an association between a track and its folded ancestor track
     *
     *  @param  trackID the track id of the track
     *  @param  foldedTrackID the track id of the ancestor track
     */
    void AddFoldedTrackAssoc(const int trackID, const int foldedTrackID);

    /**
     *  @brief  Get the track id of the folded ancestor track for a given track id
     *
     *  @param  trackID the track id of the track
     *
     *  @return The track id of the ancestor track
     */
    int GetFoldedTrackID(const int trackID) const;

    /**
     *  @brief  Check if a track can be folded back to its ancestors using truth information
     *
     *  @param  track the track in question
     *
     *  @return Whether or not the track can be folded back
     */
    bool CanTrackBeFolded(const LArSIMpleTrackData &track);

    /**
     *  @brief  Perform the folding back to the first ancestor that can't be folded
     *
     *  @param  track the track that needs folding
     *  @param  the map of track ids to the track data objects
     *  @param  foldedTrackID to take the value of the folded ancestor track id
     *  @param  foldedTrackPDG to take the value of the folded ancestor pdg code
     *  @param  foldedTrackProcess to take the value of the folded ancestor process code
     */
    void PerformTrackFolding(const G4Track *track, const std::map<int,LArSIMpleTrackData> &trackIDToTrackDataMap, int &foldedTrackID, int &foldedTrackPDG, int &foldedTrackProcess);

    /**
     *  @brief  Reset the internal members ready for the next event
     */
    void Reset();
private:
    std::map<int, int> fTrackIDToFoldedTrackID;   ///< Map of track id to the folded ancestor track id
    std::map<int, bool> fTrackIDToCanFold;        ///< Map of track id to whether it can be folded

    bool fFoldDeltaRays;                          ///< Whether or not to fold delta rays (including pair produced delta rays)
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMpleTruthFolder::SetFoldDeltaRays(const bool val)
{
    fFoldDeltaRays = val;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMpleTruthFolder::AddFoldedTrackAssoc(const int trackID, const int foldedTrackID)
{
    if (fTrackIDToFoldedTrackID.count(trackID) == 0)
        fTrackIDToFoldedTrackID.insert(std::make_pair(trackID, foldedTrackID));
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline int LArSIMpleTruthFolder::GetFoldedTrackID(const int trackID) const
{
    if (fTrackIDToFoldedTrackID.count(trackID))
        return fTrackIDToFoldedTrackID.at(trackID);
    else
        return -1;
} 

inline void LArSIMpleTruthFolder::Reset()
{
    fTrackIDToFoldedTrackID.clear();
    fTrackIDToCanFold.clear();
} 

#endif
