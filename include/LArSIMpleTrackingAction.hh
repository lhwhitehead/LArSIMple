#ifndef LArSIMpleTrackingAction_h
#define LArSIMpleTrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "G4VProcess.hh"
#include "globals.hh"

class LArSIMpleEventAction;
class G4Track;

class LArSIMpleTrackingAction : public G4UserTrackingAction
{
public:
  LArSIMpleTrackingAction(LArSIMpleEventAction*);
  virtual ~LArSIMpleTrackingAction();

  void PreUserTrackingAction(const G4Track*);
  void PostUserTrackingAction(const G4Track*);
    
private:
  LArSIMpleEventAction* fEventAction;

};

#endif
