#ifndef LArSIMpleSteppingAction_h
#define LArSIMpleSteppingAction_h

#include "G4UserSteppingAction.hh"

class LArSIMpleEventAction;
class G4Step;
class G4Track;
class G4VProcess;

class LArSIMpleSteppingAction : public G4UserSteppingAction
{
public:
    LArSIMpleSteppingAction(LArSIMpleEventAction *);
    virtual ~LArSIMpleSteppingAction();

    void UserSteppingAction(const G4Step *);
    G4VProcess *GetCurrentProcess();

private:
    LArSIMpleEventAction *fEventAction;

    void GetFoldedTrackInfo(const G4Track *, int &foldedTrackID, int &foldedTrackPDG, int &foldedTrackProcess);
};

#endif
