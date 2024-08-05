/**
 *  @file LArSIMple/src/LArSIMpleTrackData.cc
 * 
 *  @brief Implementation of the track data class.
 * 
 *  $Log: $
 */

#include <string>

#include "LArSIMpleTrackData.hh"

#include "G4Track.hh"
#include "G4VProcess.hh"

LArSIMpleTrackData::LArSIMpleTrackData() :
    fTrackID(0),
    fParentID(0),
    fPDG(0),
    fProcess(""),
    fProcessCode(LArSIMpleProcessTable::MC_PROC_UNKNOWN),
    fIsFoldable(false)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

LArSIMpleTrackData::LArSIMpleTrackData(const G4Track *track)
{
    fTrackID = track->GetTrackID();
    fParentID = track->GetParentID();
    fPDG = track->GetParticleDefinition()->GetPDGEncoding();
    if (fParentID == 0)
        fProcess = "primary";
    else
        fProcess = track->GetCreatorProcess()->GetProcessName();
    fProcessCode = LArSIMpleProcessTable::Get().GetProcessCodeFromString(fProcess);
    fIsFoldable = this->CanTrackBeFolded();
}

//------------------------------------------------------------------------------------------------------------------------------------------

LArSIMpleTrackData::LArSIMpleTrackData(const LArSIMpleTrackData &rhs)
{
    fTrackID = rhs.GetTrackID();
    fParentID = rhs.GetParentID();
    fPDG = rhs.GetPDG();
    fProcess = rhs.GetProcess();
    fProcessCode = rhs.GetProcessCode();
    fIsFoldable = rhs.IsFoldable();
}

//------------------------------------------------------------------------------------------------------------------------------------------

LArSIMpleTrackData LArSIMpleTrackData::operator=(const LArSIMpleTrackData &rhs)
{
    return LArSIMpleTrackData(rhs);
}

//------------------------------------------------------------------------------------------------------------------------------------------

LArSIMpleTrackData::~LArSIMpleTrackData()
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

bool LArSIMpleTrackData::CanTrackBeFolded() const
{
    // Primary particle first
    if (fParentID == 0)
        return false;
    else
    {
        // Check if secondaries have processes we don't want to consider as particles
        if (fProcessCode == LArSIMpleProcessTable::MC_PROC_CONV /*|| fProcess == find("LowEnConversion") != std::string::npos ||
            fProcess.find("Pair") != std::string::npos*/ || fProcessCode == LArSIMpleProcessTable::MC_PROC_COMPT ||
            fProcessCode == LArSIMpleProcessTable::MC_PROC_E_BREM || fProcessCode == LArSIMpleProcessTable::MC_PROC_MU_BREM ||
            fProcessCode == LArSIMpleProcessTable::MC_PROC_HAD_BREM || fProcessCode == LArSIMpleProcessTable::MC_PROC_PHOT ||
            fProcessCode == LArSIMpleProcessTable::MC_PROC_PHOTON_INELASTIC || fProcessCode == LArSIMpleProcessTable::MC_PROC_PHOTON_NUCLEAR ||
            fProcessCode == LArSIMpleProcessTable::MC_PROC_E_IONI || fProcessCode == LArSIMpleProcessTable::MC_PROC_MU_IONI ||
            fProcessCode == LArSIMpleProcessTable::MC_PROC_HAD_IONI || fProcessCode == LArSIMpleProcessTable::MC_PROC_ION_IONI ||
            fProcessCode == LArSIMpleProcessTable::MC_PROC_ANNIHIL || fProcessCode == LArSIMpleProcessTable::MC_PROC_MU_MINUS_CAPTURE_AT_REST ||
            fProcessCode == LArSIMpleProcessTable::MC_PROC_RADIOACTIVE_DECAY)
            /*fProcess.find("Radioactive") != std::string::npos*/
        {
            return true;
        }
        // Otherwise these are particles that we want to keep
        else
            return false;
    }
}
