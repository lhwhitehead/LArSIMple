/**
 *  @file LArSIMple/src/LArSIMpleProcessTable.cc
 * 
 *  @brief Implementation of the process table class.
 * 
 *  $Log: $
 */

#include <iostream>

#include "LArSIMpleProcessTable.hh"

LArSIMpleProcessTable::ProcessCode LArSIMpleProcessTable::GetProcessCodeFromString(const std::string &process)
{
    if (fProcessStringToCodeMap.count(process) != 0)
        return fProcessStringToCodeMap.at(process);
    else
    {
        std::cerr << "No process code found for process " << process << std::endl;
        return MC_PROC_UNKNOWN;
    }
}

//------------------------------------------------------------------------------------------------------------------------------------------

LArSIMpleProcessTable::LArSIMpleProcessTable()
{
    this->FillProcessMap();
}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMpleProcessTable::FillProcessMap()
{
    // QGSP_BERT and EM standard physics list mappings
    fProcessStringToCodeMap.insert(std::make_pair("unknown", MC_PROC_UNKNOWN));
    fProcessStringToCodeMap.insert(std::make_pair("primary", MC_PROC_PRIMARY));
    fProcessStringToCodeMap.insert(std::make_pair("compt", MC_PROC_COMPT));
    fProcessStringToCodeMap.insert(std::make_pair("phot", MC_PROC_PHOT));
    fProcessStringToCodeMap.insert(std::make_pair("annihil", MC_PROC_ANNIHIL));
    fProcessStringToCodeMap.insert(std::make_pair("eIoni", MC_PROC_E_IONI));
    fProcessStringToCodeMap.insert(std::make_pair("eBrem", MC_PROC_E_BREM));
    fProcessStringToCodeMap.insert(std::make_pair("ePairProd", MC_PROC_E_PAIR_PROD));
    fProcessStringToCodeMap.insert(std::make_pair("conv", MC_PROC_CONV));
    fProcessStringToCodeMap.insert(std::make_pair("muIoni", MC_PROC_MU_IONI));
    fProcessStringToCodeMap.insert(std::make_pair("muMinusCaptureAtRest", MC_PROC_MU_MINUS_CAPTURE_AT_REST));
    fProcessStringToCodeMap.insert(std::make_pair("neutronInelastic", MC_PROC_NEUTRON_INELASTIC));
    fProcessStringToCodeMap.insert(std::make_pair("nCapture", MC_PROC_N_CAPTURE));
    fProcessStringToCodeMap.insert(std::make_pair("hadElastic", MC_PROC_HAD_ELASTIC));
    fProcessStringToCodeMap.insert(std::make_pair("Decay", MC_PROC_DECAY));
    fProcessStringToCodeMap.insert(std::make_pair("CoulombScat", MC_PROC_COULOMB_SCAT));
    fProcessStringToCodeMap.insert(std::make_pair("muBrems", MC_PROC_MU_BREM));
    fProcessStringToCodeMap.insert(std::make_pair("muPairProd", MC_PROC_MU_PAIR_PROD));
    fProcessStringToCodeMap.insert(std::make_pair("PhotonInelastic", MC_PROC_PHOTON_INELASTIC));
    fProcessStringToCodeMap.insert(std::make_pair("hIoni", MC_PROC_HAD_IONI));
    fProcessStringToCodeMap.insert(std::make_pair("protonInelastic", MC_PROC_PROTON_INELASTIC));
    fProcessStringToCodeMap.insert(std::make_pair("pi+Inelastic", MC_PROC_PI_PLUS_INELASTIC));
    fProcessStringToCodeMap.insert(std::make_pair("CHIPSNuclearCaptureAtRest", MC_PROC_CHIPS_NUCLEAR_CAPTURE_AT_REST));
    fProcessStringToCodeMap.insert(std::make_pair("pi-Inelastic", MC_PROC_PI_MINUS_INELASTIC));
    fProcessStringToCodeMap.insert(std::make_pair("Transportation", MC_PROC_TRANSPORTATION));
    fProcessStringToCodeMap.insert(std::make_pair("Rayl", MC_PROC_RAYLEIGH));
    fProcessStringToCodeMap.insert(std::make_pair("hBrems", MC_PROC_HAD_BREM));
    fProcessStringToCodeMap.insert(std::make_pair("hPairProd", MC_PROC_HAD_PAIR_PROD));
    fProcessStringToCodeMap.insert(std::make_pair("ionIoni", MC_PROC_ION_IONI));
    fProcessStringToCodeMap.insert(std::make_pair("nKiller", MC_PROC_NEUTRON_KILLER));
    fProcessStringToCodeMap.insert(std::make_pair("ionInelastic", MC_PROC_ION_INELASTIC));
    fProcessStringToCodeMap.insert(std::make_pair("He3Inelastic", MC_PROC_HE3_INELASTIC));
    fProcessStringToCodeMap.insert(std::make_pair("alphaInelastic", MC_PROC_ALPHA_INELASTIC));
    fProcessStringToCodeMap.insert(std::make_pair("anti_He3Inelastic", MC_PROC_ANTI_HE3_INELASTIC));
    fProcessStringToCodeMap.insert(std::make_pair("anti_alphaInelastic", MC_PROC_ANTI_ALPHA_INELASTIC));
    fProcessStringToCodeMap.insert(std::make_pair("hFritiofCaptureAtRest", MC_PROC_HAD_FRITIOF_CAPTURE_AT_REST));
    fProcessStringToCodeMap.insert(std::make_pair("anti_deuteronInelastic", MC_PROC_ANTI_DEUTERON_INELASTIC));
    fProcessStringToCodeMap.insert(std::make_pair("anti_neutronInelastic", MC_PROC_ANTI_NEUTRON_INELASTIC));
    fProcessStringToCodeMap.insert(std::make_pair("anti_protonInelastic", MC_PROC_ANTI_PROTON_INELASTIC));
    fProcessStringToCodeMap.insert(std::make_pair("anti_tritonInelastic", MC_PROC_ANTI_TRITON_INELASTIC));
    fProcessStringToCodeMap.insert(std::make_pair("dInelastic", MC_PROC_DEUTERON_INELASTIC));
    fProcessStringToCodeMap.insert(std::make_pair("electronNuclear", MC_PROC_ELECTRON_NUCLEAR));
    fProcessStringToCodeMap.insert(std::make_pair("photonNuclear", MC_PROC_PHOTON_NUCLEAR));
    fProcessStringToCodeMap.insert(std::make_pair("kaon+Inelastic", MC_PROC_KAON_PLUS_INELASTIC));
    fProcessStringToCodeMap.insert(std::make_pair("kaon-Inelastic", MC_PROC_KAON_MINUS_INELASTIC));
    fProcessStringToCodeMap.insert(std::make_pair("hBertiniCaptureAtRest", MC_PROC_HAD_BERTINI_CAPTURE_AT_REST));
    fProcessStringToCodeMap.insert(std::make_pair("lambdaInelastic", MC_PROC_LAMBDA_INELASTIC));
    fProcessStringToCodeMap.insert(std::make_pair("muonNuclear", MC_PROC_MU_NUCLEAR));
    fProcessStringToCodeMap.insert(std::make_pair("tInelastic", MC_PROC_TRITON_INELASTIC));
    fProcessStringToCodeMap.insert(std::make_pair("primaryBackground", MC_PROC_PRIMARY_BACKGROUND));
    fProcessStringToCodeMap.insert(std::make_pair("RadioactiveDecay", MC_PROC_RADIOACTIVE_DECAY));
    fProcessStringToCodeMap.insert(std::make_pair("kaon0LInelastic", MC_PROC_KAON0L_INELASTIC));
}
