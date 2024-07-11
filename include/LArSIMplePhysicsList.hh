#ifndef LArSIMplePhysicsList_h
#define LArSIMplePhysicsList_h 1

#include "QGSP_BIC_HP.hh"

class LArSIMplePhysicsList : public QGSP_BIC_HP

{
public:
    LArSIMplePhysicsList();
    virtual ~LArSIMplePhysicsList();

    virtual void SetCuts();

private:
    G4double fCutForGamma;
    G4double fCutForElectron;
    G4double fCutForPositron;
    G4double fCutForProton;
};

#endif
