#include "LArSIMpleTrackData.hh"

#include "G4Track.hh"
#include "G4VProcess.hh"

LArSIMpleTrackData::LArSIMpleTrackData()
{
  fTrackID = 0;
  fParentID = 0;
  fPDG = 0;
  fIsFoldable = false;
}

LArSIMpleTrackData::LArSIMpleTrackData(const G4Track *track)
{
  fTrackID = track->GetTrackID();
  fParentID = track->GetParentID();
  fPDG = track->GetParticleDefinition()->GetPDGEncoding();
  fIsFoldable = this->CanTrackBeFolded(track);
}

LArSIMpleTrackData::LArSIMpleTrackData(const LArSIMpleTrackData &rhs)
{
  fTrackID = rhs.GetTrackID();
  fParentID = rhs.GetParentID();
  fPDG = rhs.GetPDG();
  fIsFoldable = rhs.IsFoldable();
}

LArSIMpleTrackData LArSIMpleTrackData::operator=(const LArSIMpleTrackData &rhs)
{
  LArSIMpleTrackData obj = LArSIMpleTrackData(rhs);
  return obj;
}

LArSIMpleTrackData::~LArSIMpleTrackData()
{

}

bool LArSIMpleTrackData::CanTrackBeFolded(const G4Track *track) const
{ 
  // Primary particle first
  if(fParentID == 0)
  {
    return false;
  }
  // Check if secondaries have processes we don't want to consider as particles
  else
  {
    std::string process = track->GetCreatorProcess()->GetProcessName();
    if(process.find("conv")           != std::string::npos
       || process.find("LowEnConversion") != std::string::npos
       || process.find("Pair")            != std::string::npos
       || process.find("compt")           != std::string::npos
       || process.find("Compt")           != std::string::npos
       || process.find("Brem")            != std::string::npos
       || process.find("phot")            != std::string::npos
       || process.find("Photo")           != std::string::npos
       || process.find("Ion")             != std::string::npos
       || process.find("annihil")         != std::string::npos)
    {
      return true;
    }
    // Otherwise these are particles that we want to keep
    else
    {
      return false;
    }
  }
}
