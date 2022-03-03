#include "LArSIMplePhysicsList.hh"

#include "globals.hh"

LArSIMplePhysicsList::LArSIMplePhysicsList() : QGSP_BIC_HP() {

  this->defaultCutValue = 0.05*CLHEP::mm; //standard = 0.5 mm
  fCutForGamma         = defaultCutValue;
  fCutForElectron      = defaultCutValue;
  fCutForPositron      = defaultCutValue;
  fCutForProton        = defaultCutValue; 
}


LArSIMplePhysicsList::~LArSIMplePhysicsList() 
{}

void LArSIMplePhysicsList::SetCuts()
{
  QGSP_BIC_HP::SetCuts();
  SetCutsWithDefault();
}

