/**
 *  @file LArSIMple/src/LArSIMplePrimaryGeneratorMessenger.cc
 * 
 *  @brief Implementation of the primary generator action messenger class.
 * 
 *  $Log: $
 */

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

    fUseNeutrinos = new G4UIcmdWithABool("/LArSIMple/UseNeutrinoEvents", this);
    fUseNeutrinos->SetGuidance("Use neutrino events instead of a particle gun");
    fUseNeutrinos->SetParameterName("UseNeutrinoEvents", false);
    fUseNeutrinos->SetDefaultValue(false);

    fNeutrinoDir = new G4UIdirectory("/LArSIMple/Neutrino/");
    fNeutrinoDir->SetGuidance("Commands to select I/O options for neutrinos");

    fNeutrinoFileName = new G4UIcmdWithAString("/LArSIMple/Neutrino/NeutrinoInputFile", this);
    fNeutrinoFileName->SetGuidance("Path and name of the neutrino input file");
    fNeutrinoFileName->SetParameterName("NeutrinoInputFile", true);
    fNeutrinoFileName->SetDefaultValue("neutrinos.dat");

    fNeutrinoFileType = new G4UIcmdWithAnInteger("/LArSIMple/Neutrino/NeutrinoFileType", this);
    fNeutrinoFileType->SetGuidance("0 = nuance tracker, 1 = GENIE tree");
    fNeutrinoFileType->SetParameterName("NeutrinoFileType", true);
    fNeutrinoFileType->SetDefaultValue(0);

    fUseRandomNeutrinoVertex = new G4UIcmdWithABool("/LArSIMple/Neutrino/UseRandomNeutrinoVertex", this);
    fUseRandomNeutrinoVertex->SetGuidance("Use a random vertex (at least 1m from the walls)");
    fUseRandomNeutrinoVertex->SetParameterName("UseRandomNeutrinoVertex", true);
    fUseRandomNeutrinoVertex->SetDefaultValue(true);

    fNeutrinoVertex = new G4UIcmdWith3VectorAndUnit("/LArSIMple/Neutrino/SetNeutrinoVertex", this);
    fNeutrinoVertex->SetGuidance("Neutrino interaction vertex");
    fNeutrinoVertex->SetParameterName("NeutrinoVertexX", "NeutrinoVertexY", "NeutrinoVertexZ", true);
    fNeutrinoVertex->SetDefaultValue(G4ThreeVector());

    // All options for patricle bombs
    fUseParticleBombs = new G4UIcmdWithABool("/LArSIMple/UseParticleBombs", this);
    fUseParticleBombs->SetGuidance("Use particle bombs instead of a particle gun");
    fUseParticleBombs->SetParameterName("UseParticleBombs", false);
    fUseParticleBombs->SetDefaultValue(false);

    fParticleBombDir = new G4UIdirectory("/LArSIMple/ParticleBomb/");
    fParticleBombDir->SetGuidance("Commands to select I/O options for particle bombs");

    fLeptonPdg = new G4UIcmdWithAnInteger("/LArSIMple/ParticleBomb/LeptonPdg", this);
    fLeptonPdg->SetGuidance("Lepton Pdg code (-999 for no lepton)");
    fLeptonPdg->SetParameterName("LeptonPdg", false);
    fLeptonPdg->SetDefaultValue(-999);

    fNProton = new G4UIcmdWithAnInteger("/LArSIMple/ParticleBomb/NProton", this);
    fNProton->SetGuidance("Number of protons in the particle bomb");
    fNProton->SetParameterName("NProton", false);
    fNProton->SetDefaultValue(0);

    fNNeutron = new G4UIcmdWithAnInteger("/LArSIMple/ParticleBomb/NNeutron", this);
    fNNeutron->SetGuidance("Number of neutrons in the particle bomb");
    fNNeutron->SetParameterName("NNeutron", false);
    fNNeutron->SetDefaultValue(0);

    fNPiZero = new G4UIcmdWithAnInteger("/LArSIMple/ParticleBomb/NPiZero", this);
    fNPiZero->SetGuidance("Number of pi0 in the particle bomb");
    fNPiZero->SetParameterName("NPiZero", false);
    fNPiZero->SetDefaultValue(0);

    fNPiPlus = new G4UIcmdWithAnInteger("/LArSIMple/ParticleBomb/NPiPlus", this);
    fNPiPlus->SetGuidance("Number of pi+ in the particle bomb");
    fNPiPlus->SetParameterName("NPiPlus", false);
    fNPiPlus->SetDefaultValue(0);

    fNPiMinus = new G4UIcmdWithAnInteger("/LArSIMple/ParticleBomb/NPiMinus", this);
    fNPiMinus->SetGuidance("Number of pi- in the particle bomb");
    fNPiMinus->SetParameterName("NPiMinus", false);
    fNPiMinus->SetDefaultValue(0);

    fNKZero = new G4UIcmdWithAnInteger("/LArSIMple/ParticleBomb/NKZero", this);
    fNKZero->SetGuidance("Number of K0 in the particle bomb");
    fNKZero->SetParameterName("NKZero", false);
    fNKZero->SetDefaultValue(0);

    fNKPlus = new G4UIcmdWithAnInteger("/LArSIMple/ParticleBomb/NKPlus", this);
    fNKPlus->SetGuidance("Number of K+ in the particle bomb");
    fNKPlus->SetParameterName("NPiPlus", false);
    fNKPlus->SetDefaultValue(0);

    fNKMinus = new G4UIcmdWithAnInteger("/LArSIMple/ParticleBomb/NKMinus", this);
    fNKMinus->SetGuidance("Number of K- in the particle bomb");
    fNKMinus->SetParameterName("NPiMinus", false);
    fNKMinus->SetDefaultValue(0);
}

//------------------------------------------------------------------------------------------------------------------------------------------

LArSIMplePrimaryGeneratorMessenger::~LArSIMplePrimaryGeneratorMessenger()
{
    delete fLArSIMpleDir;

    delete fUseNeutrinos;
    delete fNeutrinoDir;
    delete fNeutrinoFileName;
    delete fNeutrinoFileType;
    delete fUseRandomNeutrinoVertex;
    delete fNeutrinoVertex;

    delete fUseParticleBombs;
    delete fParticleBombDir;
    delete fLeptonPdg;
    delete fNProton;
    delete fNNeutron;
    delete fNPiZero;
    delete fNPiPlus;
    delete fNPiMinus;
    delete fNKZero;
    delete fNKPlus;
    delete fNKMinus;
}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMplePrimaryGeneratorMessenger::SetNewValue(G4UIcommand *command, G4String newValue)
{
    // Neutrino commands
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

    // Particle bomb commands
    if (command == fUseParticleBombs)
        fPrimaryGeneratorAction->SetUseParticleBombs(fUseParticleBombs->GetNewBoolValue(newValue));
    if (command == fLeptonPdg)
        fPrimaryGeneratorAction->SetParticleBombLeptonPdg(fLeptonPdg->GetNewIntValue(newValue));
    if (command == fNProton)
        fPrimaryGeneratorAction->SetParticleBombNProton(fNProton->GetNewIntValue(newValue));
    if (command == fNNeutron)
        fPrimaryGeneratorAction->SetParticleBombNNeutron(fNNeutron->GetNewIntValue(newValue));
    if (command == fNPiZero)
        fPrimaryGeneratorAction->SetParticleBombNPiZero(fNPiZero->GetNewIntValue(newValue));
    if (command == fNPiPlus)
        fPrimaryGeneratorAction->SetParticleBombNPiPlus(fNPiPlus->GetNewIntValue(newValue));
    if (command == fNPiMinus)
        fPrimaryGeneratorAction->SetParticleBombNPiMinus(fNPiMinus->GetNewIntValue(newValue));
    if (command == fNKZero)
        fPrimaryGeneratorAction->SetParticleBombNKZero(fNKZero->GetNewIntValue(newValue));
    if (command == fNKPlus)
        fPrimaryGeneratorAction->SetParticleBombNKPlus(fNKPlus->GetNewIntValue(newValue));
    if (command == fNKMinus)
        fPrimaryGeneratorAction->SetParticleBombNProton(fNKMinus->GetNewIntValue(newValue));
}
