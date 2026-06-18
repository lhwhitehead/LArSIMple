/**
 *  @file LArSIMple/src/LArSIMplePandoraMessenger.cc
 * 
 *  @brief Implementation of the pandora messenger class. 
 * 
 *  $Log: $
 */

#include "LArSIMplePandoraMessenger.hh"
#include "LArSIMplePandoraInterface.hh"

#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"
#include "G4UIparameter.hh"

LArSIMplePandoraMessenger::LArSIMplePandoraMessenger(LArSIMplePandoraInterface *pPandoraInterface) :
    fPandoraInterface(pPandoraInterface)
{
    fPandoraDir = new G4UIdirectory("/LArSIMple/Pandora/");
    fPandoraDir->SetGuidance("Commands to select pandora options");

    fCaloHitThreshold = new G4UIcmdWithADouble("/LArSIMple/Pandora/CaloHitThreshold", this);
    fCaloHitThreshold->SetGuidance("Energy threshold for creating calo hits");
    fCaloHitThreshold->SetParameterName("CaloHitThreshold", true);
    fCaloHitThreshold->SetDefaultValue(0.01);

    fApplyCaloHitThreshold = new G4UIcmdWithABool("/LArSIMple/Pandora/ApplyCaloHitThreshold", this);
    fApplyCaloHitThreshold->SetGuidance("Apply threshold for CaloHit creation");
    fApplyCaloHitThreshold->SetParameterName("ApplyCaloHitThreshold", false);
    fApplyCaloHitThreshold->SetDefaultValue(false);
}

//------------------------------------------------------------------------------------------------------------------------------------------

LArSIMplePandoraMessenger::~LArSIMplePandoraMessenger()
{
    delete fPandoraDir;
    delete fCaloHitThreshold;
    delete fApplyCaloHitThreshold;
}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMplePandoraMessenger::SetNewValue(G4UIcommand *command, G4String newValue)
{
    if (command == fCaloHitThreshold)
        fPandoraInterface->SetCaloHitThreshold(fCaloHitThreshold->GetNewDoubleValue(newValue));
    if (command == fApplyCaloHitThreshold)
        fPandoraInterface->SetApplyCaloHitThreshold(fApplyCaloHitThreshold->GetNewBoolValue(newValue));
}
