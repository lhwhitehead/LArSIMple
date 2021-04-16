#include "LArSIMpleRunAction.hh"

#include "G4Run.hh"

LArSIMpleRunAction::LArSIMpleRunAction()
{

}

LArSIMpleRunAction::~LArSIMpleRunAction()
{

}

void LArSIMpleRunAction::BeginOfRunAction(const G4Run* aRun)
{
 
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;  
  
}

void LArSIMpleRunAction::EndOfRunAction(const G4Run*)
{

}

