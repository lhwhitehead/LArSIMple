/**
 *  @file LArSIMple/src/LArSIMpleTruthFolder.cc
 * 
 *  @brief Implementation of the truth folding class.
 * 
 *  $Log: $
 */

#include "LArSIMpleTruthFolder.hh"

#include "G4Track.hh"

LArSIMpleTruthFolder::LArSIMpleTruthFolder()
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

LArSIMpleTruthFolder::~LArSIMpleTruthFolder()
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

bool LArSIMpleTruthFolder::CanTrackBeFolded(const LArSIMpleTrackData &track)
{
    // Check if we have already looked at this track
    const int thisTrackID{track.GetTrackID()};
    if (fTrackIDToCanFold.count(thisTrackID))
        return fTrackIDToCanFold.at(thisTrackID);

    // Otherwise, calculate if the track can be folded
    bool canFold;
    if (track.GetParentID() == 0)
         canFold = false;
    else
    {
        const LArSIMpleProcessTable::ProcessCode processCode{track.GetProcessCode()};
        // Check if secondaries have processes we don't want to consider as particles
        if (processCode == LArSIMpleProcessTable::MC_PROC_CONV ||
            processCode == LArSIMpleProcessTable::MC_PROC_COMPT ||
            processCode == LArSIMpleProcessTable::MC_PROC_E_BREM ||
            processCode == LArSIMpleProcessTable::MC_PROC_MU_BREM ||
            processCode == LArSIMpleProcessTable::MC_PROC_HAD_BREM || 
            processCode == LArSIMpleProcessTable::MC_PROC_PHOT ||
            processCode == LArSIMpleProcessTable::MC_PROC_PHOTON_INELASTIC ||
            processCode == LArSIMpleProcessTable::MC_PROC_PHOTON_NUCLEAR ||
            processCode == LArSIMpleProcessTable::MC_PROC_E_IONI ||
            (fFoldDeltaRays && processCode == LArSIMpleProcessTable::MC_PROC_MU_IONI) ||
            (fFoldDeltaRays && processCode == LArSIMpleProcessTable::MC_PROC_MU_PAIR_PROD) ||
            processCode == LArSIMpleProcessTable::MC_PROC_HAD_IONI ||
            processCode == LArSIMpleProcessTable::MC_PROC_ION_IONI ||
            processCode == LArSIMpleProcessTable::MC_PROC_ANNIHIL ||
            processCode == LArSIMpleProcessTable::MC_PROC_MU_MINUS_CAPTURE_AT_REST ||
            processCode == LArSIMpleProcessTable::MC_PROC_RADIOACTIVE_DECAY)
        {
            canFold = true;
        }
        // Otherwise these are particles that we want to keep
        else
            canFold = false;
    }

    // Buffer result for future use
    if (!fTrackIDToCanFold.count(thisTrackID))
        fTrackIDToCanFold[thisTrackID] = canFold;

    return canFold;
}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMpleTruthFolder::PerformTrackFolding(const G4Track *track, const std::map<int,LArSIMpleTrackData> &trackIDToTrackDataMap, int &foldedTrackID, int &foldedTrackPDG, int &foldedTrackProcess)
{
    // Have we already folded this track before?
    const int initialTrackID{track->GetTrackID()};
    const int previouslyFoldedTrackID{this->GetFoldedTrackID(track->GetTrackID())};
    if (previouslyFoldedTrackID != -1)
    {
        const LArSIMpleTrackData &trkData = trackIDToTrackDataMap.at(previouslyFoldedTrackID);
        foldedTrackID = trkData.GetTrackID();
        foldedTrackPDG = trkData.GetPDG();
        foldedTrackProcess = static_cast<int>(trkData.GetProcessCode());
        return;
    }

    // We need to iterate through the parent links until we find a track that isn't foldable
    foldedTrackID = initialTrackID;
    bool keepFolding{this->CanTrackBeFolded(trackIDToTrackDataMap.at(foldedTrackID))};
    while (keepFolding)
    {
        const LArSIMpleTrackData &trkData = trackIDToTrackDataMap.at(foldedTrackID);
        if (this->CanTrackBeFolded(trkData))
        {
            foldedTrackID = trkData.GetParentID();
        }
        else
        {
            foldedTrackID = trkData.GetTrackID();
            foldedTrackPDG = trkData.GetPDG();
            foldedTrackProcess = static_cast<int>(trkData.GetProcessCode());
            fTrackIDToFoldedTrackID[initialTrackID] = foldedTrackID;
            keepFolding = false;
        }
    }
}

