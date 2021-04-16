#ifndef LArSIMpleSteppingAction_h
#define LArSIMpleSteppingAction_h

#include "G4UserSteppingAction.hh"
#include "G4VProcess.hh"
#include "globals.hh"

class LArSIMpleEventAction;
class G4Step;
class G4Track;

class LArSIMpleSteppingAction : public G4UserSteppingAction
{
  public:
  LArSIMpleSteppingAction(LArSIMpleEventAction*);
  virtual ~LArSIMpleSteppingAction();

  void UserSteppingAction(const G4Step*);
  G4VProcess* GetCurrentProcess();
    
  private:
  LArSIMpleEventAction* fEventAction;

  void GetFoldedTrackIDAndPDG(const G4Track*, int &foldedTrackID, int &foldedTrackPDG);
};

#endif
