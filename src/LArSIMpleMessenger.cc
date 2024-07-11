#include "LArSIMpleMessenger.hh"
#include "LArSIMpleEventAction.hh"

#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"
#include "G4UIparameter.hh"

LArSIMpleMessenger::LArSIMpleMessenger(LArSIMpleEventAction *ptra) :
    fEventAction(ptra)
{
    fLArSIMpleDir = new G4UIdirectory("/LArSIMple/");
    fLArSIMpleDir->SetGuidance("Commands to select I/O options");

    fOutputFileDir = new G4UIcmdWithAString("/LArSIMple/OutputFileDir", this);
    fOutputFileDir->SetGuidance("The directory where output files will be written");
    fOutputFileDir->SetParameterName("OutputFileDir", true);
    fOutputFileDir->SetDefaultValue("");

    fOutputFilePrefix = new G4UIcmdWithAString("/LArSIMple/OutputFilePrefix", this);
    fOutputFilePrefix->SetGuidance("The base part of the output file name (to be appended with event number etc)");
    fOutputFilePrefix->SetParameterName("OutputFilePrefix", true);
    fOutputFilePrefix->SetDefaultValue("hits_3d");

    fWriteZipAndInfoFiles = new G4UIcmdWithABool("/LArSIMple/WriteZipAndInfoFiles", this);
    fWriteZipAndInfoFiles->SetGuidance("Write output files in .gz and .info (default option)");
    fWriteZipAndInfoFiles->SetParameterName("WriteZipAndInfoFiles", true);
    fWriteZipAndInfoFiles->SetDefaultValue(true);

    fWriteRootFile = new G4UIcmdWithABool("/LArSIMple/WriteRootFile", this);
    fWriteRootFile->SetGuidance("Write ROOT tree file");
    fWriteRootFile->SetParameterName("WriteRootFile", true);
    fWriteRootFile->SetDefaultValue(false);

    fFoldBackTruthInfo = new G4UIcmdWithABool("/LArSIMple/FoldBackTruthInfo", this);
    fFoldBackTruthInfo->SetGuidance("Fold back truth information so that hits from ionisation electrons are assigned to the parent etc");
    fFoldBackTruthInfo->SetParameterName("FoldBackTruthInfo", true);
    fFoldBackTruthInfo->SetDefaultValue(true);

    fHitThreshold = new G4UIcmdWithADoubleAndUnit("/LArSIMple/HitThreshold", this);
    fHitThreshold->SetGuidance("Energy threshold for saving hits");
    fHitThreshold->SetParameterName("HitThreshold", true);
    fHitThreshold->SetDefaultValue(0.01);

    fWireAngleU = new G4UIcmdWithADouble("/LArSIMple/WireAngleU", this);
    fWireAngleU->SetGuidance("Angle of the U wires to the verticle");
    fWireAngleU->SetParameterName("WireAngleU", true);
    fWireAngleU->SetDefaultValue(35.9);

    fWireAngleV = new G4UIcmdWithADouble("/LArSIMple/WireAngleV", this);
    fWireAngleV->SetGuidance("Angle of the v wires to the verticle");
    fWireAngleV->SetParameterName("WireAngleV", true);
    fWireAngleV->SetDefaultValue(-35.9);

    fWireAngleW = new G4UIcmdWithADouble("/LArSIMple/WireAngleW", this);
    fWireAngleW->SetGuidance("Angle of the W wires to the verticle");
    fWireAngleW->SetParameterName("WireAngleW", true);
    fWireAngleW->SetDefaultValue(0.0);
}

LArSIMpleMessenger::~LArSIMpleMessenger()
{
    delete fLArSIMpleDir;
    delete fOutputFileDir;
    delete fOutputFilePrefix;
    delete fWriteZipAndInfoFiles;
    delete fWriteRootFile;
    delete fFoldBackTruthInfo;
    delete fHitThreshold;
    delete fWireAngleU;
    delete fWireAngleV;
    delete fWireAngleW;
}

void LArSIMpleMessenger::SetNewValue(G4UIcommand *command, G4String newValue)
{

    if (command == fOutputFileDir)
        fEventAction->SetOutputFileDirectory(newValue);
    if (command == fOutputFilePrefix)
        fEventAction->SetOutputFilePrefix(newValue);
    if (command == fWriteZipAndInfoFiles)
        fEventAction->SetWriteZipAndInfoFiles(fWriteZipAndInfoFiles->GetNewBoolValue(newValue));
    if (command == fWriteRootFile)
        fEventAction->SetWriteRootFile(newValue);
    if (command == fFoldBackTruthInfo)
        fEventAction->SetFoldBackTruthInfo(fFoldBackTruthInfo->GetNewBoolValue(newValue));
    if (command == fHitThreshold)
        fEventAction->SetHitThreshold(fHitThreshold->GetNewDoubleValue(newValue));
    if (command == fWireAngleU)
        fEventAction->SetWireAngleU(fWireAngleU->GetNewDoubleValue(newValue));
    if (command == fWireAngleV)
        fEventAction->SetWireAngleV(fWireAngleV->GetNewDoubleValue(newValue));
    if (command == fWireAngleW)
        fEventAction->SetWireAngleW(fWireAngleW->GetNewDoubleValue(newValue));
}
