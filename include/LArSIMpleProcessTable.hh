#ifndef LARSIMPLEPROCESSTABLE_HH
#define LARSIMPLEPROCESSTABLE_HH

#include <map>
#include <string>

// This class stores the map of process code and process string. It is a
// singleton to prevent this map being created multiple times
class LArSIMpleProcessTable
{
public:
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

    static LArSIMpleProcessTable &Get()
    {
        static LArSIMpleProcessTable table;
        return table;
    };
    LArSIMpleProcessTable(LArSIMpleProcessTable const &) = delete;
    LArSIMpleProcessTable &operator=(LArSIMpleProcessTable const &) = delete;

    ProcessCode GetProcessCodeFromString(const std::string &process);

private:
    LArSIMpleProcessTable();

    void FillProcessMap();
    std::map<std::string, ProcessCode> fProcessStringToCodeMap;
};

#endif
