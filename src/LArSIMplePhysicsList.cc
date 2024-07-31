#/**
 *  @file LArSIMple/src/LArSIMplePhysicsList.cc
 * 
 *  @brief Implementation of the physics list class.
 * 
 *  $Log: $
 */

#include "LArSIMplePhysicsList.hh"

LArSIMplePhysicsList::LArSIMplePhysicsList() :
    QGSP_BIC_HP()
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

LArSIMplePhysicsList::~LArSIMplePhysicsList()
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMplePhysicsList::SetCuts()
{
    QGSP_BIC_HP::SetCuts();
    SetCutsWithDefault();
}
