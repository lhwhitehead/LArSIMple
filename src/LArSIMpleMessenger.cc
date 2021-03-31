#include "LArSIMpleMessenger.hh"

//#include "LArSIMpleDetectorConstruction.hh"
#include "LArSIMpleEventAction.hh"
//#include "LArSIMpleSteppingVerbose.hh"
#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UIparameter.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

//LArSIMpleMessenger::LArSIMpleMessenger(LArSIMpleEventAction* ptra, LArSIMpleSteppingVerbose* ptsv, LArSIMpleDetectorConstruction* ptgc)
//  :fEventAction(ptra),fSteppingVerbose(ptsv),fDetectorConstruction(ptgc)
//LArSIMpleMessenger::LArSIMpleMessenger(LArSIMpleEventAction* ptra, LArSIMpleSteppingVerbose* ptsv)
//  :fEventAction(ptra),fSteppingVerbose(ptsv)
LArSIMpleMessenger::LArSIMpleMessenger(LArSIMpleEventAction* ptra)
  :fEventAction(ptra)
{ 
  fLArSIMpleDir = new G4UIdirectory("/LArSIMple/");
  fLArSIMpleDir->SetGuidance("Commands to select I/O options");

  fOutputFileBase = new G4UIcmdWithAString("/LArSIMple/OutputFileBase",this);
  fOutputFileBase->SetGuidance("The base part of the output file name (to be appended with event number etc)");
  fOutputFileBase->SetParameterName("OutputFileBase",true);
  fOutputFileBase->SetDefaultValue("hits_3d");

  fHitThreshold = new G4UIcmdWithADoubleAndUnit("/LArSIMple/HitThreshold",this);
  fHitThreshold->SetGuidance("Energy threshold for saving hits");
  fHitThreshold->SetParameterName("HitThreshold",true);
  fHitThreshold->SetDefaultValue(0.01);
}

LArSIMpleMessenger::~LArSIMpleMessenger()
{
  delete fLArSIMpleDir;
  delete fOutputFileBase;
  delete fHitThreshold;
}

void LArSIMpleMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{

  if (command == fOutputFileBase)
    fEventAction->SetOutputFileBase(newValue);
  if (command == fHitThreshold)
    fEventAction->SetHitThreshold(fHitThreshold->GetNewDoubleValue(newValue));

}
