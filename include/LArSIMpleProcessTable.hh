/**
 *  @file   LArSIMple/include/LArSIMpleProcessTable.hh
 * 
 *  @brief  Header file for the process table class
 * 
 *  $Log: $
 */

#ifndef LARSIMPLEPROCESSTABLE_HH
#define LARSIMPLEPROCESSTABLE_HH

#include <map>
#include <string>

/**
 *  @brief Singleton class that maps an enumerated process code to the GEANT4 process string
 */
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
        MC_PROC_E_PAIR_PROD,
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
        MC_PROC_PRIMARY_BACKGROUND,
        MC_PROC_RADIOACTIVE_DECAY,
        MC_PROC_KAON0L_INELASTIC,
        MC_PROC_POSITRON_NUCLEAR
    };

    /**
     *  @brief  Get the static instance to the class
     *
     *  @return The instance of the process table
     */
    static LArSIMpleProcessTable &Get()
    {
        static LArSIMpleProcessTable table;
        return table;
    };
    LArSIMpleProcessTable(LArSIMpleProcessTable const &) = delete;
    LArSIMpleProcessTable &operator=(LArSIMpleProcessTable const &) = delete;

    /**
     *  @brief  Get the process code from the GEANT4 process string
     *
     *  @param  process the GEANT4 process name
     *
     *  @return The associated process code
     */
    ProcessCode GetProcessCodeFromString(const std::string &process);

private:
    /**
     *  @brief  Default constructor
     */
    LArSIMpleProcessTable();

    /**
     *  @brief  Fill the map of process string to process code
     */
    void FillProcessMap();

    std::map<std::string, ProcessCode> fProcessStringToCodeMap; ///< Map between process string and process code
};

#endif
