#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "G4VProcess.hh"
#include "globals.hh"

class LArSIMpleEventAction;

class LArSIMpleSteppingAction : public G4UserSteppingAction
{
public:
  LArSIMpleSteppingAction(LArSIMpleEventAction*);
  virtual ~LArSIMpleSteppingAction();

  void UserSteppingAction(const G4Step*);
  G4VProcess* GetCurrentProcess();
    
private:
  LArSIMpleEventAction* fEventAction;
};

#endif
