#/**
 *  @file LArSIMple/src/LArSIMplePhysicsList.cc
 * 
 *  @brief Implementation of the physics list class.
 * 
 *  $Log: $
 */

#include "LArSIMplePhysicsList.hh"

LArSIMplePhysicsList::LArSIMplePhysicsList() :
    QGSP_BERT()
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

LArSIMplePhysicsList::~LArSIMplePhysicsList()
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMplePhysicsList::SetCuts()
{
    QGSP_BERT::SetCuts();
    SetCutsWithDefault();
}
