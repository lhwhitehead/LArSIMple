#include <string>

#include "LArSIMpleTrackData.hh"

#include "G4Track.hh"
#include "G4VProcess.hh"

LArSIMpleTrackData::LArSIMpleTrackData()
{
    fTrackID = 0;
    fParentID = 0;
    fPDG = 0;
    fProcess = "";
    fProcessCode = LArSIMpleProcessTable::MC_PROC_UNKNOWN;
    fIsFoldable = false;
}

LArSIMpleTrackData::LArSIMpleTrackData(const G4Track *track)
{
    fTrackID = track->GetTrackID();
    fParentID = track->GetParentID();
    fPDG = track->GetParticleDefinition()->GetPDGEncoding();
    if (fParentID == 0)
        fProcess = "primary";
    else
        fProcess = track->GetCreatorProcess()->GetProcessName();
    fIsFoldable = this->CanTrackBeFolded(track);
    fProcessCode = LArSIMpleProcessTable::Get().GetProcessCodeFromString(fProcess);
}

LArSIMpleTrackData::LArSIMpleTrackData(const LArSIMpleTrackData &rhs)
{
    fTrackID = rhs.GetTrackID();
    fParentID = rhs.GetParentID();
    fPDG = rhs.GetPDG();
    fProcess = rhs.GetProcess();
    fProcessCode = rhs.GetProcessCode();
    fIsFoldable = rhs.IsFoldable();
}

LArSIMpleTrackData LArSIMpleTrackData::operator=(const LArSIMpleTrackData &rhs)
{
    return LArSIMpleTrackData(rhs);
}

LArSIMpleTrackData::~LArSIMpleTrackData()
{
}

bool LArSIMpleTrackData::CanTrackBeFolded(const G4Track *track) const
{
    // Primary particle first
    if (fParentID == 0)
    {
        std::cout << "Primary particle with pdg = " << track->GetParticleDefinition()->GetPDGEncoding() << std::endl;
        return false;
    }
    // Check if secondaries have processes we don't want to consider as particles
    else
    {
        if (fProcess.find("conv") != std::string::npos || fProcess.find("LowEnConversion") != std::string::npos ||
            fProcess.find("Pair") != std::string::npos || fProcess.find("compt") != std::string::npos ||
            fProcess.find("Compt") != std::string::npos || fProcess.find("Brem") != std::string::npos || fProcess.find("phot") != std::string::npos ||
            fProcess.find("Photo") != std::string::npos || fProcess.find("muMinusCapture") != std::string::npos ||
            fProcess.find("Radioactive") != std::string::npos || fProcess.find("eIon") != std::string::npos //)
            || fProcess.find("annih") != std::string::npos)
        {
            return true;
        }
        // Otherwise these are particles that we want to keep
        else
        {
            //      std::cout << "Unfoldable process = " << fProcess << std::endl;
            return false;
        }
    }
}
