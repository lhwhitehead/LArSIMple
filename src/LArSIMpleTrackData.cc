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
    fProcessCode(LArSIMpleProcessTable::MC_PROC_UNKNOWN)
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
        std::cout << "Warning in LArSIMpleTrackData: no particle definition found for particle with PDG code " << fPDG
                  << ", setting mass to zero" << std::endl;
        fMass = 0.;
    }

    fVertexPosition = track->GetVertexPosition();
    fVertexDirection = track->GetVertexMomentumDirection();
    fVertexKineticEnergy = track->GetVertexKineticEnergy();
    fVertexMomentum = std::sqrt((fMass + fVertexKineticEnergy) * (fMass + fVertexKineticEnergy) - fMass * fMass);
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
//    fIsFoldable = rhs.IsFoldable();
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
