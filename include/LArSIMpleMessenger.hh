#ifndef LArSIMpleMessenger_h
#define LArSIMpleMessenger_h 1

class LArSIMpleEventAction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
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
    //LArSIMpleSteppingVerbose* fSteppingVerbose;
    //LArSIMpleDetectorConstruction* fDetectorConstruction;

    G4UIdirectory*      fLArSIMpleDir;

    G4UIcmdWithAString* fOutputFileBase;
    G4UIcmdWithABool* fWriteZipAndInfoFiles;
    G4UIcmdWithABool* fWriteRootFile;

    G4UIcmdWithADoubleAndUnit* fHitThreshold;
};

#endif


