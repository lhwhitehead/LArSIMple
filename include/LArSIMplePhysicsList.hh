// ClassName:   QGSP_BIC_HP
//
// Author: 2006 G.Folger
//
// based on QGSP_BIC
// Modified:
//
//----------------------------------------------------------------------------
//
#ifndef LArSIMplePhysicsList_h
#define LArSIMplePhysicsList_h 1

#include <CLHEP/Units/SystemOfUnits.h>

#include "globals.hh"
#include "G4VModularPhysicsList.hh"
#include "CompileTimeConstraints.hh"
#include "QGSP_BIC_HP.hh"

class LArSIMplePhysicsList: public QGSP_BIC_HP

{
public:
  LArSIMplePhysicsList();
  virtual ~LArSIMplePhysicsList();

public:
  virtual void SetCuts();

private:

  //virtual void ConstructEM();
  
  G4double cutForGamma;
  G4double cutForElectron;
  G4double cutForPositron;
  G4double cutForProton;
};

#endif
