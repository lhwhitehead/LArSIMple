#ifndef LArSIMplePrimaryGeneratorMessenger_h
#define LArSIMplePrimaryGeneratorMessenger_h 1

class LArSIMplePrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;

#include "G4UImessenger.hh"
#include "globals.hh"

class LArSIMplePrimaryGeneratorMessenger: public G4UImessenger
{
  public:
    LArSIMplePrimaryGeneratorMessenger(LArSIMplePrimaryGeneratorAction* ptrpga);
    ~LArSIMplePrimaryGeneratorMessenger();

  public:
    void SetNewValue(G4UIcommand* command, G4String newValues);

  private:
    LArSIMplePrimaryGeneratorAction* fPrimaryGeneratorAction;

    G4UIdirectory*      fLArSIMpleDir;

    G4UIcmdWithABool* fUseNeutrinos;
    G4UIcmdWithAString* fNeutrinoFileName;
    G4UIcmdWithAnInteger* fNeutrinoFileType;

};

#endif


