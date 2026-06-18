/**
 *  @file   LArSIMple/include/LArSIMpleMessenger.hh
 * 
 *  @brief  Header file for the messenger class.
 * 
 *  $Log: $
 */

#ifndef LArSIMpleMessenger_h
#define LArSIMpleMessenger_h

#include "G4UImessenger.hh"

class LArSIMpleEventAction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWithABool;

/**
 *  @brief Messenger class for high level configurations
 */

class LArSIMpleMessenger : public G4UImessenger
{
public:
    /**
     *  @brief  Constructor
     *
     *  @param  pEventAction pointer to a LArSIMpleEventAction object
     */
    LArSIMpleMessenger(LArSIMpleEventAction *pEventAction);

    /**
     *  @brief  Destructor
     */
    ~LArSIMpleMessenger();

    /**
     *  @brief  Set a value in the DetectorConstruction class
     *
     *  @param  command the variable name
     *  @param  newValues the new value(s) for the variable
     */
    void SetNewValue(G4UIcommand *command, G4String newValues);

private:
    LArSIMpleEventAction *fEventAction; ///< Pointer to the event action

    G4UIdirectory *fLArSIMpleDir; ///< Pointer to the G4 config directory class

    G4UIcmdWithAString *fOutputFileDir;      ///< Command to set the output directory path
    G4UIcmdWithAString *fOutputFilePrefix;   ///< Command to set the output file prefix
    G4UIcmdWithABool *fWriteZipAndInfoFiles; ///< Command for whether to write zlib files
    G4UIcmdWithABool *fWriteRootFile;        ///< Command for whether to write ROOT files

    G4UIcmdWithABool *fFoldBackTruthInfo; ///< Command for whether to fold truth information to the primaries
    G4UIdirectory *fFoldingOptions;       ///< Pointer for the truth folding configuration options
    G4UIcmdWithABool *fFoldDeltaRays;     ///< Command for set whether to fold back delta-rays to the parent muon

    G4UIcmdWithADoubleAndUnit *fHitThreshold; ///< Command for setting the energy threshold for hit creation
    G4UIcmdWithABool *fUseHitFeatures;        ///< Command for whether to calculate neighbouring hit features
};

#endif
