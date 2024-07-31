/**
 *  @file LArSIMple/src/LArSIMpleRunAction.cc
 * 
 *  @brief Implementation of the run action class.
 * 
 *  $Log: $
 */

#include "LArSIMpleRunAction.hh"

#include "G4Run.hh"

#include <iostream>

LArSIMpleRunAction::LArSIMpleRunAction()
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

LArSIMpleRunAction::~LArSIMpleRunAction()
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMpleRunAction::BeginOfRunAction(const G4Run *aRun)
{
    std::cout << "=== Starting Run " << aRun->GetRunID() << std::endl;
}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMpleRunAction::EndOfRunAction(const G4Run *aRun)
{
    std::cout << "=== Ending Run " << aRun->GetRunID() << std::endl;
}
