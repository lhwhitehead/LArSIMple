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
  fProcessCode = MC_PROC_UNKNOWN;
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
  fProcessCode = this->ConvertProcessStringToCode(fProcess);
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
  if(fParentID == 0)
  {
    std::cout << "Primary particle with pdg = " << track->GetParticleDefinition()->GetPDGEncoding() << std::endl;
    return false;
  }
  // Check if secondaries have processes we don't want to consider as particles
  else
  {
    if(fProcess.find("conv")           != std::string::npos
       || fProcess.find("LowEnConversion") != std::string::npos
       || fProcess.find("Pair")            != std::string::npos
       || fProcess.find("compt")           != std::string::npos
       || fProcess.find("Compt")           != std::string::npos
       || fProcess.find("Brem")            != std::string::npos
       || fProcess.find("phot")            != std::string::npos
       || fProcess.find("Photo")           != std::string::npos
       || fProcess.find("muMinusCapture")  != std::string::npos
       || fProcess.find("Radioactive")     != std::string::npos
       || fProcess.find("Ion")             != std::string::npos)
//       || process.find("annihil")         != std::string::npos)
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

LArSIMpleTrackData::ProcessCode LArSIMpleTrackData::ConvertProcessStringToCode(const std::string &process)
{
  // QGSP_BERT and EM standard physics list mappings
  if (process == "unknown") return MC_PROC_UNKNOWN;
  if (process == "primary") return MC_PROC_PRIMARY;
  if (process == "compt") return MC_PROC_COMPT;
  if (process == "phot") return MC_PROC_PHOT;
  if (process == "annihil") return MC_PROC_ANNIHIL;
  if (process == "eIoni") return MC_PROC_E_IONI;
  if (process == "eBrem") return MC_PROC_E_BREM;
  if (process == "conv") return MC_PROC_CONV;
  if (process == "muIoni") return MC_PROC_MU_IONI;
  if (process == "muMinusCaptureAtRest") return MC_PROC_MU_MINUS_CAPTURE_AT_REST;
  if (process == "neutronInelastic") return MC_PROC_NEUTRON_INELASTIC;
  if (process == "nCapture") return MC_PROC_N_CAPTURE;
  if (process == "hadElastic") return MC_PROC_HAD_ELASTIC;
  if (process == "Decay") return MC_PROC_DECAY;
  if (process == "CoulombScat") return MC_PROC_COULOMB_SCAT;
  if (process == "muBrems") return MC_PROC_MU_BREM;
  if (process == "muPairProd") return MC_PROC_MU_PAIR_PROD;
  if (process == "PhotonInelastic") return MC_PROC_PHOTON_INELASTIC;
  if (process == "hIoni") return MC_PROC_HAD_IONI;
  if (process == "protonInelastic") return MC_PROC_PROTON_INELASTIC;
  if (process == "pi+Inelastic") return MC_PROC_PI_PLUS_INELASTIC;
  if (process == "CHIPSNuclearCaptureAtRest") return MC_PROC_CHIPS_NUCLEAR_CAPTURE_AT_REST;
  if (process == "pi-Inelastic") return MC_PROC_PI_MINUS_INELASTIC;
  if (process == "Transportation") return MC_PROC_TRANSPORTATION;
  if (process == "Rayl") return MC_PROC_RAYLEIGH;
  if (process == "hBrems") return MC_PROC_HAD_BREM;
  if (process == "hPairProd") return MC_PROC_HAD_PAIR_PROD;
  if (process == "ionIoni") return MC_PROC_ION_IONI;
  if (process == "nKiller") return MC_PROC_NEUTRON_KILLER;
  if (process == "ionInelastic") return MC_PROC_ION_INELASTIC;
  if (process == "He3Inelastic") return MC_PROC_HE3_INELASTIC;
  if (process == "alphaInelastic") return MC_PROC_ALPHA_INELASTIC;
  if (process == "anti_He3Inelastic") return MC_PROC_ANTI_HE3_INELASTIC;
  if (process == "anti_alphaInelastic") return MC_PROC_ANTI_ALPHA_INELASTIC;
  if (process == "hFritiofCaptureAtRest") return MC_PROC_HAD_FRITIOF_CAPTURE_AT_REST;
  if (process == "anti_deuteronInelastic") return MC_PROC_ANTI_DEUTERON_INELASTIC;
  if (process == "anti_neutronInelastic") return MC_PROC_ANTI_NEUTRON_INELASTIC;
  if (process == "anti_protonInelastic") return MC_PROC_ANTI_PROTON_INELASTIC;
  if (process == "anti_tritonInelastic") return MC_PROC_ANTI_TRITON_INELASTIC;
  if (process == "dInelastic") return MC_PROC_DEUTERON_INELASTIC;
  if (process == "electronNuclear") return MC_PROC_ELECTRON_NUCLEAR;
  if (process == "photonNuclear") return MC_PROC_PHOTON_NUCLEAR;
  if (process == "kaon+Inelastic") return MC_PROC_KAON_PLUS_INELASTIC;
  if (process == "kaon-Inelastic") return MC_PROC_KAON_MINUS_INELASTIC;
  if (process == "hBertiniCaptureAtRest") return MC_PROC_HAD_BERTINI_CAPTURE_AT_REST;
  if (process == "lambdaInelastic") return MC_PROC_LAMBDA_INELASTIC;
  if (process == "muonNuclear") return MC_PROC_MU_NUCLEAR;
  if (process == "tInelastic") return MC_PROC_TRITON_INELASTIC;
  if (process == "primaryBackground") return MC_PROC_PRIMARY_BACKGROUND;

  return MC_PROC_UNKNOWN;
}

