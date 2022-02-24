#ifndef LArSIMpleMessenger_h
#define LArSIMpleMessenger_h 

class LArSIMpleEventAction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWithABool;

#include "G4UImessenger.hh"
#include "globals.hh"

class LArSIMpleMessenger: public G4UImessenger
{
  public:
    LArSIMpleMessenger(LArSIMpleEventAction* ptra);
    ~LArSIMpleMessenger();

  public:
    void SetNewValue(G4UIcommand* command, G4String newValues);

  private:
    LArSIMpleEventAction* fEventAction;

    G4UIdirectory*      fLArSIMpleDir;

    G4UIcmdWithAString* fOutputFileDir;
    G4UIcmdWithAString* fOutputFilePrefix;
    G4UIcmdWithABool* fWriteZipAndInfoFiles;
    G4UIcmdWithABool* fWriteRootFile;

    G4UIcmdWithABool* fFoldBackTruthInfo;

    G4UIcmdWithADoubleAndUnit* fHitThreshold;

    G4UIcmdWithADouble* fWireAngleU;
    G4UIcmdWithADouble* fWireAngleV;
    G4UIcmdWithADouble* fWireAngleW;
};

#endif


