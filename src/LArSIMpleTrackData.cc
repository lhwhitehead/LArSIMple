/**
 *  @file LArSIMple/src/LArSIMpleTrackData.cc
 * 
 *  @brief Implementation of the track data class.
 * 
 *  $Log: $
 */

#include <string>

#include "LArSIMpleTrackData.hh"

#include "G4ParticleTable.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"

LArSIMpleTrackData::LArSIMpleTrackData() :
    fTrackID(0),
    fParentID(0),
    fPDG(0),
    fIsPrimary(false),
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
    fIsPrimary = false;
    if (fParentID == 0)
    {
        fProcess = "primary";
        fIsPrimary = true;
    }
    else
        fProcess = track->GetCreatorProcess()->GetProcessName();
    fProcessCode = LArSIMpleProcessTable::Get().GetProcessCodeFromString(fProcess);

    G4ParticleDefinition *particle{G4ParticleTable::GetParticleTable()->FindParticle(fPDG)};
    if (particle != nullptr)
        fMass = particle->GetPDGMass();
    else
    {
        std::cout << "Warning in LArSIMpleTrackData: no particle definition found for particle with PDG code " << fPDG << ", setting mass to zero" << std::endl;
        fMass = 0.;
    }

    fVertexPosition = track->GetVertexPosition();
    fVertexDirection = track->GetVertexMomentumDirection();
    fVertexKineticEnergy = track->GetVertexKineticEnergy();
    fVertexMomentum = std::sqrt((fMass + fVertexKineticEnergy)*(fMass + fVertexKineticEnergy) - fMass*fMass);

    // Can we fold this back to the parent?
    fIsFoldable = this->CanTrackBeFolded(); 
}

//------------------------------------------------------------------------------------------------------------------------------------------

LArSIMpleTrackData::LArSIMpleTrackData(const LArSIMpleTrackData &rhs)
{
    fTrackID = rhs.GetTrackID();
    fParentID = rhs.GetParentID();
    fPDG = rhs.GetPDG();
    fIsPrimary = rhs.IsPrimary();
    fProcess = rhs.GetProcess();
    fProcessCode = rhs.GetProcessCode();
    fMass = rhs.GetMass();
    fVertexPosition = rhs.GetVertexPosition();
    fVertexDirection = rhs.GetVertexDirection();
    fVertexKineticEnergy = rhs.GetVertexKineticEnergy();
    fVertexMomentum = rhs.GetVertexMomentum();
    fEndPosition = rhs.GetEndPosition();
    fEndDirection = rhs.GetEndDirection();
    fEndKineticEnergy = rhs.GetEndKineticEnergy();
    fEndMomentum = rhs.GetEndMomentum();
    fIsFoldable = rhs.IsFoldable();
}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMpleTrackData::SetEndPointInfo(const G4Track *track)
{
    fEndPosition = track->GetPosition();
    fEndDirection = track->GetMomentumDirection();
    fEndKineticEnergy = track->GetKineticEnergy();
    fEndMomentum = track->GetMomentum().mag();
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
        if (fProcessCode == LArSIMpleProcessTable::MC_PROC_CONV ||
            fProcessCode == LArSIMpleProcessTable::MC_PROC_COMPT ||
            fProcessCode == LArSIMpleProcessTable::MC_PROC_E_BREM ||
            fProcessCode == LArSIMpleProcessTable::MC_PROC_E_PAIR_PROD ||
            fProcessCode == LArSIMpleProcessTable::MC_PROC_MU_BREM ||
            fProcessCode == LArSIMpleProcessTable::MC_PROC_HAD_BREM ||
            fProcessCode == LArSIMpleProcessTable::MC_PROC_PHOT ||
            fProcessCode == LArSIMpleProcessTable::MC_PROC_PHOTON_INELASTIC ||
            fProcessCode == LArSIMpleProcessTable::MC_PROC_PHOTON_NUCLEAR ||
            fProcessCode == LArSIMpleProcessTable::MC_PROC_E_IONI ||
            fProcessCode == LArSIMpleProcessTable::MC_PROC_MU_IONI ||
            fProcessCode == LArSIMpleProcessTable::MC_PROC_HAD_IONI ||
            fProcessCode == LArSIMpleProcessTable::MC_PROC_ION_IONI ||
            fProcessCode == LArSIMpleProcessTable::MC_PROC_ANNIHIL ||
            fProcessCode == LArSIMpleProcessTable::MC_PROC_MU_MINUS_CAPTURE_AT_REST ||
            fProcessCode == LArSIMpleProcessTable::MC_PROC_RADIOACTIVE_DECAY)
        {
            return true;
        }
        // Otherwise these are particles that we want to keep
        else
            return false;
    }
}
