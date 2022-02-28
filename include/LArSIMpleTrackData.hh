#ifndef LArSIMpleTrackData_h
#define LArSIMpleTrackData_h

#include <string>

class G4Track;

class LArSIMpleTrackData
{
  // Add an enumeration for Geant4 process strings
  enum ProcessCode
  {
    MC_PROC_INCIDENT_NU = -1,
    MC_PROC_UNKNOWN,
    MC_PROC_PRIMARY,
    MC_PROC_COMPT,
    MC_PROC_PHOT,
    MC_PROC_ANNIHIL,
    MC_PROC_E_IONI,
    MC_PROC_E_BREM,
    MC_PROC_CONV,
    MC_PROC_MU_IONI,
    MC_PROC_MU_MINUS_CAPTURE_AT_REST,
    MC_PROC_NEUTRON_INELASTIC,
    MC_PROC_N_CAPTURE,
    MC_PROC_HAD_ELASTIC,
    MC_PROC_DECAY,
    MC_PROC_COULOMB_SCAT,
    MC_PROC_MU_BREM,
    MC_PROC_MU_PAIR_PROD,
    MC_PROC_PHOTON_INELASTIC,
    MC_PROC_HAD_IONI,
    MC_PROC_PROTON_INELASTIC,
    MC_PROC_PI_PLUS_INELASTIC,
    MC_PROC_CHIPS_NUCLEAR_CAPTURE_AT_REST,
    MC_PROC_PI_MINUS_INELASTIC,
    MC_PROC_TRANSPORTATION,
    MC_PROC_RAYLEIGH,
    MC_PROC_HAD_BREM,
    MC_PROC_HAD_PAIR_PROD,
    MC_PROC_ION_IONI,
    MC_PROC_NEUTRON_KILLER,
    MC_PROC_ION_INELASTIC,
    MC_PROC_HE3_INELASTIC,
    MC_PROC_ALPHA_INELASTIC,
    MC_PROC_ANTI_HE3_INELASTIC,
    MC_PROC_ANTI_ALPHA_INELASTIC,
    MC_PROC_HAD_FRITIOF_CAPTURE_AT_REST,
    MC_PROC_ANTI_DEUTERON_INELASTIC,
    MC_PROC_ANTI_NEUTRON_INELASTIC,
    MC_PROC_ANTI_PROTON_INELASTIC,
    MC_PROC_ANTI_TRITON_INELASTIC,
    MC_PROC_DEUTERON_INELASTIC,
    MC_PROC_ELECTRON_NUCLEAR,
    MC_PROC_PHOTON_NUCLEAR,
    MC_PROC_KAON_PLUS_INELASTIC,
    MC_PROC_KAON_MINUS_INELASTIC,
    MC_PROC_HAD_BERTINI_CAPTURE_AT_REST,
    MC_PROC_LAMBDA_INELASTIC,
    MC_PROC_MU_NUCLEAR,
    MC_PROC_TRITON_INELASTIC,
    MC_PROC_PRIMARY_BACKGROUND
  };

  public:
  LArSIMpleTrackData();
  LArSIMpleTrackData(const G4Track* track);
  LArSIMpleTrackData(const LArSIMpleTrackData &rhs);
  LArSIMpleTrackData operator=(const LArSIMpleTrackData &rhs);

  ~LArSIMpleTrackData();

  int GetTrackID() const {return fTrackID;};
  int GetParentID() const {return fParentID;};
  int GetPDG() const {return fPDG;};
  std::string GetProcess() const {return fProcess;};
  ProcessCode GetProcessCode() const {return fProcessCode;};
  bool IsFoldable() const {return fIsFoldable;};

  private:
  bool CanTrackBeFolded(const G4Track* track) const;
  ProcessCode ConvertProcessStringToCode(const std::string &process);

  int fTrackID;
  int fParentID;
  int fPDG;
  std::string fProcess;
  ProcessCode fProcessCode;
  bool fIsFoldable;
};

#endif

