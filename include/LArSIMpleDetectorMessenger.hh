#ifndef LArSIMpleDetectorMessenger_h
#define LArSIMpleDetectorMessenger_h 1

class LArSIMpleDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithABool;
class G4UIcmdWithADoubleAndUnit;

#include "G4UImessenger.hh"
#include "globals.hh"

class LArSIMpleDetectorMessenger: public G4UImessenger
{
 public:
  LArSIMpleDetectorMessenger(LArSIMpleDetectorConstruction* ptgc);
  ~LArSIMpleDetectorMessenger();
  
 public:
  void     SetNewValue(G4UIcommand* command, G4String newValues);
  
 private:

  LArSIMpleDetectorConstruction* fDetectorConstruction;

  G4UIdirectory*      fLArSIMpleDir;

  // Size of the LAr volume
  G4UIcmdWithADoubleAndUnit* fWorldWidth;
  G4UIcmdWithADoubleAndUnit* fWorldHeight;
  G4UIcmdWithADoubleAndUnit* fWorldLength;

  // Size of the LAr volume
  G4UIcmdWithADoubleAndUnit* fDetectorWidth;
  G4UIcmdWithADoubleAndUnit* fDetectorHeight;
  G4UIcmdWithADoubleAndUnit* fDetectorLength;

  // Check for Overlaps
  G4UIcmdWithABool* fCheckOverlaps;
};

#endif


