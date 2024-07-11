#include "LArSIMpleRunAction.hh"

#include "G4Run.hh"

#include <iostream>

LArSIMpleRunAction::LArSIMpleRunAction()
{
}

LArSIMpleRunAction::~LArSIMpleRunAction()
{
}

void LArSIMpleRunAction::BeginOfRunAction(const G4Run *aRun)
{
    std::cout << "### Run " << aRun->GetRunID() << " start." << std::endl;
}

void LArSIMpleRunAction::EndOfRunAction(const G4Run *)
{
}
