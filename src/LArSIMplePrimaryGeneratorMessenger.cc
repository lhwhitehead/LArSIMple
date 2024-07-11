#include "LArSIMplePrimaryGeneratorMessenger.hh"
#include "LArSIMpleNeutrinoInputParser.hh"
#include "LArSIMplePrimaryGeneratorAction.hh"

#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"
#include "G4UIparameter.hh"

LArSIMplePrimaryGeneratorMessenger::LArSIMplePrimaryGeneratorMessenger(LArSIMplePrimaryGeneratorAction *ptrpga) :
    fPrimaryGeneratorAction(ptrpga)
{
    fLArSIMpleDir = new G4UIdirectory("/LArSIMple/");
    fLArSIMpleDir->SetGuidance("Commands to select I/O options");

    fNeutrinoFileName = new G4UIcmdWithAString("/LArSIMple/NeutrinoInputFile", this);
    fNeutrinoFileName->SetGuidance("Path and name of the neutrino input file");
    fNeutrinoFileName->SetParameterName("NeutrinoInputFile", true);
    fNeutrinoFileName->SetDefaultValue("neutrinos.dat");

    fUseNeutrinos = new G4UIcmdWithABool("/LArSIMple/UseNeutrinoEvents", this);
    fUseNeutrinos->SetGuidance("Use neutrino events instead of a particle gun");
    fUseNeutrinos->SetParameterName("UseNeutrinoEvents", false);
    fUseNeutrinos->SetDefaultValue(false);

    fNeutrinoFileType = new G4UIcmdWithAnInteger("/LArSIMple/NeutrinoFileType", this);
    fNeutrinoFileType->SetGuidance("0 = nuance tracker, 1 = GENIE tree");
    fNeutrinoFileType->SetParameterName("NeutrinoFileType", true);
    fNeutrinoFileType->SetDefaultValue(0);

    fUseRandomNeutrinoVertex = new G4UIcmdWithABool("/LArSIMple/UseRandomNeutrinoVertex", this);
    fUseRandomNeutrinoVertex->SetGuidance("Use a random vertex (at least 1m from the walls)");
    fUseRandomNeutrinoVertex->SetParameterName("UseRandomNeutrinoVertex", true);
    fUseRandomNeutrinoVertex->SetDefaultValue(true);

    fNeutrinoVertex = new G4UIcmdWith3VectorAndUnit("/LArSIMple/SetNeutrinoVertex", this);
    fNeutrinoVertex->SetGuidance("Neutrino interaction vertex");
    fNeutrinoVertex->SetParameterName("NeutrinoVertexX", "NeutrinoVertexY", "NeutrinoVertexZ", true);
    fNeutrinoVertex->SetDefaultValue(G4ThreeVector());
}

LArSIMplePrimaryGeneratorMessenger::~LArSIMplePrimaryGeneratorMessenger()
{
    delete fLArSIMpleDir;
    delete fNeutrinoFileName;
    delete fNeutrinoFileType;
    delete fUseNeutrinos;
    delete fUseRandomNeutrinoVertex;
    delete fNeutrinoVertex;
}

void LArSIMplePrimaryGeneratorMessenger::SetNewValue(G4UIcommand *command, G4String newValue)
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
        if (newValue == "2")
            type = LArSIMpleNeutrinoInputType::kGiBUUText;
        fPrimaryGeneratorAction->SetNeutrinoFileType(type);
    }
    if (command == fNeutrinoVertex)
        fPrimaryGeneratorAction->SetNeutrinoVertex(fNeutrinoVertex->GetNew3VectorValue(newValue));
    if (command == fUseRandomNeutrinoVertex)
        fPrimaryGeneratorAction->SetUseRandomNeutrinoVertex(fUseRandomNeutrinoVertex->GetNewBoolValue(newValue));
}
