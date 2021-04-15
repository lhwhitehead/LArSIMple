#include "LArSIMplePrimaryGeneratorMessenger.hh"
#include "LArSIMplePrimaryGeneratorAction.hh"
#include "LArSIMpleNeutrinoInputParser.hh"

#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UIparameter.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"

//LArSIMplePrimaryGeneratorMessenger::LArSIMplePrimaryGeneratorMessenger(LArSIMplePrimaryGeneratorAction* ptra, LArSIMpleSteppingVerbose* ptsv, LArSIMpleDetectorConstruction* ptgc)
//  :fPrimaryGeneratorAction(ptra),fSteppingVerbose(ptsv),fDetectorConstruction(ptgc)
//LArSIMplePrimaryGeneratorMessenger::LArSIMplePrimaryGeneratorMessenger(LArSIMplePrimaryGeneratorAction* ptra, LArSIMpleSteppingVerbose* ptsv)
//  :fPrimaryGeneratorAction(ptra),fSteppingVerbose(ptsv)
LArSIMplePrimaryGeneratorMessenger::LArSIMplePrimaryGeneratorMessenger(LArSIMplePrimaryGeneratorAction* ptrpga)
  :fPrimaryGeneratorAction(ptrpga)
{ 
  fLArSIMpleDir = new G4UIdirectory("/LArSIMple/");
  fLArSIMpleDir->SetGuidance("Commands to select I/O options");

  fNeutrinoFileName = new G4UIcmdWithAString("/LArSIMple/NeutrinoInputFile",this);
  fNeutrinoFileName->SetGuidance("Path and name of the neutrino input file");
  fNeutrinoFileName->SetParameterName("NeutrinoInputFile",true);
  fNeutrinoFileName->SetDefaultValue("neutrinos.dat");

  fUseNeutrinos = new G4UIcmdWithABool("/LArSIMple/UseNeutrinoEvents",this);
  fUseNeutrinos->SetGuidance("Use neutrino events instead of a particle gun");
  fUseNeutrinos->SetParameterName("UseNeutrinoEvents",false);
  fUseNeutrinos->SetDefaultValue(false);

  fNeutrinoFileType = new G4UIcmdWithAnInteger("/LArSIMple/NeutrinoFileType",this);
  fNeutrinoFileType->SetGuidance("0 = nuance tracker, 1 = GENIE tree");
  fNeutrinoFileType->SetParameterName("NeutrinoFileType",true);
  fNeutrinoFileType->SetDefaultValue(0);
}

LArSIMplePrimaryGeneratorMessenger::~LArSIMplePrimaryGeneratorMessenger()
{
  delete fLArSIMpleDir;
  delete fNeutrinoFileName;
  delete fNeutrinoFileType;
  delete fUseNeutrinos;
}

void LArSIMplePrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if (command == fUseNeutrinos)
    fPrimaryGeneratorAction->SetUseNeutrinos(fUseNeutrinos->GetNewBoolValue(newValue));
  if (command == fNeutrinoFileName)
    fPrimaryGeneratorAction->SetNeutrinoFileName(newValue);
  if (command == fNeutrinoFileType)
  {
    LArSIMpleNeutrinoInputType type(LArSIMpleNeutrinoInputType::kNotSet);
    if (newValue == "0")
      type = LArSIMpleNeutrinoInputType::kNuanceTracker;
    if (newValue == "1")
      type = LArSIMpleNeutrinoInputType::kGENIETree;
    fPrimaryGeneratorAction->SetNeutrinoFileType(type);   
  }
}
