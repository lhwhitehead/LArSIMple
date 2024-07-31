/**
 *  @file LArSIMple/src/LArSIMpleDetectorMessenger.cc
 * 
 *  @brief Implementation of the detector messenger class.
 * 
 *  $Log: $
 */

#include "LArSIMpleDetectorMessenger.hh"
#include "LArSIMpleDetectorConstruction.hh"

#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"
#include "G4UIparameter.hh"

LArSIMpleDetectorMessenger::LArSIMpleDetectorMessenger(LArSIMpleDetectorConstruction *pDetector) :
    fDetectorConstruction(pDetector)
{
    fLArSIMpleDir = new G4UIdirectory("/LArSIMple/");
    fLArSIMpleDir->SetGuidance("Commands to select I/O options");

    fWorldWidth = new G4UIcmdWithADoubleAndUnit("/LArSIMple/WorldWidth", this);
    fWorldWidth->SetGuidance("Width of the world volume");
    fWorldWidth->SetParameterName("DetectorWidth", true, true);
    fWorldWidth->SetDefaultValue(1000.);
    fWorldWidth->SetDefaultUnit("cm");

    fWorldHeight = new G4UIcmdWithADoubleAndUnit("/LArSIMple/WorldHeight", this);
    fWorldHeight->SetGuidance("Height of the world volume");
    fWorldHeight->SetParameterName("WorldHeight", true, true);
    fWorldHeight->SetDefaultValue(1000.);
    fWorldHeight->SetDefaultUnit("cm");

    fWorldLength = new G4UIcmdWithADoubleAndUnit("/LArSIMple/WorldLength", this);
    fWorldLength->SetGuidance("Length of the world volume");
    fWorldLength->SetParameterName("WorldLength", true, true);
    fWorldLength->SetDefaultValue(1000.);
    fWorldLength->SetDefaultUnit("cm");

    fDetectorWidth = new G4UIcmdWithADoubleAndUnit("/LArSIMple/DetectorWidth", this);
    fDetectorWidth->SetGuidance("Width of the detector LAr volume");
    fDetectorWidth->SetParameterName("DetectorWidth", true, true);
    fDetectorWidth->SetDefaultValue(500.);
    fDetectorWidth->SetDefaultUnit("cm");

    fDetectorHeight = new G4UIcmdWithADoubleAndUnit("/LArSIMple/DetectorHeight", this);
    fDetectorHeight->SetGuidance("Height of the detector LAr volume");
    fDetectorHeight->SetParameterName("DetectorHeight", true, true);
    fDetectorHeight->SetDefaultValue(500.);
    fDetectorHeight->SetDefaultUnit("cm");

    fDetectorLength = new G4UIcmdWithADoubleAndUnit("/LArSIMple/DetectorLength", this);
    fDetectorLength->SetGuidance("Length of the detector LAr volume");
    fDetectorLength->SetParameterName("DetectorLength", true, true);
    fDetectorLength->SetDefaultValue(500.);
    fDetectorLength->SetDefaultUnit("cm");

    fVoxeliseLAr = new G4UIcmdWithABool("/LArSIMple/VoxeliseLAr", this);
    fVoxeliseLAr->SetGuidance("Voxelise the main LAr volume");
    fVoxeliseLAr->SetParameterName("VoxeliseLAr", true, true);
    fVoxeliseLAr->SetDefaultValue(true);

    fCheckOverlaps = new G4UIcmdWithABool("/LArSIMple/CheckOverlaps", this);
    fCheckOverlaps->SetGuidance("Check overlaps when building geometry");
    fCheckOverlaps->SetParameterName("CheckOverlaps", true, true);
    fCheckOverlaps->SetDefaultValue(true);
}

//------------------------------------------------------------------------------------------------------------------------------------------

LArSIMpleDetectorMessenger::~LArSIMpleDetectorMessenger()
{
    delete fWorldWidth;
    delete fWorldHeight;
    delete fWorldLength;
    delete fDetectorWidth;
    delete fDetectorHeight;
    delete fDetectorLength;
    delete fVoxeliseLAr;
    delete fCheckOverlaps;
    delete fLArSIMpleDir;
}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMpleDetectorMessenger::SetNewValue(G4UIcommand *command, G4String newValue)
{
    if (command == fWorldWidth)
        fDetectorConstruction->SetWorldWidth(fWorldWidth->GetNewDoubleValue(newValue));
    if (command == fWorldHeight)
        fDetectorConstruction->SetWorldHeight(fWorldHeight->GetNewDoubleValue(newValue));
    if (command == fWorldLength)
        fDetectorConstruction->SetWorldLength(fWorldLength->GetNewDoubleValue(newValue));

    if (command == fDetectorWidth)
        fDetectorConstruction->SetLArWidth(fDetectorWidth->GetNewDoubleValue(newValue));
    if (command == fDetectorHeight)
        fDetectorConstruction->SetLArHeight(fDetectorHeight->GetNewDoubleValue(newValue));
    if (command == fDetectorLength)
        fDetectorConstruction->SetLArLength(fDetectorLength->GetNewDoubleValue(newValue));

    if (command == fVoxeliseLAr)
        fDetectorConstruction->SetVoxeliseLAr(fVoxeliseLAr->GetNewBoolValue(newValue));
    if (command == fCheckOverlaps)
        fDetectorConstruction->SetCheckOverlaps(fCheckOverlaps->GetNewBoolValue(newValue));
}
