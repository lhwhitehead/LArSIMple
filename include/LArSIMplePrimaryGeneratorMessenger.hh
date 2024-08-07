/**
 *  @file   LArSIMple/include/LArSIMplePrimaryGeneratorMessenger.hh
 * 
 *  @brief  Header file for the primary generator action messenger class
 * 
 *  $Log: $
 */

#ifndef LArSIMplePrimaryGeneratorMessenger_h
#define LArSIMplePrimaryGeneratorMessenger_h

#include "G4UImessenger.hh"

class LArSIMplePrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;
class G4UIcmdWith3VectorAndUnit;

/**
 *  @brief Class to setup the primary generator action class from the configuration files
 */
class LArSIMplePrimaryGeneratorMessenger : public G4UImessenger
{
public:
    /**
     *  @brief  Constructor
     *
     *  @param  ptrpga the pointer to the primary generator action class
     */
    LArSIMplePrimaryGeneratorMessenger(LArSIMplePrimaryGeneratorAction *ptrpga);

    /**
     *  @brief  Destructor
     */
    ~LArSIMplePrimaryGeneratorMessenger();

    /**
     *  @brief  Set a new value for a variable in the primary generator action class
     *
     *  @param  command the pointer to the command read from the configuration file
     *  @param  newValues the new values read from the configuration file
     */
    void SetNewValue(G4UIcommand *command, G4String newValues);

private:
    LArSIMplePrimaryGeneratorAction *fPrimaryGeneratorAction;    ///< Pointer to the primary generator action object

    G4UIdirectory *fLArSIMpleDir;                                ///< Directory under which all commands are defined

    G4UIdirectory *fNeutrinoDir;                                 ///< Directory under which all neutrino commands are defined
    G4UIcmdWithABool *fUseNeutrinos;                             ///< Whether to use neutrino events
    G4UIcmdWithAString *fNeutrinoFileName;                       ///< File name and path for an input neutrino event file
    G4UIcmdWithAnInteger *fNeutrinoFileType;                     ///< The type of neutrino input file
    G4UIcmdWithABool *fUseRandomNeutrinoVertex;                  ///< Whether to randomise the neutrino vertex
    G4UIcmdWith3VectorAndUnit *fNeutrinoVertex;                  ///< Position of a fixed neutrino vertex

    G4UIdirectory *fParticleBombDir;                             ///< Directory under which all particle bomb commands are defined
    G4UIcmdWithABool *fUseParticleBombs;                         ///< Whether to use particle bombs
    G4UIcmdWithAnInteger *fLeptonPdg;                            ///< The PDG code of the lepton to use (-999 for no lepton)
    G4UIcmdWithAnInteger *fNProton;                              ///< The number of protons in the particle bomb
    G4UIcmdWithAnInteger *fNNeutron;                             ///< The number of neutrons in the particle bomb
    G4UIcmdWithAnInteger *fNPiZero;                              ///< The number of pi0 in the particle bomb
    G4UIcmdWithAnInteger *fNPiPlus;                              ///< The number of pi+ in the particle bomb
    G4UIcmdWithAnInteger *fNPiMinus;                             ///< The number of pi- in the particle bomb
    G4UIcmdWithAnInteger *fNKZero;                               ///< The number of K0 in the particle bomb
    G4UIcmdWithAnInteger *fNKPlus;                               ///< The number of K+ in the particle bomb
    G4UIcmdWithAnInteger *fNKMinus;                              ///< The number of K- in the particle bomb
};

#endif
