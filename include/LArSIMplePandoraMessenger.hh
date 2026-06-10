/**
 *  @file   LArSIMple/include/LArSIMplePandoraMessenger.hh
 * 
 *  @brief  Header file for the pandora messenger class.
 * 
 *  $Log: $
 */

#ifndef LArSIMplePandoraMessenger_h
#define LArSIMplePandoraMessenger_h

#include "G4UImessenger.hh"

class LArSIMplePandoraWriter;
class G4UIdirectory;
class G4UIcmdWithADouble;
class G4UIcmdWithABool;

/**
 *  @brief Messenger class for pandora input configurations
 */

class LArSIMplePandoraMessenger : public G4UImessenger
{
public:
    /**
     *  @brief  Constructor
     *
     *  @param  pEventAction pointer to a LArSIMpleEventAction object
     */
    LArSIMplePandoraMessenger(LArSIMplePandoraWriter *pPandoraWriter);

    /**
     *  @brief  Destructor
     */
    ~LArSIMplePandoraMessenger();

    /**
     *  @brief  Set a value in the DetectorConstruction class
     *
     *  @param  command the variable name
     *  @param  newValues the new value(s) for the variable
     */
    void SetNewValue(G4UIcommand *command, G4String newValues);

private:
    LArSIMplePandoraWriter *fPandoraWriter; ///< Pointer to the pandora writer class

    G4UIdirectory *fPandoraDir; ///< Pointer to the G4 config directory class

    G4UIcmdWithADouble *fCaloHitThreshold;        ///< Command for setting the energy threshold for calo hit creation
    G4UIcmdWithABool *fApplyCaloHitThreshold;     ///< Command for whether to apply the energy threshold cut
};

#endif
